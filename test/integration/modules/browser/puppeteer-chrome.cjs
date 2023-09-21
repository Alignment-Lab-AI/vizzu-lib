const fs = require('fs')
const puppeteer = require('puppeteer')

class PuppeteerChrome {
  initializing

  #browserReady
  #pagesReady
  #pageReady

  constructor(headless = true) {
    this.initializing = this.#startBrowser(headless)
    this.logs = []
  }

  async #startBrowser(headless) {
    if (headless) headless = 'new'
    this.#browserReady = puppeteer.launch({
      headless,
      args: ['--force-device-scale-factor=1', '--start-maximized', '--verbose']
    })
    this.#pagesReady = this.#browserReady.then((browser) => {
      return browser.pages()
    })
    this.#pageReady = this.#pagesReady.then((pages) => {
      return pages[0]
    })
    this.#pageReady.then((page) => {
      page
        .on('console', (message) => {
          this.logs.push(message.text())
        })
        .on('pageerror', ({ message }) => this.logs.push(message))
        .on('response', (response) => this.logs.push(`${response.status()} ${response.url()}`))
        .on('requestfailed', (request) =>
          this.logs.push(`${request.failure().errorText} ${request.url()}`)
        )
        .on('popup', popup => {
          popup.close();
        });
    })
    return this.#browserReady
  }

  async closeBrowser(browserLog) {
    this.#browserReady.then((browser) => {
      browser.close()
      fs.appendFile(browserLog, this.logs.join('űn'), (err) => {
        if (err) {
          throw err
        }
      })
    })
  }

  getUrl(url) {
    return this.#pageReady.then(page => {
      return page.goto(url)
    })
  }

  executeScript(script) {
    return this.#pageReady.then(page => {
      return page.evaluate(script)
    })
  }

  waitUntilTitleIs(title, timeout) {
    return this.#pageReady.then(page => {
      return page.waitForFunction((expectedTitle) => document.title === expectedTitle, {}, title, {
        timeout
      })
    })
  }
}

module.exports = PuppeteerChrome
