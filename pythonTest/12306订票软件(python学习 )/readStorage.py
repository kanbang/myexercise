import db


class Storage():
    def __init__(self):
        self.db = db.Db()

    def readCommentSetting(self):
        data = dict()

        #读取default setting
        data['defaultSetting'] = self.db.getGroup('defaultSetting')

        #读取车票查询信息
        data['ticketQuerySetting'] = self.db.get('ticketQuerySetting')

        #读取登录信息
        data['loginData'] = self.db.getGroup('loginData')

        return data


if __name__ == '__main__':

    storage = Storage()
    data = storage.readCommentSetting()
    for i in data:
        print(i)
        print(data[i])