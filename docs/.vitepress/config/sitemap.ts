import type { UserConfig } from 'vitepress'

/**
 * 站点地图配置
 */
export const sitemapConfig: UserConfig['sitemap'] = {
  hostname: 'https://pixwit.cn',
  transformItems(items) {
    const now = new Date().toISOString()

    return items.map((item) => {
      const url = item.url

      // 首页 - 最高优先级，每天更新
      if (url === '/') {
        return {
          ...item,
          lastmod: now,
          changefreq: 'daily',
          priority: 1.0
        }
      }

      // 下载页 - 高优先级，每周更新
      if (url === '/download.html') {
        return {
          ...item,
          lastmod: now,
          changefreq: 'weekly',
          priority: 0.9
        }
      }

      // 使用手册首页 - 高优先级
      if (url === '/guide/') {
        return {
          ...item,
          lastmod: now,
          changefreq: 'weekly',
          priority: 0.9
        }
      }

      // 核心功能页面 - 中高优先级
      if (url.includes('/guide/screenshot') ||
          url.includes('/guide/recording') ||
          url.includes('/guide/ocr') ||
          url.includes('/guide/translation') ||
          url.includes('/guide/video') ||
          url.includes('/guide/table_extraction')) {
        return {
          ...item,
          lastmod: now,
          changefreq: 'monthly',
          priority: 0.8
        }
      }

      // 其他指南页面 - 中等优先级
      if (url.includes('/guide/')) {
        return {
          ...item,
          lastmod: now,
          changefreq: 'monthly',
          priority: 0.7
        }
      }

      // 默认 - 低优先级
      return {
        ...item,
        lastmod: now,
        changefreq: 'monthly',
        priority: 0.5
      }
    })
  }
}
