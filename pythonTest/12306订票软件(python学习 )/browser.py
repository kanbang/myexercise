#!/usr/bin/env python
#-*- coding:utf-8 -*-

import os.path, collections, time
from PyQt4 import QtGui, QtCore, QtWebKit, QtNetwork

allowedExtensions = ['.html', '.css', '.js', '.xhtml', '.dhtml']


class CookieJar(QtNetwork.QNetworkCookieJar):
    def __init__(self, ctr):
        self.ctr = ctr
        self.init = False
        self.cookieStorage = []
        QtNetwork.QNetworkCookieJar.__init__(self)
        self.time = time.time()
        self.updateCookie()
        self.initCookie()
        self.init = True

    def initCookie(self):
        self.setAllCookies([QtNetwork.QNetworkCookie.parseCookies(c)[0] for c in self.cookieStorage])

    def readCookie(self):
        return [c.toRawForm().data().decode() for c in self.allCookies()]


    def saveCookie(self):
        self.cookieStorage = [c.toRawForm().data().decode() for c in self.allCookies()]

    def getSyncCookie(self):
        return self.ctr.getSyncCookie()

    def updateCookie(self):
        if time.time() - self.time < 5 and self.init is True:
            return

        syncCookie = self.getSyncCookie()
        selfCookieMap = {v.split(';')[0].split('=')[0]: v for v in
                         [c.toRawForm().data().decode() for c in self.allCookies()] if v is not None}

        selfCookieMap.update(syncCookie)
        self.cookieStorage = selfCookieMap.values()
        self.initCookie()
        self.time = time.time()


class NetworkAccessManager(QtNetwork.QNetworkAccessManager):
    def __init__(self, ctr, allowedExtensions, cache_size=100):
        self.ctr = ctr
        QtNetwork.QNetworkAccessManager.__init__(self)

        self.cookie = CookieJar(self)
        self.setCookieJar(self.cookie)

        # initialize the manager cache
        cache = QtNetwork.QNetworkDiskCache()
        #QDesktopServices.storageLocation(QDesktopServices.CacheLocation)
        cache.setCacheDirectory('webCache')

        # need to convert cache value to bytes
        cache.setMaximumCacheSize(cache_size * 1024 * 1024)
        self.setCache(cache)

        self.allowedExtensions = allowedExtensions

        self.connect(self, QtCore.SIGNAL('sslErrors(QNetworkReply*, const QList<QSslError>&)'), self.sslErrors)

    def sslErrors(self, reply, errors):
        '''
        requiredCert = reply.sslConfiguration().peerCertificate()
        defaultConfig = QtNetwork.QSslConfiguration.defaultConfiguration()
        caList = defaultConfig.caCertificates()
        caList.append(requiredCert)
        defaultConfig.setCaCertificates(caList)
        QtNetwork.QSslConfiguration.setDefaultConfiguration(defaultConfig)
        reply.setSslConfiguration(defaultConfig)'''
        reply.ignoreSslErrors()

    def createRequest(self, operation, request, data):
        self.cookie.updateCookie()

        if data is not None:
            print('******')
            print(data)

        #operation == self.GetOperation:
        request.setAttribute(QtNetwork.QNetworkRequest.CacheLoadControlAttribute, QtNetwork.QNetworkRequest.PreferCache)

        request = self.bookTicket(request)

        '''
        for i in (dir(request)):
            print(i)
    
        rawHeaderList = request.rawHeaderList()
        for i in rawHeaderList:
            print(request.rawHeader(i))
        '''

        reply = QtNetwork.QNetworkAccessManager.createRequest(self, operation, request, data)
        reply.error.connect(self.catchError)

        #reply.downloadProgress.connect(self.progress)
        reply.finished.connect(self.finished)

        return reply

    def finished(self):
        self.ctr.loadFinished()

    def bookTicket(self, request):

        if (request.url().toString().startswith(
                'https://dynamic.12306.cn/otsweb/order/querySingleAction.do?method=submutOrderRequest')
            or request.url().toString().startswith('http://127.0.0.1/env.php')):
            print('match')
            request.setRawHeader('Host', 'dynamic.12306.cn')
            request.setRawHeader('Accept', 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8')

            request.setRawHeader('Referer', 'https://dynamic.12306.cn/otsweb/order/querySingleAction.do?method=init')
            request.setRawHeader('User-Agent', 'Mozilla/5.0 (MSIE 9.0 Windows NT 6.1 Trident/5.0)')
            request.setRawHeader('Connection', 'keep-alive')
            request.setRawHeader('Content-Type', 'application/x-www-form-urlencoded')
        '''
(Request-Line)	POST /otsweb/order/querySingleAction.do?method=submutOrderRequest HTTP/1.1
Host	dynamic.12306.cn
User-Agent	Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:21.0) Gecko/20100101 Firefox/21.0
Accept	text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
Accept-Language	zh-cn,zh;q=0.8,en-us;q=0.5,en;q=0.3
Accept-Encoding	gzip, deflate
Referer	https://dynamic.12306.cn/otsweb/order/querySingleAction.do?method=init
Cookie	JSESSIONID=22FF47CF0FFEAC7FBD65DB984C7C836F; BIGipServerotsweb=2446590218.48160.0000
Connection	keep-alive
Content-Type	application/x-www-form-urlencoded
Content-Length	737

        '''
        return request

    def isForbidden(self, request):
        extensions = os.path.splitext(request.url().toString())

        if extensions is not None and extensions[1].lower() in self.allowedExtensions:
            return True
        else:
            return False


    def catchError(self, eid):
        if eid not in (301, ):
            print('Error:', eid, self.sender().url().toString())


    def readCookie(self):
        self.cookie.readCookie()


    def closeEvent(self):
        self.cookie.saveCookie()


    def getSyncCookie(self):
        return self.ctr.getSyncCookie()


class WebPage(QtWebKit.QWebPage):
    def __init__(self):
        QtWebKit.QWebPage.__init__(self)
        self.userAgent = 'Mozilla/5.0 (MSIE 9.0 Windows NT 6.1 Trident/5.0)'

    def userAgentForUrl(self, url):
        return self.userAgent

    def javaScriptAlert(self, frame, message):
        #pass
        print('Alert:', message)

    def javaScriptConsoleMessage(self, message, line_number, source_id):
        pass
        #print('Console:', message, line_number, source_id)


class WebView(QtWebKit.QWebView):
    _windows = set()

    def __init__(self, ctr, parent=None):

        '''
        connect(ui->webView,SIGNAL(linkClicked(QUrl)),this,SLOT(lick(QUrl)));//连接信号和槽
ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);//当一个链接被激活
ui.webView->page()->mainFrame()->evaluateJavaScript("Test()");
        '''
        super(WebView, self).__init__(parent)
        self.ctr = ctr

        self.settings().setAttribute(QtWebKit.QWebSettings.DeveloperExtrasEnabled, True)
        self.settings().setAttribute(QtWebKit.QWebSettings.JavascriptEnabled, True)
        self.settings().setAttribute(QtWebKit.QWebSettings.JavascriptCanOpenWindows, True)
        self.settings().setAttribute(QtWebKit.QWebSettings.PluginsEnabled, True)

        webPage = WebPage()
        self.manager = NetworkAccessManager(self, allowedExtensions)
        webPage.setNetworkAccessManager(self.manager)
        self.setPage(webPage)
        self.connect(self, QtCore.SIGNAL('linkClicked(QUrl)'), lambda x: self.require(x))
        self.page().setLinkDelegationPolicy(QtWebKit.QWebPage.DelegateAllLinks)

        #self.require('http://127.0.0.1/env.php')
        #self.load(QtCore.QUrl('http://www.12306.cn/mormhweb/'))

    def loadFinished(self):
        self.page().mainFrame().evaluateJavaScript("test()");
        print('loadFinished')

    def closeEvent(self, event):
        self.manager.closeEvent()
        #print(self.manager.readCookie())
        self._removeWindow(self)
        event.accept()

    def createWindow(self, webWindowType):
        window = self.newWindow()
        if webWindowType == QtWebKit.QWebPage.WebModalDialog:
            window.setWindowModality(QtCore.Qt.ApplicationModal)
        window.show()
        return window

    def getSyncCookie(self):
        return self.ctr.getSyncCookie()

    def require(self, url):
        self.load(QtCore.QUrl(url))

    def post(self, data):
        if data is None or len(data) < 0:
            return
        formData = ''
        for i, v in data.items():
            if not i.lower() == 'url':
                formData += '<input type="text" name="%s" value="%s" />' % (i, v)

        form = '<form action="%s" method="post" name="postForm">%s<input type="submit" value="submit"></form>' % (
            data['url'], formData)
        #script = '<script>window.onload=function(){document.postForm.submit();}</script>'
        script = '<script>function test(){alert("hello world")}</script>'
        html = '<html><body>%s%s</body></html>' % (form, script)

        self.setHtml(html)

    def bookTicket(self, ticketData, condition):

        url = 'http://127.0.0.1/env.php'
        #url = 'https://dynamic.12306.cn/otsweb/order/querySingleAction.do?method=submutOrderRequest'

        data = ticketData.split("#")
        query = collections.OrderedDict()

        query['station_train_code'] = data[0]
        query['train_date'] = condition["orderRequest.train_date"]
        query['seattype_num'] = condition['seatTypeAndNum']
        query['from_station_telecode'] = data[4]
        query['to_station_telecode'] = data[5]
        query['include_student'] = condition['includeStudent']
        query['from_station_telecode_name'] = condition['orderRequest.from_station_name']
        query['to_station_telecode_name'] = condition['orderRequest.to_station_name']
        query['round_train_date'] = condition["roundTrainDate"]
        query['round_start_time_str'] = condition['roundStartTimeStr']
        query['single_round_type'] = condition['singleRoundType']
        query['train_pass_type'] = condition['trainPassType']
        query['train_class_arr'] = condition['trainClassArr']
        query['start_time_arr'] = condition['orderRequest.start_time_str']
        query['lishi'] = data[1]
        query['train_start_time'] = data[2]
        query['trainno4'] = data[3]
        query['arrive_time'] = data[6]
        query['from_station_name'] = data[7]
        query['to_station_name'] = data[8]
        query['from_station_no'] = data[9]
        query['to_station_no'] = data[10]
        query['ypInfoDetail'] = data[11]
        query['mmStr'] = data[12]
        query['locationCode'] = data[13]
        query['url'] = url

        self.post(query)


class SimpleBrowsers(QtGui.QDialog):
    def __init__(self, ctr, parent=None):
        super(SimpleBrowsers, self).__init__(parent)
        self.ctr = ctr
        self.webView = WebView(self)

        page = self.webView.page()
        #page.settings().setAttribute(QtWebKit.QWebSettings.JavascriptEnabled, False)
        page.settings().setAttribute(QtWebKit.QWebSettings.PluginsEnabled, True)
        page.settings().setAttribute(QtWebKit.QWebSettings.DeveloperExtrasEnabled, True)
        self.webInspector = QtWebKit.QWebInspector(self)
        self.webInspector.setPage(page)

        shortcut = QtGui.QShortcut(self)
        shortcut.setKey(QtCore.Qt.Key_F12)
        shortcut.activated.connect(self.toggleInspector)
        self.webInspector.setVisible(False)

        self.splitter = QtGui.QSplitter(self)
        self.splitter.setOrientation(QtCore.Qt.Vertical)

        self.verticalLayout = QtGui.QVBoxLayout()
        self.verticalLayout.addWidget(self.splitter)

        self.splitter.addWidget(self.webView)
        self.splitter.addWidget(self.webInspector)

        self.setLayout(self.verticalLayout)

    def closeEvent(self, event):
        self.webView.closeEvent(event)
        event.accept()

    def getSyncCookie(self):
        return self.ctr.getSyncCookie()

    def bookTicket(self, ticketData, condition):
        self.webView.bookTicket(ticketData, condition)

    def post(self, data):
        self.webView.post(data)

    def toggleInspector(self):
        self.webInspector.setVisible(not self.webInspector.isVisible())


class Ctr:
    def __init__(self):
        self.ticketData = 'K528#05:45#08:00#630000K5280C#GZQ#HYQ#13:45#广州#衡阳#01#05#1*****30124*****00081*****01023*****0028#A86124DD6525AE6502C2860A53F45662115F625406DEA2133AEA577F#Q6'
        self.condition = {'trainPassType': 'QB', 'orderRequest.to_station_name': '衡阳', 'orderRequest.trainCodeText': '',
                          'orderRequest.from_station_telecode': 'GZQ', 'roundStartTimeStr': '00:00--24:00',
                          'orderRequest.from_station_name': '广州', 'singleRoundType': 1,
                          'orderRequest.start_time_str': '00:00--24:00', 'trainClassArr': 'QB#D#Z#T#K#QT#',
                          'orderRequest.to_station_telecode': 'HYQ', 'roundTrainDate': '2013-06-27',
                          'orderRequest.train_date': '2013-06-27', 'seatTypeAndNum': '', 'includeStudent': '00'}


    '''
{'station_train_code':	'G101',
'train_date	:'2013-06-26',
'seattype_num':'',
'from_station_telecode'	:'VNP',
'to_station_telecode':	'AOH',
'include_student'	:'00',
'from_station_telecode_name':	'åäº¬',
'to_station_telecode_name'	:'ä¸æµ·',
'round_train_date'	:'2013-06-26',
'round_start_time_str'	:'00:00--24:00',
'single_round_type'	:'1',
'train_pass_type':'QB',
'train_class_arr':	'QB#D#Z#T#K#QT#',
'start_time_str':	'00:00--24:00',
'lishi'	:'05:23',
'train_start_time':	'07:00',
'trainno4'	:'240000G10102',
'arrive_time'	:'12:23',
'from_station_name' :	'åäº¬å',
'to_station_name'	:'ä¸æµ·è¹æ¡¥',
'from_station_no'	:'01',
'to_station_no'	:'08',
'ypInfoDetail'	:'O*****0216M*****01219*****0021',
'mmStr'	:'0C18275B4E6471D2D28186C83D976EDB16A862912EA6B33F9A4775D0',
'locationCode'	:'P3'}
       '''

    def getSyncCookie(self):
        return {'JSESSIONID': 'JSESSIONID=6538C6EA7DB3ECFC1D3EE8BAF6301896; domain=dynamic.12306.cn; path=/otsweb',
                'BIGipServerotsweb': 'BIGipServerotsweb=2111045898.62495.0000; domain=dynamic.12306.cn; path=/'}


'''
if __name__ == '__main__':
    import sys

    app = QtGui.QApplication(sys.argv)
    c = Ctr()
    b = SimpleBrowsers(c)
    b.bookTicket(c.ticketData, c.condition)
    b.show()
    app.exec_()'''