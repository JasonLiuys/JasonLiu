import scrapy
import selenium
import time
import random
from lxml import etree
from scrapy.http import Request
from selenium import webdriver  
from Bio.items import BioItem

class BioSpider(scrapy.Spider):
    name = 'bio'
    allowed_domains = ['ncbi.nlm.nih.gov']
    start_urls=['https://www.ncbi.nlm.nih.gov/assembly/?term=Mycobacterium+tuberculosis']
    num = 0
    
    def __init__(self):
        self.driver = webdriver.Chrome(executable_path="C:/Program Files (x86)/Google/Chrome/Application/chromedriver.exe")
        self.driver.get('https://www.ncbi.nlm.nih.gov/assembly/?term=Mycobacterium+tuberculosis') 
    
    def parse(self , response):
        url_list = response.xpath("//div[@class='rslt']/p/a/@href").extract()
        for url in url_list:
            urll="https://www.ncbi.nlm.nih.gov" + url
            yield Request(urll,callback=self.parse_item,dont_filter=True)
        
        for i in range(2,276):
            next_page = self.driver.find_element_by_xpath("//a[@class='active page_link next']")  
            next_page.click()
            sleepTime = random.randint(1,3)
            time.sleep(sleepTime)
            links = self.driver.find_elements_by_xpath("//div[contains(@class,'rslt')]/p/a")
            for link in links:
                value = link.get_attribute("href")
                yield Request(value,callback=self.parse_item,dont_filter=True)
        
   
    def parse_item(self , response):
        print ("---------")
        title = response.xpath("//div[@id='summary']/h1/text()").extract()  
        href = response.xpath("//div[@id='summary']/dl/dd[3]/a/@href").extract()
        for i in href:
            href1 = "https://www.ncbi.nlm.nih.gov"+ i 
            yield Request(href1, meta={'title':title}, callback=self.parse_bio,dont_filter=True)
        
        
    def parse_bio(self , response):
        print ("++++++++++")
        try:
            bioUrl = response.url.split("/")[-2]
            #print (bioUrl) 
            sampleName = []
            strain = []
            geoLocation = []
            isoSource = []
            title = []
            bioSample = []
            
            title = response.meta["title"]
            print(title)
            bioSample = bioUrl
            sampleName = response.xpath("//table[@class='docsum']//th[contains(text(),'sample name')]/following-sibling::*[1]/text()").extract()
            strain = response.xpath("//table[@class='docsum']//th[contains(text(),'strain')]/following-sibling::*[1]/text()").extract()
            geoLocation = response.xpath("//table[@class='docsum']//th[contains(text(),'geographic location')]/following-sibling::*[1]/a/text()").extract()
            isoSource = response.xpath("//table[@class='docsum']//th[contains(text(),'isolation source')]/following-sibling::*[1]/text()").extract()
            
            if len(title) == 0:
                title = "*"
            if len(bioSample) == 0:
                bioSample = "*"
            if len(sampleName) == 0:
                sampleName = "*"
            if len(strain) == 0:
                strain = "*"
            if len(geoLocation) == 0:
                geoLocation = "*"
            if len(isoSource) == 0:
                isoSource = "*"
            
            print ("-------wait-------")
            items = BioItem()  
            items['title'] = title
            items['bioSample'] = bioSample
            items['sampleName'] = sampleName
            items['strain'] = strain
            items['geoLocation'] = geoLocation
            items['isoSource'] = isoSource
            self.num = self.num + 1
            print (self.num)
            yield items
            
            #position = self.bioSamples.index(bioUrl)
            #print (position)
            #list.insert(index, obj)
            #self.sampleNames.insert(position,sampleName[0])
            #self.geoLocations.insert(position,geoLocation[0])
            #self.isoSources.insert(position,isoSource[0])
            #self.strains.insert(position,strain[0])
            #self.num = self.num + 1 
        except:
            print("can't get it !!!!")
    
        

        