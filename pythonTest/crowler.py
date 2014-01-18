#-*- coding:utf-8 -*-

import urllib2
from BeautifulSoup import BeautifulSoup

headers = {'User-Agent':'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.4 (KHTML, like Gecko) Chrome/22.0.1229.94 Safari/537.4'}

def getMainPageScoreAndUrl(url):
    #print url
    req = urllib2.Request( url = url, headers = headers)
    page = urllib2.urlopen(req).read()
    soup = BeautifulSoup(page)
    #print soup.originalEncoding
    #print soup
    datas=[]
    for tag in soup.findAll('span', {'class':'play-icon'}):
        ptag=tag.parent
        href='http://www.senanb.com/'+ptag['href']

        ptag=ptag.parent
        ptag=ptag.find('strong', {'class':'ratbar-num'})
        if ptag == None: continue

        score=float(ptag.string)
        #score=ptag.contents[0]
        datas.append((score,href))
    soup.close()
    return datas

def filterMainPageScores(datas, minScore, maxScore):
    newDatas=[]
    for data in datas:
        if data[0] >= minScore and data[0] <= maxScore:
            newDatas.append(data)
    return newDatas

def getChildPageScoreAndNum(url):
    req = urllib2.Request( url = url, headers = headers)
    page = urllib2.urlopen(req).read()
    soup = BeautifulSoup(page)
    tag=soup.find('span', {'class':'Goldnum'})
    if tag==None:
        return None

    score=float(tag.string)
    tag=tag.nextSibling.find('span',{'class':'Golder'})
    if tag==None:
        return None

    num=int(tag.string)
    soup.close()
    return (score,num)

def filterChildPageScores(datas, minScore, maxScore):
    newDatas=[]
    for data in datas:
        sn=getChildPageScoreAndNum(data[1])
        if sn==None:continue
        if sn[0]>= minScore and sn[0]<= maxScore:
            newDatas.append((data[0],data[1],sn[0],sn[1]))
    return newDatas

def printChildPageScoreAndUrl(datas):
    for data in datas:
        print 'Old:{0} New:{2} Num:{3} url:{1}'.format(data[0],data[1],data[2],data[3])

def test_crowler():
    #url="http://www.senanb.com/vlist/2/index-{0}.html"
    url="http://www.senanb.com/vlist/4/index-{0}.html"
    for i in range(2,60):
        datas=getMainPageScoreAndUrl(url.format(i))
        if len(datas)==0:continue

        datas=filterMainPageScores(datas,8.0,10.0)
        if len(datas)==0:continue
        
        #print url.format(i)
        datas=filterChildPageScores(datas,8.3,10.0)
        printChildPageScoreAndUrl(datas)

if __name__=='__main__':
    test_crowler()
