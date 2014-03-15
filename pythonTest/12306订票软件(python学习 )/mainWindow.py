from PyQt4 import QtCore, QtGui
import uiPersonalDetail
import uiPersonelContacts
import uiTickerOrderInfo
import uiTicketQuery
import uiAddPersonalContacts
import uiUpdate
import threading
import time
import datetime
import webbrowser


class RunClock(threading.Thread):
    def __init__(self, ctr):
        threading.Thread.__init__(self)
        self.ctr = ctr

    def run(self):
        while True:
            self.ctr.updateClock()
            time.sleep(1)


class TitleBar(QtGui.QFrame):
    def __init__(self):
        super(TitleBar, self).__init__()
        self.setMouseTracking(True)
        self.setStyleSheet(
            "TitleBar{background-color: #000000;border:none;border:none;background-image:url(images/login.png)}")

    def mousePressEvent(self, event):
        #鼠标点击事件
        if event.button() == QtCore.Qt.LeftButton:
            self.parent().parent().mousePressEvent__(event)

    def mouseMoveEvent(self, event):
        #鼠标移动事件
        if event.buttons() == QtCore.Qt.LeftButton:
            self.parent().parent().mouseMoveEvent__(event)

    def mouseDoubleClickEvent(self, event):
        if event.button() == QtCore.Qt.LeftButton:
            self.parent().parent().showMaximized__(event)


class LabelButton(QtGui.QLabel):
    def __init__(self, ID):
        super(LabelButton, self).__init__()
        self.setMouseTracking(True)
        self.ID = ID

    def mouseReleaseEvent(self, event):  #注:
        #鼠标点击事件
        self.window.btnHandle(self.ID)

    def enterEvent(self, event):
        #鼠标进入时间
        self.window.btnEnter(self.ID)

    def leaveEvent(self, event):
        #鼠标离开事件
        self.window.btnLeave(self.ID)

    def setWindowObject(self, window):
        self.window = window


class FuncBarButton(QtGui.QPushButton):
    buttons = []

    def __init__(self, ID):
        super(FuncBarButton, self).__init__()
        self.setMouseTracking(True)
        self.ID = ID
        self.buttons.append(self)
        self.disableActived()

    def mousePressEvent(self, event):  #注:
        #鼠标点击事件
        for i in self.buttons:
            i.disableActived()
        self.actived()

    def setWindowObject(self, window):
        self.window = window

    def actived(self):
        self.setStyleSheet(
            'QPushButton{background-color: #fd0c45;border:none;border:1px solid #2e2e2e;border-bottom:none;color:#FFFFFF}')
        self.window.changeStackedWidget(self.ID)

    def disableActived(self):
        self.setStyleSheet(
            'QPushButton{background-color: #1f1f1f;border:none;border:1px solid #2e2e2e;border-bottom:none;color:#FFFFFF}')


class MainWindow(QtGui.QMainWindow):
    def __init__(self, ctr, parent=None):
        super(MainWindow, self).__init__(parent)

        self.ctr = ctr
        self.maxWindow = False
        self.mouseDown = False
        self.left = False
        self.right = False
        self.bottom = False
        self.setMouseTracking(True)
        self.currentNtpTime = []
        self.resize(1024, 550)
        desktop = QtGui.QApplication.desktop()
        width = desktop.width()
        height = desktop.height()
        self.move((width - self.width()) / 2, (height - self.height()) / 2)
        #self.setWindowFlags(QtCore.Qt.FramelessWindowHint | QtCore.Qt.WindowStaysOnTopHint)
        self.setWindowFlags(QtCore.Qt.FramelessWindowHint)
        self.connect(self, QtCore.SIGNAL("resized()"), self.resizeEvent)
        self.activateWindow()

        self.trayIcon = QtGui.QSystemTrayIcon(self)
        self.trayIcon.setIcon(QtGui.QIcon("images/favorite.ico"))
        self.trayIcon.show()
        self.trayIcon.activated.connect(self.trayClick_)       #点击托盘
        self.setWindowIcon(QtGui.QIcon("images/favorite.ico"))
        self.trayMenu()

        #__________*********centralWidget*********__________
        self.centralWidget = QtGui.QWidget(self)
        self.centralWidget.setObjectName("centralWidget")
        self.setStyleSheet(("MainWindow{padding:20px; background-color:#FFFFFF;border:2px solid #f64a6b; }"))

        #__________*********centralWidgetOutSideHorizontalLayout*********__________
        self.centralWidgetOutSideHorizontalLayout = QtGui.QHBoxLayout(self.centralWidget)
        self.centralWidgetOutSideHorizontalLayout.setSpacing(0)
        self.centralWidgetOutSideHorizontalLayout.setMargin(2)
        self.centralWidgetOutSideHorizontalLayout.setObjectName("centralWidgetOutSideHorizontalLayout")

        #__________*********centralWidgetMainVerticalLayout*********__________
        self.centralWidgetMainVerticalLayout = QtGui.QVBoxLayout()
        self.centralWidgetMainVerticalLayout.setSpacing(0)
        self.centralWidgetMainVerticalLayout.setObjectName("centralWidgetMainVerticalLayout")

        #__________*********titleBar*********__________
        titleBar = self.buildTitleBar()
        self.centralWidgetMainVerticalLayout.addWidget(titleBar)

        #__________*********centralWidgetMainHorizontalLayout*********__________
        self.centralWidgetMainHorizontalLayout = QtGui.QHBoxLayout()
        self.centralWidgetMainHorizontalLayout.setSpacing(0)
        self.centralWidgetMainHorizontalLayout.setObjectName("horizontalLayout")

        #__________*********funcBar*********__________
        funcBar = self.buildFuncBar()
        self.centralWidgetMainHorizontalLayout.addWidget(funcBar)

        self.body = QtGui.QFrame(self.centralWidget)
        self.body.setFrameShape(QtGui.QFrame.StyledPanel)
        self.body.setFrameShadow(QtGui.QFrame.Raised)
        self.body.setLineWidth(0)
        self.body.setObjectName("body")

        #__________*********stackedWidget*********__________
        self.stackedWidget = QtGui.QStackedWidget()
        bodyLayout = QtGui.QHBoxLayout()
        bodyLayout.addWidget(self.stackedWidget)
        self.body.setLayout(bodyLayout)
        self.stackedWidget.setObjectName("stackedWidget")

        #__________*********个人资料*********__________
        self.personalDetail = uiPersonalDetail.PersonalDetailPanel()
        self.personalDetail.setObjectName("个人资料")
        personalDetailData = self.ctr.getPersonalDetail()
        self.personalDetail.build(self.ctr.getPersonalDetail())
        self.stackedWidget.addWidget(self.personalDetail)

        #__________*********订单记录*********__________
        self.orderInfo = uiTickerOrderInfo.TicketOrderInfoPanel(self)
        self.orderInfo.setObjectName("订单记录")
        self.stackedWidget.addWidget(self.orderInfo)

        #__________*********常用联系人*********__________
        self.personalContacts = uiPersonelContacts.PersonalContactsPenal(self)
        self.personalContacts.setObjectName("常用联系人")
        self.personalContacts.build()
        self.stackedWidget.addWidget(self.personalContacts)

        ########################## 车票查询
        self.ticketQuery = uiTicketQuery.TickerQueryPanel(self)
        self.ticketQuery.setObjectName("车票查询")
        self.ticketQuery.build('')
        self.ticketQuery.bindSetting(self.ctr.getTicketSetting())
        self.stackedWidget.addWidget(self.ticketQuery)

        #__________*********添加常用联系人*********__________
        self.uiAddPersonalContacts = uiAddPersonalContacts.AddPersonalContactsPanel(self)
        self.uiAddPersonalContacts.setObjectName("添加常用联系人")
        self.stackedWidget.addWidget(self.uiAddPersonalContacts)

        #__________*********检查更新*********__________

        self.uiUpdate = uiUpdate.uiUpdate(self)
        self.uiUpdate.setObjectName("检查更新")
        self.uiUpdate.build()
        self.stackedWidget.addWidget(self.uiUpdate)

        #__________*********END*********__________
        self.centralWidgetMainHorizontalLayout.addWidget(self.body)
        self.centralWidgetMainVerticalLayout.addLayout(self.centralWidgetMainHorizontalLayout)
        self.centralWidgetOutSideHorizontalLayout.addLayout(self.centralWidgetMainVerticalLayout)
        self.setCentralWidget(self.centralWidget)
        self.stackedWidget.setCurrentIndex(0)
        QtCore.QMetaObject.connectSlotsByName(self)

        self.threadRunClock = RunClock(self)
        self.threadRunClock.setDaemon(True)

        self.threadRunClock.start()

    def buildTitleBar(self):
        topBar = TitleBar()
        topBar.setParent(self.centralWidget)
        topBar.setCursor(QtCore.Qt.ClosedHandCursor)
        topBar.setMaximumSize(QtCore.QSize(16777215, 30))
        topBar.setMinimumSize(QtCore.QSize(16777215, 30))
        topBar.setFrameShape(QtGui.QFrame.StyledPanel)
        topBar.setFrameShadow(QtGui.QFrame.Raised)
        topBar.setLineWidth(0)
        topBar.setObjectName("topBar")

        titleLogo = LabelButton(0)
        titleLogo.setParent(topBar)
        titleLogo.setWindowObject(self)
        titleLogo.setGeometry(QtCore.QRect(2, 0, 26, 26))
        titleLogo.setObjectName("titleLogo")
        titleLogo.setPixmap(QtGui.QPixmap("images/titleLogo.png"))

        self.softTitle = self.getSettings('softTitle')
        self.softTitleText = QtGui.QLabel(self.softTitle, topBar)
        self.softTitleText.move(35, 4)
        self.softTitleText.setMinimumSize(200, 15)
        self.softTitleText.setStyleSheet("QLabel{color:#000000;}")

        self.minButton = LabelButton(1)
        self.minButton.setParent(topBar)
        self.minButton.setWindowObject(self)
        self.minButton.setGeometry(QtCore.QRect(355, 0, 14, 14))
        self.minButton.setObjectName("minButton")
        self.minButton.setPixmap(QtGui.QPixmap("images/min.png"))
        self.minButton.setCursor(QtCore.Qt.PointingHandCursor)

        self.closeButton = LabelButton(2)
        self.closeButton.setParent(topBar)
        self.closeButton.setWindowObject(self)
        self.closeButton.setGeometry(QtCore.QRect(505, 0, 14, 14))
        self.closeButton.setObjectName("closeButton")
        self.closeButton.setPixmap(QtGui.QPixmap("images/close.png"))
        self.closeButton.setCursor(QtCore.Qt.PointingHandCursor)
        return topBar

    def buildFuncBar(self):
        funcBar = QtGui.QFrame(self.centralWidget)
        funcBar.setMinimumSize(QtCore.QSize(200, 0))
        funcBar.setMaximumSize(QtCore.QSize(200, 16777215))
        funcBar.setStyleSheet("QFrame{background-color: #0f0f0f;border:none;padding:0px;}")
        funcBar.setFrameShape(QtGui.QFrame.StyledPanel)
        funcBar.setFrameShadow(QtGui.QFrame.Raised)
        funcBar.setLineWidth(0)
        funcBar.setObjectName("funcBar")

        #个人资料
        personalDetailBar = FuncBarButton(1)
        personalDetailBar.setStyleSheet(
            'FuncBarButton{background-color: #fd0c45;border:none;border:1px solid #2e2e2e;border-bottom:none;color:#FFFFFF}')
        personalDetailBar.setParent(funcBar)
        personalDetailBar.setText('个人资料')
        personalDetailBar.setWindowObject(self)
        personalDetailBar.setGeometry(QtCore.QRect(0, 40, funcBar.width(), 40))
        personalDetailBar.setObjectName("personalDetailBar")
        personalDetailBar.setCursor(QtCore.Qt.PointingHandCursor)

        #订单信息
        orderInfoBar = FuncBarButton(2)
        orderInfoBar.setParent(funcBar)
        orderInfoBar.setWindowObject(self)
        orderInfoBar.setText('订单信息')
        orderInfoBar.setGeometry(QtCore.QRect(0, 80, funcBar.width(), 40))
        orderInfoBar.setObjectName("orderInfoBar")
        orderInfoBar.setCursor(QtCore.Qt.PointingHandCursor)

        #常用联系人
        personContactsBar = FuncBarButton(3)
        personContactsBar.setParent(funcBar)
        personContactsBar.setWindowObject(self)
        personContactsBar.setText('常用联系人')
        personContactsBar.setGeometry(QtCore.QRect(0, 120, funcBar.width(), 40))
        personContactsBar.setObjectName("ticketQueryBar")
        personContactsBar.setCursor(QtCore.Qt.PointingHandCursor)

        #车票查询
        ticketQueryBar = FuncBarButton(4)
        ticketQueryBar.setParent(funcBar)
        ticketQueryBar.setText('车票查询')
        ticketQueryBar.setWindowObject(self)
        ticketQueryBar.setGeometry(QtCore.QRect(0, 160, funcBar.width(), 40))
        ticketQueryBar.setObjectName("personContactsBar")
        ticketQueryBar.setCursor(QtCore.Qt.PointingHandCursor)

        #更新升级
        updateBar = FuncBarButton(6)
        updateBar.setParent(funcBar)
        updateBar.setText('更新升级')
        updateBar.setWindowObject(self)
        updateBar.setGeometry(QtCore.QRect(0, 200, funcBar.width(), 40))
        updateBar.setObjectName("updateBar")
        updateBar.setCursor(QtCore.Qt.PointingHandCursor)
        return funcBar

    def getPersonalContactsDetailColData(self, row, field):
        return self.ctr.getPersonalContactsDetailColData(row, field)

    def setPassengers(self, passengers):
        self.ctr.setPassengers(passengers)

    def getPassengersCount(self):
        return self.ctr.getPassengersCount()

    def deletePassengers(self, passengers):
        return self.ctr.deletePassengers(passengers)

    def getSettings(self, type):
        return self.ctr.getSettings(type)

    #时间间隔
    def setIntervalRefreshTime(self, data):
        self.ctr.setIntervalRefreshTime(data)

    def setFavoriteTrain(self, data):
        self.ctr.setFavoriteTrain(data)

    #最小座位数
    def setMinimumTicketNumber(self, data):
        self.ctr.setMinimumTicketNumber(data)

    def setAutoTicketQuery(self, data):
        self.ctr.setAutoTicketQuery(data)

    #期望座位类型
    def setSeatType(self, seat):
        self.ctr.setSeatType(seat)

    #期望卧铺座位类型
    def setSleepBeddingType(self, data):
        self.ctr.setSleepBeddingType(data)
        #是否自动订票

    def setAutoOrderTicket(self, data):
        self.ctr.setAutoOrderTicket(data)

    def onlyDisplayFilterTrain(self, data):
        self.ctr.onlyDisplayFilterTrain(data)

    def setStartQueryTime(self, data):
        self.ctr.setStartQueryTime(data)

    #匹配车次提前
    def setAutoTopSelectedTrain(self, data):
        self.ctr.setAutoTopSelectedTrain(data)

    def changeStackedWidget(self, index):
        self.stackedWidget.setCurrentIndex(index - 1)

    def addPersonalContact(self, data):
        return self.ctr.addPersonalContact(data)

    def getPersonalContactsCounts(self):
        return self.ctr.getPersonalContactsCounts()

    def getPassengers(self, col):
        return self.ctr.getPassengers(col)

    def bookTicket(self, trainString):
        return self.ctr.bookTicket(trainString)

    def ticketsQuery(self, query, loop):
        return self.ctr.ticketsQuery(query, loop)

    def getStationCode(self, station):
        return self.ctr.getStationCode(station)

    def deletePersonalContact(self, person):
        return self.ctr.deletePersonalContact(person)

    def deleteSeatType(self, seat):
        return self.ctr.deleteSeatType(seat)

    def getUnPayOrderTicket(self):
        return self.ctr.getUnPayOrderTicket()

    def payUnPayTicket(self, id):
        self.ctr.payUnPayTicket(id)

    def deleteUnPayTicket(self, deleteData):
        return self.ctr.deleteUnPayTicket(deleteData)

    def getTicketOrderRecorder(self, submitData):
        self.ctr.getTicketOrderRecorder(submitData)

    def getIntervalRefreshTime(self):
        return self.ctr.getIntervalRefreshTime()

    def updateClock(self):

        dt = datetime.datetime.fromtimestamp(int(time.time()))
        self.softTitleText.setText(
            self.softTitle + '(' + dt.strftime("%H:%M:%S") + ')')

    def resizeEvent(self, event):
        width = self.width()
        self.minButton.setGeometry(QtCore.QRect(width - 29 - 20, 5, 14, 14))
        self.closeButton.setGeometry(QtCore.QRect(width - 29, 5, 14, 14))

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

    def getSeatType(self, col):
        return self.ctr.getSeatType(col)

    def getSeatTypeCount(self):
        return self.ctr.getSeatTypeCount()

    def showMaximized__(self, event):
        if event.buttons() == QtCore.Qt.LeftButton:
            if not self.maxWindow:
                self.showMaximized()
                self.maxWindow = True
            else:
                self.maxWindow = False
                self.showNormal()


    def btnHandle(self, ID):
        #最小化
        if ID == 1:
            self.hide()
        elif ID == 2:
            #关闭
            self.close()


    def btnEnter(self, ID):
        #鼠标进入
        if ID == 1:
            self.minButton.setPixmap(QtGui.QPixmap("images/min.png"))
        elif ID == 2:
            self.closeButton.setPixmap(QtGui.QPixmap("images/close.png"))


    def btnLeave(self, ID):
        self.minButton.setPixmap(QtGui.QPixmap("images/min.png"))
        self.closeButton.setPixmap(QtGui.QPixmap("images/close.png"))


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


    def closeEvent(self, event):
        self.ctr.savingStorage()
        self.trayIcon.setVisible(False)
        event.accept()


    def trayMenu(self):
        #右击托盘弹出的菜单

        self.trayIcon.setToolTip("订票精灵")
        self.showNormal_ = QtGui.QAction("显示窗口", self, triggered=self.showNormal)
        self.update_ = QtGui.QAction("升级", self, triggered=self.update)
        self.quitAction = QtGui.QAction("退出", self, triggered=self.closeWindow)
        self._12306_ = QtGui.QAction("访问12306", self, triggered=self._12306)
        self.sinaWb_ = QtGui.QAction("新浪微薄", self, triggered=self.sinaWb)

        self.trayIconMenu = QtGui.QMenu(self)
        #self.trayIconMenu.addSeparator()
        self.trayIconMenu.addAction(self.showNormal_)
        self.trayIconMenu.addAction(self.update_)
        self.trayIconMenu.addAction(self.quitAction)
        self.trayIconMenu.addAction(self._12306_)
        self.trayIconMenu.addAction(self.sinaWb_)

        self.trayIcon.setContextMenu(self.trayIconMenu)

    def update(self):
        fh = open('./version.txt')
        version = fh.read()
        fh.close()
        webbrowser.open_new('http://9ep.cn/update/version/' + str(version))

    def closeWindow(self):
        self.trayIcon.setVisible(False)
        QtGui.qApp.quit()

    def _12306(self):
        webbrowser.open_new('http://www.12306.cn/mormhweb/')

    def sinaWb(self):
        webbrowser.open_new('http://weibo.com/shuangelaide')

    def trayClick_(self, reason=None):
        #双击托盘
        if reason == QtGui.QSystemTrayIcon.DoubleClick:
            self.showNormal()
        else:
            pass
