# -*- coding: utf-8 -*-

# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: https://doc.scrapy.org/en/latest/topics/item-pipeline.html
import re
import json

class PoemPipeline(object):
    def __init__(self):
        self.file = open('spring.txt', 'wb')
        self.file2 = open('spring_num.txt', 'wb')

    def process_item(self, item, spider):
        title = str(item['title']).strip() + '\n' #删除空格
        title = re.sub(u"\\(.*?\\)|\\{.*?}|\\[.*?]", "", title)  #删除括号
        title = re.sub(u"\\（.*?）|\\{.*?}|\\[.*?]|\\【.*?】", "", title)

        author = str(item['author']).strip() + '\n'

        poem = ""
        is_poem = 1
        for line in item['poem']:
            line = str(line).strip()  #删除空格
            line = re.sub(u"\\(.*?\\)|\\{.*?}|\\[.*?]", "", line)  #删除括号
            line = re.sub(u"\\（.*?）|\\{.*?}|\\[.*?]|\\【.*?】", "", line)
            str_len = len(line)
            if (str_len != 6 and str_len != 8 and str_len != 12 and str_len != 16) : #判断是否为诗
                is_poem = 0
            poem += str(line + '\n')

        if (is_poem == 1):
            self.file.write(title.encode("utf-8"))
            self.file.write(author.encode("utf-8"))
            self.file.write(poem.encode("utf-8"))
            self.file2.write(b"done!" + b'\n')

        return item

    def close_spider(self, spider):
        self.file.close()
