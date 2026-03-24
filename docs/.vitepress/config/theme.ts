import type { DefaultTheme } from 'vitepress'

/**
 * 主题配置 - 除 nav 和 sidebar 外的其他 themeConfig 配置
 */
export const themeConfig: Omit<DefaultTheme.Config, 'nav' | 'sidebar'> = {
  logo: '/logo.svg',

  outline: {
    level: [2, 3],
    label: '本页目录'
  },

  docFooter: {
    prev: '上一页',
    next: '下一页'
  },

  darkModeSwitchLabel: '外观',
  sidebarMenuLabel: '菜单',
  returnToTopLabel: '返回顶部',

  footer: {
    message: '© 2025 PixWit|粤ICP备2025481742号-2',
    copyright: ''
  },

  socialLinks: [
    { icon: 'github', link: 'https://github.com/yun-cun-li/PixWit' }
  ]
}
