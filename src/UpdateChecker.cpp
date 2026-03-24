/*
 * CapStep - Smart Screenshot Tool
 */

#include "DesktopConfig.h"
#include "UpdateChecker.h"

#include <QAbstractButton>
#include <QDateTime>
#include <QDesktopServices>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkRequest>
#include <QPushButton>
#include <QSettings>
#include <QStringList>
#include <QUrl>

#include <memory>
#include <optional>


class UrlValidator {
public:
    static bool isMetadataUrlSafe(const QString &urlString)
    {
        QUrl url;
        if (!parseHttpsUrl(urlString, url, "metadata")) {
            return false;
        }

        const QString host = url.host().toLower();
        const QString path = url.path();
        if (host == "gitee.com" && path.startsWith("/yun-meng-song/cap-step-releases/")) {
            return true;
        }
        if ((host == "pixwit.cn" || host == "www.pixwit.cn") && path.endsWith("/latest.json")) {
            return true;
        }

        qWarning() << "[UpdateChecker] Rejected untrusted metadata source:" << urlString;
        return false;
    }

    static bool isDownloadUrlSafe(const QString &urlString)
    {
        QUrl url;
        if (!parseHttpsUrl(urlString, url, "download")) {
            return false;
        }

        const QString host = url.host().toLower();
        const QString path = url.path();
        if (host == "github.com" && path.startsWith("/yun-cun-li/PixWit/releases/download/")) {
            return true;
        }
        if (host == "gitee.com" && path.startsWith("/bjs1999/PixWit/releases/download/")) {
            return true;
        }

        qWarning() << "[UpdateChecker] Rejected untrusted download URL:" << urlString;
        return false;
    }

    static bool openUrlSafely(const QString &urlString)
    {
        if (!isDownloadUrlSafe(urlString)) {
            qWarning() << "[UpdateChecker] Blocked unsafe URL:" << urlString;
            return false;
        }
        return QDesktopServices::openUrl(QUrl(urlString));
    }

private:
    static bool parseHttpsUrl(const QString &urlString, QUrl &url, const char *purpose)
    {
        if (urlString.isEmpty()) {
            return false;
        }

        url = QUrl(urlString);
        if (!url.isValid()) {
            qWarning() << "[UpdateChecker] Invalid" << purpose << "URL format:" << urlString;
            return false;
        }

        const QString scheme = url.scheme().toLower();
        if (scheme != "https") {
            qWarning() << "[UpdateChecker] Rejected unsafe" << purpose << "URL scheme:" << scheme;
            return false;
        }

        return true;
    }
};

struct ParsedUpdateInfo {
    QString latestVersion;
    QMap<QString, DownloadOption> downloadOptions;
    QString releaseNotes;
};

class UpdateRequestBuilder {
public:
    QNetworkRequest buildRequest(const QString &url) const
    {
        QNetworkRequest request(QUrl(url));
        request.setHeader(QNetworkRequest::UserAgentHeader, DesktopConfig::HttpUserAgent);
        request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
        return request;
    }
};

class VersionInfoParser {
public:
    std::optional<ParsedUpdateInfo> parse(const QByteArray &jsonData) const
    {
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            qWarning() << "[UpdateChecker] JSON parse error:" << parseError.errorString();
            return std::nullopt;
        }

        if (!doc.isObject()) {
            qWarning() << "[UpdateChecker] Invalid JSON format";
            return std::nullopt;
        }

        QJsonObject obj = doc.object();
        ParsedUpdateInfo info;
        info.latestVersion = obj.value("version").toString();
        info.releaseNotes = obj.value("releaseNotes").toString();
        info.downloadOptions = parseDownloadOptions(obj);

        if (info.latestVersion.isEmpty() || info.downloadOptions.isEmpty()) {
            qWarning() << "[UpdateChecker] Missing required fields in JSON";
            return std::nullopt;
        }

        qDebug() << "[UpdateChecker] Parsed version info:";
        qDebug() << "  - Version:" << info.latestVersion;
        qDebug() << "  - Download options:" << info.downloadOptions.size();
        qDebug() << "  - Release Notes:" << info.releaseNotes;

        return info;
    }

private:
    QMap<QString, DownloadOption> parseDownloadOptions(const QJsonObject &rootObj) const
    {
        QMap<QString, DownloadOption> options;

        QJsonObject downloadsObj = rootObj.value("downloads").toObject();
        if (downloadsObj.isEmpty()) {
            qWarning() << "[UpdateChecker] No 'downloads' section found in JSON";
            return options;
        }

        // Parse each download option (e.g., "portable_7z", "setup")
        for (const QString &key : downloadsObj.keys()) {
            QJsonObject downloadObj = downloadsObj.value(key).toObject();
            if (downloadObj.isEmpty()) {
                continue;
            }

            DownloadOption option;
            option.name = downloadObj.value("name").toString();
            option.description = downloadObj.value("description").toString();
            option.size = downloadObj.value("size").toString();
            option.type = key;  // e.g., "portable_7z", "setup"

            // Prefer GitHub URL, fallback to Gitee
            QString githubUrl = downloadObj.value("github").toString();
            QString giteeUrl = downloadObj.value("gitee").toString();

            if (!githubUrl.isEmpty() && UrlValidator::isDownloadUrlSafe(githubUrl)) {
                option.url = githubUrl;
            } else if (!giteeUrl.isEmpty() && UrlValidator::isDownloadUrlSafe(giteeUrl)) {
                option.url = giteeUrl;
            } else {
                qWarning() << "[UpdateChecker] No valid download URL found for" << key;
                continue;
            }

            if (!option.name.isEmpty() && !option.url.isEmpty()) {
                options.insert(key, option);
                qDebug() << "[UpdateChecker] Parsed download option:" << key << "->" << option.name;
            }
        }

        return options;
    }
};

class UpdatePreferences {
public:
    explicit UpdatePreferences(QObject *parent)
        : m_settings(std::make_unique<QSettings>(DesktopConfig::SettingsOrgName,
                                                 DesktopConfig::SettingsAppName,
                                                 parent))
    {
    }

    void setAutoCheckEnabled(bool enabled)
    {
        m_settings->setValue(DesktopConfig::SettingKeyAutoCheckEnabled, enabled);
    }

    void saveLastCheckTime(const QDateTime &time)
    {
        m_settings->setValue(DesktopConfig::SettingKeyLastCheckTime, time);
    }

    QDateTime loadLastCheckTime() const
    {
        return m_settings->value(DesktopConfig::SettingKeyLastCheckTime).toDateTime();
    }

private:
    std::unique_ptr<QSettings> m_settings;
};

class UpdateResponseHandler {
public:
    int compareVersions(const QString &version1, const QString &version2) const
    {
        QStringList parts1 = version1.split(".");
        QStringList parts2 = version2.split(".");

        int maxLength = qMax(parts1.length(), parts2.length());

        for (int i = 0; i < maxLength; ++i) {
            int num1 = (i < parts1.length()) ? parts1[i].toInt() : 0;
            int num2 = (i < parts2.length()) ? parts2[i].toInt() : 0;

            if (num1 > num2) {
                return 1;
            }
            if (num1 < num2) {
                return -1;
            }
        }

        return 0;
    }

    bool isUpdateAvailable(const QString &latestVersion, const QString &currentVersion) const
    {
        return compareVersions(latestVersion, currentVersion) > 0;
    }
};

UpdateChecker::UpdateChecker(QObject *parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_checkTimer(new QTimer(this))
    , m_currentVersion(DesktopConfig::DefaultCurrentVersion)
    , m_updateUrl(DesktopConfig::UpdateCheckUrl)
    , m_autoCheckEnabled(true)
    , m_checkIntervalHours(24)
    , m_isChecking(false)
    , m_requestBuilder(std::make_unique<UpdateRequestBuilder>())
    , m_versionParser(std::make_unique<VersionInfoParser>())
    , m_preferences(std::make_unique<UpdatePreferences>(this))
    , m_responseHandler(std::make_unique<UpdateResponseHandler>())
{
    connect(m_networkManager, &QNetworkAccessManager::finished,
            this, &UpdateChecker::onNetworkReply);

    connect(m_checkTimer, &QTimer::timeout,
            this, &UpdateChecker::onTimerTriggered);

    qDebug() << "[UpdateChecker] Initialized";
}

UpdateChecker::~UpdateChecker()
{
    stopAutoCheck();
    qDebug() << "[UpdateChecker] Destroyed";
}

void UpdateChecker::setCurrentVersion(const QString &version)
{
    m_currentVersion = version;
    qDebug() << "[UpdateChecker] Current version set to:" << version;
}

QString UpdateChecker::currentVersion() const
{
    return m_currentVersion;
}

void UpdateChecker::setUpdateUrl(const QString &url)
{
    m_updateUrl = url;
    qDebug() << "[UpdateChecker] Update URL set to:" << url;
}

void UpdateChecker::startAutoCheck(int checkIntervalHours)
{
    if (!m_autoCheckEnabled) {
        qDebug() << "[UpdateChecker] Auto-check is disabled";
        return;
    }

    m_checkIntervalHours = checkIntervalHours;

    if (shouldCheckUpdate()) {
        qDebug() << "[UpdateChecker] Starting immediate check";
        QTimer::singleShot(DesktopConfig::StartupCheckDelayMs, this, &UpdateChecker::checkForUpdates);
    }

    int intervalMs = checkIntervalHours * 60 * 60 * 1000;
    m_checkTimer->start(intervalMs);

    qDebug() << "[UpdateChecker] Auto-check started, interval:" << checkIntervalHours << "hours";
}

void UpdateChecker::stopAutoCheck()
{
    if (m_checkTimer->isActive()) {
        m_checkTimer->stop();
        qDebug() << "[UpdateChecker] Auto-check stopped";
    }
}

void UpdateChecker::checkForUpdates()
{
    if (m_isChecking) {
        qDebug() << "[UpdateChecker] Already checking for updates";
        return;
    }

    if (m_updateUrl.isEmpty()) {
        qDebug() << "[UpdateChecker] Update URL is empty";
        emit checkFailed("Update URL not configured");
        return;
    }

    if (!UrlValidator::isMetadataUrlSafe(m_updateUrl)) {
        emit checkFailed("Update URL is not trusted");
        return;
    }

    m_isChecking = true;
    emit checkStarted();

    qDebug() << "[UpdateChecker] Checking for updates from:" << m_updateUrl;

    QNetworkRequest request = m_requestBuilder->buildRequest(m_updateUrl);
    m_networkManager->get(request);
}

void UpdateChecker::setAutoCheckEnabled(bool enabled)
{
    m_autoCheckEnabled = enabled;
    m_preferences->setAutoCheckEnabled(enabled);

    if (enabled) {
        qDebug() << "[UpdateChecker] Auto-check enabled";
    } else {
        qDebug() << "[UpdateChecker] Auto-check disabled";
        stopAutoCheck();
    }
}

bool UpdateChecker::isAutoCheckEnabled() const
{
    return m_autoCheckEnabled;
}

void UpdateChecker::onNetworkReply(QNetworkReply *reply)
{
    m_isChecking = false;
    emit checkFinished();

    if (reply->error() != QNetworkReply::NoError) {
        QString errorMsg = reply->errorString();
        qWarning() << "[UpdateChecker] Network error:" << errorMsg;
        emit checkFailed(errorMsg);
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    reply->deleteLater();

    qDebug() << "[UpdateChecker] Received update info:" << data.size() << "bytes";

    std::optional<ParsedUpdateInfo> parsed = m_versionParser->parse(data);
    if (!parsed.has_value()) {
        qWarning() << "[UpdateChecker] Failed to parse version info";
        emit checkFailed("Failed to parse update information");
        return;
    }

    m_latestVersion = parsed->latestVersion;
    m_downloadOptions = parsed->downloadOptions;
    m_releaseNotes = parsed->releaseNotes;

    QDateTime now = QDateTime::currentDateTime();
    m_preferences->saveLastCheckTime(now);
    qDebug() << "[UpdateChecker] Saved last check time:" << now.toString();

    if (m_responseHandler->isUpdateAvailable(m_latestVersion, m_currentVersion)) {
        qDebug() << "[UpdateChecker] New version available:" << m_latestVersion;
        emit updateAvailable(m_latestVersion, m_downloadOptions, m_releaseNotes);
        showUpdateDialog();
    } else {
        qDebug() << "[UpdateChecker] Already up to date";
        emit noUpdateAvailable();
    }
}

void UpdateChecker::onTimerTriggered()
{
    qDebug() << "[UpdateChecker] Timer triggered, checking for updates";
    checkForUpdates();
}

bool UpdateChecker::shouldCheckUpdate() const
{
    QDateTime lastCheck = m_preferences->loadLastCheckTime();
    qDebug() << "[UpdateChecker] Loaded last check time:" << lastCheck.toString();

    if (!lastCheck.isValid()) {
        qDebug() << "[UpdateChecker] No previous check time, should check";
        return true;
    }

    QDateTime now = QDateTime::currentDateTime();
    qint64 hoursSinceLastCheck = lastCheck.secsTo(now) / 3600;

    bool shouldCheck = hoursSinceLastCheck >= m_checkIntervalHours;

    qDebug() << "[UpdateChecker] Hours since last check:" << hoursSinceLastCheck
             << "/ Required:" << m_checkIntervalHours
             << "/ Should check:" << shouldCheck;

    return shouldCheck;
}

void UpdateChecker::showUpdateDialog()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("发现新版本");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(QString("发现新版本 %1").arg(m_latestVersion));

    QString updateText = QStringLiteral("当前版本：%1\n\n更新内容：\n%2\n\n请选择下载方式：")
                        .arg(m_currentVersion)
                        .arg(m_releaseNotes);

    msgBox.setInformativeText(updateText);

    if (m_downloadOptions.size() == 1) {
        auto it = m_downloadOptions.begin();
        QString buttonText = QString("下载 %1").arg(it.value().name);
        QPushButton* downloadBtn = msgBox.addButton(buttonText, QMessageBox::AcceptRole);
        msgBox.addButton("取消", QMessageBox::RejectRole);
        msgBox.setDefaultButton(downloadBtn);

        if (msgBox.exec() == QMessageBox::AcceptRole) {
            UrlValidator::openUrlSafely(it.value().url);
        }
    } else if (m_downloadOptions.size() > 1) {
        QPushButton* cancelBtn = msgBox.addButton("取消", QMessageBox::RejectRole);

        QMap<QString, QPushButton*> downloadButtons;
        for (auto it = m_downloadOptions.begin(); it != m_downloadOptions.end(); ++it) {
            QString buttonText = QString("下载 %1").arg(it.value().name);
            QPushButton* button = msgBox.addButton(buttonText, QMessageBox::ActionRole);
            downloadButtons[it.key()] = button;
        }

        msgBox.exec();

        QAbstractButton* clickedButton = msgBox.clickedButton();
        if (clickedButton && clickedButton != cancelBtn) {
            for (auto it = downloadButtons.begin(); it != downloadButtons.end(); ++it) {
                if (it.value() == clickedButton) {
                    QString selectedKey = it.key();
                    if (m_downloadOptions.contains(selectedKey)) {
                        UrlValidator::openUrlSafely(m_downloadOptions[selectedKey].url);
                    }
                    break;
                }
            }
        }
    }
}
