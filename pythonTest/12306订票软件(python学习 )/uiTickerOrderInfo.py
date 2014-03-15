from PyQt4 import QtCore, QtGui
import alertWindow


class unPayOrderTableModel(QtCore.QAbstractTableModel):
    def __init__(self, ctr, parent=None):
        super(unPayOrderTableModel, self).__init__(parent)
        self.ctr = ctr
        self.horizontalHeaderLabels = ['车次信息', '座位信息', '旅客信息', '车票状态']
        self.ticketData = []

    def rowCount(self, parent=QtCore.QModelIndex()):
        return len(self.ticketData) - 1

    def columnCount(self, parent=QtCore.QModelIndex()):
        return len(self.horizontalHeaderLabels)

    def headerData(self, col, orientation, role):
        if orientation == QtCore.Qt.Horizontal and role == QtCore.Qt.DisplayRole:
            return self.horizontalHeaderLabels[col]

    def data(self, index, role=QtCore.Qt.DisplayRole):
        if not index.isValid():
            return None
        if role in [QtCore.Qt.DisplayRole, QtCore.Qt.EditRole]:
            row = index.row()
            col = index.column()
            return self.ticketData[row][col]

    def payTicket(self, id):
        self.emit(QtCore.SIGNAL("layoutAboutToBeChanged()"))
        self.ctr.payUnpayTicket(id)
        self.emit(QtCore.SIGNAL("layoutChanged()"))

    def deleteUnPayTicket(self):

        if len(self.ticketData) > 1:
            self.emit(QtCore.SIGNAL("layoutAboutToBeChanged()"))
            returns = self.ctr.deleteUnPayTicket(self.ticketData[-1])
            if returns['s'] == 'Y':
                text_ = '取消成功'
                self.ctr.showDeleteUnPayOrderButton(False)
            else:
                if returns['e']:
                    text_ = returns['e']
                else:
                    text_ = '失败了再查询一下试试'
            alertWindow.AlertWindow(text=text_).exec_()
            self.query()

            self.emit(QtCore.SIGNAL("layoutChanged()"))

    def query(self):
        self.emit(QtCore.SIGNAL("layoutAboutToBeChanged()"))
        self.ticketData = self.ctr.getUnPayOrderTicket()
        if len(self.ticketData) > 1:
            self.ctr.showDeleteUnPayOrderButton(True)
        else:
            self.ctr.showDeleteUnPayOrderButton(False)
        self.emit(QtCore.SIGNAL("layoutChanged()"))


class TableView(QtGui.QTableView):
    def __init__(self, parent=None):
        super(TableView, self).__init__(parent)

        self.verticalHeader().setVisible(False)
        #self.horizontalHeader().setVisible(False)
        self.setEditTriggers(QtGui.QAbstractItemView.NoEditTriggers)

        self.horizontalHeader().setResizeMode(QtGui.QHeaderView.Stretch)
        #self.verticalHeader().setResizeMode(QtGui.QHeaderView.Stretch)
        self.setVerticalScrollMode(QtGui.QAbstractItemView.ScrollPerItem)
        self.setFrameShape(QtGui.QFrame.NoFrame)

        self.resizeRowsToContents()


class TicketOrderInfoPanel(QtGui.QWidget):
    def __init__(self, ctr, parent=None):
        super(TicketOrderInfoPanel, self).__init__(parent)
        self.ctr = ctr
        self.queryType = None

        self.centralWidget = QtGui.QWidget(self)
        self.queryPenal = QtGui.QWidget(self)
        self.queryPenal.resize(800, 300)
        self.queryPenal.setMaximumSize(QtCore.QSize(16777215, 45))
        self.queryPenal.setMinimumSize(QtCore.QSize(16777215, 45))
        self.displayWidget = QtGui.QWidget(self)

        payButtonGroup = QtGui.QButtonGroup()
        self.unPayOrderText = QtGui.QLabel('未支付查询')
        self.unPayOrder = QtGui.QRadioButton()
        self.connect(self.unPayOrder, QtCore.SIGNAL("clicked()"), self.unPayQuery)
        self.payOrderText = QtGui.QLabel('支付查询')
        self.payOrder = QtGui.QRadioButton()
        self.connect(self.payOrder, QtCore.SIGNAL("clicked()"), self.payQuery)
        payButtonGroup.addButton(self.unPayOrder)
        payButtonGroup.addButton(self.payOrder)

        payButtonLayout = QtGui.QHBoxLayout()
        payButtonLayout.addWidget(self.unPayOrder)
        payButtonLayout.addWidget(self.unPayOrderText)
        payButtonLayout.addWidget(self.payOrder)
        payButtonLayout.addWidget(self.payOrderText)

        self.searchTypeText = QtGui.QLabel('查询日期类型:')
        self.searchType = QtGui.QComboBox()
        self.searchType.setCursor(QtCore.Qt.PointingHandCursor)
        self.searchType.setStyleSheet("QComboBox{padding:4px;}")

        self.searchType.addItem('0')
        self.searchType.addItem('1')
        self.searchType.setItemText(0, '按订票日期查询')
        self.searchType.setItemText(1, '按乘车日期查询')

        self.startTimeText = QtGui.QLabel('起始日期：')
        self.startTime = QtGui.QDateEdit()
        self.startTime.setDate(QtCore.QDate.currentDate())
        self.startTime.setDisplayFormat(("yyyy.MM.dd"))
        self.startTime.setStyleSheet("QDateEdit{padding:3px;}")

        self.endTime = QtGui.QDateEdit()
        self.endTime.setDisplayFormat(("yyyy.MM.dd"))
        self.endTimeText = QtGui.QLabel('结束日期：')
        self.endTime.setStyleSheet("QDateEdit{padding:3px;}")

        self.searchButton = QtGui.QPushButton('查询')
        self.connect(self.searchButton, QtCore.SIGNAL("clicked()"), self.queryUnPayTicket)
        self.searchButton.setCursor(QtCore.Qt.PointingHandCursor)
        self.searchButton.setStyleSheet("QPushButton{width:100px;}")

        self.deleteUnPayOrderButton = QtGui.QPushButton('取消未付款订单')
        self.connect(self.deleteUnPayOrderButton, QtCore.SIGNAL("clicked()"), self.deleteUnPayTicket_)
        self.deleteUnPayOrderButton.setCursor(QtCore.Qt.PointingHandCursor)
        self.deleteUnPayOrderButton.setVisible(False)

        queryLayout = QtGui.QHBoxLayout()
        queryLayout.addWidget(self.searchTypeText)
        queryLayout.addWidget(self.searchType)
        queryLayout.addWidget(self.startTimeText)
        queryLayout.addWidget(self.startTime)
        queryLayout.addWidget(self.endTimeText)
        queryLayout.addWidget(self.endTime)
        queryLayout.addWidget(self.deleteUnPayOrderButton)
        queryLayout.addWidget(self.searchButton)
        queryLayout.setMargin(0)
        queryLayout.setSpacing(20)
        queryLayout2 = QtGui.QHBoxLayout()
        queryLayout2.addLayout(payButtonLayout)
        queryLayout2.addStretch()
        queryLayout2.addLayout(queryLayout)
        self.queryPenal.setLayout(queryLayout2)

        self.model = unPayOrderTableModel(self)
        self.table = TableView()
        self.table.setModel(self.model)
        spacerItem = QtGui.QSpacerItem(0, 20000, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        tableLayout = QtGui.QHBoxLayout()
        tableLayout.addWidget(self.table)
        tableLayout.addItem(spacerItem)

        notice = QtGui.QTextBrowser()
        notice.setMaximumSize(QtCore.QSize(16777215, 192))
        notice.setMinimumSize(QtCore.QSize(16777215, 192))

        notice.setStyleSheet("TicketOrderInfoPanel{margin-top:40px;border:none}")
        notice.append('''<p>温馨提示：</p>
                          <p>1.在本网站可查询乘车日为当前日期前30天以内的历史订单。</p>
                          <p>2.本网站仅办理不晚于开车前2小时尚未换取纸质车票的退票、改签业务。</p>
                          <p>3.在本网站办理退票，只能逐次单张办理。</p>
                          <p>4.车票只能改签一次，已经改签的车票不能再次改签。</p>
                          <p>5.退票、改签成功后可使用订单查询功能确认订单状态，如有疑问请致电12306人工客服查询。</p>
                          ''')
        displayLayout = QtGui.QVBoxLayout()
        displayLayout.addLayout(tableLayout)
        displayLayout.addWidget(notice)
        self.displayWidget.setLayout(displayLayout)

        layout = QtGui.QVBoxLayout()
        layout.setMargin(0)
        layout.setSpacing(0)
        layout.addWidget(self.queryPenal)
        layout.addWidget(self.displayWidget)
        self.setLayout(layout)
        self.unPayQuery()

    def showDeleteUnPayOrderButton(self, status):
        self.deleteUnPayOrderButton.setVisible(status)

    def deleteUnPayTicket_(self):
        self.model.deleteUnPayTicket()

    def unPayQuery(self):
        self.queryType = 1
        self.unPayOrder.setChecked(True)
        self.payOrder.setChecked(False)
        self.searchTypeText.setVisible(False)
        self.searchType.setVisible(False)
        self.startTimeText.setVisible(False)
        self.startTime.setVisible(False)
        self.endTime.setVisible(False)
        self.endTimeText.setVisible(False)

    def payQuery(self):
        self.queryType = 2
        self.unPayOrder.setChecked(False)
        self.payOrder.setChecked(True)
        self.searchTypeText.setVisible(True)
        self.searchType.setVisible(True)
        self.startTimeText.setVisible(True)
        self.startTime.setVisible(True)
        self.endTime.setVisible(True)
        self.endTimeText.setVisible(True)

    def queryUnPayTicket(self):
        self.model.query()

    def getUnPayOrderTicket(self):
        return self.ctr.getUnPayOrderTicket()

    def getTicketOrderRecorder(self, queryData):
        return self.ctr.getTicketOrderRecorder(queryData)

    def payUnPayTicket(self, id):
        self.ctr.payUnPayTicket(id)

    def deleteUnPayTicket(self, deleteData):
        return self.ctr.deleteUnPayTicket(deleteData)

