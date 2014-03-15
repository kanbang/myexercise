from PyQt4 import QtCore, QtGui
import uiTicketQuerySeatType
import uiTicketQueryPersonContacts
import alertWindow
import datetime
import time
import json


class WaitStartQuery(QtCore.QThread):
    def __init__(self, ctr):
        QtCore.QThread.__init__(self, parent=None)
        self.ctr = ctr
        self.stop_ = False

    def stop(self):
        self.stop_ = True

    def run(self):
        while True and not self.stop_:
            if int(time.time()) >= int(self.ctr.getStartQueryUnixTime()):
                break
            else:
                time.sleep(1)


class KeepQuery(QtCore.QThread):
    def __init__(self, ctr, student):
        QtCore.QThread.__init__(self, parent=None)
        self.ctr = ctr
        self.stop_ = False
        self.student = student

    def stop(self):
        self.stop_ = True

    def run(self):
        while True and not self.stop_ and self.ctr.autoTicketQuery:
            if self.ctr.conditionQuery(self.student, True):
                self.stop_ = True
                self.alive = False
            else:
                time.sleep(int(self.ctr.getIntervalRefreshTime()))


class TrainPassTypeButton(QtGui.QCheckBox):
    allButton = {}

    def __init__(self, ID):
        super(TrainPassTypeButton, self).__init__()
        self.setMouseTracking(True)
        self.ID = ID
        self.allButton.update({ID: self})

    def mouseReleaseEvent(self, event):

        if self.isChecked():
            self.setChecked(False)
            if self.ID == 'QB':
                self.disableAll()
            else:
                self.checkButtonStatus()
        else:
            self.setChecked(True)
            if self.ID == 'QB':
                self.enableAll()
            else:
                self.checkButtonStatus()

    def disableAll(self):
        for i, v in self.allButton.items():
            v.setChecked(False)

    def enableAll(self):
        for i, v in self.allButton.items():
            v.setChecked(True)

    def checkButtonStatus(self):
        allChecked = True
        for i, v in self.allButton.items():
            if i == 'QB':
                continue
            else:
                if not v.isChecked():
                    allChecked = False

        self.allButton['QB'].setChecked(allChecked)


class ButtonDelegate(QtGui.QItemDelegate):
    def __init__(self, parent=None):
        QtGui.QItemDelegate.__init__(self, parent)
        self.width = 55
        self.height = 29

    def createEditor(self, parent, option, index):
        button = QtGui.QPushButton(parent)
        button.setStyleSheet('QPushButton{border:solid}')
        button.resize(100, 10)

        left = option.rect.x() + (option.rect.width() - self.width) / 2
        top = option.rect.y() + (option.rect.height() - self.height) / 2
        button.setGeometry(left, top, self.width, self.height)
        return button

    def paint(self, painter, option, index):
        opt = QtGui.QStyleOptionButton()
        opt.text = '订票'
        left = option.rect.x() + (option.rect.width() - self.width) / 2
        top = option.rect.y() + (option.rect.height() - self.height) / 2
        opt.rect = QtCore.QRect(left, top, self.width, self.height)
        QtGui.QApplication.style().drawControl(QtGui.QStyle.CE_PushButton, opt, painter)

    def updateEditorGeometry(self, editor, option, index):
        pass


class QueryTableModel(QtCore.QAbstractTableModel):
    def __init__(self, ctr, parent=None):
        super(QueryTableModel, self).__init__(parent)

        self.ctr = ctr
        self.trainData = None
        self.horizontalHeaderLabels = ['车次', '发站', '到站', '出发时间', '历时', '商务座', '特等座', '一等座', '二等座', '高级软座', '软卧', '硬卧',
                                       '软座', '硬座', '无座', '其他', '订票']

        self.headerFields = ['station_train_code', 'start_station_name', 'end_station_name', 'start_time', 'lishi',
                             'swz_num',
                             'tz_num', 'zy_num', 'ze_num', 'gr_num', 'rw_num', 'yw_num', 'rz_num', 'yz_num', 'wz_num',
                             'qt_num', 'buttonTextInfo']

        self.queryData = []

    def rowCount(self, parent=QtCore.QModelIndex()):
        return len(self.queryData) if not self.queryData is None else 0

    def columnCount(self, parent=QtCore.QModelIndex()):
        return len(self.horizontalHeaderLabels)

    def headerData(self, col, orientation, role):
        if orientation == QtCore.Qt.Horizontal and role == QtCore.Qt.DisplayRole:
            return self.horizontalHeaderLabels[col]

    def getTrainString(self, row):
        try:
            return self.queryData[row][16]
        except Exception:
            return None

    def data(self, index, role=QtCore.Qt.DisplayRole):
        if not index.isValid():
            return None
        if role in [QtCore.Qt.DisplayRole, QtCore.Qt.EditRole]:
            row = index.row()
            col = index.column()

            try:
                return self.queryData[row][self.headerFields[col]]
            except Exception:
                pass

    def getTrainData(self):
        if not self.trainData is None and len(self.trainData) > 0:
            return self.trainData
        else:
            return None

    def updateData(self, data):
        self.emit(QtCore.SIGNAL("layoutAboutToBeChanged()"))
        self.trainData = eval(data, type('Dummy', (dict,), dict(__getitem__=lambda s, n: n))())
        self.queryData = self.trainData['train']
        self.trainData.pop('train')
        self.emit(QtCore.SIGNAL("layoutChanged()"))

    def clear(self):
        self.emit(QtCore.SIGNAL("layoutAboutToBeChanged()"))
        self.queryData = []
        self.emit(QtCore.SIGNAL("layoutChanged()"))


class QueryTableView(QtGui.QTableView):
    def __init__(self, parent=None):
        super(QueryTableView, self).__init__(parent)

        self.verticalHeader().setVisible(False)
        #self.horizontalHeader().setVisible(False)
        self.setEditTriggers(QtGui.QAbstractItemView.NoEditTriggers)
        self.horizontalHeader().setResizeMode(QtGui.QHeaderView.Stretch)
        #self.verticalHeader().setResizeMode(QtGui.QHeaderView.Stretch)
        self.setVerticalScrollMode(QtGui.QAbstractItemView.ScrollPerItem)
        self.setFrameShape(QtGui.QFrame.NoFrame)
        self.resizeRowsToContents()


class TickerQueryPanel(QtGui.QWidget):
    def __init__(self, ctr, parent=None):
        super(TickerQueryPanel, self).__init__(parent)
        self.ctr = ctr
        self.built = False
        self.from_station_telecode = ''
        self.to_station_telecode = ''
        #00 student : 0X00
        self.includeStudent = 'ADULT'
        self.passengers = []
        self.seatTypes = []
        self.fromStationName = ''
        self.toStationName = ''
        self.autoTicketQuery = None
        self.clock = None
        self.ctr.ctr.signalUpdateTrainDate.connect(self.trainDates)
        self.lastQueryTIme = 0

    def build(self, setting):
        #__________*********query setting*********__________
        self.roundButtonGroup = QtGui.QButtonGroup()
        self.singleRound = QtGui.QRadioButton()
        self.singleRound.setCursor(QtCore.Qt.PointingHandCursor)
        self.singleRoundText = QtGui.QLabel('单程')
        self.singleRound.setChecked(True)
        self.connect(self.singleRound, QtCore.SIGNAL("clicked()"), lambda: self.ticketRoundChange(1))

        #__________*********往返*********__________
        self.doubleRound = QtGui.QRadioButton()
        self.doubleRound.setCursor(QtCore.Qt.PointingHandCursor)
        self.doubleRoundText = QtGui.QLabel('往返')

        self.doubleRoundDate = QtGui.QDateEdit()
        self.doubleRoundDate.setDisplayFormat(("yyyy.MM.dd"))
        self.doubleRoundDate.setVisible(False)
        self.doubleRoundDateText = QtGui.QLabel('返回日期:')
        self.doubleRoundDateText.setVisible(False)

        self.roundButtonGroup.addButton(self.singleRound)
        self.roundButtonGroup.addButton(self.doubleRound)
        self.connect(self.doubleRound, QtCore.SIGNAL("clicked()"), lambda: self.ticketRoundChange(2))

        # self.doubleRoundTime = QtGui.QComboBox()
        # self.doubleRoundTime.addItem('00:00--24:00', '00:00--24:00')
        # self.doubleRoundTime.addItem('00:00--06:00', '00:00--06:00')
        # self.doubleRoundTime.addItem('06:00--12:00', '06:00--12:00')
        # self.doubleRoundTime.addItem('12:00--18:00', '12:00--18:00')
        # self.doubleRoundTime.addItem('18:00--24:00', '18:00--24:00')
        # self.doubleRoundTime.setCursor(QtCore.Qt.PointingHandCursor)
        # self.doubleRoundTime.setVisible(False)
        #
        # self.doubleRoundTimeText = QtGui.QLabel('返回时间:')
        # self.doubleRoundTimeText.setVisible(False)

        #__________*********往返*********__________
        #
        # self.studentButton = QtGui.QPushButton('学生票查询')
        # self.connect(self.studentButton, QtCore.SIGNAL("clicked()"), lambda: self.query('0X00'))
        # self.studentButton.setCursor(QtCore.Qt.PointingHandCursor)

        self.normalSearchButton = QtGui.QPushButton('普通票查询')
        self.normalSearchButton.setCursor(QtCore.Qt.PointingHandCursor)
        self.connect(self.normalSearchButton, QtCore.SIGNAL("clicked()"), lambda: self.query('ADULT'))

        hLayout = QtGui.QHBoxLayout()
        hLayout.addWidget(self.singleRound)
        hLayout.addWidget(self.singleRoundText)

        hLayout.addWidget(self.doubleRound)
        hLayout.addWidget(self.doubleRoundText)

        hLayout.addWidget(self.doubleRoundDateText)
        hLayout.addWidget(self.doubleRoundDate)
        #
        # hLayout.addWidget(self.doubleRoundTimeText)
        # hLayout.addWidget(self.doubleRoundTime)

        hLayout.addStretch()
        # hLayout.addWidget(self.studentButton)
        hLayout.addWidget(self.normalSearchButton)

        hLayout.setAlignment(QtCore.Qt.AlignLeft | QtCore.Qt.AlignVCenter)
        hLayout.setSpacing(10)

        self.fromStationNameInput = QtGui.QLineEdit()
        fromStationNameText = QtGui.QLabel('出发地:')

        self.toStationNameInput = QtGui.QLineEdit()
        toStationNameText = QtGui.QLabel('目的地:')

        self.trainDate = QtGui.QDateEdit()
        self.trainDate.setDisplayFormat(("yyyy.MM.dd"))
        trainDateText = QtGui.QLabel('出发日期:')

        self.trainNo = QtGui.QLineEdit()
        trainNoText = QtGui.QLabel('出发车次:')
        self.connect(self.trainNo, QtCore.SIGNAL("textChanged(const QString &)"), self.setFavoriteTrain)

        self.startTimeString = QtGui.QComboBox()
        self.startTimeString.addItem('00:00--23:59', '00:00--23:59')
        self.startTimeString.addItem('00:00--06:00', '00:00--06:00')
        self.startTimeString.addItem('06:00--12:00', '06:00--12:00')
        self.startTimeString.addItem('06:00--18:00', '06:00--18:00')
        self.startTimeString.addItem('12:00--18:00', '12:00--18:00')
        self.startTimeString.addItem('12:00--20:00', '12:00--20:00')
        self.startTimeString.addItem('12:00--23:59', '12:00--23:59')
        self.startTimeString.addItem('18:00--23:59', '18:00--23:59')
        self.startTimeString.setCursor(QtCore.Qt.PointingHandCursor)
        startTimeStringText = QtGui.QLabel('出发时间:')

        gLayout = QtGui.QGridLayout()
        gLayout.addWidget(fromStationNameText, 0, 0, 1, 1)
        gLayout.addWidget(self.fromStationNameInput, 0, 1, 1, 1)
        gLayout.addWidget(toStationNameText, 0, 2, 1, 1)
        gLayout.addWidget(self.toStationNameInput, 0, 4, 1, 1)
        gLayout.addWidget(trainDateText, 0, 5, 1, 1)
        gLayout.addWidget(self.trainDate, 0, 6, 1, 1)

        gLayout.addWidget(trainNoText, 1, 0, 1, 1)
        gLayout.addWidget(self.trainNo, 1, 1, 1, 1)

        gLayout.addWidget(startTimeStringText, 1, 5, 1, 1)
        gLayout.addWidget(self.startTimeString, 1, 6, 1, 1)

        gLayout.setSpacing(10)

        self.trainTypeQBCheckBox = TrainPassTypeButton('QB')
        self.trainTypeQBCheckBox.setCursor(QtCore.Qt.PointingHandCursor)
        trainTypeQB = QtGui.QLabel('全部')

        self.trainTypeDCheckBox = TrainPassTypeButton('D')
        self.trainTypeDCheckBox.setCursor(QtCore.Qt.PointingHandCursor)
        trainTypeD = QtGui.QLabel('动车')

        self.trainTypeZCheckBox = TrainPassTypeButton('Z')
        self.trainTypeZCheckBox.setCursor(QtCore.Qt.PointingHandCursor)
        trainTypeZ = QtGui.QLabel('Z字头')

        self.trainTypeTCheckBox = TrainPassTypeButton('T')
        self.trainTypeTCheckBox.setCursor(QtCore.Qt.PointingHandCursor)
        train_type_T = QtGui.QLabel('T字头')

        self.trainTypeKCheckBox = TrainPassTypeButton('K')
        self.trainTypeKCheckBox.setCursor(QtCore.Qt.PointingHandCursor)
        trainTypeK = QtGui.QLabel('K字头')

        # self.trainTypeQTCheckBox = TrainPassTypeButton('QT')
        # self.trainTypeQTCheckBox.setCursor(QtCore.Qt.PointingHandCursor)
        # trainTypeQT = QtGui.QLabel('其他')

        trainTypeLayout = QtGui.QHBoxLayout()
        trainTypeLayout.addWidget(self.trainTypeQBCheckBox)
        trainTypeLayout.addWidget(trainTypeQB)

        trainTypeLayout.addWidget(self.trainTypeDCheckBox)
        trainTypeLayout.addWidget(trainTypeD)

        trainTypeLayout.addWidget(self.trainTypeZCheckBox)
        trainTypeLayout.addWidget(trainTypeZ)

        trainTypeLayout.addWidget(self.trainTypeTCheckBox)
        trainTypeLayout.addWidget(train_type_T)

        trainTypeLayout.addWidget(self.trainTypeKCheckBox)
        trainTypeLayout.addWidget(trainTypeK)

        # trainTypeLayout.addWidget(self.trainTypeQTCheckBox)
        # trainTypeLayout.addWidget(trainTypeQT)
        trainTypeLayout.addStretch()

        # self.trainPassTypeButtonGroup = QtGui.QButtonGroup()
        # trainPassTypeQB = QtGui.QLabel('全部')
        # self.trainTypeQBRadioButton = QtGui.QRadioButton()
        # self.trainTypeQBRadioButton.setCursor(QtCore.Qt.PointingHandCursor)
        #
        # trainPassTypeSF = QtGui.QLabel('始发')
        # self.trainTypeSFRadionButton = QtGui.QRadioButton()
        # self.trainTypeSFRadionButton.setCursor(QtCore.Qt.PointingHandCursor)
        #
        # trainPassTypeLG = QtGui.QLabel('路过')
        # self.trainTypeLGRadioButton = QtGui.QRadioButton()
        # self.trainTypeLGRadioButton.setCursor(QtCore.Qt.PointingHandCursor)
        #
        # self.trainPassTypeButtonGroup.addButton(self.trainTypeQBRadioButton)
        # self.trainPassTypeButtonGroup.addButton(self.trainTypeSFRadionButton)
        # self.trainPassTypeButtonGroup.addButton(self.trainTypeLGRadioButton)

        # trainPassTypeLayout = QtGui.QHBoxLayout()
        # trainPassTypeLayout.addStretch()
        # trainPassTypeLayout.addWidget(self.trainTypeQBRadioButton)
        # trainPassTypeLayout.addWidget(trainPassTypeQB)
        #
        # trainPassTypeLayout.addWidget(self.trainTypeSFRadionButton)
        # trainPassTypeLayout.addWidget(trainPassTypeSF)
        #
        # trainPassTypeLayout.addWidget(self.trainTypeLGRadioButton)
        # trainPassTypeLayout.addWidget(trainPassTypeLG)

        trainTypesLayout = QtGui.QHBoxLayout()
        trainTypesLayout.addLayout(trainTypeLayout)
        trainTypesLayout.addStretch()
        # trainTypesLayout.addLayout(trainPassTypeLayout)

        layout = QtGui.QVBoxLayout()

        layout.addLayout(hLayout)
        layout.addLayout(gLayout)
        layout.addLayout(trainTypesLayout)

        #__________*********train*********__________
        self.ticketTable = self.buildTrainTable()
        ticketTableLayout = QtGui.QHBoxLayout()
        spacerItem = QtGui.QSpacerItem(0, 20000, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        ticketTableLayout.addWidget(self.ticketTable)
        ticketTableLayout.addItem(spacerItem)
        layout.addLayout(ticketTableLayout)

        #__________*********setting*********__________
        intervalRefreshTimeText = QtGui.QLabel('刷新时间间隔:')
        self.intervalRefreshTime = QtGui.QComboBox()
        self.intervalRefreshTime.addItem('4', '4')
        self.intervalRefreshTime.addItem('5', '5')
        self.intervalRefreshTime.addItem('6', '6')
        self.intervalRefreshTime.addItem('7', '7')
        self.connect(self.intervalRefreshTime, QtCore.SIGNAL("currentIndexChanged(QString)"),
                     self.setIntervalRefreshTime)

        autoTicketQueryText = QtGui.QLabel('自动刷新')
        self.autoTicketQuery_ = QtGui.QCheckBox()
        self.autoTicketQuery_.setFixedSize(17, 17)
        self.connect(self.autoTicketQuery_, QtCore.SIGNAL("clicked()"), self.setAutoTicketQuery)

        minimumTicketNumberText = QtGui.QLabel('最小票数:')
        self.minimumTicketNumber = QtGui.QComboBox()
        self.minimumTicketNumber.addItem('1', '1')
        self.minimumTicketNumber.addItem('2', '2')
        self.minimumTicketNumber.addItem('3', '3')
        self.minimumTicketNumber.addItem('4', '4')
        self.connect(self.minimumTicketNumber, QtCore.SIGNAL("currentIndexChanged(QString)"),
                     self.setMinimumTicketNumber)

        # sleepBeddingTypeText = QtGui.QLabel('卧铺优先选择:')
        # self.sleepBeddingType = QtGui.QComboBox()
        # self.sleepBeddingType.addItem('随机', '0')
        # self.sleepBeddingType.addItem('上铺', '1')
        # self.sleepBeddingType.addItem('中铺', '2')
        # self.sleepBeddingType.addItem('下铺', '3')
        # self.connect(self.sleepBeddingType, QtCore.SIGNAL("currentIndexChanged(QString)"), self.setSleepBeddingType)

        self.startQueryTime = QtGui.QTimeEdit()
        startQueryTImeText = QtGui.QLabel('查询有效开始点')
        self.startQueryTime.setDisplayFormat(("hh:mm:ss"))
        self.connect(self.startQueryTime, QtCore.SIGNAL("timeChanged(QTime)"), self.setStartQueryTime)

        autoOrderTicketText = QtGui.QLabel('当找到符合T要求的车次和席别时，自动转到预定界面')
        self.autoOrderTicket = QtGui.QCheckBox()
        self.connect(self.autoOrderTicket, QtCore.SIGNAL("clicked()"), self.setAutoOrderTicket)

        autoTopSelectedTrainText = QtGui.QLabel('当找到符合要求的车次和席别时，顶部显示')
        self.autoTopSelectedTrain = QtGui.QCheckBox()
        self.connect(self.autoTopSelectedTrain, QtCore.SIGNAL("clicked()"), self.setAutoTopSelectedTrain)

        onlyDisplayFilterTrainText = QtGui.QLabel('只显示过滤后的车次')
        self.onlyDisplayFilterTrainBox = QtGui.QCheckBox()
        self.connect(self.onlyDisplayFilterTrainBox, QtCore.SIGNAL("clicked()"), self.onlyDisplayFilterTrain)

        self.ticketQuerySeatTypeTable = self.buildTicketQuerySeatTypeTable()
        self.ticketQueryPersonContactsTable = self.buildTicketQueryPersonContactsTable()

        hlayout = QtGui.QHBoxLayout()

        hlayout.addWidget(intervalRefreshTimeText)
        hlayout.addWidget(self.intervalRefreshTime)

        hlayout.addWidget(autoTicketQueryText)
        hlayout.addWidget(self.autoTicketQuery_)

        hlayout.addWidget(minimumTicketNumberText)
        hlayout.addWidget(self.minimumTicketNumber)

        # hlayout.addWidget(sleepBeddingTypeText)
        #hlayout.addWidget(self.sleepBeddingType)

        hlayout.addWidget(startQueryTImeText)
        hlayout.addWidget(self.startQueryTime)
        hlayout.addStretch()

        hlayout2 = QtGui.QHBoxLayout()
        hlayout2.addWidget(self.autoOrderTicket)
        hlayout2.addWidget(autoOrderTicketText)
        hlayout2.addWidget(self.autoTopSelectedTrain)
        hlayout2.addWidget(autoTopSelectedTrainText)

        hlayout2.addWidget(self.onlyDisplayFilterTrainBox)
        hlayout2.addWidget(onlyDisplayFilterTrainText)
        hlayout2.setAlignment(QtCore.Qt.AlignLeft | QtCore.Qt.AlignHCenter)

        vlayout2 = QtGui.QVBoxLayout()
        vlayout2.addLayout(hlayout)
        vlayout2.addLayout(hlayout2)
        layout.addLayout(vlayout2)

        layout.addWidget(self.ticketQuerySeatTypeTable)
        layout.addWidget(self.ticketQueryPersonContactsTable)
        layout.addStretch()

        self.setLayout(layout)
        self.built = True

    def buildTicketQueryPersonContactsTable(self):
        ticketQueryPersonContactsTable = uiTicketQueryPersonContacts.uiTicketQueryPersonContacts(self)
        ticketQueryPersonContactsTable.build()
        return ticketQueryPersonContactsTable

    def buildTicketQuerySeatTypeTable(self):
        ticketQuerySeatTypeTable = uiTicketQuerySeatType.uiTicketQuerySeatType(self)
        ticketQuerySeatTypeTable.build()
        return ticketQuerySeatTypeTable

    def buildTrainTable(self):

        self.model = QueryTableModel(self)
        table = QueryTableView()
        table.doubleClicked.connect(lambda x: self.cellClick(x))
        #table.setItemDelegateForColumn(16, ButtonDelegate(table))
        table.setModel(self.model)
        return table


    def cellClick(self, cell):
        row = cell.row()
        col = cell.column()
        if col == 16:
            trainString = self.model.getTrainString(row)
            if trainString.startswith('<a'):
                alertWindow.AlertWindow(text='当前车次不能预订车票').exec_()
            else:
                self.bookTicket(trainString)

    def bookTicket(self, trainString):
        self.ctr.bookTicket(trainString)

    def ticketsQuery(self, query, loop):
        self.model.clear()
        return self.ctr.ticketsQuery(query, loop)

    def ticketRoundChange(self, type):

        def enable():
            self.doubleRoundDate.setVisible(True)
            self.doubleRoundDateText.setVisible(True)
            # self.doubleRoundTime.setVisible(True)
            # self.doubleRoundTimeText.setVisible(True)

        def disable():
            self.doubleRoundDate.setVisible(False)
            self.doubleRoundDateText.setVisible(False)
            # self.doubleRoundTime.setVisible(False)
            # self.doubleRoundTimeText.setVisible(False)

        if type == 1:
            if self.singleRound.isChecked():
                disable()
            else:
                enable()

        else:
            if self.doubleRound.isChecked():
                enable()
            else:
                disable()

    def bindSetting(self, setting):
        if setting['ticketQuerySetting'] is None or len(setting['ticketQuerySetting']) == 0:
            return

        ticketQuerySetting = setting['ticketQuerySetting']

        if int(ticketQuerySetting['singleRoundType']) == 1:
            self.singleRound.setChecked(True)
            self.ticketRoundChange(1)
        else:
            self.doubleRound.setChecked(True)
            self.ticketRoundChange(2)
            roundTrainDate = ticketQuerySetting['orderRequest.roundTrainDate'].split('-')
            qD = QtCore.QDate(int(roundTrainDate[0]), int(roundTrainDate[1]), int(roundTrainDate[2]))
            self.doubleRoundDate.setDate(qD)

            # for i in range(self.doubleRoundTime.count()):
            #     if self.doubleRoundTime.itemData(i) == ticketQuerySetting['roundStartTimeStr']:
            #         self.doubleRoundTime.setCurrentIndex(i)
            #         break

        self.fromStationNameInput.setText(ticketQuerySetting['orderRequest.from_station_name'])
        self.fromStationName = ticketQuerySetting['orderRequest.from_station_name']

        self.toStationNameInput.setText(ticketQuerySetting['orderRequest.to_station_name'])
        self.toStationName = ticketQuerySetting['orderRequest.to_station_name']

        self.from_station_telecode = ticketQuerySetting['orderRequest.from_station_telecode']
        self.to_station_telecode = ticketQuerySetting['orderRequest.to_station_telecode']

        orderRequest = ticketQuerySetting['orderRequest.train_date'].split('-')
        qD = QtCore.QDate(int(orderRequest[0]), int(orderRequest[1]), int(orderRequest[2]))
        self.trainDate.setDate(qD)

        for i in range(self.startTimeString.count()):
            if self.startTimeString.itemData(i) == ticketQuerySetting['orderRequest.start_time_str']:
                self.startTimeString.setCurrentIndex(i)
                break

        trainClassArr = ticketQuerySetting['trainClassArr'].split('#')

        for i in trainClassArr:
            if i == 'QB':
                self.trainTypeQBCheckBox.setChecked(True)
            elif i == 'D':
                self.trainTypeDCheckBox.setChecked(True)
            elif i == 'Z':
                self.trainTypeZCheckBox.setChecked(True)
            elif i == 'K':
                self.trainTypeKCheckBox.setChecked(True)
            elif i == 'T':
                self.trainTypeTCheckBox.setChecked(True)
                # elif i == 'QT':
                #     self.trainTypeQTCheckBox.setChecked(True)

        # trainPassType = ticketQuerySetting['trainPassType'].split('#')
        #
        # for i in trainPassType:
        #     if i == 'QB':
        #         self.trainTypeQBRadioButton.setChecked(True)
        #     if i == 'SF':
        #         self.trainTypeSFRadionButton.setChecked(True)
        #     if i == 'LG':
        #         self.trainTypeLGRadioButton.setChecked(True)

        ###################################################################
        defaultSetting = setting['defaultSetting']

        self.trainNo.setText(','.join(defaultSetting['favoriteTrain']))

        if int(defaultSetting['autoTicketQuery']) == 1:
            self.autoTicketQuery_.setChecked(True)
            self.autoTicketQuery = True
        else:
            self.autoTicketQuery_.setChecked(False)
            self.autoTicketQuery = False

        for i in range(self.intervalRefreshTime.count()):
            if self.intervalRefreshTime.itemData(i) == defaultSetting['intervalRefreshTime']:
                self.intervalRefreshTime.setCurrentIndex(i)
                break

        for i in range(self.minimumTicketNumber.count()):
            if self.minimumTicketNumber.itemData(i) == defaultSetting['minimumTicketNumber']:
                self.minimumTicketNumber.setCurrentIndex(i)
                break
                #
                # for i in range(self.sleepBeddingType.count()):
                #     if self.sleepBeddingType.itemData(i) == defaultSetting['sleepBeddingType']:
                #         self.sleepBeddingType.setCurrentIndex(i)
                #         break

        if int(defaultSetting['autoOrderTicket']) == 1:
            self.autoOrderTicket.setChecked(True)
        else:
            self.autoOrderTicket.setChecked(False)

        if int(defaultSetting['autoTopSelectedTrain']) == 1:
            self.autoTopSelectedTrain.setChecked(True)
        else:
            self.autoTopSelectedTrain.setChecked(False)

        if int(defaultSetting['onlyDisplayFilterTrain']) == 1:
            self.onlyDisplayFilterTrainBox.setChecked(True)
        else:
            self.onlyDisplayFilterTrainBox.setChecked(False)

        if not defaultSetting['startQueryTime'] is None:
            startQueryTime = defaultSetting['startQueryTime'].split(':')
            qT = QtCore.QTime(int(startQueryTime[0]), int(startQueryTime[1]), int(startQueryTime[2]))
            self.startQueryTime.setTime(qT)

    def setQueryButton(self, statues):
        # self.studentButton.setEnabled(statues)
        self.normalSearchButton.setEnabled(statues)

    def getStartQueryUnixTime(self):
        queryTime = self.startQueryTime.time()
        now = datetime.date.fromtimestamp(time.time())
        queryTime = datetime.datetime(now.year, now.month, now.day, queryTime.hour(), queryTime.minute(),
                                      queryTime.second())

        return time.mktime(queryTime.timetuple())

    def query(self, student):

        #print('******query******')
        if time.time() - self.lastQueryTIme < 2:
            return
            #print('******query2******')
        #self.setQueryButton(False)
        self.lastQueryTIme = time.time()

        if int(time.time()) >= int(self.getStartQueryUnixTime()):
            #print('******query3******')
            if self.autoTicketQuery:
                #print('******query4******')
                self.thread = KeepQuery(self, student)
                self.thread.start()
                self.connect(self.thread, QtCore.SIGNAL("finished()"), self.finished)
            else:
                #print('******query5******')
                try:
                    self.thread.stop()
                    self.thread.exit()
                except Exception:
                    pass

                self.conditionQuery(student)
                self.setQueryButton(True)
        else:
            #print(6)
            self.waitStartQuery = WaitStartQuery(self)
            self.waitStartQuery.start()
            self.connect(self.waitStartQuery, QtCore.SIGNAL("finished()"), lambda: self.query(student))


    def finished(self):

        trainData = self.model.getTrainData()
        print('##########finished##########')
        print(trainData)

        if not trainData is None and not trainData['seat'] is None and len(trainData['seat']) > 0:
            self.ticketsQuery(trainData, False)
        else:
            self.conditionQuery(self.includeStudent)

        self.setQueryButton(True)

    def conditionQuery(self, student, loop=False):
        self.includeStudent = student
        query = {}
        query['includeStudent'] = self.includeStudent

        query['seatTypeAndNum'] = ''
        query['roundTrainDate'] = ''
        query['roundStartTimeStr'] = ''
        query['orderRequest.from_station_name'] = self.fromStationNameInput.text().strip()

        if len(query['orderRequest.from_station_name']) == 0:
            self.popWd('出发地不能为空', loop)
            return False

        query['orderRequest.to_station_name'] = self.toStationNameInput.text().strip()

        if len(query['orderRequest.to_station_name']) == 0:
            self.popWd('出发地不能为空', loop)
            return False

        query['orderRequest.from_station_telecode'] = self.getStationCode(query['orderRequest.from_station_name'])
        query['orderRequest.to_station_telecode'] = self.getStationCode(query['orderRequest.to_station_name'])

        if query['orderRequest.from_station_telecode'] is None:
            self.popWd('目的地不能为空', loop)
            return False

        if query['orderRequest.to_station_telecode'] is None:
            self.popWd('目的地不正确', loop)
            return False

        dayObject = self.trainDate.date()

        train_date_ = datetime.date(dayObject.year(), dayObject.month(), dayObject.day())
        train_date_time = time.mktime(train_date_.timetuple())
        days20 = datetime.timedelta(days=20)
        days20seconds = days20.total_seconds()
        nowTimes = time.time()

        if train_date_time > nowTimes + days20seconds:
            self.popWd('出发日期不对 已大于20天', loop)
            return False

        query['orderRequest.train_date'] = '-'.join(
            [str(dayObject.year()), str(dayObject.month() if dayObject.month() > 9 else '0' + str(dayObject.month())),
             str(dayObject.day() if dayObject.day() > 9 else '0' + str(dayObject.day()))])
        query['orderRequest.trainCodeText'] = '' #self.trainNo.text()
        query['orderRequest.start_time_str'] = self.startTimeString.currentText()

        if (self.singleRound.isChecked()):
            query['singleRoundType'] = 1
            query['roundTrainDate'] = query['orderRequest.train_date']
            query['roundStartTimeStr'] = query['orderRequest.start_time_str']
        else:
            query['singleRoundType'] = 2
            dayObject = self.doubleRoundDate.date()

            roundTrainDate_ = datetime.date(dayObject.year(), dayObject.month(), dayObject.day())
            roundTrainDate_time = time.mktime(roundTrainDate_.timetuple())
            days20 = datetime.timedelta(days=20)
            days20seconds = days20.total_seconds()
            nowTimes = time.time()

            if roundTrainDate_time > nowTimes + days20seconds:
                self.popWd('往返日期不对', loop)
                return False

            query['orderRequest.roundTrainDate'] = '-'.join([str(dayObject.year()), str(
                dayObject.month() if dayObject.month() > 9 else '0' + str(dayObject.month())),
                                                             str(dayObject.day() if dayObject.day() > 9 else '0' + str(
                                                                 dayObject.day()))])
            # query['roundStartTimeStr'] = self.doubleRoundTime.currentText()

        trainClassArr = []

        if self.trainTypeQBCheckBox.isChecked():
            trainClassArr.append('QB#')

        if self.trainTypeDCheckBox.isChecked():
            trainClassArr.append('D#')

        if self.trainTypeZCheckBox.isChecked():
            trainClassArr.append('Z#')

        if self.trainTypeTCheckBox.isChecked():
            trainClassArr.append('T#')

        if self.trainTypeKCheckBox.isChecked():
            trainClassArr.append('K#')
            #
        # if self.trainTypeQTCheckBox.isChecked():
        #     trainClassArr.append('QT#')

        query['trainClassArr'] = ''.join(trainClassArr)

        # trainPassType = []
        # if self.trainTypeQBRadioButton.isChecked():
        #     trainPassType.append('QB')
        #
        # if self.trainTypeSFRadionButton.isChecked():
        #     trainPassType.append('SF')
        #
        # if self.trainTypeLGRadioButton.isChecked():
        #     trainPassType.append('LG')
        #
        # query['trainPassType'] = '#'.join(trainPassType)
        return self.ticketsQuery(query, loop)

    def trainDates(self, trainDates):
        self.model.updateData(json.dumps(trainDates))

    def popWd(self, text_, loop):

        if loop:
            return
        else:
            alertWindow.AlertWindow(text=text_).exec_()

    def getStationCode(self, station):
        return self.ctr.getStationCode(station)

    def setPassengers(self, passengers):
        self.ctr.setPassengers(passengers)

    def getPassengersCount(self):
        return self.ctr.getPassengersCount()

    def deletePassengers(self, passengers):
        return self.ctr.deletePassengers(passengers)

    def setFavoriteTrain(self):
        self.ctr.setFavoriteTrain(self.trainNo.text().strip().rstrip(','))

        #时间间隔

    def setIntervalRefreshTime(self):
        self.ctr.setIntervalRefreshTime(self.intervalRefreshTime.currentText())

    #最小座位数
    def setMinimumTicketNumber(self):
        self.ctr.setMinimumTicketNumber(self.minimumTicketNumber.currentText())

    def setAutoTicketQuery(self):
        self.autoTicketQuery = self.autoTicketQuery_.isChecked()
        self.ctr.setAutoTicketQuery(int(self.autoTicketQuery_.isChecked()))

    #期望座位类型
    def setSeatType(self, seatTypes):
        self.ctr.setSeatType(seatTypes)

    def removeSeatType(self, seat):
        self.seatType.remove(seat)

        #

    # #期望卧铺座位类型
    # def setSleepBeddingType(self):
    #     self.ctr.setSleepBeddingType(self.sleepBeddingType.itemData(self.sleepBeddingType.currentIndex()))

    #是否自动订票
    def setAutoOrderTicket(self):
        self.ctr.setAutoOrderTicket(int(self.autoOrderTicket.isChecked()))

    def onlyDisplayFilterTrain(self):
        self.ctr.onlyDisplayFilterTrain(int(self.onlyDisplayFilterTrainBox.isChecked()))

    #匹配车次提前
    def setAutoTopSelectedTrain(self):
        self.ctr.setAutoTopSelectedTrain(int(self.autoTopSelectedTrain.isChecked()))

    def setStartQueryTime(self):
        time = self.startQueryTime.time()
        self.ctr.setStartQueryTime(str(time.hour()) + ':' + str(time.minute()) + ':' + str(time.second()))

    def getSeatType(self, col):
        return self.ctr.getSeatType(col)

    def getSeatTypeCount(self):
        return self.ctr.getSeatTypeCount()

    def getSettings(self, type):
        return self.ctr.getSettings(type)

    def deleteSeatType(self, seat):
        self.ctr.deleteSeatType(seat)

    def getPersonalContactsDetailColData(self, row, field):
        return self.ctr.getPersonalContactsDetailColData(row, field)

    def getPersonalContactsCounts(self):
        return self.ctr.getPersonalContactsCounts()

    def getPassengers(self, col):
        return self.ctr.getPassengers(col)

    def getIntervalRefreshTime(self):
        return self.ctr.getIntervalRefreshTime()
