import { defineConfig } from 'vitepress'

import {
  headConfig,
  navConfig,
  sidebarConfig,
  siteConfig,
  sitemapConfig,
  themeConfig
} from './config'

export default defineConfig({
  ...siteConfig,
  head: headConfig,
  sitemap: sitemapConfig,
  themeConfig: {
    ...themeConfig,
    nav: navConfig,
    sidebar: sidebarConfig
  }
})
