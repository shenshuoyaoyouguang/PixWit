import type { DefaultTheme } from 'vitepress'

/**
 * 侧边栏配置 - 使用手册
 */
export const sidebarConfig: DefaultTheme.Sidebar = {
  '/guide/': [
    {
      text: '快速开始',
      items: [
        { text: '介绍', link: '/guide/' },
        { text: '安装指南', link: '/guide/installation' },
      ]
    },
    {
      text: '核心功能',
      items: [
        { text: '📸 截图功能', link: '/guide/screenshot' },
        { text: '🔍 文字识别', link: '/guide/ocr' },
        { text: '🌐 图片翻译', link: '/guide/translation' },
        { text: '🎬 录屏功能', link: '/guide/recording' },
        { text: '🖼️ 图片处理', link: '/guide/image' },
        { text: '🎞️ 视频处理', link: '/guide/video' },
        {
          text: '▦⤴提取表格',
          link: '/guide/table_extraction_guide'
        },
        {
          text: '🤖 深度解读',
          link: '/guide/ai_analysis_guide'
        },
        {
          text: '🕒历史记录',
          link: '/guide/screenshot-hist'
        }
      ]
    },
  ]
}
