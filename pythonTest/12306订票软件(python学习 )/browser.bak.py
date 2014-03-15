#!/usr/bin/env python
#-*- coding:utf-8 -*-

import os.path, collections
from PyQt4 import QtGui, QtCore, QtWebKit, QtNetwork

allowedExtensions = ['.html', '.css', '.js', '.xhtml', '.dhtml']


class CookieJar(QtNetwork.QNetworkCookieJar):
    def __init__(self, ctr):
        self.ctr = ctr
        self.cookieStorage = []
        QtNetwork.QNetworkCookieJar.__init__(self)
        self.updateCookie()
        self.initCookie()

    def initCookie(self):
        self.setAllCookies([QtNetwork.QNetworkCookie.parseCookies(c)[0] for c in self.cookieStorage])

    def readCookie(self):
        return [c.toRawForm().data().decode() for c in self.allCookies()]


    def saveCookie(self):
        self.cookieStorage = [c.toRawForm().data().decode() for c in self.allCookies()]

    def getSyncCookie(self):
        return self.ctr.getSyncCookie()

    def updateCookie(self):
        syncCookie = self.getSyncCookie()
        selfCookieMap = {v.split(';')[0].split('=')[0]: v for v in
                         [c.toRawForm().data().decode() for c in self.allCookies()] if v is not None}

        selfCookieMap.update(syncCookie)
        self.cookieStorage = selfCookieMap.values()
        self.initCookie()


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

        print('******')
        print(data)
        print('--------')

        #operation == self.GetOperation:
        request.setAttribute(QtNetwork.QNetworkRequest.CacheLoadControlAttribute, QtNetwork.QNetworkRequest.PreferCache)

        if (request.url().toString().startswith(
                'https://dynamic.12306.cn/otsweb/order/querySingleAction.do?method=submutOrderRequest')):
            print('match')
            request.setRawHeader('Host', 'dynamic.12306.cn')
            request.setRawHeader('Accept', 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8')

            request.setRawHeader('Referer', 'https://dynamic.12306.cn/otsweb/order/querySingleAction.do?method=init')
            request.setRawHeader('User-Agent', 'Mozilla/5.0 (MSIE 9.0 Windows NT 6.1 Trident/5.0)')
            request.setRawHeader('X-Requested-With', 'XMLHttpRequest')
            request.setRawHeader('Content-Type', 'application/x-www-form-urlencoded')

        '''
        for i in (dir(request)):
            print(i)
    
        rawHeaderList = request.rawHeaderList()
        for i in rawHeaderList:
            print(request.rawHeader(i))
        '''

        reply = QtNetwork.QNetworkAccessManager.createRequest(self, operation, request, data)
        reply.error.connect(self.catchError)

        return reply


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
        pass
        #print('Alert:', message)

    def javaScriptConsoleMessage(self, message, line_number, source_id):
        pass
        #print('Console:', message, line_number, source_id)


class WebView(QtWebKit.QWebView):
    _windows = set()

    def __init__(self, ctr, parent=None):

        '''
        connect(ui->webView,SIGNAL(linkClicked(QUrl)),this,SLOT(lick(QUrl)));//连接信号和槽
ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);//当一个链接被激活
        '''
        super(WebView, self).__init__(parent)
        self.ctr = ctr

        self.settings().setAttribute(QtWebKit.QWebSettings.JavascriptEnabled, True)
        self.settings().setAttribute(QtWebKit.QWebSettings.JavascriptCanOpenWindows, True)
        #self.settings().setAttribute(QtWebKit.QWebSettings.PluginsEnabled, True)
        #self.settings().setAttribute(QtWebKit.QWebSettings.DeveloperExtrasEnabled, True)

        webPage = WebPage()
        self.manager = NetworkAccessManager(self, allowedExtensions)
        webPage.setNetworkAccessManager(self.manager)
        self.setPage(webPage)
        self.connect(self, QtCore.SIGNAL('linkClicked(QUrl)'), lambda x: self.require(x))
        self.page().setLinkDelegationPolicy(QtWebKit.QWebPage.DelegateAllLinks)

        #self.require('http://127.0.0.1/env.php')

    @classmethod
    def _removeWindow(cls, window):
        if window in cls._windows:
            cls._windows.remove(window)

    @classmethod
    def newWindow(cls):
        window = cls()
        cls._windows.add(window)
        return window

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

        form = '<form action="%s" method="post" name="postForm">%s</form>' % (data['url'], formData)
        script = '<script>window.onload=function(){document.postForm.submit();}</script>'
        html = '<html><body>%s%s</body></html>' % (form, script)

        self.setHtml(html)

    def bookTicket(self, ticketData, condition):

        url = 'https://dynamic.12306.cn/otsweb/order/querySingleAction.do?method=submutOrderRequest'
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
        self.verticalLayout = QtGui.QVBoxLayout()
        self.verticalLayout.addWidget(self.webView)
        self.setLayout(self.verticalLayout)

    def closeEvent(self, event):
        self.webView.closeEvent(event)
        event.accept()

    def getSyncCookie(self):
        return self.ctr.getSyncCookie()

    def bookTicket(self, ticketData, condition):
        self.webView.bookTicket(ticketData, condition)

