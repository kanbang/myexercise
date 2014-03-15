from  bs4 import BeautifulSoup
import re

from pyquery import PyQuery as pq


class Bs():
    @classmethod
    def soup(cls, html):
        return BeautifulSoup(html)

    @classmethod
    def getTicketInfo(cls, html):

        m = re.search('.*目前您还有未处理的订单.请您到.未完成订单.进行处理.*', html)
        if m:
            return 'existUupPayOrder'

        soup = Bs.soup(html)
        info = soup.find_all('input')
        data = {}
        for i in info:
            try:
                soup2 = Bs.soup(str(i))
                input = soup2.select('input')

                data[input[0]['name']] = input[0]['value']
            except Exception:
                pass

        passengerOrderSeat = soup.select('#passenger_1_seat')[0]
        passengerOrderSeat = Bs.soup(str(passengerOrderSeat))
        passengerOrderSeat = passengerOrderSeat.select('option')
        passengerOrderSeatSeatData = []
        for i in passengerOrderSeat:
            passengerOrderSeatSeatData.append(i.string)

        priceOrderTable = soup.select('.qr_box')[0]
        priceOrderTable = Bs.soup(str(priceOrderTable))
        priceOrderTable = priceOrderTable.select('tr')[:-1]
        priceOrderTableData = []
        for i in priceOrderTable:
            one = Bs.soup(str(i))
            td = one.select('td')
            info = [i.string.strip() for i in td]
            priceOrderTableData.append(info)
        data.update(
            {'passengerOrderSeatSeatData': passengerOrderSeatSeatData, "priceOrderTableData": priceOrderTableData})
        return data

    @classmethod
    def checkLoginSuccess(cls, html, user):
        soup = Bs.soup(html)
        try:
            child = soup.select('input[name="userDTO.loginUserDTO.user_name"]')
            if child[0]['value'] == user:
                return True
        except Exception:
            return False

    @classmethod
    def getPersonalDetail(cls, html):
        soup = Bs.soup(html)
        detail = {}
        try:
            selector = pq(html)
            detail['username'] = selector.find('input[name="userDTO.loginUserDTO.user_name"]').eq(0).val()
            detail['birthday'] = selector.find('input[name="userDTO.born_date"]').eq(0).val()
            detail['realname'] = selector.find('input[name="userDTO.loginUserDTO.name"]').eq(0).val()
            detail['sex'] = '男' if selector.find('input[name="userDTO.sex_code"]:checked').eq(0).val() == 'M' else '女'
            detail['cardType'] = selector.find('input[name="userDTO.loginUserDTO.id_type_name"]').eq(0).val()
            detail['cardNo'] = selector.find('input[name="userDTO.loginUserDTO.id_no"]').eq(0).val()
            detail['mobileNo'] = selector.find('input[name="userDTO.mobile_no"]').eq(0).val()
            detail['passengerType'] = selector.find('select[name="userDTO.loginUserDTO.user_type"]').eq(0).find(
                'option:selected').html()
        except Exception:
            pass
        return detail

    @classmethod
    def getTicketOrderRecorder(cls, html):
        return

    @classmethod
    def getTicketOrderRecorderPageToken(cls, html):
        soup = Bs.soup(html)
        result = soup.select('input[name="org.apache.struts.taglib.html.TOKEN"]')
        return result[0]['value']

    @classmethod
    def getAddPersonalContactPageToken(cls, html):
        soup = Bs.soup(html)
        result = soup.select('input[name="org.apache.struts.taglib.html.TOKEN"]')
        return result[0]['value']

    @classmethod
    def getPersonalContactPageToken(cls, html):
        soup = Bs.soup(html)
        result = soup.select('input[name="org.apache.struts.taglib.html.TOKEN"]')
        return result[0]['value']

    @classmethod
    def getUnPayOrderTicket(cls, html):
        data = []
        try:
            soup = Bs.soup(html)
            token = soup.select('input[name="org.apache.struts.taglib.html.TOKEN"]')[1]['value']
            table = soup.select('table.table_clist')
            soup = Bs.soup(str(table[0]).replace('th', 'td'))
            tr = soup.select('tr')
            count = len(tr)
            for i, v in enumerate(tr):
                if i in range(count - 1):
                    td = v.find_all('td')
                    row = []
                    for j in td:
                        tdData = [re.sub(r'\s+', '', str(i)) for i in j.contents]
                        tdData = [re.sub(r'^<input.*?/>', '', str(i)) for i in tdData]
                        row.append(''.join(tdData))

                    row = [i.rstrip('<br/>') for i in row]
                    row = [i.replace('<br/>', '**') for i in row]
                    data.append(row)
            data = data[1:]
            data = [i for i in data if len(i) == 4]

            m = re.search('.*由于您取消次数过多.今日将不能继续受理您的订票请求*', html)
            if m:
                data.append({'id': 0, 'token': 0})
            else:
                order = soup.select('.long_button_u')[1]['onclick'].split('\'')[1]
                data.append({'id': order, 'token': token})
        except Exception:
            pass
        return data

    @classmethod
    def getOrderPayPage(cls, html):
        m = re.search('.*?席位已成功锁定.*?则系统将自动取消本次交易.*?', html)
        if m:
            soup = Bs.soup(html)
            table = soup.select('table.table_list')
            soup = Bs.soup(str(table[0]))
            tr = soup.select('tr')
            data = []

            for i, v in enumerate(tr):
                if i in [1, 2]:
                    td = v.find_all('td')
                    row = []
                    for j in td:
                        row.append(j.string)
                    data.append(row)
            return data

    @classmethod
    def deleteUnPayTicket(cls, html):
        m = re.search('.*取消订单成功.*', html)
        if m:
            return {'s': 'Y'}

        m = re.search('.*系统忙.请稍后再试.*', html)
        if m:
            return {'s': 'N', 'e': '系统忙 请稍后再试'}

        m = re.search('.*请不要重复提交.*', html)
        if m:
            return {'s': 'N', 'e': '请不要重复提交'}

        return {'s': 'N', 'e': ''}

    @classmethod
    def getOrderPageInfo(cls, html):
        token = re.sub(".*globalRepeatSubmitToken\s*=\s*'([^']+)';.*", r'\1', html, flags=re.S)
        key_check_isChange = re.sub(".*?'key_check_isChange'\s*:\s*'([^']*)'.*", r'\1', html, flags=re.S)
        leftTicketStr = re.sub(".*?'leftTicketStr'\s*:\s*'([^']*)'.*", r'\1', html, flags=re.S)
        f = re.sub('.*?请核对以下信息(.*)乘车人信息.*', r'\1', html, flags=re.S)

        #selector = pq(f)
        #df = []
        #
        #for i in selector.find('strong'):
        #    df.append(pq(i).html())
        #
        #print({'token': token, 'key_check_isChange': key_check_isChange, 'leftTicketStr': leftTicketStr,
        #       'orderTips': '***'.join(df)})

        return {'token': token, 'key_check_isChange': key_check_isChange, 'leftTicketStr': leftTicketStr}
