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
 * @file ScreenshotHistory.h
 * @brief 截图历史管理器
 */

#ifndef SCREENSHOTHISTORY_H
#define SCREENSHOTHISTORY_H

#include <QObject>
#include <QPixmap>
#include <QString>

/**
 * @class ScreenshotHistory
 * @brief 管理截图历史保存
 *
 * 负责将截图保存到应用数据目录的历史文件夹中
 */
class ScreenshotHistory : public QObject
{
    Q_OBJECT

public:
    explicit ScreenshotHistory(QObject *parent = nullptr);

    /**
     * @brief 保存截图到历史文件夹
     * @param screenshot 要保存的截图
     * @return 是否保存成功
     */
    bool save(const QPixmap &screenshot);

    /**
     * @brief 获取历史文件夹路径
     * @return 历史文件夹路径
     */
    QString getHistoryPath() const;

    /**
     * @brief 生成默认文件名
     * @return 生成的文件名
     */
    static QString generateFileName();

private:
    bool ensureHistoryDirExists();

    QString m_historyPath;
};

#endif // SCREENSHOTHISTORY_H
