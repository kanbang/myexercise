# -*- coding: utf-8 -*-

from PyQt4 import QtGui, Qt, QtCore

import sys
import json
import collections
import time
import copy
import threading

import db
import readStorage
import config
import orderConfirm
import stringHander
import queryCore
import filter
import login
import mainWindow
import station
import alertWindow
import log
import stringHander


class KeepAlive(threading.Thread):
    def __init__(self, ctr, keepAliveTime):
        threading.Thread.__init__(self)
        self.ctr = ctr
        self.stop = False
        self.keepAliveTime = int(keepAliveTime)

    def run(self):
        while True and not self.stop:
            self.ctr._keepAlive()
            time.sleep(self.keepAliveTime)

    def stop_(self):
        self.stop = True


class SimpleController(QtCore.QObject):
    signalUpdateTrainDate = QtCore.pyqtSignal(dict)

    def __init__(self):

        QtCore.QObject.__init__(self)
        self.config = config.Config()
        self.login = False
        self.loginTime = None

        print('_______strp1_________')
        self.processEvents()
        self.queryCore = queryCore.Core(self)
        self.processEvents()
        print('_______strp2_________')
        self.db = db.Db()
        storage = readStorage.Storage()
        self.storageData = storage.readCommentSetting()
        self.config.updateSequenceDateFromStorage(self.storageData)
        self.config.set('stations', station.getStations())
        self.onBooking = False
        print('_______strp3_________')

        app = QtGui.QApplication(sys.argv)

        print('_______strp4_________')
        #login
        autoLogin = False
        try:
            self.processEvents()
            print('_______strp5_________')
            if self.getLoginData()['username'] == self.getPersonalDetail()['username']:
                self.processEvents()
                autoLogin = True
        except Exception:
            pass
        print('_______strp6_________')

        if not autoLogin:
            print('_______strp7_________')
            self.processEvents()
            lg = login.Login(self)
            self.processEvents()
        print('_______strp8_________')
        if autoLogin or lg.exec_():
            if not autoLogin:
                self.processEvents()
                if not lg.getLogin():
                    return
            print('_______strp9_________')
            self.processEvents()
            self.getPersonalContacts()
            self.processEvents()
            print('_______strp10_________')
            main = mainWindow.MainWindow(self)
            main.setWindowIcon(QtGui.QIcon("images/favorite.ico"))
            main.show()

            self.keepAlive = KeepAlive(self, self.config.get('keepAliveTime'))
            self.keepAlive.setDaemon(True)
            self.keepAlive.start()
        sys.exit(app.exec_())

    #################################################################
    ## 用户登录
    #################################################################

    #keep alive
    def _keepAlive(self):
        pass
        self.queryCore.keepAlive()

    #用户登录资料
    def getLoginData(self):
        return self.config.get('loginData')

    #车票配置
    def getTicketSetting(self):

        return {
            'ticketQuerySetting': self.config.get('ticketQuerySetting'),
            'defaultSetting': self.config.get('defaultSetting')
        }

    def getSettings(self, type):
        return self.config.get(type)

    #用户登录
    def submitLoginForm(self, loginData):
        self.processEvents()
        self.config.set('loginData', loginData)
        loginStatus = self.queryCore.login(loginData)
        if loginStatus:
            if int(loginData['remember']) == 1:
                self.db.setDicts(
                    {'username': loginData['username'], 'password': loginData['password'],
                     'remember': loginData['remember']},
                    'loginData')
            else:
                self.db.deleteGroup('loginData')

            self.login = True
            self.loginTime = time.time()
            return self.login
        else:
            return False

    #更新验证码
    def refreshLoginCode(self):
        return self.queryCore.getRandCodeImage()

    #################################################################
    ## 常用联系人
    #################################################################

    #删除常用联系人
    def deletePersonalContact(self, person):
        personData = self.getPersonalContactsDetail(person)

        if self.queryCore.deletePersonalContact(personData):
            #删除联系人
            self.config.removeFromDictsSequence('personalContacts', person)
            return True
        return False

    #添加常用联系人
    def addPersonalContact(self, submitData):
        log_ = log.Log('addPersonalContact.log', 'a')
        log_.write(json.dumps(submitData) + "\n" + '...................................................' + "\n")
        if self.queryCore.addPersonalContact(submitData):
            #存储联系人
            sex = {
                'M': '男', 'F': '女', '': '-'
            }
            passengerTypeArr = self.config.get('passengerTypeArr')
            cardTypeArr = self.config.get('cardTypeArr')

            submitData['passenger_name'] = submitData['name']
            submitData['passenger_id_no'] = submitData['card_no']
            submitData['sex_name'] = sex[submitData['sex_code']]
            submitData['passenger_id_type_name'] = cardTypeArr[submitData['card_type']]
            submitData['passenger_type_name'] = passengerTypeArr[submitData['passenger_type']]
            log_.write(
                json.dumps(submitData) + "\n" + '####################################################' + "\n\n\n")
            self.config.extendDictsSequence('personalContacts', {submitData['name']: submitData})
            return True
        return False

    def processEvents(self):
        QtGui.QApplication.processEvents()

    #常用联系人
    def getPersonalContacts(self):

    #for i in range(10):

        personalContacts = self.queryCore.getPersonalContacts()
        if personalContacts['recordCount'] == 0:
            return self.config.get('personalContacts')

        #存储联系人
        personalContactsData = {v['passenger_name']: v for v in personalContacts['rows']}
        self.config.extendDictsSequence('personalContacts', personalContactsData)


    def getPersonalContactsDetailColData(self, row, field):
        personalContacts = self.config.get('personalContacts')
        for i, v in enumerate(personalContacts):
            if i == row:
                if field in personalContacts[v]:
                    return personalContacts[v][field]
                else:
                    return None

    def getPersonalContactsCounts(self):
        personalContacts = self.config.get('personalContacts')
        return (len(personalContacts))

    #常用联系人资料
    def getPersonalContactsDetail(self, person):
        if person:
            return self.config.getDictsSequence('personalContacts', person)

    #个人资料
    def getPersonalDetail(self):
        personalDetail = self.queryCore.getPersonalDetail()
        self.config.set('personalDetail', personalDetail)
        return personalDetail

    #################################################################
    ## 订单记录
    #################################################################

    #订单未付款记录
    def getUnPayOrderTicket(self):
        return self.queryCore.getUnPayOrderTicket()

    #订单记录
    def getTicketOrderRecorder(self, queryData):
        queryData = stringHander.stringToDicts(queryData)
        return self.queryCore.getTicketOrderRecorder(queryData)


    def deleteUnPayTicket(self, deleteData):
        return self.queryCore.deleteUnPayTicket(deleteData)

    def payUnPayTicket(self, id):
        self.payUnPayTicket(id)

    #################################################################
    ## 车票查询
    #################################################################
    #station code
    def getStationCode(self, station):
        if station:
            return self.config.getDictsSequence('stations', station)
        return None

    #车票查询
    def ticketsQuery(self, submitData, loop=False):

        print('((((((((((((((((()))))))))))))))))))')
        print(submitData)
        print(loop)
        print('((((((((((((((((()))))))))))))))))))')

        self.processEvents()
        autoOrderTicket = self.config.getDictsSequence('defaultSetting', 'autoOrderTicket')

        def validateAutoBookTicket(autoOrderTicket, sortedTrains):

            if int(autoOrderTicket) == 1 and not sortedTrains['seat'] is None and not sortedTrains[
                'filterTrain'] is None and len(sortedTrains['filterTrain']) > 0:
                return True
            else:
                return False

        if 'filterTrain' in submitData:

            self.bookTicket(submitData['filterTrain'], seat=submitData['seat'])

        else:
            print(submitData)
            queryData = collections.OrderedDict()
            queryData["orderRequest.train_date"] = submitData['orderRequest.train_date']
            queryData['orderRequest.from_station_telecode'] = submitData['orderRequest.from_station_telecode']
            queryData['orderRequest.to_station_telecode'] = submitData['orderRequest.to_station_telecode']
            queryData['orderRequest.train_no'] = submitData['orderRequest.trainCodeText']
            # queryData['trainPassType'] = submitData['trainPassType']
            queryData['trainClass'] = submitData['trainClassArr']
            queryData['includeStudent'] = submitData['includeStudent']
            queryData['seatTypeAndNum'] = ''
            queryData['orderRequest.start_time_str'] = submitData['orderRequest.start_time_str']
            queryData['orderRequest.roundTrainDate'] = getattr(submitData, 'orderRequest.roundTrainDate',
                                                               submitData['orderRequest.train_date'])
            trains = self.queryCore.ticketQuery(queryData)
            self.config.set('ticketQuerySetting', submitData)

            #存储订票信息
            self.db.set('ticketQuerySetting', submitData, 'ticketQuerySetting')
            self.processEvents()

            #fliter
            trainFilter = filter.Filter()

            sortedTrains = trainFilter.filter(submitData['orderRequest.start_time_str'], trains, self.config, loop)

            if sortedTrains is not None and len(sortedTrains) > 0:
                if validateAutoBookTicket(autoOrderTicket, sortedTrains):
                    data = {'train': sortedTrains['trains'], 'filterTrain': sortedTrains['filterTrain'],
                            'seat': sortedTrains['seat'], 'loop': loop}
                else:
                    data = {'train': sortedTrains['trains'], 'filterTrain': '', 'seat': '', 'loop': loop}

                self.signalUpdateTrainDate.emit(data)

                if not sortedTrains['seat'] is None:
                    if loop:
                        if int(autoOrderTicket) == 1:
                            return True
                        else:
                            return False
                    else:
                        self.bookTicket(sortedTrains['filterTrain'], seat=sortedTrains['seat'])
                else:
                    return False
            else:
                return False

    #################################################################

    ## 订单生成

    #################################################################

    #订单生成页面
    def bookTicket(self, filterTrain, seat=None):

        if self.onBooking:
            return False

        print(seat)
        print(filterTrain)

        ticketQuerySetting = self.config.get('ticketQuerySetting')
        defaultSetting = self.config.get('defaultSetting')
        seatTypePrice = self.config.get('seatTypePrice')
        print('--------7---------')
        secretStr = filterTrain['secretStr']

        train_date = time.strftime('%Y-%m-%d', time.strptime(filterTrain['start_train_date'], '%Y%m%d'))

        if getattr(ticketQuerySetting, 'orderRequest.roundTrainDate', False):
            back_train_date = time.strftime('%Y-%m-%d', time.strptime(ticketQuerySetting['orderRequest.roundTrainDate'],
                                                                      '%Y-%m-%d'))
        else:
            back_train_date = train_date
        print('--------8---------')
        purpose_codes = 'ADULT'
        query_from_station_name = filterTrain['from_station_name']
        query_to_station_name = filterTrain['to_station_name']
        train_no = filterTrain['train_no']
        to_station_no = filterTrain['to_station_no']
        from_station_no = filterTrain['from_station_no']
        seat_types = filterTrain['seat_types']

        #checkUser
        if self.queryCore.checkUser():
            print('***checkUser****')

        #queryTicketPrice
        priceTips = self.queryCore.queryTicketPrice(train_no, from_station_no, to_station_no, seat_types, train_date,
                                                    seatTypePrice)
        print('--------9---------')
        #submitOrderRequest
        if self.queryCore.submitOrderRequest(secretStr, train_date, back_train_date, purpose_codes,
                                             query_from_station_name,
                                             query_to_station_name):
            print('***submitOrderRequest****')
        else:
            print('***submitOrderRequest failed****')

        #getOrderPageInfo
        print('***getOrderPageInfo****')
        pageInfo = self.queryCore.getOrderPageInfo()
        print('--------10---------')
        pageInfo.update({'priceTips': stringHander.orderTips(priceTips),
                         'seat': seat,
                         'orderTips': stringHander.orderTips(filterTrain)
        })
        self.onBooking = True

        def startBookTicket(self, pageInfo, filterTrain, ticketQuerySetting, defaultSetting):

            #orderConfirm
            orderConfirmDialog = orderConfirm.Confirm(self, pageInfo)

            if orderConfirmDialog.exec_():
                orderConfirmInfo = orderConfirmDialog.getOrderInput()
                passenger = []
                for p in defaultSetting['ticketPassengers']:
                    passenger.append(self.getPersonalContactsDetail(p))
                orderConfirmInfo['seat'] = seat

                return self.queryCore.bookTicket(pageInfo, filterTrain, ticketQuerySetting, passenger, orderConfirmInfo)
            else:
                return True

        if not startBookTicket(self, pageInfo, filterTrain, ticketQuerySetting, defaultSetting):
            startBookTicket(self, pageInfo, filterTrain, ticketQuerySetting, defaultSetting)
        self.onBooking = False

    def refreshImage(self):
        img = self.queryCore.getTicketOrderImage()
        if len(img) == 0:
            self.queryCore.resetCookie()
            img = self.queryCore.getTicketOrderImage()
        else:
            return img

    def checkOrderImageCode(self, code, token):
        return self.queryCore.checkOrderImageCode(code, token)

        #################################################################

    ## 配置
    #################################################################

    def getConfig(self):
        return self.config


    def setPassengers(self, passenger):
        orderTicketsLimit = self.config.get('orderTicketsLimit')
        passengers = self.config.getDictsSequence('defaultSetting', 'ticketPassengers')

        if len(passengers) >= int(orderTicketsLimit):
            return False
        else:
            try:
                passengers.remove(passenger)
            except Exception:
                pass
            passengers.append(passenger)

        self.config.setDictsSequence('defaultSetting', 'ticketPassengers', passengers)
        #存储订票人
        self.db.set('ticketPassengers', passengers, 'defaultSetting')

    def getPassengersCount(self):
        return len(self.config.getDictsSequence('defaultSetting', 'ticketPassengers'))

    def getPassengers(self, col):
        try:
            return self.config.getDictsSequence('defaultSetting', 'ticketPassengers')[col]
        except Exception:
            return None

    def deletePassengers(self, passengers):
        ticketPassengers = self.config.getDictsSequence('defaultSetting', 'ticketPassengers')
        try:
            ticketPassengers.remove(passengers)
        except Exception:
            pass
        self.db.set('ticketPassengers', ticketPassengers, 'defaultSetting')
        return True

    #订单验证码
    def setOrderCode(self, data):
        self.config.setDictsSequence('defaultSetting', 'orderTicketImageCode',
                                     {'value': data, 'createTime': time.time()})

    def setFavoriteTrain(self, data):
        if data is not None:
            self.config.setDictsSequence('defaultSetting', 'favoriteTrain', data.split(','))
            #存储
            self.db.set('favoriteTrain', data.split(','), 'defaultSetting', )

    #时间间隔
    def setIntervalRefreshTime(self, data):
        self.config.setDictsSequence('defaultSetting', 'intervalRefreshTime', data)
        #存储
        self.db.set('intervalRefreshTime', data, 'defaultSetting')

    def getIntervalRefreshTime(self):
        return self.config.getDictsSequence('defaultSetting', 'intervalRefreshTime')

    #最小座位数
    def setMinimumTicketNumber(self, data):
        self.config.setDictsSequence('defaultSetting', 'minimumTicketNumber', data)
        #存储
        self.db.set('minimumTicketNumber', data, 'defaultSetting')

    def setAutoTicketQuery(self, data):
        self.config.setDictsSequence('defaultSetting', 'autoTicketQuery', data)
        #存储
        self.db.set('autoTicketQuery', data, 'defaultSetting')

    #期望座位类型
    def setSeatType(self, seat):
        seats = self.config.getDictsSequence('defaultSetting', 'seatType')

        try:
            index = seats.index(seat)
            if not index is None:
                return
        except Exception:
            pass

        seats.append(seat)
        self.config.setDictsSequence('defaultSetting', 'seatType', seats)
        #存储
        self.db.set('seatType', seats, 'defaultSetting')

    def deleteSeatType(self, seat):
        seats = self.config.getDictsSequence('defaultSetting', 'seatType')
        try:
            seats.remove(seat)
        except Exception:
            pass

        self.config.setDictsSequence('defaultSetting', 'seatType', seats)
        self.db.set('seatType', seats, 'defaultSetting')

    def getSeatType(self, col):
        try:
            return self.config.getDictsSequence('defaultSetting', 'seatType')[col]
        except Exception:
            return None

    def getSeatTypeCount(self):
        return len(self.config.getDictsSequence('defaultSetting', 'seatType'))

        #

    # #期望卧铺座位类e
    # def setSleepBeddingType(self, data):
    #     self.config.setDictsSequence('defaultSetting', 'sleepBeddingType', data)
    #     #存储
    #     self.db.set('sleepBeddingType', data, 'defaultSetting')

    #是否自动订票
    def setAutoOrderTicket(self, data):
        self.config.setDictsSequence('defaultSetting', 'autoOrderTicket', data)
        #存储
        self.db.set('autoOrderTicket', data, 'defaultSetting')

    #是否自动订票
    def onlyDisplayFilterTrain(self, data):
        self.config.setDictsSequence('defaultSetting', 'onlyDisplayFilterTrain', data)
        #存储
        self.db.set('onlyDisplayFilterTrain', data, 'defaultSetting')

    #匹配车次提前
    def setAutoTopSelectedTrain(self, data):
        self.config.setDictsSequence('defaultSetting', 'autoTopSelectedTrain', data)
        #存储
        self.db.set('autoTopSelectedTrain', data, 'defaultSetting')

    def setStartQueryTime(self, data):
        self.config.setDictsSequence('defaultSetting', 'startQueryTime', data)
        #存储
        self.db.set('startQueryTime', data, 'defaultSetting')

    #存储数据
    def savingStorage(self):
        self.db.savingStorage()

    #同步cookie
    def syncCookie(self, cookie):
        self.config.set('cookie', cookie)

    def getSyncCookie(self):
        return self.config.get('cookie')

        #################################################################

## runer
#################################################################

class runLoop():
    @classmethod
    def loop(self):
        SimpleController()


if __name__ == '__main__':
    SimpleController()







