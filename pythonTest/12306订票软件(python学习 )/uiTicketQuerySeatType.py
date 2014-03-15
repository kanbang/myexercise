from PyQt4 import QtCore, QtGui


class TableModel(QtCore.QAbstractTableModel):
    def __init__(self, ctr, parent=None):
        super(TableModel, self).__init__(parent)
        self.ctr = ctr
        self.seatTypeArr = self.ctr.getSettings('seatTypeArr')

    def rowCount(self, parent=QtCore.QModelIndex()):
        return 1

    def columnCount(self, parent=QtCore.QModelIndex()):
        return self.ctr.getSeatTypeCount()

    def data(self, index, role=QtCore.Qt.DisplayRole, raw=False):
        if not index.isValid():
            return None
        if role in [QtCore.Qt.DisplayRole, QtCore.Qt.EditRole]:
            row = index.row()
            col = index.column()
            if raw:
                return self.ctr.getSeatType(col)
            else:
                return self.seatTypeArr[self.ctr.getSeatType(col)]


    def deleteSeatType(self, seat):
        self.emit(QtCore.SIGNAL("layoutAboutToBeChanged()"))
        self.ctr.deleteSeatType(seat)
        self.emit(QtCore.SIGNAL("layoutChanged()"))

    def setSeatType(self, seat):
        self.emit(QtCore.SIGNAL("layoutAboutToBeChanged()"))
        self.ctr.setSeatType(seat)
        self.emit(QtCore.SIGNAL("layoutChanged()"))


class TableView(QtGui.QTableView):
    def __init__(self, parent=None):
        super(TableView, self).__init__(parent)

        self.verticalHeader().setVisible(False)
        self.horizontalHeader().setVisible(False)
        self.setEditTriggers(QtGui.QAbstractItemView.NoEditTriggers)

        self.horizontalHeader().setResizeMode(QtGui.QHeaderView.Stretch)
        self.verticalHeader().setResizeMode(QtGui.QHeaderView.Stretch)
        self.setVerticalScrollMode(QtGui.QAbstractItemView.ScrollPerItem)
        self.setFrameShape(QtGui.QFrame.NoFrame)
        self.setMaximumSize(QtCore.QSize(16777215, 30))
        self.setHorizontalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOff)
        self.resizeRowsToContents()


class uiTicketQuerySeatType(QtGui.QWidget):
    def __init__(self, ctr, parent=None):
        super(uiTicketQuerySeatType, self).__init__(parent)
        self.ctr = ctr

    def build(self):
        self.model = TableModel(self.ctr)
        self.setMaximumSize(QtCore.QSize(16777215, 36))
        table = TableView()
        table.setModel(self.model)

        table.doubleClicked.connect(lambda x: self.cellClick(x))
        seatTypeText = QtGui.QLabel('席别优先选择:')
        seatTypeText.setStyleSheet("QLabel{margin-right:20px;}")
        seatTypeText.setAlignment(QtCore.Qt.AlignLeft | QtCore.Qt.AlignCenter)
        self.seatType = QtGui.QComboBox()
        self.seatType.addItem('特等座', 'P')
        self.seatType.addItem('高级软卧', '6')
        self.seatType.addItem('软卧', '4')
        self.seatType.addItem('硬卧', '3')
        self.seatType.addItem('软座', '2')
        self.seatType.addItem('硬座', '1')
        self.seatType.addItem('硬座(无座)', 'empty')
        self.seatType.addItem('一等座', 'M')
        self.seatType.addItem('二等座', 'O')
        self.seatType.addItem('商务座', '9')
        self.connect(self.seatType, QtCore.SIGNAL("currentIndexChanged(QString)"), self.addSeatType)

        layout = QtGui.QHBoxLayout()
        layout.setSpacing(0)
        layout.setMargin(0)
        layout.addWidget(seatTypeText)
        layout.addWidget(self.seatType)
        layout.addWidget(table)
        self.setLayout(layout)

    #期望卧铺座位类型
    def addSeatType(self):
        self.model.setSeatType(self.seatType.itemData(self.seatType.currentIndex()))

    #期望卧铺座位类型
    def setSeatType(self, seat):
        self.ctr.setSeatType(seat)

    def cellClick(self, cell):
        row = cell.row()
        col = cell.column()
        seat = self.model.data(self.model.index(0, col), raw=True)
        self.model.deleteSeatType(seat)

    def deleteSeatType(self, seat):
        self.ctr.deleteSeatType(seat)

    def getSeatType(self, col):
        return self.ctr.getSeatType(col)

    def getSeatTypeCount(self):
        return self.ctr.getSeatTypeCount()

    def getSettings(self, type):
        return self.ctr.getSettings(type)



