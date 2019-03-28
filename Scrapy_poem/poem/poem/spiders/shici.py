# -*- coding: utf-8 -*-
import scrapy
from poem.items import PoemItem


class ShiciSpider(scrapy.Spider):
    name = 'shici'
    allowed_domains = ['shicimingju.com']
    base_url = 'http://www.shicimingju.com'

    page = 1
    first_urls = 'http://www.shicimingju.com/shicimark/miaoxiechuntian_{}_0__0.html'
    start_urls = [first_urls.format(page)]

    def parse(self, response):
        link_list = response.xpath('//h3')
        for link in link_list:
            href = self.base_url + link.xpath('./a/@href').extract()[0]
            print("href:" + href)
            yield scrapy.Request(url=href, callback=self.parse_content, dont_filter=True)

        if self.page < 5:
            self.page += 1
            yield scrapy.Request(url=self.first_urls.format(self.page), callback=self.parse)

    def parse_content(self, response):
        item = PoemItem()
        item['title'] = response.xpath('//h1[@class="shici-title"]/text()').extract()[0]
        item['author'] = response.xpath('//div[@class="shici-info"]/a/text()').extract()[0]
        item['poem'] = response.xpath('//div[@class="shici-content"]/text()').extract()
        print(item['title'])
        print(item['author'])
        print(item['poem'])
        yield item


