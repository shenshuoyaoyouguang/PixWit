import type { UserConfig } from 'vitepress'

/**
 * 站点基础配置
 */
export const siteConfig: Pick<UserConfig, 'title' | 'description' | 'lang' | 'base' | 'cleanUrls' | 'appearance'> = {
  title: 'PixWit',
  description: '截图 · 录屏 · 简易剪辑',
  lang: 'zh-CN',
  // 直接服务器部署使用根路径
  base: '/',

  // 启用简洁 URL（移除 .html 后缀）
  cleanUrls: true,

  appearance: 'dark', // 默认暗黑模式
}
