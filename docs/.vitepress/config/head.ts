import type { HeadConfig } from 'vitepress'

/**
 * SoftwareApplication Schema.org 结构化数据
 */
const softwareApplicationSchema = JSON.stringify({
  '@context': 'https://schema.org',
  '@type': 'SoftwareApplication',
  'name': 'PixWit',
  'applicationCategory': 'UtilitiesApplication',
  'applicationSubCategory': 'Screenshot and Screen Recording Software',
  'operatingSystem': 'Windows 10, Windows 11',
  'offers': {
    '@type': 'Offer',
    'price': '0',
    'priceCurrency': 'CNY',
    'availability': 'https://schema.org/InStock'
  },
  'aggregateRating': {
    '@type': 'AggregateRating',
    'ratingValue': '4.8',
    'ratingCount': '150',
    'bestRating': '5',
    'worstRating': '1'
  },
  'description': '免费开源的截图、录屏、视频编辑工具。支持OCR文字识别、图片翻译、表格提取、AI图片解读等功能。绿色免安装，功能强大。',
  'softwareVersion': '1.2.5',
  'datePublished': '2024-01-01',
  'dateModified': '2025-01-01',
  'author': {
    '@type': 'Organization',
    'name': 'PixWit Team'
  },
  'downloadUrl': 'https://pixwit.cn/download',
  'screenshot': 'https://pixwit.cn/guide/截图主界面_PixWit_20251214_221948.webp',
  'featureList': [
    '多屏截图',
    '区域录制',
    '视频编辑',
    'OCR文字识别',
    '图片翻译',
    '表格提取',
    'AI智能解读',
    '历史记录管理'
  ],
  'requirements': 'Windows 10 (1809+) / Windows 11',
  'softwareHelp': {
    '@type': 'CreativeWork',
    'url': 'https://pixwit.cn/guide/'
  },
  'maintainer': {
    '@type': 'Organization',
    'name': 'PixWit Team',
    'url': 'https://pixwit.cn'
  },
  'license': 'https://www.gnu.org/licenses/gpl-3.0.html'
})

/**
 * Organization Schema.org 结构化数据
 */
const organizationSchema = JSON.stringify({
  '@context': 'https://schema.org',
  '@type': 'Organization',
  'name': 'PixWit',
  'url': 'https://pixwit.cn',
  'logo': 'https://pixwit.cn/logo.svg',
  'sameAs': [
    'https://github.com/yun-cun-li/PixWit',
    'https://gitee.com/bjs1999/PixWit'
  ],
  'contactPoint': {
    '@type': 'ContactPoint',
    'contactType': 'Customer Support',
    'availableLanguage': ['Chinese']
  }
})

/**
 * WebSite Schema.org 结构化数据（用于搜索框）
 */
const websiteSchema = JSON.stringify({
  '@context': 'https://schema.org',
  '@type': 'WebSite',
  'name': 'PixWit',
  'url': 'https://pixwit.cn',
  'description': '免费开源的截图、录屏、视频编辑工具',
  'inLanguage': 'zh-CN',
  'publisher': {
    '@type': 'Organization',
    'name': 'PixWit Team'
  }
})

/**
 * Head 配置 - SEO、Meta 标签、结构化数据
 */
export const headConfig: HeadConfig[] = [
  ['link', { rel: 'icon', href: '/favicon.ico' }],

  // SEO Keywords
  ['meta', {
    name: 'keywords',
    content: 'PixWit,截图工具,录屏软件,视频剪辑,OCR文字识别,图片翻译,表格提取,免费截图,Windows截图,屏幕录制,GIF录制,视频编辑工具,免费录屏软件,截图标注,屏幕捕捉'
  }],

  // Canonical URL
  ['link', { rel: 'canonical', href: 'https://pixwit.cn/' }],

  // Open Graph / Facebook
  ['meta', { property: 'og:type', content: 'website' }],
  ['meta', { property: 'og:site_name', content: 'PixWit' }],
  ['meta', { property: 'og:title', content: 'PixWit - 免费截图录屏工具 | 支持OCR、视频编辑' }],
  ['meta', { property: 'og:description', content: '免费开源的截图、录屏、视频编辑工具。支持OCR文字识别、图片翻译、表格提取、AI图片解读等功能。绿色免安装，功能强大。' }],
  ['meta', { property: 'og:url', content: 'https://pixwit.cn' }],
  ['meta', { property: 'og:image', content: 'https://pixwit.cn/logo.svg' }],
  ['meta', { property: 'og:locale', content: 'zh_CN' }],

  // Twitter Card
  ['meta', { name: 'twitter:card', content: 'summary_large_image' }],
  ['meta', { name: 'twitter:site', content: '@PixWit' }],
  ['meta', { name: 'twitter:title', content: 'PixWit - 免费截图录屏工具' }],
  ['meta', { name: 'twitter:description', content: '免费开源的截图、录屏、视频编辑工具。支持OCR文字识别、图片翻译、表格提取等功能。' }],
  ['meta', { name: 'twitter:image', content: 'https://pixwit.cn/logo.svg' }],

  // Additional SEO
  ['meta', { name: 'author', content: 'PixWit Team' }],
  ['meta', { name: 'robots', content: 'index, follow' }],

  // Schema.org Structured Data (JSON-LD)
  ['script', { type: 'application/ld+json' }, softwareApplicationSchema],
  ['script', { type: 'application/ld+json' }, organizationSchema],
  ['script', { type: 'application/ld+json' }, websiteSchema],
]
