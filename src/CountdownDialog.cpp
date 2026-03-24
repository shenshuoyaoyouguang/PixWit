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
 * @file CountdownDialog.cpp
 * @brief 延迟截图倒计时对话框实现
 */

#include "CountdownDialog.h"
#include <QVBoxLayout>
#include <QGuiApplication>
#include <QScreen>

CountdownDialog::CountdownDialog(int delayMs, QWidget *parent)
    : QWidget(parent, Qt::ToolTip | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
    , m_countdownLabel(nullptr)
    , m_countdownTimer(nullptr)
    , m_remainingSeconds(0)
    , m_delayMs(delayMs)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    setupUI();
}

CountdownDialog::~CountdownDialog()
{
}

void CountdownDialog::setupUI()
{
    m_countdownLabel = new QLabel(this);
    m_countdownLabel->setAlignment(Qt::AlignCenter);
    m_countdownLabel->setStyleSheet(
        "QLabel { "
        "background-color: rgba(0, 0, 0, 180); "
        "color: #00FF00; "
        "font-size: 48px; "
        "font-weight: bold; "
        "border-radius: 10px; "
        "padding: 20px 40px; "
        "}"
    );

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_countdownLabel);
    layout->setContentsMargins(0, 0, 0, 0);

    resize(300, 200);
    centerOnScreen();
}

void CountdownDialog::centerOnScreen()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen) return;

    QRect screenGeo = screen->geometry();
    move(screenGeo.center().x() - width() / 2,
         screenGeo.center().y() - height() / 2);
}

void CountdownDialog::start()
{
    m_remainingSeconds = m_delayMs / 1000;
    m_countdownLabel->setText(QString("准备弹窗\n%1").arg(m_remainingSeconds));

    m_countdownTimer = new QTimer(this);
    m_countdownTimer->setInterval(1000);
    connect(m_countdownTimer, &QTimer::timeout, this, &CountdownDialog::updateCountdown);
    m_countdownTimer->start();

    // 延迟截图完成后关闭倒计时窗口
    QTimer::singleShot(m_delayMs + 500, this, &CountdownDialog::onTimeout);

    show();
}

void CountdownDialog::updateCountdown()
{
    m_remainingSeconds--;
    if (m_remainingSeconds > 0) {
        m_countdownLabel->setText(QString("准备弹窗\n%1").arg(m_remainingSeconds));
    } else {
        m_countdownLabel->setText("截图中...");
    }
}

void CountdownDialog::onTimeout()
{
    if (m_countdownTimer) {
        m_countdownTimer->stop();
    }
    emit finished();
    close();
}
