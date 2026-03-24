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
 * @file DesktopConfig.h
 * @brief 桌面端运行时配置常量 - 单一 truth source
 *
 * 集中管理应用名称、组织名、版本、单实例标识符、更新URL等常量。
 * 所有桌面端代码应从此处引用，避免硬编码分散在各文件中。
 */

#ifndef DESKTOPCONFIG_H
#define DESKTOPCONFIG_H

#include <QString>

namespace DesktopConfig {

// ============================================================================
// 应用基本信息 (Application Identity)
// ============================================================================

/// 应用程序名称（QSettings、QLockFile等系统标识用）
constexpr const char* AppName = "CapStep";

/// 组织名称
constexpr const char* OrganizationName = "CapStep Team";

/// 应用程序版本号
constexpr const char* ApplicationVersion = "0.1.3";

// ============================================================================
// 单实例机制标识符 (Single Instance Identifiers)
// ============================================================================

/// 单实例锁文件名（QLockFile用）
constexpr const char* LockFileName = "CapStep.lock";

/// 本地socket服务器名（QLocalServer/QLocalSocket用）
constexpr const char* LocalServerName = "CapStepInstance";

/// 单实例锁文件获取超时（毫秒）
constexpr int LockFileTimeoutMs = 100;

/// 单实例socket连接超时（毫秒）
constexpr int SingleInstanceSocketTimeoutMs = 1000;

// ============================================================================
// 窗口与UI文本 (Window and UI Text)
// ============================================================================

/// 主窗口标题
inline const QString MainWindowTitle = QStringLiteral("CapStep - 截图工具");

/// 系统托盘图标提示文本
constexpr const char* TrayIconTooltip = "CapStep";

// ============================================================================
// 更新检查配置 (Update Checker)
// ============================================================================

/// 更新检查URL（Gitee仓库）
constexpr const char* UpdateCheckUrl = "https://gitee.com/yun-meng-song/cap-step-releases/raw/master/latest.json";

/// HTTP User-Agent
constexpr const char* HttpUserAgent = "CapStep-UpdateChecker/1.0";

/// 默认当前版本（UpdateChecker内部回退值）
constexpr const char* DefaultCurrentVersion = "0.0.5";

/// 启动后延迟检查时间（毫秒）
constexpr int StartupCheckDelayMs = 5000;

/// 自动更新检查间隔（小时）
constexpr int AutoCheckIntervalHours = 1;

// ============================================================================
// 设置存储键名 (Settings Keys)
// ============================================================================

/// 开机自动启动注册表键名（Windows）
constexpr const char* AutoStartRegistryKey = "CapStep";

/// QSettings组织名（UpdateChecker独立存储用）
constexpr const char* SettingsOrgName = "CapStep";

/// QSettings应用名（UpdateChecker独立存储用）
constexpr const char* SettingsAppName = "UpdateChecker";

/// 自动检查更新启用设置键
constexpr const char* SettingKeyAutoCheckEnabled = "autoCheckEnabled";

/// 上次检查时间设置键
constexpr const char* SettingKeyLastCheckTime = "lastCheckTime";

// ============================================================================
// 文件系统路径 (File System Paths)
// ============================================================================

/// 截图历史子目录路径（相对于QStandardPaths::AppDataLocation）
constexpr const char* HistoryRelativePath = "/ScreenshotHistory/images";

} // namespace DesktopConfig

#endif // DESKTOPCONFIG_H
