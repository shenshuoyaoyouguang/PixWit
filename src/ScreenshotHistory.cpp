/*
 * CapStep - Smart Screenshot Tool
 * Copyright (C) 2024-2025 CapStep Development Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

/**
 * @file ScreenshotHistory.cpp
 * @brief 截图历史管理器实现
 */

#include "ScreenshotHistory.h"
#include <QStandardPaths>
#include <QDir>
#include <QDateTime>
#include <QDebug>

ScreenshotHistory::ScreenshotHistory(QObject *parent)
    : QObject(parent)
{
    QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    m_historyPath = appData + "/ScreenshotHistory/images";
}

bool ScreenshotHistory::save(const QPixmap &screenshot)
{
    if (screenshot.isNull()) {
        return false;
    }

    if (!ensureHistoryDirExists()) {
        return false;
    }

    QString filePath = m_historyPath + "/" + generateFileName();

    if (screenshot.save(filePath)) {
        qDebug() << "[History] Screenshot saved to:" << filePath;
        return true;
    } else {
        qWarning() << "[History] Failed to save screenshot to:" << filePath;
        return false;
    }
}

QString ScreenshotHistory::getHistoryPath() const
{
    return m_historyPath;
}

QString ScreenshotHistory::generateFileName()
{
    return QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".png";
}

bool ScreenshotHistory::ensureHistoryDirExists()
{
    QDir dir;
    if (dir.exists(m_historyPath)) {
        return true;
    }

    if (dir.mkpath(m_historyPath)) {
        return true;
    }

    qWarning() << "[History] Failed to create history folder:" << m_historyPath;
    return false;
}
