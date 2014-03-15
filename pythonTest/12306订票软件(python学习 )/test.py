import urllib
import collections
import http.cookiejar
import urllib.request
import random, sys
import time, stringHander

import bs


class Core():
    def __init__(self):

        self.setCookies()
        self.setOpener()

    def setCookies(self):

        """ 设置cookie"""
        self.cookieFile = "./cookies.txt"
        self.cookies = http.cookiejar.MozillaCookieJar(self.cookieFile)
        #cookies = http.cookiejar.CookieJar()

        try:
            """加载已存在的cookie，尝试此cookie是否还有效"""
            self.cookies.load(ignore_discard=True, ignore_expires=True)
        except Exception:
            """加载失败，说明从未登录过，需创建一个cookie kong 文件"""
            self.saveCookie()
            #self.cookies.save(self.cookieFile, ignore_discard=True, ignore_expires=True)

        """将cookie带入到open中"""
        #opener =  urllib.request.build_opener(urllib.request.HTTPCookieProcessor(self.cookies))
        self.setOpener()
        """"loginprofile"""
        url = "https://kyfw.12306.cn/otn/login/init"
        response = self.opener.open(urllib.request.Request(url))
        response.read()
        self.saveCookie()
        #self.cookies.save(self.cookieFile, ignore_discard=True, ignore_expires=True)

    def resetCookie(self):
        self.cookies.clear(domain='kyfw.12306.cn')
        self.saveCookie()
        self.setCookies()

    def saveCookie(self):
        self.cookies.save(self.cookieFile, ignore_discard=True, ignore_expires=True)


    def readAllCookie(self):
        cookieMap = {}
        if self.cookies is not None:
            for cookie in self.cookies:
                one = cookie.name + '=' + str(cookie.value) + (
                    '; expires=' + str(cookie.expires) if cookie.expires is not None else '' ) + '; domain=' + str(
                    cookie.domain) + '; path=' + str(cookie.path)
                cookieMap.update({cookie.name: one})
        return cookieMap

    def updateCookie(self):
        self.saveCookie()
        self.cookies.load(ignore_discard=True, ignore_expires=True)
        self.setOpener()

    def setOpener(self):
        self.opener = urllib.request.build_opener(urllib.request.HTTPCookieProcessor(self.cookies))


    def getLoginRand(self):
        loginRandUrl = 'https://dynamic.12306.cn/otsweb/loginAction.do?method=loginAysnSuggest&_=1362754489477'
        loginRandReq = urllib.request.Request(loginRandUrl)
        loginRand = self.opener.open(loginRandReq).read()
        json = eval(loginRand, type('Dummy', (dict,), dict(__getitem__=lambda s, n: n))())
        self.updateCookie()
        return json['loginRand']

    def keepAlive(self):
        keepAliveUrl = 'https://dynamic.12306.cn/otsweb/loginAction.do?method=loginAysnSuggest&_=1362754489477'
        req = urllib.request.Request(keepAliveUrl)
        loginRand = self.opener.open(req).read()
        self.updateCookie()


    def getRandCodeImage(self):

        while True:
            randCodeUrl = 'https://kyfw.12306.cn/otn/passcodeNew/getPassCodeNew?module=login&rand=sjrand&' + str(
                random.randint(1, 800))
            req = urllib.request.Request(randCodeUrl)
            req.add_header('Host', 'kyfw.12306.cn')
            req.add_header('User-Agent', 'Mozilla/5.0 (MSIE 9.0 Windows NT 6.1 Trident/5.0)')
            req.add_header('Referer', 'https://kyfw.12306.cn/otn/login/init')
            req.add_header('Connection', 'keep-alive')
            randCode = self.opener.open(req).read()

            if randCode is None or len(randCode) == 0:
                self.resetCookie()
                time.sleep(3)
            else:
                break

        file = 'code.webp'
        img = open(file, "wb")
        img.write(randCode)
        img.close()
        return file

    def checkLoginCodeValidate(self, code):
        url = 'https://kyfw.12306.cn/otn/passcodeNew/checkRandCodeAnsyn'

        postData = collections.OrderedDict()
        postData['randCode'] = code
        postData['rand'] = 'sjrand'
        print(urllib.parse.urlencode(postData).encode())
        req = urllib.request.Request(url, urllib.parse.urlencode(postData).encode())
        req.add_header('Host', 'kyfw.12306.cn')
        req.add_header('User-Agent', 'Mozilla/5.0 (MSIE 9.0 Windows NT 6.1 Trident/5.0)')
        req.add_header('Referer', 'https://kyfw.12306.cn/otn/login/init')
        req.add_header('Connection', 'keep-alive')
        req.add_header('X-Requested-With', 'XMLHttpRequest')
        req.add_header('Content-Type', 'application/x-www-form-urlencoded; charset=UTF-8')
        response = self.opener.open(req)

        #{"validateMessagesShowId":"_validatorMessage","status":true,"httpstatus":200,"data":"N","messages":[],"validateMessages":{}}

        return response.read().decode()

    def loginAyncSuggest(self, user, password, randCode):

        url = 'https://kyfw.12306.cn/otn/login/loginAysnSuggest'
        postData = collections.OrderedDict()

        postData['loginUserDTO.user_name'] = user
        postData['userDTO.password'] = password
        postData['randCode'] = randCode
        req = urllib.request.Request(url, urllib.parse.urlencode(postData).encode())
        req.add_header('Host', 'kyfw.12306.cn')
        req.add_header('User-Agent', 'Mozilla/5.0 (MSIE 9.0 Windows NT 6.1 Trident/5.0)')
        req.add_header('Referer', 'https://kyfw.12306.cn/otn/login/init')
        req.add_header('Connection', 'keep-alive')
        req.add_header('X-Requested-With', 'XMLHttpRequest')
        req.add_header('Content-Type', 'application/x-www-form-urlencoded; charset=UTF-8')

        response = self.opener.open(req)
        #{"validateMessagesShowId":"_validatorMessage","status":true,"httpstatus":200,"data":{"loginCheck":"Y"},"messages":[],"validateMessages":{}}

        return response.read().decode()

    def login(self):

        loginUrl = "https://kyfw.12306.cn/otn/login/userLogin"

        loginData = urllib.parse.urlencode({'_json_att': ''})
        loginData = loginData.encode()
        req = urllib.request.Request(loginUrl, loginData)
        req.add_header('Referer', 'https://kyfw.12306.cn/otn/login/init')
        req.add_header('User-Agent', 'Mozilla/5.0 (MSIE 9.0 Windows NT 6.1 Trident/5.0)')
        req.add_header('Content-Type', 'application/x-www-form-urlencoded')
        self.opener.open(req)

        def checkLoginSuccess():
            url = 'https://kyfw.12306.cn/otn/modifyUser/initQueryUserInfo'
            req = urllib.request.Request(url, urllib.parse.urlencode({'_json_att': ''}))
            req.add_header('Referer', 'https://kyfw.12306.cn/otn/userSecurity/init')
            req.add_header('User-Agent', 'Mozilla/5.0 (MSIE 9.0 Windows NT 6.1 Trident/5.0)')
            req.add_header('Content-Type', 'application/x-www-form-urlencoded')
            req.add_header('Host', 'kyfw.12306.cn')
            return self.opener.open(req).read()

        return self.checkLoginSuccess()

    def checkLoginSuccess(self):
        url = 'https://kyfw.12306.cn/otn/modifyUser/initQueryUserInfo'
        req = urllib.request.Request(url, urllib.parse.urlencode({'_json_att': ''}).encode())
        req.add_header('Referer', 'https://kyfw.12306.cn/otn/userSecurity/init')
        req.add_header('User-Agent', 'Mozilla/5.0 (MSIE 9.0 Windows NT 6.1 Trident/5.0)')
        req.add_header('Content-Type', 'application/x-www-form-urlencoded')
        req.add_header('Host', 'kyfw.12306.cn')
        return bs.Bs.checkLoginSuccess(self.opener.open(req).read(), 'xuzhixiong_909090')

    def getPersonalContacts(self, pageIndex=0):
        getPersonalContacts = 'https://dynamic.12306.cn/otsweb/passengerAction.do?method=getPagePassengerAll'
        data = urllib.parse.urlencode({"pageIndex": pageIndex, "pageSize": 7, 'passenger_name': ''})
        data = data.encode()
        req = urllib.request.Request(getPersonalContacts, data)
        req.add_header('Origin', 'https://dynamic.12306.cn')
        req.add_header('Referer',
                       'https://dynamic.12306.cn/otsweb/passengerAction.do?method=initUsualPassenger12306')
        req.add_header('User-Agent', 'Mozilla/5.0 (MSIE 9.0; Windows NT 6.1; Trident/5.0)')
        req.add_header('X-Requested-With', 'XMLHttpRequest')
        json = self.opener.open(req).read()
        self.updateCookie()
        contacts = eval(json, type('Dummy', (dict,), dict(__getitem__=lambda s, n: n))())
        return contacts

    ################     new ticket before order     #################
    def checkUser(self):
        url = 'https://kyfw.12306.cn/otn/login/checkUser'
        #{"validateMessagesShowId":"_validatorMessage","status":true,"httpstatus":200,"data":{"flag":true},"messages":[],"validateMessages":{}}
        data = collections.OrderedDict()
        data['_json_att'] = ''

        req = urllib.request.Request(url, urllib.parse.urlencode(data).encode())
        req.add_header('Host', 'kyfw.12306.cn')
        req.add_header('User-Agent',
                       'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.114 Safari/537.36')
        req.add_header('Referer', 'https://kyfw.12306.cn/otn/leftTicket/init')
        req.add_header('Content-Type', 'application/x-www-form-urlencoded; charset=UTF-8')
        req.add_header('X-Requested-With', 'XMLHttpRequest')
        response = self.opener.open(req)
        self.updateCookie()
        return stringHander.checkUser(response.read().decode())


    def submitOrderRequest(self, secretStr, train_date, back_train_date, purpose_codes, query_from_station_name,
                           query_to_station_name, tour_flag='dc'):
        url = 'https://kyfw.12306.cn/otn/leftTicket/submitOrderRequest'
        #{"validateMessagesShowId":"_validatorMessage","status":true,"httpstatus":200,"messages":[],"validateMessages":{}}
        '''
        secretStr=MjAxMy0xMi0zMSMwMCNLNTI4IzIwOjA5IzA4OjAwIzYzMDAwMEs1MjgwRCNHWlEjU05IIzA0OjA5I%2BW5v%2BW3niPkuIrmtbfljZcjMDEjMTkjMTAyMDEwMzMzMzQwNjU2MDAwMjAxMDIwMTAwMzk4MzAzNTUwMDAwMSNRNyMxMzg4NDE1MjY5MTg0IzE5MzE4RUExQzUwQ0ZENEQxQzM2M0U1MjIxMTUxMDA5OEZFODA5QTAxOTZFQTNDRjMwN0UwMjM2
        &train_date=2013-12-31&
        back_train_date=2013-12-30
        &tour_flag=dc&purpose_codes=ADULT&
        query_from_station_name=å¹¿å·&
        query_to_station_name=ä¸æµ·&undefined
        '''
        data = ['secretStr=' + secretStr,
                'train_date=' + train_date,
                'back_train_date=' + back_train_date,
                'tour_flag=' + tour_flag,
                'purpose_codes=' + purpose_codes,
                'query_from_station_name=' + query_from_station_name,
                'query_to_station_name=' + query_to_station_name,
                'undefined']

        req = urllib.request.Request(url, '&'.join(data).encode())
        req.add_header('Host', 'kyfw.12306.cn')
        req.add_header('User-Agent',
                       'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.114 Safari/537.36')
        req.add_header('Referer', 'https://kyfw.12306.cn/otn/leftTicket/init')
        req.add_header('Content-Type', 'application/x-www-form-urlencoded; charset=UTF-8')
        req.add_header('X-Requested-With', 'XMLHttpRequest')
        response = self.opener.open(req)
        self.updateCookie()
        return stringHander.submitOrderRequest(response.read().decode())

    ################     new ticket token    #################

    def getTekon(self):
        url = 'https://kyfw.12306.cn/otn/confirmPassenger/initDc'

        data = collections.OrderedDict()
        data['_json_att'] = ''

        req = urllib.request.Request(url, urllib.parse.urlencode(data).encode())
        req.add_header('Host', 'kyfw.12306.cn')
        req.add_header('User-Agent',
                       'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.114 Safari/537.36')
        req.add_header('Referer', 'https://kyfw.12306.cn/otn/leftTicket/init')
        req.add_header('Content-Type', 'application/x-www-form-urlencoded')
        response = self.opener.open(req)
        self.updateCookie()
        return stringHander.getTekon(response.read().decode())


    ################     new ticket oreder     #################
    def checkRandCodeAnsyn(self, code, token):
        url = 'https://kyfw.12306.cn/otn/passcodeNew/checkRandCodeAnsyn'
        #randCode=ka75&rand=randp&_json_att=&REPEAT_SUBMIT_TOKEN=2a8f974bd77816babf8c0a5a1f653422
        data = collections.OrderedDict()
        data['randCode'] = code
        data['rand'] = 'randp'
        data['_json_att'] = ''
        data['REPEAT_SUBMIT_TOKEN'] = token
        #{"validateMessagesShowId":"_validatorMessage","status":true,"httpstatus":200,"data":"Y","messages":[],"validateMessages":{}}

        req = urllib.request.Request(url, urllib.parse.urlencode(data).encode())
        req.add_header('Host', 'kyfw.12306.cn')
        req.add_header('User-Agent',
                       'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.114 Safari/537.36')
        req.add_header('Referer', 'https://kyfw.12306.cn/otn/confirmPassenger/initDc')
        req.add_header('Content-Type', 'application/x-www-form-urlencoded; charset=UTF-8')
        req.add_header('X-Requested-With', 'XMLHttpRequest')
        response = self.opener.open(req)
        self.updateCookie()
        return stringHander.checkRandCodeAnsyn(response.read().decode())

    def getPassengerTicketStr(self, seat, person):
        _list = []
        for i in person:
            _list.append(seat + ",0," + i['ticket_type'] + "," + i['name'] + "," + i['id_type'] + "," + i[
                'id_no'] + "," + '' + "," + "N")

        return '_'.join(_list)

    def getOldPassengers(self, person):
        _list = []
        for i in person:
            _list.append(i['name'] + "," + i['id_type'] + "," + i['id_no'] + "," + i['passenger_type'])

        return '_'.join(_list) + '_'


    def checkOrderInfo(self, randCode, token, passengerTicketStr, oldPassengerStr, cancel_flag=2,
                       tour_flag='dc', bed_level_order_num='000000000000000000000000000000'):
        url = 'https://kyfw.12306.cn/otn/confirmPassenger/checkOrderInfo'
        #{"validateMessagesShowId":"_validatorMessage","status":true,"httpstatus":200,"data":{"submitStatus":true},"messages":[],"validateMessages":{}}
        '''
        cancel_flag=2&
        bed_level_order_num=000000000000000000000000000000&
        passengerTicketStr=1%2C0%2C1%2C%E6%A1%8D%E5%A4%BA%E5%A4%BA%2C1%2C431121198907177432%2C13664654645%2CN&
        oldPassengerStr=%E6%A1%8D%E5%A4%BA%E5%A4%BA%2C1%2C431121198907177432%2C1_&
        tour_flag=dc&
        randCode=ka75&
        _json_att=&
        REPEAT_SUBMIT_TOKEN=2a8f974bd77816babf8c0a5a1f653422

        cancel_flag=2&bed_level_order_num=000000000000000000000000000000&
        passengerTicketStr=1,0,1,桍夺夺,1,431121198907177432,13664654645,N&
        oldPassengerStr=桍夺夺,1,431121198907177432,1_&
        tour_flag=dc&randCode=ka75&_json_att=&
        REPEAT_SUBMIT_TOKEN=2a8f974bd77816babf8c0a5a1f653422



        '''
        data = collections.OrderedDict()

        data['cancel_flag'] = cancel_flag
        data['bed_level_order_num'] = bed_level_order_num
        data['passengerTicketStr'] = passengerTicketStr
        data['oldPassengerStr'] = oldPassengerStr
        data['tour_flag'] = tour_flag
        data['randCode'] = randCode
        data['_json_att'] = ''
        data['REPEAT_SUBMIT_TOKEN'] = token
        print(data)
        req = urllib.request.Request(url, urllib.parse.urlencode(data).encode())
        req.add_header('Host', 'kyfw.12306.cn')
        req.add_header('User-Agent',
                       'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.114 Safari/537.36')
        req.add_header('Referer', 'https://kyfw.12306.cn/otn/confirmPassenger/initDc')
        req.add_header('Content-Type', 'application/x-www-form-urlencoded; charset=UTF-8')
        req.add_header('X-Requested-With', 'XMLHttpRequest')
        response = self.opener.open(req)
        self.updateCookie()
        print(response.read().decode())
        return stringHander.checkOrderInfo(response.read().decode())


    def getQueueCount(self, train_date, train_no, stationTrainCode, seatType, fromStationTelecode, toStationTelecode,
                      leftTicket, purpose_codes, token):
        url = 'https://kyfw.12306.cn/otn/confirmPassenger/getQueueCount'

        #{"validateMessagesShowId":"_validatorMessage","status":true,"httpstatus":200,"data":{"count":"50","ticket":"1020103333406560002010201003983035500001","op_2":"false","countT":"0","op_1":"true"},"messages":[],"validateMessages":{}}
        '''
        train_date=Tue+Dec+31+2013+00%3A00%3A00+GMT%2B0800+(CST)&
        train_no=630000K5280D&
        stationTrainCode=K528&
        seatType=1&fromStationTelecode=GZQ&toStationTelecode=SNH&
        leftTicket=1020103333406560002010201003983035500001&
        purpose_codes=00&_json_att=&
        REPEAT_SUBMIT_TOKEN=2a8f974bd77816babf8c0a5a1f653422

        train_date=Tue+Dec+31+2013+00:00:00+GMT+0800+(CST)&train_no=630000K5280D&
        stationTrainCode=K528&seatType=1&fromStationTelecode=GZQ&
        toStationTelecode=SNH&leftTicket=1020103333406560002010201003983035500001&
        purpose_codes=00&_json_att=&REPEAT_SUBMIT_TOKEN=2a8f974bd77816babf8c0a5a1f653422
        '''
        data = collections.OrderedDict()

        data['train_date'] = time.strftime('%a %b %d %Y %H:%M:%S GMT 0800 (CST)', time.strptime(train_date, '%Y%m%d'))
        data['train_no'] = train_no
        data['stationTrainCode'] = stationTrainCode
        data['seatType'] = seatType
        data['fromStationTelecode'] = fromStationTelecode
        data['toStationTelecode'] = toStationTelecode
        data['leftTicket'] = leftTicket
        data['purpose_codes'] = purpose_codes
        data['_json_att	'] = ''
        data['REPEAT_SUBMIT_TOKEN'] = token

        req = urllib.request.Request(url, urllib.parse.urlencode(data).encode())
        req.add_header('Host', 'kyfw.12306.cn')
        req.add_header('User-Agent',
                       'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.114 Safari/537.36')
        req.add_header('Referer', 'https://kyfw.12306.cn/otn/confirmPassenger/initDc')
        req.add_header('Content-Type', 'application/x-www-form-urlencoded; charset=UTF-8')
        req.add_header('X-Requested-With', 'XMLHttpRequest')
        response = self.opener.open(req)
        self.updateCookie()
        return stringHander.checkOrderInfo(response.read().decode())


    def confirmSingleForQueue(self, passengerTicketStr, oldPassengerStr, randCode, purpose_codes, key_check_isChange,
                              leftTicketStr, train_location, token):
        url = 'https://kyfw.12306.cn/otn/confirmPassenger/confirmSingleForQueue'
        #{"validateMessagesShowId":"_validatorMessage","status":true,"httpstatus":200,"data":{"submitStatus":true},"messages":[],"validateMessages":{}}
        '''
        passengerTicketStr=1%2C0%2C1%2C%E6%A1%8D%E5%A4%BA%E5%A4%BA%2C1%2C431121198907177432%2C13664654645%2CN&
        oldPassengerStr=%E6%A1%8D%E5%A4%BA%E5%A4%BA%2C1%2C431121198907177432%2C1_&randCode=ka75&
        purpose_codes=00&key_check_isChange=DAA2E9A3B57113825F6A1747EB40D253572D03861557AA85B019FA2D&
        leftTicketStr=1020103333406560002010201003983035500001&
        train_location=Q7&_json_att=&REPEAT_SUBMIT_TOKEN=2a8f974bd77816babf8c0a5a1f653422

        passengerTicketStr=1,0,1,桍夺夺,1,431121198907177432,13664654645,N&
        oldPassengerStr=桍夺夺,1,431121198907177432,1_&randCode=ka75&
        purpose_codes=00&
        key_check_isChange=DAA2E9A3B57113825F6A1747EB40D253572D03861557AA85B019FA2D&
        leftTicketStr=1020103333406560002010201003983035500001&train_location=Q7&
        _json_att=&REPEAT_SUBMIT_TOKEN=2a8f974bd77816babf8c0a5a1f653422
        '''

        data = collections.OrderedDict()
        data['passengerTicketStr'] = passengerTicketStr
        data['oldPassengerStr'] = oldPassengerStr
        data['randCode'] = randCode
        data['purpose_codes'] = purpose_codes
        data['key_check_isChange'] = key_check_isChange
        data['leftTicketStr'] = leftTicketStr
        data['train_location'] = train_location
        data['_json_att'] = ''
        data['REPEAT_SUBMIT_TOKEN'] = token

        req = urllib.request.Request(url, urllib.parse.urlencode(data).encode())
        req.add_header('Host', 'kyfw.12306.cn')
        req.add_header('User-Agent',
                       'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.114 Safari/537.36')
        req.add_header('Referer', 'https://kyfw.12306.cn/otn/confirmPassenger/initDc')
        req.add_header('Content-Type', 'application/x-www-form-urlencoded; charset=UTF-8')
        req.add_header('X-Requested-With', 'XMLHttpRequest')
        response = self.opener.open(req)
        self.updateCookie()
        return stringHander.confirmSingleForQueue(response.read().decode())


    def waitCount(self, token):
        url = 'https://kyfw.12306.cn/otn/confirmPassenger/queryOrderWaitTime?random={random}&tourFlag={tourFlag}&_json_att=&REPEAT_SUBMIT_TOKEN={token}'
        url = url.format(random=time.time(), tourFlag='dc', token=token)
        #{"validateMessagesShowId":"_validatorMessage","status":true,"httpstatus":200,"data":{"queryOrderWaitTimeStatus":true,"count":0,"waitTime":-1
        # ,"requestId":5823401326154130780,"waitCount":0,"tourFlag":"dc","orderId":"E772191986"},"messages":[],"validateMessages":{}}

        req = urllib.request.Request(url)
        req.add_header('Host', 'kyfw.12306.cn')
        req.add_header('User-Agent',
                       'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.114 Safari/537.36')
        req.add_header('Referer', 'https://kyfw.12306.cn/otn/confirmPassenger/initDc')
        req.add_header('Content-Type', 'application/x-www-form-urlencoded; charset=UTF-8')
        req.add_header('X-Requested-With', 'XMLHttpRequest')
        response = self.opener.open(req)
        self.updateCookie()
        return stringHander.confirmSingleForQueue(response.read().decode())


    def resultOrderForDcQueue(self, order_no, token):
        url = 'https://kyfw.12306.cn/otn/confirmPassenger/resultOrderForDcQueue'
        #{"validateMessagesShowId":"_validatorMessage","status":true,"httpstatus":200,"data":{"submitStatus":true},"messages":[],"validateMessages":{}}
        data = collections.OrderedDict()

        data['orderSequence_no'] = order_no
        data['_json_att	'] = ''
        data['REPEAT_SUBMIT_TOKEN'] = token

        req = urllib.request.Request(url, urllib.parse.urlencode(data).encode())
        req.add_header('Host', 'kyfw.12306.cn')
        req.add_header('User-Agent',
                       'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.114 Safari/537.36')
        req.add_header('Referer', 'https://kyfw.12306.cn/otn/confirmPassenger/initDc')
        req.add_header('Content-Type', 'application/x-www-form-urlencoded; charset=UTF-8')
        req.add_header('X-Requested-With', 'XMLHttpRequest')
        response = self.opener.open(req)
        self.updateCookie()
        return stringHander.resultOrderForDcQueue(response.read().decode())

        ########################

    def getOrderImageCode(self):
        url = 'https://kyfw.12306.cn/otn/passcodeNew/getPassCodeNew?module=passenger&rand=randp&' + str(
            random.randint(1, 800))

        req = urllib.request.Request(url)
        req.add_header('Host', 'kyfw.12306.cn')
        req.add_header('User-Agent',
                       'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.114 Safari/537.36')
        req.add_header('Referer', 'https://kyfw.12306.cn/otn/login/init')

        file = 'order.webp'
        img = open(file, "wb")
        img.write(self.opener.open(req).read())
        img.close()
        return file

    def checkOrderImageCode(self, randCode, token):
        url = 'https://kyfw.12306.cn/otn/passcodeNew/checkRandCodeAnsyn'
        data = collections.OrderedDict()

        data['randCode'] = randCode
        data['rand'] = 'randp'
        data['_json_att'] = ''
        data['REPEAT_SUBMIT_TOKEN'] = token

        req = urllib.request.Request(url, urllib.parse.urlencode(data).encode())
        req.add_header('Host', 'kyfw.12306.cn')
        req.add_header('User-Agent',
                       'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.114 Safari/537.36')
        req.add_header('Referer', 'https://kyfw.12306.cn/otn/confirmPassenger/initDc')
        req.add_header('Content-Type', 'application/x-www-form-urlencoded; charset=UTF-8')
        req.add_header('X-Requested-With', 'XMLHttpRequest')
        response = self.opener.open(req)
        self.updateCookie()
        return stringHander.checkOrderImageCode(response.read().decode())


if __name__ == '__main__':
    core = Core()
    #core.getRandCodeImage()
    #print(core.checkLoginCodeValidate('s9xy'))
    #print(core.loginAyncSuggest('xuzhixiong_909090','city909124951','s9xy'))

    request = urllib.request.urlopen(
        'https://kyfw.12306.cn/otn/leftTicket/query?leftTicketDTO.train_date=2013-12-31&leftTicketDTO.from_station=GZQ&leftTicketDTO.to_station=QVQ&purpose_codes=ADULT')

    request = stringHander.trimTrains(request.read().decode())
    s = request[0]

    '''s = {
        "train_no": "63000K912607", "station_train_code": "K9126", "start_station_telecode": "GZQ",
        "start_station_name": "广州",
        "end_station_telecode": "AOQ", "end_station_name": "永州", "from_station_telecode": "GZQ",
        "from_station_name": "广州",
        "to_station_telecode": "QWQ", "to_station_name": "祁阳", "start_time": "23:35", "arrive_time": "07:31",
        "day_difference": "1", "train_class_name": "", "lishi": "07:56", "canWebBuy": "Y", "lishiValue": "476",
        "yp_info": "1008603563403050001410086003243015600000", "control_train_day": "20991231",
        "start_train_date": "20131231",
        "seat_feature": "W3431333", "yp_ex": "10401030", "train_seat_feature": "3", "seat_types": "1413",
        "location_code": "Q6",
        "from_station_no": "01", "to_station_no": "06", "control_day": 19, "sale_time": "1200", "is_support_card": "0",
        "gg_num": "--", "gr_num": "--", "qt_num": "--", "rw_num": "14", "rz_num": "--", "tz_num": "--", "wz_num": "有",
        "yb_num": "--", "yw_num": "无", "yz_num": "有", "ze_num": "--", "zy_num": "--", "swz_num": "--",
        "secretStr": "MjAxMy0xMi0zMSMwMCNLOTEyNiMwNzo1NiMyMzozNSM2MzAwMEs5MTI2MDcjR1pRI1FXUSMwNzozMSPlub%2Flt54j56WB6ZizIzAxIzA2IzEwMDg2MDM1NjM0MDMwNTAwMDE0MTAwODYwMDMyNDMwMTU2MDAwMDAjUTYjMTM4ODQ1NDY0NzQ1NSNGRkQ3RUEzQUE5QzdDOEZGNTg4MDM4NzlEQkFERTcwOEUzQkE5RUYwNEIzN0Q3QzJFMTQ5QzQzMQ%3D%3D"}
    '''
    person = {'黄丽娟': {'mobile_no': '15813314912', 'passenger_type': '1', 'passenger_id_no': '452623198603040343',
                      'passenger_type_name': '成人', 'passenger_id_type_name': '二代身份证', 'passenger_name': '黄丽娟',
                      'passenger_id_type_code': '1', 'isUserSelf': 'N'},
    }

    ticket_person = [{'name': v['passenger_name'], 'passenger_type': v['passenger_type'],
                      'ticket_type': v['passenger_type'],
                      'id_type': v['passenger_id_type_code'], 'id_no': v['passenger_id_no'],
                      'mobile_no': v['mobile_no']} for i, v in person.items()]

    seat = s['seat_types'][0]

    secretStr = s['secretStr']
    train_date = back_train_date = time.strftime('%Y-%m-%d', time.strptime(s['start_train_date'], '%Y%m%d'))
    purpose_codes = 'ADULT'
    query_from_station_name = s['from_station_name']
    query_to_station_name = s['to_station_name']

    code = 'gjmh'
    token = 'aa0b2bf58f830b376ed88692c8b4f559'

    if len(code) == 0:
        token = core.getTekon()
        core.getOrderImageCode()
        print('token====' + token)
        print('*******create order code ok********')
        sys.exit()
    else:
        if core.checkOrderImageCode(code, token):
            print('*******checkOrderImageCode ok********')
        else:
            print('*******checkOrderImageCode failed********')
            sys.exit()

    if core.checkUser():
        print('*******checkUser ok********')

    if core.submitOrderRequest(secretStr, train_date, back_train_date, purpose_codes, query_from_station_name,
                               query_to_station_name):
        print('*******submitOrderRequest ok ********')

    passengerTicketStr = core.getPassengerTicketStr(seat, ticket_person)

    oldPassengerStr = core.getOldPassengers(ticket_person)

    print('passengerTicketStr=====' + passengerTicketStr)

    print('oldPassengerStr====' + oldPassengerStr)

    if core.checkOrderInfo(code, token, passengerTicketStr, oldPassengerStr):
        print('*******checkOrderInfo ok ********')