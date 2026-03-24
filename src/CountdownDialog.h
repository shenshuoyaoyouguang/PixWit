/*
 * CapStep - Smart Screenshot Tool
 * Copyright (C) 2024-2025 CapStep Development Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/**
 * @file CountdownDialog.h
 * @brief 延迟截图倒计时对话框
 */

#ifndef COUNTDOWNDIALOG_H
#define COUNTDOWNDIALOG_H

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QScreen>

/**
 * @class CountdownDialog
 * @brief 延迟截图倒计时显示对话框
 *
 * 显示倒计时提示窗口，居中显示在屏幕上
 */
class CountdownDialog : public QWidget
{
    Q_OBJECT

public:
    explicit CountdownDialog(int delayMs = 1000, QWidget *parent = nullptr);
    ~CountdownDialog();

    /**
     * @brief 开始倒计时
     */
    void start();

signals:
    /**
     * @brief 倒计时完成信号
     */
    void finished();

private slots:
    void updateCountdown();
    void onTimeout();

private:
    void setupUI();
    void centerOnScreen();

    QLabel *m_countdownLabel;
    QTimer *m_countdownTimer;
    int m_remainingSeconds;
    int m_delayMs;
};

#endif // COUNTDOWNDIALOG_H
