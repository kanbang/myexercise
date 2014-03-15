from PyQt4 import QtGui, Qt, QtCore
from datetime import datetime, date, time


class TitleBar(QtGui.QFrame):
    def __init__(self):
        super(TitleBar, self).__init__()
        self.setMouseTracking(True)
        self.Title = QtGui.QLabel(self)
        self.Title.move(10, 7)
        self.setStyleSheet(
            "TitleBar{background-color: #000000;border:none;border:none;background-image:url(images/login.png);padding:20px; border:2px solid #f64a6b; border-bottom:none}")

    def mousePressEvent(self, event):
        #鼠标点击事件
        if event.button() == QtCore.Qt.LeftButton:
            self.parent().mousePressEvent__(event)

    def mouseMoveEvent(self, event):
        #鼠标移动事件
        if event.buttons() == QtCore.Qt.LeftButton:
            self.parent().mouseMoveEvent__(event)

    def setTitle(self, text):
        self.Title.setText(text)


class Confirm(QtGui.QDialog):
    def __init__(self, ctr, confirmOderInfo, parent=None):
        super(Confirm, self).__init__(parent)
        self.config = ctr.getConfig()

        self.ctr = ctr
        self.resize(750, 50)
        self.left = False
        self.right = False
        self.bottom = False
        self.mouseDown = False
        self.setMouseTracking(True)
        self.confirmOderInfo = confirmOderInfo
        print(self.confirmOderInfo)

        desktop = QtGui.QApplication.desktop()
        width = desktop.width()
        height = desktop.height()
        self.move((width - self.width()) / 2, (height - self.height()) / 2)

        self.setWindowFlags(QtCore.Qt.FramelessWindowHint | QtCore.Qt.WindowStaysOnTopHint)

        topBar = TitleBar()
        topBar.setParent(self)
        topBar.setCursor(QtCore.Qt.ClosedHandCursor)
        topBar.setMaximumSize(QtCore.QSize(16777215, 30))
        topBar.setMinimumSize(QtCore.QSize(16777215, 30))
        topBar.setFrameShape(QtGui.QFrame.StyledPanel)
        topBar.setFrameShadow(QtGui.QFrame.Raised)
        topBar.setLineWidth(0)
        topBar.setObjectName("topBar")

        displayWidget = QtGui.QWidget()

        #ticketInfo = QtGui.QLabel(self.confirmOderInfo['orderTips'])
        #ticketInfo2 = QtGui.QLabel(self.confirmOderInfo['priceTips'])
        table = QtGui.QTableWidget()

        table.setColumnCount(6)
        #table.setShowGrid(False)
        table.verticalHeader().setVisible(False)
        table.setEditTriggers(QtGui.QAbstractItemView.NoEditTriggers)
        table.setHorizontalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOff)
        #table.horizontalHeader().setStretchLastSection(True)
        table.horizontalHeader().setResizeMode(QtGui.QHeaderView.Stretch)
        table.verticalHeader().setResizeMode(QtGui.QHeaderView.Stretch)

        #setVerticalScrollMode(QAbstractItemView::ScrollPerItem);//垂直滚动条按项移动
        table.setAutoScroll(False)

        #table.horizontalHeader().setVisible(False)
        table.setHorizontalHeaderLabels(['序号', '席别', '票种', '姓名', '证件类型', '证件号码', '手机号码'])

        ticketPassengers = self.config.getDictsSequence('defaultSetting', 'ticketPassengers')
        seatTypeArr = self.config.get('seatTypeArr')
        table.setRowCount(len(ticketPassengers))

        for i, v in enumerate(ticketPassengers):
            c = ctr.getPersonalContactsDetail(v)

            for j in range(table.columnCount()):
                if j == 0:
                    newItem = str(i + 1)
                if j == 1:
                    newItem = seatTypeArr[self.confirmOderInfo['seat']] if not self.confirmOderInfo[
                                                                                   'seat'] is None else '席位未订'
                if j == 2:
                    newItem = c['passenger_type']
                if j == 3:
                    newItem = c['passenger_name']
                if j == 4:
                    newItem = c['passenger_id_type_name']
                if j == 5:
                    newItem = str(c['passenger_id_no'])
                if j == 6:
                    newItem = str(c['mobile_no'])

                newItem = QtGui.QTableWidgetItem(newItem)
                newItem.setTextAlignment(QtCore.Qt.AlignHCenter | QtCore.Qt.AlignVCenter)
                table.setItem(i, j, newItem)

        self.codeTitle = QtGui.QLabel('输入验证码：')
        self.codeMessage = QtGui.QLabel()
        self.refreshImageBtn = QtGui.QPushButton('点击更新验证码')
        self.imageLabel = QtGui.QLabel()
        self.refreshImage()
        self.imageCodeInput = QtGui.QLineEdit()
        self.imageCodeInput.setMaxLength(4)
        self.imageCodeInput.setValidator(QtGui.QRegExpValidator(Qt.QRegExp("[A-Za-z0-9]+"), self))
        #
        # self.seatTitle = QtGui.QLabel('席位选择：')
        # self.seatMessage = QtGui.QLabel('当前席位：无')

        # self.seatType = QtGui.QComboBox()
        # seats = {i: v for i, v in seatTypeArr.items() for j in ['商务座', '特等座'] if j == v}
        # for i, v in seats.items():
        #     self.seatType.addItem(v, i)
        #
        # self.connect(self.seatType, QtCore.SIGNAL("currentIndexChanged(QString)"), self.setSeatType)
        # self.seatBindSeating(seats)

        buttonAccept = QtGui.QPushButton('确定')
        buttonAccept.setStyleSheet(("QPushButton{margin-top:25px; padding:4px 20px}"))
        buttonCancel = QtGui.QPushButton('取消')
        buttonCancel.setStyleSheet(("QPushButton{margin-top:25px; padding:4px 20px}"))

        buttonBoxLayout = QtGui.QHBoxLayout()
        buttonBoxLayout.addStretch()
        buttonBoxLayout.addWidget(buttonCancel)
        buttonBoxLayout.addWidget(buttonAccept)

        gridLayout = QtGui.QGridLayout()
        gridLayout.addWidget(self.codeTitle, 0, 0)
        gridLayout.addWidget(self.codeMessage, 0, 1)
        gridLayout.addWidget(self.imageLabel, 1, 0)
        gridLayout.addWidget(self.refreshImageBtn, 1, 1)
        gridLayout.addWidget(self.imageCodeInput, 1, 2)
        # gridLayout.addWidget(self.seatTitle, 2, 0)
        # gridLayout.addWidget(self.seatMessage, 2, 1)
        # gridLayout.addWidget(self.seatType, 2, 2)

        VLayout = QtGui.QVBoxLayout()
        #VLayout.addWidget(ticketInfo)
        #VLayout.addWidget(ticketInfo2)
        VLayout.addWidget(table)
        VLayout.addLayout(gridLayout)
        VLayout.addLayout(buttonBoxLayout)
        displayWidget.setLayout(VLayout)

        self.connect(buttonAccept, QtCore.SIGNAL("clicked()"), self, QtCore.SLOT("accept()"))
        self.connect(buttonCancel, QtCore.SIGNAL("clicked()"), self, QtCore.SLOT("reject()"))

        self.connect(self.refreshImageBtn, QtCore.SIGNAL("clicked()"), self.refreshImage)
        topBar.setTitle('验证码')
        self.activateWindow()

        layout = QtGui.QVBoxLayout()
        layout.addWidget(topBar)
        layout.addWidget(displayWidget)
        layout.setSpacing(0)
        layout.setMargin(0)
        self.setLayout(layout)
        self.setStyleSheet(("QDialog{padding:20px; background-color:#FFFFFF;border:2px solid #f64a6b; }"))

    # def seatBindSeating(self, seats):
    #
    #     seatType = self.config.getDictsSequence('defaultSetting', 'seatType')
    #     seat = self.confirmOderInfo['seat']
    #     seats_ = []
    #     if seat is None:
    #         seats_ = seatType
    #     else:
    #         seats_.append(seat)
    #
    #     s = [i for i in seats_ if i in seats]
    #     found = False
    #     if len(s) > 0:
    #         s = s[0]
    #         for i in range(self.seatType.count()):
    #             if self.seatType.itemData(i) == s:
    #                 found = True
    #                 self.seatType.setCurrentIndex(i)
    #                 self.seatType.emit(QtCore.SIGNAL("currentIndexChanged(QString)"), '')
    #                 break
    #         if not found:
    #             self.setSeatType()
    #     else:
    #         self.setSeatType()

    #
    # def setSeatType(self):
    #     pass
    #     seatTypeArr = self.config.get('seatTypeArr')
    #     seat = self.seatType.itemData(self.seatType.currentIndex())
    #     self.seatMessage.setText('当前席位：%s' % seatTypeArr[seat])
    #     self.confirmOderInfo['seat'] = seat

    def refreshImage(self):
        gif = QtGui.QMovie()
        gif.setFileName(self.ctr.refreshImage())
        gif.start()
        self.imageLabel.setMovie(gif)

    def reject(self):
        QtGui.QDialog.reject(self)


    def accept(self):
        value = self.getOrderInput()
        if len(value['imageCode']) != 4:
            self.codeMessage.setText('输入不完整!!!')
        elif not self.ctr.checkOrderImageCode(self.imageCodeInput.text().lower(), self.confirmOderInfo['token']):
            self.imageCodeInput.setText('')
            self.codeMessage.setText('输入错误!!!')
        else:
            QtGui.QDialog.accept(self)


    def getOrderInput(self):
        return {'imageCode': self.imageCodeInput.text().lower(), 'seat': self.confirmOderInfo['seat']}


    def mousePressEvent__(self, event):
        #鼠标点击事件
        if event.button() == QtCore.Qt.LeftButton:
            self.dragPosition = event.globalPos() - self.frameGeometry().topLeft()
            event.accept()


    def mouseMoveEvent__(self, event):
    #鼠标移动事件
        if event.buttons() == QtCore.Qt.LeftButton:
            self.move(event.globalPos() - self.dragPosition)
            event.accept()


    def mousePressEvent(self, event):
        self.oldMousePosition = event.pos();
        if event.button() == QtCore.Qt.LeftButton:
            self.mouseDown = True


    def mouseMoveEvent(self, event):
        x = int(event.x())
        y = int(event.y())

        if self.mouseDown:

            dx = x - self.oldMousePosition.x()
            dy = y - self.oldMousePosition.y()

            g = self.geometry()

            if self.left:
                g.setLeft(g.left() + dx)

            if self.right:
                g.setRight(g.right() + dx)

            if self.bottom:
                g.setBottom(g.bottom() + dy)

            self.setGeometry(g)

            self.oldMousePosition = QtCore.QPoint(self.oldMousePosition.x() if self.left else event.x(), event.y())

        else:
            r = self.rect()
            self.left = abs(x - r.left()) <= 5
            self.right = abs(x - r.right()) <= 5
            self.bottom = abs(y - r.bottom()) <= 5
            hor = self.left or self.right

            if hor and self.bottom:
                if self.left:
                    self.setCursor(QtCore.Qt.SizeBDiagCursor)
                else:
                    self.setCursor(QtCore.Qt.SizeFDiagCursor)
            elif hor:
                self.setCursor(QtCore.Qt.SizeHorCursor)

            elif self.bottom:

                self.setCursor(QtCore.Qt.SizeVerCursor)
            else:
                self.setCursor(QtCore.Qt.ArrowCursor)


    def mouseReleaseEvent(self, event):
        self.mouseDown = False

