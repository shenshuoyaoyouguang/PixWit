/*
 * CapStep - Smart Screenshot Tool
 * Copyright (C) 2024-2025 CapStep Development Team
 */

#include "EditWindowManager.h"
#include "ScreenshotEditWindow.h"
#include <QApplication>
#include <QScreen>
#include <QDebug>

EditWindowManager::EditWindowManager(QObject *parent)
    : QObject(parent)
{
}

ScreenshotEditWindow* EditWindowManager::showEditWindow(
    const QPixmap &screenshot,
    const QPoint &initialPos,
    const QPoint &lastEditPos,
    const EditWindowCallbacks &callbacks)
{
    ScreenshotEditWindow *editWindow = new ScreenshotEditWindow(screenshot, QPoint());

    // 连接保存信号
    if (callbacks.onSave) {
        connect(editWindow, &ScreenshotEditWindow::saveRequested,
                this, [editWindow, callbacks]() {
            callbacks.onSave(editWindow, editWindow->getScreenshot());
        });
    }

    // 连接复制信号
    if (callbacks.onCopy) {
        connect(editWindow, &ScreenshotEditWindow::copyRequested,
                this, [editWindow, callbacks]() {
            callbacks.onCopy(editWindow, editWindow->getScreenshot());
        });
    }

    // 连接贴图信号
    if (callbacks.onCreateStickyNote) {
        connect(editWindow, &ScreenshotEditWindow::createStickyNoteRequested,
                this, [editWindow, callbacks]() {
            QPoint posToUse = editWindow->pos();
            callbacks.onCreateStickyNote(editWindow, editWindow->getScreenshot(), posToUse);
            editWindow->deleteLater();
        });
    }

    // 连接关闭信号
    if (callbacks.onClose) {
        connect(editWindow, &ScreenshotEditWindow::closeRequested,
                this, [editWindow, callbacks]() {
            callbacks.onClose(editWindow);
            editWindow->deleteLater();
        });
    }

    // 计算期望位置
    QPoint desired = initialPos;
    if (desired.isNull()) {
        if (!lastEditPos.isNull()) {
            desired = lastEditPos;
        } else if (QWidget *aw = QApplication::activeWindow()) {
            desired = aw->pos();
        } else {
            desired = QPoint(100, 100);
        }
    }

    // 计算安全位置
    QPoint finalPos = calculateWindowPosition(desired, editWindow->size(), lastEditPos);
    editWindow->move(finalPos);

    // 显示窗口
    editWindow->show();
    editWindow->raise();
    editWindow->activateWindow();

    m_lastPosition = editWindow->pos();
    emit positionUpdated(m_lastPosition);

    return editWindow;
}

QPoint EditWindowManager::calculateWindowPosition(
    const QPoint &desiredPos,
    const QSize &windowSize,
    const QPoint &)
{
    QScreen *scr = QGuiApplication::screenAt(desiredPos);
    if (!scr) {
        scr = QGuiApplication::primaryScreen();
    }

    QRect avail = scr ? scr->availableGeometry() : QRect(0, 0, 1920, 1080);

    QPoint finalPos;
    if (windowSize.width() > avail.width() || windowSize.height() > avail.height()) {
        finalPos = avail.topLeft();
    } else {
        int x = qBound(avail.left(), desiredPos.x(), avail.right() - windowSize.width() + 1);
        int y = qBound(avail.top(), desiredPos.y(), avail.bottom() - windowSize.height() + 1);
        finalPos = QPoint(x, y);
    }

    return finalPos;
}
