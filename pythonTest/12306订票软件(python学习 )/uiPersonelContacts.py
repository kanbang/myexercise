from PyQt4 import QtCore, QtGui


class ButtonDelegate(QtGui.QItemDelegate):
    def __init__(self, parent=None):
        QtGui.QItemDelegate.__init__(self, parent)
        self.width = 100
        self.height = 30

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
        opt.text = '删除'
        left = option.rect.x() + (option.rect.width() - self.width) / 2
        top = option.rect.y() + (option.rect.height() - self.height) / 2
        opt.rect = QtCore.QRect(left, top, self.width, self.height)
        QtGui.QApplication.style().drawControl(QtGui.QStyle.CE_PushButton, opt, painter)

    def updateEditorGeometry(self, editor, option, index):
        pass


class QTableButton(QtGui.QPushButton):
    def __init__(self, ctr, x, person, parent=None):
        super(QTableButton, self).__init__(parent)
        self.ctr = ctr
        self.x = x
        self.person = person
        self.setText('删除')

    def mousePressEvent(self, event):
        if event.button() == QtCore.Qt.LeftButton:
            print(self.x)
            self.ctr.deletePersonalContact(self.x, self.person)


class TableModel(QtCore.QAbstractTableModel):
    def __init__(self, ctr, parent=None):
        super(TableModel, self).__init__(parent)

        self.sex = {
            'M': '男', 'F': '女', '': '-'
        }
        self.colField = ['passenger_name', 'sex_code', 'passenger_id_type_name', 'passenger_id_no', 'mobile_no',
                         'passenger_type_name']
        self.ctr = ctr
        self.horizontalHeaderLabels = ['姓名', '姓别', '证件类型', '证件号码', '手机/电话', '旅客类型', '操作']

    def rowCount(self, parent=QtCore.QModelIndex()):
        return self.ctr.getPersonalContactsCounts()

    def columnCount(self, parent=QtCore.QModelIndex()):
        return len(self.colField) + 1

    def headerData(self, col, orientation, role):
        if orientation == QtCore.Qt.Horizontal and role == QtCore.Qt.DisplayRole:
            return self.horizontalHeaderLabels[col]

    def data(self, index, role=QtCore.Qt.DisplayRole):
        if not index.isValid():
            return None
        if role in [QtCore.Qt.DisplayRole, QtCore.Qt.EditRole]:
            row = index.row()
            col = index.column()
            if col == len(self.colField):
                pass;
            else:
                dt = self.ctr.getPersonalContactsDetailColData(row, self.colField[col])
                if col == 1:
                    return self.sex[dt] if not dt is None else '-'
                else:
                    return dt

    def deletePersonalContact(self, person):
        self.emit(QtCore.SIGNAL("layoutAboutToBeChanged()"))
        self.ctr.deletePersonalContact(person)
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


class PersonalContactsPenal(QtGui.QWidget):
    def __init__(self, ctr, parent=None):
        super(PersonalContactsPenal, self).__init__(parent)
        self.ctr = ctr

    def build(self):

        self.model = TableModel(self.ctr)
        table = TableView()
        table.setModel(self.model)
        table.doubleClicked.connect(lambda x: self.cellClick(x))
        table.setItemDelegateForColumn(6, ButtonDelegate(table))

        layout = QtGui.QVBoxLayout()
        addButton = QtGui.QPushButton('添加')
        self.connect(addButton, QtCore.SIGNAL("clicked()"), self.openAddPersonalContactsPanel)
        addButton.setCursor(QtCore.Qt.PointingHandCursor)

        spacerItem = QtGui.QSpacerItem(0, 20000, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        tableLayout = QtGui.QHBoxLayout()
        tableLayout.addWidget(table)
        tableLayout.addItem(spacerItem)

        layout.addWidget(addButton)
        layout.addLayout(tableLayout)
        self.setLayout(layout)

    def openAddPersonalContactsPanel(self):
        self.ctr.changeStackedWidget(5)

    def cellClick(self, cell):
        row = cell.row()
        col = cell.column()
        if self.model.columnCount() - 1 == col:
            person = self.model.data(self.model.index(row, 0))
            self.model.deletePersonalContact(person)

    def deletePersonalContact(self, index, person):
        if self.ctr.deletePersonalContact(person):
            print(1)
        else:
            print(2)


