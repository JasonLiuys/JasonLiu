# -*- coding: utf-8 -*-

# Define here the models for your scraped items
#
# See documentation in:
# https://doc.scrapy.org/en/latest/topics/items.html

import scrapy


class BioItem(scrapy.Item):
    title = scrapy.Field()  #ASM19595v2
    bioSample = scrapy.Field()  #SAMEA3138326
    sampleName = scrapy.Field()  #AE000516
    geoLocation = scrapy.Field()  #Russia
    isoSource = scrapy.Field()   #sputum
    strain = scrapy.Field()    #EAI5/NITR206
    #link = scrapy.Field()   #链接