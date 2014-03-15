import marshal


class Db():
    def __init__(self):
        self.fileName = 'storage.db'

        try:
            storage = open(self.fileName, 'rb')
            self.db = marshal.loads(storage.read())
            storage.close()
        except Exception:
            self.db = {}

    def set(self, item, value, groups=''):
        if item and not value is None:
            self.db[str(item)] = {'value': value, 'group': groups}

    def setDicts(self, dicts, groups=''):
        if dicts:
            for item, value in dicts.items():
                self.db[str(item)] = {'value': value, 'group': groups}

    def get(self, item):
        if item:
            try:
                value = self.db[str(item)]
                value2 = value['value']
                return value2

            except Exception:
                return None

    def getGroup(self, groups):
        groups_ = {}
        if groups:
            for i, v in self.db.items():
                if v['group'] == groups:
                    value = v['value']
                    groups_.update({i: value})
        return groups_

    def deleteGroup(self, groups):
        if groups:
            self.db = {i: v for i, v in self.db.items() if not v['group'] == str(groups)}

    def getAll(self):
        all_ = {}
        for i, v in self.db.items():
            all_.update({i: v})
        return all_

    def savingStorage(self):
        storage = open(self.fileName, 'wb')
        storage.write(marshal.dumps(self.db))
        storage.close()


if __name__ == '__main__':
    db = Db()
    #db.set('1', 2, 'defaultSetting')
    #db.set('01', 2, '10')
    #db.setDicts({11: 11, 221: 22, 33: 33}, groups='11110')
    #db.savingStorage()
    s = db.getAll()
    #db.deleteGroup('defaultSetting')

    for i, v in s.items():
        #pass
        print(i)
        print(v)
        #print(db.deleteGroup('defaultSetting'))

        #print(db.clear())
