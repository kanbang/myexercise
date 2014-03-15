class Config(object):
    def __new__(cls, *args, **kw):
        if not hasattr(cls, '_instance'):
            orig = super(Config, cls)
            cls._instance = orig.__new__(cls, *args, **kw)
        return cls._instance

    data = {}

    #run storage
    data['softTitle'] = '尾尾订票:9ep.cn'
    data['personalDetail'] = None
    data['personalContacts'] = {}
    data['ticketPassengers'] = None
    data['orderTicketsLimit'] = 3
    #data['seat'] = ''
    data['keepAliveTime'] = 60
    #default filter / train setting
    data['defaultSetting'] = {
        'ticketPassengers': [],
        'startQueryTime': '00:00:00',
        'seatType': [],
        'sleepBeddingType': '3',
        'minimumTicketNumber': 2,
        'autoTicketQuery': 0,
        'favoriteTrain': [],
        'autoTopSelectedTrain': 0,
        'autoOrderTicket': 1,
        'onlyDisplayFilterTrain': 1,
        'intervalRefreshTime': 6,
        'orderTicketImageCode': {
            'createTime': '',
            'value': ''
        }
    }

    data['seatTypeInQuery'] = {
        'swz_num': '商务座',
        'tz_num': '特等座',
        'zy_num': '一等座',
        'ze_num': '二等座',
        'gr_num': '高级软卧',
        'rw_num': '软卧',
        'yw_num': '硬座',
        'rz_num': '软座',
        'yz_num': '硬座',
        'wz_num': '无座',
        'qt_num': '其他'

    }
    data['seatTypeArr'] = {

        '9': '商务座',
        'P': '特等座',
        'M': '一等座',
        'O': '二等座',
        '6': '高级软卧',
        '4': '软卧',
        '3': '硬卧',
        '2': '软座',
        '1': '硬座',
        'empty': '无座',
    }

    data['seatTypePrice'] = {
        '9': '商务座',
        'P': '特等座',
        'M': '一等座',
        'O': '二等座',
        '6': '高级软卧',
        '4': '软卧',
        '3': '硬卧',
        '2': '软座',
        '1': '硬座',
        'swz': '商务座',
        'tz': '特等座',
        'zy': '一等座',
        'ze': '二等座',
        'gr': '高级软卧',
        'rw': '软卧',
        'yw': '硬座',
        'rz': '软座',
        'yz': '硬座',
        'wz': '无座',
        'qt': '其他'
    }
    data['seatTypeToQuery'] = {

        '9': 'swz_num',
        'P': 'tz_num',
        'M': 'zy_num',
        'O': 'ze_num',
        '6': 'gr_num',
        '4': 'rw_num',
        '3': 'yw_num',
        '2': 'rz_num',
        '1': 'yz_num',
        'empty': 'wz_num',

    }

    data['sleepBeddingTypeArr'] = {
        '0': '随机',
        '3': '上铺',
        '2': '中铺',
        '1': '下铺',
    }

    data['cardTypeArr'] = {
        '1': '二代身份证',
        '2': '一代身份证',
        'C': '港澳通行证',
        'G': '台湾通行证',
        'B': '护照',
    }

    data['passengerTypeArr'] = {
        '1': '成人',
        '2': '儿童',
        '3': '学生',
        '4': '伤残军人',
    }

    def get(self, item):
        if item in self.data:
            return self.data[item]

    def set(self, item, value):
        self.data[item] = value

    def removeFromDictsSequence(self, sequence, item):
        if sequence in self.data and item in self.data[sequence]:
            del self.data[sequence][item]

    def extendDictsSequence(self, sequence, extend):

        if not self.data[sequence]:
            self.data[sequence] = {}

        self.data[sequence].update(extend)

    def setDictsSequence(self, sequence, item, value):
        if not self.data[sequence]:
            self.data[sequence] = {}

        self.data[sequence][item] = value

    def getDictsSequence(self, sequence, item=None):
        if sequence:
            if not item and sequence in self.data:
                return self.data[sequence]
            elif item and sequence in self.data and item in self.data[sequence]:
                return self.data[sequence][item]
        return []

    def getAll(self):
        return self.data

    def setDicts(self, dicts):
        for (k, v) in dicts.items():
            self.data[k] = v

    def updateSequenceDateFromStorage(self, storageData):
        if storageData and isinstance(storageData, dict):
            for i in storageData:
                if not i in self.data:
                    self.data[i] = storageData[i]
                else:
                    if isinstance(storageData[i], (list, tuple)):
                        self.data[i].extend(storageData[i])
                    elif isinstance(storageData[i], dict):
                        self.data[i].update(storageData[i])


if __name__ == '__main__':
    config = Config()
    config.extendDictsSequence('personalDetail', {'name': '111'})
    config.extendDictsSequence('personalDetail', {'sex': '111dfsfsdfsdfdsf'})
    config.extendDictsSequence('personalDetail', {'fffffffff': '111eeeeeeeeeeeeeee'})

    print(config.getAll()['personalDetail'])

    config.removeFromDictsSequence('personalDetail', 'sex')

    print(config.getAll()['personalDetail'])

    print(config.getDictsSequence('personalDetail', 'fffffffff'))

    seatTypeArrInCols = (config.get('seatTypeArrInCols'))

    print(seatTypeArrInCols.keys())


