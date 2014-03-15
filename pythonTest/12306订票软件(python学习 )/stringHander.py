# -*- coding: utf-8 -*-

import re
import execjs
import time


def unexpectedResponse(html):
    if re.search('.*?网络可能存在问题，请您重试一下！.*?', html, re.S):
        return True
    else:
        return False


def trimTrains(html):
    tmp = []
    try:
        trains = eval(html.strip(), type('Dummy', (dict,), dict(__getitem__=lambda s, n: n))())

        if isinstance(trains, dict) and len(trains) > 0:
            if trains['messages']:
                return None
            for i in trains['data']:
                queryLeftNewDTO = i['queryLeftNewDTO']
                queryLeftNewDTO.update({'secretStr': i['secretStr'], 'buttonTextInfo': i['buttonTextInfo']})
                tmp.append(queryLeftNewDTO)
        return tmp
    except:
        return tmp


def stringToDicts(html):
    try:
        return eval(html.strip(), type('Dummy', (dict,), dict(__getitem__=lambda s, n: n))())
    except:
        return False


def getPersonalContacts(html):
    try:
        script = re.sub('.*?<script.*?preserve.*?>(.*?passengers.*?)<\/script>.*?', r'\1', html.strip(), flags=re.S)

        return {'recordCount': re.sub('.*?totlePage\s*=\s*(\d+)\s*;.*', r'\1', script, flags=re.S),
                'rows': eval(re.sub('.*?passengers=(\[.*?\]).*', r'\1', script, flags=re.S),
                             type('Dummy', (dict,), dict(__getitem__=lambda s, n: n))())}
    except:
        return False


def addPersonalContact(html):
    try:

        if eval(html.strip(), type('Dummy', (dict,), dict(__getitem__=lambda s, n: n))())['data']['flag'] == 'true':
            return True
        else:
            print('________addPersonalContact___________')
            print(html)
            return False
    except:
        print(html)
        return False


def deletePersonalContact(html):
    try:
        if eval(html.strip(), type('Dummy', (dict,), dict(__getitem__=lambda s, n: n))())['data']['flag'] == 'true':
            return True
        else:
            print('________deletePersonalContact___________')
            print(html)
            return False
    except:
        print(html)
        return False


def checkRandCodeAnsyn(html):
    try:
        if eval(html.strip(), type('Dummy', (dict,), dict(__getitem__=lambda s, n: n))())['data'] == 'Y':
            return True
        else:
            print('________checkRandCodeAnsyn___________')
            print(html)
            return False
    except:
        print(html)
        return False


def getQueueCount(html):
    try:
        if not eval(html.strip(), type('Dummy', (dict,), dict(__getitem__=lambda s, n: n))())['status'] == 'false':
            return True
        else:
            print('________getQueueCount___________')
            print(html)
            return False
    except:
        return False


def confirmSingleForQueue(html):
    try:
        if eval(html.strip(), type('Dummy', (dict,), dict(__getitem__=lambda s, n: n))())['data'][
            'submitStatus'] == 'true':
            return True
        else:
            print('________confirmSingleForQueue___________')
            print(html)
            return False
    except:
        print(html)
        return False


def waitCount(html):
    try:
        order = str(eval(html.strip(), type('Dummy', (dict,), dict(__getitem__=lambda s, n: n))())['data']['orderId'])
        if len(order) > 4:
            return order
        else:
            print('________waitCount___________')
            print(html)
            return False
    except:
        print(html)
        return False


def resultOrderForDcQueue(html):
    try:
        if eval(html.strip(), type('Dummy', (dict,), dict(__getitem__=lambda s, n: n))())['data'][
            'submitStatus'] == 'true':
            return True
        else:
            print('________resultOrderForDcQueue___________')
            print(html)
            return False
    except:
        print(html)
        return False


def checkUser(html):
    try:
        if eval(html.strip(), type('Dummy', (dict,), dict(__getitem__=lambda s, n: n))())['data']['flag'] == 'true':
            return True
        else:
            print('________checkUser___________')
            print(html)
            return False
    except:
        print(html)
        return False


def submitOrderRequest(html):
    try:
        if eval(html.strip(), type('Dummy', (dict,), dict(__getitem__=lambda s, n: n))())['messages'] == []:
            return True
        else:
            print('________submitOrderRequest___________')
            print(html)
            return False
    except:
        print(html)
        return False


def checkOrderInfo(html):
    try:
        if eval(html.strip(), type('Dummy', (dict,), dict(__getitem__=lambda s, n: n))())['data'][
            'submitStatus'] == 'true':
            return True
        else:
            print('________checkOrderInfo___________')
            print(html)
            return False
    except:
        print(html)
        return False


def checkOrderImageCode(html):
    try:
        if eval(html.strip(), type('Dummy', (dict,), dict(__getitem__=lambda s, n: n))())['data'] == 'Y':
            return True
        else:
            print('________checkOrderImageCode___________')
            print(html)
            return False
    except:
        print(html)
        return False


def queryTicketPrice(seats, html):
    return 'queryTicketPrice' #json.dumps(eval(html, type('Dummy', (dict,), dict(__getitem__=lambda s, n: n))())['data'])


def getUnPayOrder(html):
    try:
        s = eval(html, type('Dummy', (dict,), dict(__getitem__=lambda s, n: n))())
        pay = s['data']['orderDBList'][0]['tickets'][0]['ticket_status_name']
        start = s['data']['orderDBList'][0]['tickets'][0]['start_train_date_page']
        coach_no = s['data']['orderDBList'][0]['tickets'][0]['coach_no']
        seat_no = s['data']['orderDBList'][0]['tickets'][0]['seat_name']
        seat_type_name = s['data']['orderDBList'][0]['tickets'][0]['seat_type_name']
        passenger_name = s['data']['orderDBList'][0]['tickets'][0]['passengerDTO']['passenger_name']
        passenger_id_type_name = s['data']['orderDBList'][0]['tickets'][0]['passengerDTO']['passenger_id_type_name']
        from_station_name = s['data']['orderDBList'][0]['tickets'][0]['stationTrainDTO']['from_station_name']
        to_station_name = s['data']['orderDBList'][0]['tickets'][0]['stationTrainDTO']['to_station_name']
        station_train_code = s['data']['orderDBList'][0]['tickets'][0]['stationTrainDTO']['station_train_code']
        return [print(start + '*' + from_station_name + '*' + to_station_name + '*' + station_train_code),
                coach_no + '*' + seat_no + '*' + seat_type_name, passenger_name + '*' + passenger_id_type_name, pay]
    except:
        print(html)
        return False


def getQueueCountData(data):
    return execjs.eval('new Date(' + data + ').toString()')


def checkLoginCodeValidate(html):
    try:
        if eval(html, type('Dummy', (dict,), dict(__getitem__=lambda s, n: n))())['data'] == 'Y':
            return True
        else:
            print('________checkLoginCodeValidate___________')
            print(html)
            return False
    except:
        print(html)
        return False


def loginAyncSuggest(html):
    try:
        if eval(html, type('Dummy', (dict,), dict(__getitem__=lambda s, n: n))())['data']['loginCheck'] == 'Y':
            return True
        else:
            print('________loginAyncSuggest___________')
            print(html)
            return False
    except:
        print(html)
        return False


def orderTips(filterTrain):
    try:
        return ' '.join([time.strftime('%Y-%m-%d', time.strptime(filterTrain['start_train_date'], '%Y%m%d')),
                         filterTrain['station_train_code'] + '次',
                         filterTrain['start_station_name'] + '站',
                         '(' + filterTrain['start_time'] + '开' + ')',
                         '--',
                         filterTrain['to_station_name'] + '站',
                         '(' + filterTrain['arrive_time'] + '到' + ')',
                         filterTrain['lishi']])
    except:
        return ' '


def priceTips(price):
    return 'priceTips'

