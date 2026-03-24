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
 * @file EditWindowManager.h
 * @brief 截图编辑窗口管理器
 */

#ifndef EDITWINDOWMANAGER_H
#define EDITWINDOWMANAGER_H

#include <QObject>
#include <QPixmap>
#include <QPoint>
#include <QSize>
#include <functional>

class ScreenshotEditWindow;

/**
 * @struct EditWindowCallbacks
 * @brief 编辑窗口回调函数集合
 */
struct EditWindowCallbacks {
    std::function<void(ScreenshotEditWindow*, const QPixmap&)> onSave;
    std::function<void(ScreenshotEditWindow*, const QPixmap&)> onCopy;
    std::function<void(ScreenshotEditWindow*, const QPixmap&, const QPoint&)> onCreateStickyNote;
    std::function<void(ScreenshotEditWindow*)> onClose;
};

/**
 * @class EditWindowManager
 * @brief 管理截图编辑窗口的创建和生命周期
 *
 * 负责创建编辑窗口、计算窗口位置、连接信号
 */
class EditWindowManager : public QObject
{
    Q_OBJECT

public:
    explicit EditWindowManager(QObject *parent = nullptr);

    /**
     * @brief 显示截图编辑窗口
     * @param screenshot 截图内容
     * @param initialPos 期望的初始位置
     * @param lastEditPos 上一次编辑窗口位置（用于智能定位）
     * @param callbacks 回调函数集合
     * @return 创建的编辑窗口指针
     */
    ScreenshotEditWindow* showEditWindow(
        const QPixmap &screenshot,
        const QPoint &initialPos,
        const QPoint &lastEditPos,
        const EditWindowCallbacks &callbacks
    );

    /**
     * @brief 计算编辑窗口的安全位置
     * @param desiredPos 期望位置
     * @param windowSize 窗口大小
     * @param lastEditPos 上一次编辑位置
     * @return 计算后的安全位置
     */
    static QPoint calculateWindowPosition(
        const QPoint &desiredPos,
        const QSize &windowSize,
        const QPoint &lastEditPos
    );

signals:
    /**
     * @brief 编辑窗口位置更新信号
     * @param newPos 新位置
     */
    void positionUpdated(const QPoint &newPos);

private:
    QPoint m_lastPosition;
};

#endif // EDITWINDOWMANAGER_H
