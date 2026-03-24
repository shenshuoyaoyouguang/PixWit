import type { DefaultTheme } from 'vitepress'

/**
 * 导航栏配置
 */
export const navConfig: DefaultTheme.NavItem[] = [
  { text: '首页', link: '/' },
  { text: '使用手册', link: '/guide/' },
  { text: '下载', link: '/download' },
  {
    text: '社区',
    items: [
      { text: 'QQ 群：133125470', link: '#' },
      { text: 'GitHub', link: 'https://github.com/yun-cun-li/PixWit' },
      { text: 'Gitee', link: 'https://gitee.com/bjs1999/PixWit' },
    ]
  }
]
