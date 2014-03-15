from PyQt4 import QtCore, QtGui


class TablePassengersModel(QtCore.QAbstractTableModel):
    def __init__(self, ctr, parent=None):
        super(TablePassengersModel, self).__init__(parent)
        self.ctr = ctr

    def rowCount(self, parent=QtCore.QModelIndex()):
        return 1

    def columnCount(self, parent=QtCore.QModelIndex()):
        return self.ctr.getPassengersCount()

    def data(self, index, role=QtCore.Qt.DisplayRole):
        if not index.isValid():
            return None
        if role in [QtCore.Qt.DisplayRole, QtCore.Qt.EditRole]:
            row = index.row()
            col = index.column()
            return self.ctr.getPassengers(col)

    def deletePassengers(self, passengers):
        self.emit(QtCore.SIGNAL("layoutAboutToBeChanged()"))
        self.ctr.deletePassengers(passengers)
        self.emit(QtCore.SIGNAL("layoutChanged()"))

    def setPassengers(self, passengers):
        self.emit(QtCore.SIGNAL("layoutAboutToBeChanged()"))
        self.ctr.setPassengers(passengers)
        self.emit(QtCore.SIGNAL("layoutChanged()"))


class TableContactsModel(QtCore.QAbstractTableModel):
    def __init__(self, ctr, parent=None):
        super(TableContactsModel, self).__init__(parent)
        self.ctr = ctr

    def rowCount(self, parent=QtCore.QModelIndex()):
        return 1

    def columnCount(self, parent=QtCore.QModelIndex()):
        return self.ctr.getPersonalContactsCounts()

    def data(self, index, role=QtCore.Qt.DisplayRole):
        if not index.isValid():
            return None
        if role in [QtCore.Qt.DisplayRole, QtCore.Qt.EditRole]:
            row = index.row()
            col = index.column()
            return self.ctr.getPersonalContactsDetailColData(col, 'passenger_name')


class TableContactsView(QtGui.QTableView):
    def __init__(self, parent=None):
        super(TableContactsView, self).__init__(parent)

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


class TablePassengersView(QtGui.QTableView):
    def __init__(self, parent=None):
        super(TablePassengersView, self).__init__(parent)

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


class uiTicketQueryPersonContacts(QtGui.QWidget):
    def __init__(self, ctr, parent=None):
        super(uiTicketQueryPersonContacts, self).__init__(parent)
        self.ctr = ctr
        self.setStyleSheet("QWidget{background-color:#ffffff}")

    def build(self):
        self.contactsModel = TableContactsModel(self)
        contactsTable = TableContactsView()
        contactsTable.setModel(self.contactsModel)
        contactsTable.doubleClicked.connect(lambda x: self.cellClickPicketPassenger(x))

        passengersText = QtGui.QLabel('选择乘客:')
        passengersText.setAlignment(QtCore.Qt.AlignLeft | QtCore.Qt.AlignCenter)
        self.passengersModel = TablePassengersModel(self)
        passengersTable = TablePassengersView()
        passengersTable.setModel(self.passengersModel)
        passengersTable.doubleClicked.connect(lambda x: self.cellClickDeletePassenger(x))
        passengersTable.setStyleSheet("TablePassengersView{margin-left:20px;}")

        layout = QtGui.QHBoxLayout()
        layout.setSpacing(0)
        layout.setMargin(0)
        layout.addWidget(passengersText)
        layout.addWidget(passengersTable)
        vLayout = QtGui.QVBoxLayout()
        vLayout.setSpacing(10)
        vLayout.setMargin(0)
        vLayout.addWidget(contactsTable)
        vLayout.addLayout(layout)
        vLayout.addStretch()
        self.setLayout(vLayout)

    def cellClickPicketPassenger(self, cell):
        row = cell.row()
        col = cell.column()
        passengers = self.contactsModel.data(self.contactsModel.index(0, col))
        self.passengersModel.setPassengers(passengers)

    def cellClickDeletePassenger(self, cell):
        row = cell.row()
        col = cell.column()
        passengers = self.passengersModel.data(self.passengersModel.index(0, col))
        self.passengersModel.deletePassengers(passengers)

    def getPassengersCount(self):
        return self.ctr.getPassengersCount()

    def getPassengers(self, col):
        return self.ctr.getPassengers(col)

    def deletePassengers(self, passengers):
        return self.ctr.deletePassengers(passengers)

    def setPassengers(self, passengers):
        return self.ctr.setPassengers(passengers)

    def getPersonalContactsDetailColData(self, row, field):
        return self.ctr.getPersonalContactsDetailColData(row, field)

    def getPersonalContactsCounts(self):
        return self.ctr.getPersonalContactsCounts()





