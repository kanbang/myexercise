# -*- coding: utf-8 -*-
import urllib
import collections
import http.cookiejar
import urllib.request
import random
import time
import http.client
import stringHander
import bs


class Core():
    def __init__(self, ctr):
        self.ctr = ctr
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

        try:
            response = self.opener.open(urllib.request.Request('https://kyfw.12306.cn/otn/index/init'))
            response.read()
        except:
            pass

        self.saveCookie()
        #self.cookies.save(self.cookieFile, ignore_discard=True, ignore_expires=True)

    def resetCookie(self):
        self.cookies.clear(domain='kyfw.12306.cn')
        self.saveCookie()
        self.setCookies()

    def saveCookie(self):
        self.cookies.save(self.cookieFile, ignore_discard=True, ignore_expires=True)
        self.ctr.syncCookie(self.readAllCookie())

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

    def keepAlive(self):

        try:
            keepAliveUrl = 'https://kyfw.12306.cn/otn/leftTicket/init'
            req = urllib.request.Request(keepAliveUrl)
            self.opener.open(req).read()
            self.updateCookie()
        except:
            pass

    def getRandCodeImage(self):
        randCode = ''
        while True:
            randCodeUrl = 'https://kyfw.12306.cn/otn/passcodeNew/getPassCodeNew?module=login&rand=sjrand&' + str(
                random.randint(1, 800))
            req = urllib.request.Request(randCodeUrl)
            req.add_header('Host', 'kyfw.12306.cn')
            req.add_header('User-Agent', 'Mozilla/5.0 (MSIE 9.0 Windows NT 6.1 Trident/5.0)')
            req.add_header('Referer', 'https://kyfw.12306.cn/otn/login/init')
            req.add_header('Connection', 'keep-alive')

            try:
                randCode = self.opener.open(req).read()
            except:
                randCode = None

            if randCode is None or len(randCode) == 0:
                #self.resetCookie()
                time.sleep(0.1)
            else:
                break

        file = 'code.webp'
        img = open(file, "wb")
        img.write(randCode)
        img.close()
        return file

    def checkLoginCodeValidate(self, code):

        def run(self, code):
            try:
                url = 'https://kyfw.12306.cn/otn/passcodeNew/checkRandCodeAnsyn'
                data = collections.OrderedDict()
                data['randCode'] = code
                data['rand'] = 'sjrand'
                req = urllib.request.Request(url, urllib.parse.urlencode(data).encode())
                req.add_header('Host', 'kyfw.12306.cn')
                req.add_header('User-Agent',
                               'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.114 Safari/537.36')
                req.add_header('Referer', 'https://kyfw.12306.cn/otn/login/init')
                req.add_header('Connection', 'keep-alive')
                req.add_header('X-Requested-With', 'XMLHttpRequest')
                req.add_header('Content-Type', 'application/x-www-form-urlencoded; charset=UTF-8')
                response = self.opener.open(req)

                return {'s': True, 'r': response.read().decode()}
            except: 
                return {'s': False, 'r': ''}

        re = run(self, code)

        while not re['s']:
            re = run(self, code)

        return stringHander.checkLoginCodeValidate(re['r'])

    def loginAyncSuggest(self, user, password, randCode):
        #{"validateMessagesShowId":"_validatorMessage","status":true,"httpstatus":200,"data":{"loginCheck":"Y"},"messages":[],"validateMessages":{}}
        def run(self, user, password, randCode):
            try:
                url = 'https://kyfw.12306.cn/otn/login/loginAysnSuggest'
                postData = collections.OrderedDict()

                postData['loginUserDTO.user_name'] = user
                postData['userDTO.password'] = password
                postData['randCode'] = randCode
                req = urllib.request.Request(url, urllib.parse.urlencode(postData).encode())
                req.add_header('Host', 'kyfw.12306.cn')
                req.add_header('User-Agent',
                               'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.114 Safari/537.36')
                req.add_header('Referer', 'https://kyfw.12306.cn/otn/login/init')
                req.add_header('Connection', 'keep-alive')
                req.add_header('X-Requested-With', 'XMLHttpRequest')
                req.add_header('Content-Type', 'application/x-www-form-urlencoded; charset=UTF-8')
                response = self.opener.open(req)
                return {'s': True, 'r': response.read().decode()}
            except:
                return {'s': False, 'r': ''}

        re = run(self, user, password, randCode)

        while not re['s']:
            re = run(self, user, password, randCode)

        return stringHander.loginAyncSuggest(re['r'])

    def login(self, loginData):

        #data = {'username': username, 'password': password, 'remember': rememberMe, 'logInCode': imageCode}

        if self.checkLoginCodeValidate(loginData['logInCode']) \
            and self.loginAyncSuggest(loginData['username'], loginData['password'], loginData['logInCode']):
            try:
                url = "https://kyfw.12306.cn/otn/login/userLogin"
                req = urllib.request.Request(url, urllib.parse.urlencode({'_json_att': ''}).encode())
                req.add_header('Referer', 'https://kyfw.12306.cn/otn/login/init')
                req.add_header('User-Agent',
                               'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.114 Safari/537.36')
                req.add_header('Content-Type', 'application/x-www-form-urlencoded')
                self.opener.open(req)
            except:
                return False

            return self.checkLoginSuccess(loginData['username'])
        else:
            return False

    def checkLoginSuccess(self, user):

        def run(self):
            try:
                url = 'https://kyfw.12306.cn/otn/modifyUser/initQueryUserInfo'
                req = urllib.request.Request(url, urllib.parse.urlencode({'_json_att': ''}).encode())
                req.add_header('Referer', 'https://kyfw.12306.cn/otn/userSecurity/init')
                req.add_header('User-Agent',
                               'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.114 Safari/537.36')
                req.add_header('Content-Type', 'application/x-www-form-urlencoded')
                req.add_header('Host', 'kyfw.12306.cn')
                response = self.opener.open(req)
                return {'s': True, 'r': response.read().decode()}
            except:
                return {'s': False, 'r': ''}

        re = run(self)

        while not re['s']:
            re = run(self)

        return bs.Bs.checkLoginSuccess(re['r'], user)

    def getUnPayOrderTicket(self):

        try:
            url = 'https://kyfw.12306.cn/otn/queryOrder/queryMyOrderNoComplete'
            #{"validateMessagesShowId":"_validatorMessage","status":true,"httpstatus":200,"data":{"submitStatus":true},"messages":[],"validateMessages":{}}
            data = collections.OrderedDict()
            data['_json_att	'] = ''
            req = urllib.request.Request(url, urllib.parse.urlencode(data).encode())
            req.add_header('Host', 'kyfw.12306.cn')
            req.add_header('User-Agent',
                           'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.114 Safari/537.36')
            req.add_header('Referer', 'https://kyfw.12306.cn/otn/queryOrder/initNoComplete')
            req.add_header('Content-Type', 'application/x-www-form-urlencoded; charset=UTF-8')
            req.add_header('X-Requested-With', 'XMLHttpRequest')
            response = self.opener.open(req)
            self.updateCookie()
        except:
            return False

        return stringHander.getUnPayOrder(response.read().decode())


    def getTicketOrderRecorder(self, query):
        pass

    def getPersonalDetail(self):

        def run(self):
            try:
                url = 'https://kyfw.12306.cn/otn/modifyUser/initQueryUserInfo'
                req = urllib.request.Request(url)
                req.add_header('Host', 'kyfw.12306.cn')
                req.add_header('Referer', 'https://kyfw.12306.cn/otn/passengers/init')
                req.add_header('User-Agent',
                               'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.114 Safari/537.36')
                req.add_header('Content-Type', 'application/x-www-form-urlencoded')
                self.updateCookie()
                response = self.opener.open(req)
                return {'s': True, 'r': response.read().decode()}
            except:
                return {'s': False, 'r': ''}

        re = run(self)

        while not re['s']:
            re = run(self)

        return bs.Bs.getPersonalDetail(re['r'])


    def deletePersonalContact(self, query):

        def run(self, query):
            try:
                url = 'https://kyfw.12306.cn/otn/passengers/delete'
                personData = collections.OrderedDict()

                personData['passenger_name'] = query['passenger_name']
                personData['passenger_id_type_code'] = query['passenger_type']
                personData['passenger_id_no'] = query['passenger_id_no']
                personData['isUserSelf'] = 'N'

                req = urllib.request.Request(url, urllib.parse.urlencode(personData).encode())
                req.add_header('Host', 'kyfw.12306.cn')
                req.add_header('Referer', 'https://kyfw.12306.cn/otn/passengers/init')
                req.add_header('User-Agent',
                               'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.114 Safari/537.36')
                req.add_header('X-Requested-With', 'XMLHttpRequest')
                req.add_header('Content-Type', 'application/x-www-form-urlencoded; charset=UTF-8')
                response = self.opener.open(req)
                return {'s': True, 'r': response.read().decode()}
            except:
                return {'s': False, 'r': ''}

        re = run(self, query)

        while not re['s']:
            re = run(self, query)

        return stringHander.deletePersonalContact(re['r'])

    def addPersonalContact(self, query):
        def run(self, query):
            try:
                getPersonalContactsUrl = 'https://kyfw.12306.cn/otn/passengers/add'
                personData = collections.OrderedDict()
                personData['passenger_name'] = query['name']
                personData['sex_code'] = query['sex_code']
                personData['_birthDate'] = query['born_date']
                personData['country_code'] = query['country_code']
                personData['passenger_id_type_code'] = query['card_type']
                personData['passenger_id_no'] = query['card_no']
                personData['mobile_no'] = query['mobile_no']
                personData['phone_no'] = ''
                personData['email'] = ''
                personData['address'] = ''
                personData['postalcode'] = ''
                personData['passenger_type'] = 1
                personData['studentInfoDTO.province_code'] = 11
                personData['studentInfoDTO.school_code'] = ''
                personData['studentInfoDTO.school_name'] = '简码/汉字'
                personData['studentInfoDTO.department'] = ''
                personData['studentInfoDTO.school_class'] = ''
                personData['studentInfoDTO.student_no'] = ''
                personData['studentInfoDTO.school_system'] = 1
                personData['studentInfoDTO.enter_year'] = time.strftime("%Y")
                personData['studentInfoDTO.preference_card_no'] = ''
                personData['studentInfoDTO.preference_from_station_name'] = ''
                personData['studentInfoDTO.preference_from_station_code'] = ''
                personData['studentInfoDTO.preference_to_station_name'] = ''
                personData['studentInfoDTO.preference_to_station_code'] = ''

                personData = urllib.parse.urlencode(personData)
                personData = personData.encode()

                req = urllib.request.Request(getPersonalContactsUrl, personData)
                req.add_header('Host', 'kyfw.12306.cn')
                req.add_header('Referer', 'https://kyfw.12306.cn/otn/passengers/addInit')
                req.add_header('User-Agent',
                               'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.114 Safari/537.36')
                req.add_header('Content-Type', 'application/x-www-form-urlencoded; charset=UTF-8')
                req.add_header('X-Requested-With', 'XMLHttpRequest')
                response = self.opener.open(req)
                self.updateCookie()
                return {'s': True, 'r': response.read().decode()}
            except:
                return {'s': False, 'r': ''}

        re = run(self, query)

        while not re['s']:
            re = run(self, query)

        return stringHander.addPersonalContact(re['r'])


    def getPersonalContacts(self):
        def run(self):
            try:
                url = 'https://kyfw.12306.cn/otn/passengers/init'
                req = urllib.request.Request(url, urllib.parse.urlencode({'_json_att': ''}).encode())
                req.add_header('Host', 'kyfw.12306.cn')
                req.add_header('Referer', 'https://kyfw.12306.cn/otn/userSecurity/init')
                req.add_header('User-Agent',
                               'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.114 Safari/537.36')
                req.add_header('Content-Type', 'application/x-www-form-urlencoded')
                response = self.opener.open(req)
                self.updateCookie()
                return {'s': True, 'r': response.read().decode()}
            except:
                return {'s': False, 'r': ''}

        re = run(self)

        while not re['s']:
            re = run(self)

        return stringHander.getPersonalContacts(re['r'])

    def ticketQuery(self, query):
        def run(self, query):

            try:
                url = 'https://kyfw.12306.cn/otn/leftTicket/query?'
                queryDate = ['leftTicketDTO.train_date=' + str(query['orderRequest.train_date']),
                             'leftTicketDTO.from_station=' + str(query['orderRequest.from_station_telecode']),
                             'leftTicketDTO.to_station=' + str(query['orderRequest.to_station_telecode']),
                             'purpose_codes=' + str(query['includeStudent'])]

                response = urllib.request.urlopen(url + '&'.join(queryDate))
                return {'s': True, 'r': response.read().decode()}
            except:
                return {'s': False, 'r': ''}

        re = run(self, query)

        while not re['s']:
            re = run(self, query)

        return stringHander.trimTrains(re['r'])


    def getTicketOrderImage(self):
        def run(self):
            try:
                url = 'https://kyfw.12306.cn/otn/passcodeNew/getPassCodeNew?module=passenger&rand=randp&' + str(
                    random.randint(1, 800))

                req = urllib.request.Request(url)
                req.add_header('Host', 'kyfw.12306.cn')
                req.add_header('User-Agent',
                               'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.114 Safari/537.36')
                req.add_header('Referer', 'https://kyfw.12306.cn/otn/login/init')
                response = self.opener.open(req)
                return {'s': True, 'r': response.read()}
            except:
                return {'s': False, 'r': ''}

        re = run(self)

        while not re['s']:
            re = run(self)

        file = 'order.webp'
        img = open(file, "wb")
        img.write(re['r'])
        img.close()
        return file

    def checkOrderImageCode(self, randCode, token):
        def run(self, randCode, token):
            try:
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
                return {'s': True, 'r': response.read().decode()}
            except:
                return {'s': False, 'r': ''}

        re = run(self, randCode, token)

        while not re['s']:
            re = run(self, randCode, token)

        return stringHander.checkOrderImageCode(re['r'])


    def deleteUnPayTicket(self, deleteData):
        pass

    ################     new ticket before order     #################

    def bookTicket(self, pageInfo, filterTrain, ticketQuerySetting, passenger, orderConfirmInfo):
        print(' ################bookTicket ################ ')
        print(' ################bookTicket ################ ')
        print(' ################bookTicket ################ ')

        key_check_isChange = pageInfo['key_check_isChange']
        token = pageInfo['token']
        leftTicketStr = pageInfo['leftTicketStr']
        purpose_codes = ticketQuerySetting['includeStudent']

        if purpose_codes == 'ADULT':
            purpose_codes = '00'

        randCode = orderConfirmInfo['imageCode']
        seat = orderConfirmInfo['seat']
        train_no = filterTrain['train_no']
        stationTrainCode = filterTrain['station_train_code']
        fromStationTelecode = filterTrain['from_station_telecode']
        toStationTelecode = filterTrain['to_station_telecode']
        train_location = filterTrain['location_code']
        order_no = ''
        getQueueCountData = stringHander.getQueueCountData(time.strftime('%Y,%m,%d',
                                                                         time.strptime(filterTrain['start_train_date'],
                                                                                       '%Y%m%d')))
        #checkRandCodeAnsyn
        if self.checkRandCodeAnsyn(randCode, token):
            print('*******checkRandCodeAnsyn ok********')
        else:
            print('*******checkRandCodeAnsyn failed********')
            return False

        #checkOrderInfo
        if self.checkOrderInfo(randCode, token, seat, passenger):
            print('*******checkOrderInfo ok********')
        else:
            print('*******checkOrderInfo failed********')
            return False

        #getQueueCount
        if self.getQueueCount(getQueueCountData, train_no, stationTrainCode, seat, fromStationTelecode,
                              toStationTelecode,
                              leftTicketStr, purpose_codes, token):
            print('*******getQueueCount ok********')
        else:
            print('*******getQueueCount failed********')
            return False

        #confirmSingleForQueue
        if self.confirmSingleForQueue(seat, passenger, randCode, purpose_codes, key_check_isChange,
                                      leftTicketStr, train_location, token):
            print('*******confirmSingleForQueue ok********')
        else:
            print('*******confirmSingleForQueue failed********')
            return False

        #waitCount
        print('*******waitCount ********')
        while True:
            order_no = self.waitCount(token)
            if order_no:
                break
            else:
                time.sleep(0.1)

        #resultOrderForDcQueue
        if self.resultOrderForDcQueue(order_no, token):
            print('*******resultOrderForDcQueue ok********')
        else:
            print('*******resultOrderForDcQueue failed********')
            return False

        print('*******book tikcet ok********')

        return True

    def checkUser(self):
        def run(self):
            try:
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
                return {'s': True, 'r': response.read().decode()}
            except:
                return {'s': False, 'r': ''}

        re = run(self)

        while not re['s']:
            re = run(self)

        return stringHander.checkUser(re['r'])

    def queryTicketPrice(self, train_no, from_station_no, to_station_no, seat_types, train_date, seats):
        def run(self, train_no, from_station_no, to_station_no, seat_types, train_date, seats):
            try:
                url = 'https://kyfw.12306.cn/otn/leftTicket/queryTicketPrice?train_no={train_no}&from_station_no={from_station_no}&to_station_no={to_station_no}&seat_types={seat_types}&train_date={train_date}'

                req = urllib.request.Request(
                    url.format(train_no=train_no, from_station_no=from_station_no, to_station_no=to_station_no,
                               seat_types=seat_types, train_date=train_date))
                req.add_header('Host', 'kyfw.12306.cn')
                req.add_header('User-Agent',
                               'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.114 Safari/537.36')
                req.add_header('Referer', 'https://kyfw.12306.cn/otn/leftTicket/init')
                req.add_header('Content-Type', 'application/x-www-form-urlencoded; charset=UTF-8')
                req.add_header('X-Requested-With', 'XMLHttpRequest')
                response = self.opener.open(req)
                self.updateCookie()
                return {'s': True, 'r': response.read().decode()}
            except:
                return {'s': False, 'r': ''}

        re = run(self, train_no, from_station_no, to_station_no, seat_types, train_date, seats)

        while not re['s']:
            re = run(self, train_no, from_station_no, to_station_no, seat_types, train_date, seats)

        return stringHander.queryTicketPrice(seats, re['r'])

    def submitOrderRequest(self, secretStr, train_date, back_train_date, purpose_codes, query_from_station_name,
                           query_to_station_name, tour_flag='dc'):
        def run(self, secretStr, train_date, back_train_date, purpose_codes, query_from_station_name,
                query_to_station_name, tour_flag):
            try:
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
                return {'s': True, 'r': response.read().decode()}
            except:
                return {'s': False, 'r': ''}

        re = run(self, secretStr, train_date, back_train_date, purpose_codes, query_from_station_name,
                 query_to_station_name, tour_flag)

        while not re['s']:
            re = run(self, secretStr, train_date, back_train_date, purpose_codes, query_from_station_name,
                     query_to_station_name, tour_flag)

        return stringHander.submitOrderRequest(re['r'])

    ################     new ticket token    #################

    def getOrderPageInfo(self):
        def run(self):
            try:
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
                return {'s': True, 'r': response.read().decode()}
            except:
                return {'s': False, 'r': ''}

        re = run(self)

        while not re['s']:
            re = run(self)

        return bs.Bs.getOrderPageInfo(re['r'])


    ################     new ticket oreder     #################
    def checkRandCodeAnsyn(self, code, token):
        def run(self, code, token):
            try:
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
                return {'s': True, 'r': response.read().decode()}
            except:
                return {'s': False, 'r': ''}

        re = run(self, code, token)

        while not re['s']:
            re = run(self, code, token)

        return stringHander.checkRandCodeAnsyn(re['r'])

    def getPassengerTicketStr(self, seat, person):
        _list = []
        for i in person:
            _list.append(
                seat + ",0," + str(i['passenger_type']) + "," + str(i['passenger_name']) + "," + str(
                    i['passenger_id_type_code']) + "," + str(i[
                    'passenger_id_no']) + "," + '' + "," + "N")

        return '_'.join(_list)

    def getOldPassengers(self, person):
        _list = []
        for i in person:
            _list.append(
                str(i['passenger_name']) + "," + str(i['passenger_id_type_code']) + "," + str(
                    i['passenger_id_no']) + "," + str(i['passenger_type']))

        return '_'.join(_list) + '_'

    def checkOrderInfo(self, randCode, token, seat, passenger, cancel_flag=2,
                       tour_flag='dc', bed_level_order_num='000000000000000000000000000000'):

        def run(self, randCode, token, seat, passenger, cancel_flag,
                tour_flag, bed_level_order_num):
            try:
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

                cancel_flag=2&
                bed_level_order_num=000000000000000000000000000000&
                passengerTicketStr=1,0,1,桍夺夺,1,431121198907177432,13664654645,N&
                oldPassengerStr=桍夺夺,1,431121198907177432,1_&
                tour_flag=dc
                &randCode=ka75
                &_json_att=&
                REPEAT_SUBMIT_TOKEN=2a8f974bd77816babf8c0a5a1f653422


                '''
                data = [
                    'cancel_flag=' + str(cancel_flag),
                    'bed_level_order_num=' + str(bed_level_order_num),
                    'passengerTicketStr=' + str(self.getPassengerTicketStr(seat, passenger)),
                    'oldPassengerStr=' + str(self.getOldPassengers(passenger)),
                    'tour_flag=' + str(tour_flag),
                    'randCode=' + str(randCode),
                    '_json_att=',
                    'REPEAT_SUBMIT_TOKEN=' + str(token)]

                req = urllib.request.Request(url, '&'.join(data).encode())
                req.add_header('Host', 'kyfw.12306.cn')
                req.add_header('User-Agent',
                               'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.114 Safari/537.36')
                req.add_header('Referer', 'https://kyfw.12306.cn/otn/confirmPassenger/initDc')
                req.add_header('Content-Type', 'application/x-www-form-urlencoded; charset=UTF-8')
                req.add_header('X-Requested-With', 'XMLHttpRequest')
                response = self.opener.open(req)
                self.updateCookie()
                return {'s': True, 'r': response.read().decode()}
            except:
                return {'s': False, 'r': ''}

        re = run(self, randCode, token, seat, passenger, cancel_flag,
                 tour_flag, bed_level_order_num)

        while not re['s']:
            re = run(self, randCode, token, seat, passenger, cancel_flag,
                     tour_flag, bed_level_order_num)

        return stringHander.checkOrderInfo(re['r'])


    def getQueueCount(self, train_date, train_no, stationTrainCode, seatType, fromStationTelecode, toStationTelecode,
                      leftTicket, purpose_codes, token):
        def run(self, train_date, train_no, stationTrainCode, seatType, fromStationTelecode, toStationTelecode,
                leftTicket, purpose_codes, token):
            try:
                url = 'https://kyfw.12306.cn/otn/confirmPassenger/getQueueCount'

                #{"validateMessagesShowId":"_validatorMessage","status":true,"httpstatus":200,"data":{"count":"50","ticket":"1020103333406560002010201003983035500001","op_2":"false","countT":"0","op_1":"true"},"messages":[],"validateMessages":{}}
                '''
                train_date=Tue+Dec+31+2013+00%3A00%3A00+GMT%2B0800+(CST)&
                train_no=630000K5280D&
                stationTrainCode=K528&
                seatType=1&
                fromStationTelecode=GZQ&
                toStationTelecode=SNH&
                leftTicket=1020103333406560002010201003983035500001&
                purpose_codes=00&_json_att=&
                REPEAT_SUBMIT_TOKEN=2a8f974bd77816babf8c0a5a1f653422

                train_date=Tue+Dec+31+2013+00:00:00+GMT+0800+(CST)&train_no=630000K5280D&
                stationTrainCode=K528&seatType=1&fromStationTelecode=GZQ&
                toStationTelecode=SNH&leftTicket=1020103333406560002010201003983035500001&
                purpose_codes=00
                &_json_att=&
                REPEAT_SUBMIT_TOKEN=2a8f974bd77816babf8c0a5a1f653422
                '''
                data = collections.OrderedDict()

                data['train_date'] = train_date
                data['train_no'] = train_no
                data['stationTrainCode'] = stationTrainCode
                data['seatType'] = seatType
                data['fromStationTelecode'] = fromStationTelecode
                data['toStationTelecode'] = toStationTelecode
                data['leftTicket'] = leftTicket
                data['purpose_codes'] = purpose_codes
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
                return {'s': True, 'r': response.read().decode()}
            except:
                return {'s': False, 'r': ''}

        re = run(self, train_date, train_no, stationTrainCode, seatType, fromStationTelecode, toStationTelecode,
                 leftTicket, purpose_codes, token)

        while not re['s']:
            re = run(self, train_date, train_no, stationTrainCode, seatType, fromStationTelecode, toStationTelecode,
                     leftTicket, purpose_codes, token)

        return stringHander.getQueueCount(re['r'])

    def confirmSingleForQueue(self, seat, passenger, randCode, purpose_codes, key_check_isChange,
                              leftTicketStr, train_location, token):
        def run(self, seat, passenger, randCode, purpose_codes, key_check_isChange,
                leftTicketStr, train_location, token):
            try:
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
                leftTicketStr=1020103333406560002010201003983035500001&
                train_location=Q7&
                _json_att=&REPEAT_SUBMIT_TOKEN=2a8f974bd77816babf8c0a5a1f653422
                '''

                data = collections.OrderedDict()
                data['passengerTicketStr'] = self.getPassengerTicketStr(seat, passenger)
                data['oldPassengerStr'] = self.getOldPassengers(passenger)
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
                return {'s': True, 'r': response.read().decode()}
            except:
                return {'s': False, 'r': ''}

        re = run(self, seat, passenger, randCode, purpose_codes, key_check_isChange,
                 leftTicketStr, train_location, token)

        while not re['s']:
            re = run(self, seat, passenger, randCode, purpose_codes, key_check_isChange,
                     leftTicketStr, train_location, token)

        return stringHander.confirmSingleForQueue(re['r'])

    def waitCount(self, token):
        def run(self, token):
            try:
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
                return {'s': True, 'r': response.read().decode()}
            except:
                return {'s': False, 'r': ''}

        re = run(self, token)

        while not re['s']:
            re = run(self, token)

        return stringHander.waitCount(re['r'])

    def resultOrderForDcQueue(self, order_no, token):

        def run(self, order_no, token):
            try:
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
                return {'s': True, 'r': response.read().decode()}
            except:
                return {'s': False, 'r': ''}

        re = run(self, order_no, token)

        while not re['s']:
            re = run(self, order_no, token)

        return stringHander.resultOrderForDcQueue(re['r'])



