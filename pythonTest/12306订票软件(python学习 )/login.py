# -*- coding: utf-8 -*-
from PyQt4 import QtGui, Qt, QtCore
import webbrowser


class LabelBtn(QtGui.QLabel):
    def __init__(self, ID):
        super(LabelBtn, self).__init__()
        self.setMouseTracking(True)
        self.ID = ID

    def mouseReleaseEvent(self, event):  #注:
        #鼠标点击事件
        self.parent().btnHandle(self.ID)

    def enterEvent(self, event):
        #鼠标进入时间
        self.parent().btnEnter(self.ID)

    def leaveEvent(self, event):
        #鼠标离开事件
        self.parent().btnLeave(self.ID)


class ImageCode(QtGui.QLabel):
    def __init__(self, ID):
        super(ImageCode, self).__init__()
        self.setMouseTracking(True)
        self.ID = ID

    def mouseReleaseEvent(self, event):  #注:
        #鼠标点击事件
        self.parent().refreshImageCode()

    def enterEvent(self, event):
        #鼠标进入时间
        #self.parent().btnEnter(self.ID)
        pass

    def leaveEvent(self, event):
        #鼠标离开事件
        #self.parent().btnLeave(self.ID)
        pass

    def setImage(self, img):
        gif = QtGui.QMovie()
        gif.setFileName(img)
        gif.start()
        self.setMovie(gif)


class Login(QtGui.QDialog):
    def __init__(self, ctr, parent=None):
        super(Login, self).__init__(parent)
        print('_______strp33_________')
        self.ctr = ctr
        self.setFixedSize(347, 264)
        self.autoLogin = False
        self.loginStatus = False
        self.setWindowIcon(QtGui.QIcon("images/favorite.ico"))
        #self.setAttribute(QtCore.Qt.WA_DeleteOnClose)

        #窗口居中显示
        desktop = QtGui.QApplication.desktop()
        width = desktop.width()
        height = desktop.height()
        self.move((width - self.width()) / 2, (height - self.height()) / 2)
        self.setMouseTracking(True)
        self.setStyleSheet('QDialog{background-image:url(images/login.png);border:2px solid #f64a6b;}')
        #无边框
        self.setWindowFlags(Qt.Qt.FramelessWindowHint | QtCore.Qt.WindowStaysOnTopHint)
        #显示托盘信息

        self.trayIcon = QtGui.QSystemTrayIcon(self)
        self.trayIcon.setIcon(QtGui.QIcon("images/favorite.ico"))

        self.trayIcon.activated.connect(self.trayClick)       #点击托盘
        self.trayMenu()                                       #右键菜单
        self.trayIcon.show()

        labelUser = QtGui.QLabel("账号", self)
        labelUser.setGeometry(QtCore.QRect(125, 95, 50, 25))
        labelPassword = QtGui.QLabel("密码", self)
        labelPassword.setGeometry(QtCore.QRect(125, 130, 50, 25))

        labelCheckCode = QtGui.QLabel("验证码", self)
        labelCheckCode.setGeometry(QtCore.QRect(110, 165, 70, 25))

        labelRemember = QtGui.QLabel("记住密码", self)
        labelRemember.setGeometry(QtCore.QRect(180, 193, 70, 25))

        self.labelFlower = QtGui.QLabel(self)
        self.labelFlower.setGeometry(QtCore.QRect(2, 2, 175, 197))
        self.labelFlower.setPixmap(QtGui.QPixmap("images/flowers.png"))

        self.username = QtGui.QLineEdit("", self)
        self.username.setGeometry(QtCore.QRect(160, 95, 150, 25))
        self.username.setStyleSheet(
            "QLineEdit{background-color:#ffffff;background-position:center;border-radius:4px;border:#F0F000 solid 1px;}")

        self.password = QtGui.QLineEdit('', self)
        self.password.setGeometry(QtCore.QRect(160, 130, 150, 25))
        self.password.setEchoMode(QtGui.QLineEdit.Password)
        self.password.setValidator(QtGui.QRegExpValidator(Qt.QRegExp("[A-Za-z0-9]+"), self))
        self.password.setStyleSheet(
            "QLineEdit{background-color:#ffffff;background-position:center;border-radius:4px;border:#F0F000 solid 1px;}")

        self.imageCodeInput = QtGui.QLineEdit("", self)
        self.imageCodeInput.setGeometry(QtCore.QRect(160, 165, 60, 25))
        self.imageCodeInput.setMaxLength(4)
        self.imageCodeInput.setStyleSheet(
            "QLineEdit{background-color:#ffffff;background-position:center;border-radius:4px;border:#F0F000 solid 1px;}")

        self.rememberMe = QtGui.QCheckBox('', self)
        self.rememberMe.setStyleSheet("QCheckBox{background:transparent;}")
        self.rememberMe.setGeometry(QtCore.QRect(156, 196, 17, 17))
        self.rememberMe.setCursor(QtCore.Qt.PointingHandCursor)

        self.imageCode = ImageCode(3)
        self.imageCode.setParent(self)
        self.imageCode.setGeometry(QtCore.QRect(231, 165, 78, 25))
        self.imageCode.setCursor(QtCore.Qt.PointingHandCursor)
        self.refreshImageCode()

        self.loginButton = QtGui.QPushButton("登录", self)
        self.loginButton.setGeometry(QtCore.QRect(100, 220, 176, 30))
        self.loginButton.setStyleSheet(
            "QPushButton{background-image:url(images/loginButton.png);background-position:center;border-radius:4px;border:#F0F000 solid 1px;}")
        self.loginButton.setCursor(QtCore.Qt.PointingHandCursor)

        self.minButton = LabelBtn(1)               #定义最小化按钮 ID:1
        self.minButton.setParent(self)
        self.minButton.setGeometry(300, 7, 14, 14)
        self.minButton.setStyleSheet("LabelBtn{background:url(images/min.png);}")
        self.minButton.setCursor(QtCore.Qt.PointingHandCursor)

        self.closeButton = LabelBtn(2)              #定义关闭按钮 ID:2
        self.closeButton.setParent(self)
        self.closeButton.setGeometry(320, 7, 14, 14)
        self.closeButton.setStyleSheet("LabelBtn{background:url(images/close.png); }")
        self.closeButton.setCursor(QtCore.Qt.PointingHandCursor)

        self.connect(self.loginButton, QtCore.SIGNAL("clicked()"), self.login)

        self.autoFillLoginData()

    def autoFillLoginData(self):
        loginData = self.ctr.getLoginData()

        if 'remember' in loginData and int(loginData['remember']) == 1:
            self.username.setText(loginData['username'])
            self.password.setText(loginData['password'])
            self.rememberMe.setChecked(True)
            self.autoLogin = True

    def refreshImageCode(self):
        self.imageCode.setImage(self.ctr.refreshLoginCode())

    def login(self):

        username = self.username.text()
        password = self.password.text()
        rememberMe = int(self.rememberMe.isChecked())
        imageCode = self.imageCodeInput.text().lower()
        data = {'username': username, 'password': password, 'remember': rememberMe, 'logInCode': imageCode}

        if self.ctr.submitLoginForm(data):
            self.trayIcon.setVisible(False)
            self.loginStatus = True
            QtGui.QDialog.accept(self)
        else:
            self.refreshImageCode()
            self.imageCodeInput.setText('')

    def btnHandle(self, ID):
        #最小化
        if ID == 1:
            self.hide()
        elif ID == 2:
            #关闭
            self.trayIcon.hide()
            self.close()
            QtGui.QDialog.accept(self)

    def getLogin(self):
        return self.loginStatus

    def btnEnter(self, ID):
        #鼠标进入
        if ID == 1:
            pass
            self.minButton.setPixmap(QtGui.QPixmap("images/min.png"))
        elif ID == 2:
            self.closeButton.setPixmap(QtGui.QPixmap("images/close.png"))

    def btnLeave(self, ID):
        #鼠标离开
        '''false.png这张图片是不存在的，目的是要在鼠标
         离开后还原背景，因为默认按钮我已经PS在背景上了'''
        self.minButton.setPixmap(QtGui.QPixmap("images/min.png"))
        self.closeButton.setPixmap(QtGui.QPixmap("images/close.png"))


    def trayMenu(self):
        #右击托盘弹出的菜单

        self.trayIcon.setToolTip("订票精灵")
        self.showNormal = QtGui.QAction("显示窗口", self, triggered=self.showNormal)
        self.update_ = QtGui.QAction("升级", self, triggered=self.update)
        self.quitAction = QtGui.QAction("退出", self, triggered=self.closeWindow)
        self._12306_ = QtGui.QAction("访问12306", self, triggered=self._12306)
        self.sinaWb_ = QtGui.QAction("新浪微薄", self, triggered=self.sinaWb)

        self.trayIconMenu = QtGui.QMenu(self)
        #self.trayIconMenu.addSeparator()
        self.trayIconMenu.addAction(self.showNormal)
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
        self.close()
        QtGui.QDialog.accept(self)

    def _12306(self):
        webbrowser.open_new('http://www.12306.cn/mormhweb/')

    def sinaWb(self):
        webbrowser.open_new('http://weibo.com/shuangelaide')

    def resizeEvent(self, event):
        #重绘窗体背景
        pass

    def trayClick(self, reason):
        #双击托盘
        if reason == QtGui.QSystemTrayIcon.DoubleClick:
            self.showNormal()
        else:
            pass

    def mousePressEvent(self, event):
        #鼠标点击事件
        if event.button() == QtCore.Qt.LeftButton:
            self.dragPosition = event.globalPos() - self.frameGeometry().topLeft()
            self.setCursor(QtCore.Qt.ClosedHandCursor)
            event.accept()

    def mouseMoveEvent(self, event):
        #鼠标移动事件
        if event.buttons() == QtCore.Qt.LeftButton:
            self.move(event.globalPos() - self.dragPosition)
            self.setCursor(QtCore.Qt.ClosedHandCursor)
            event.accept()

    def mouseReleaseEvent(self, event):
        self.m_mouse_down = False
        self.setCursor(QtCore.Qt.ArrowCursor)


if __name__ == "__main__":
    import sys

    app = QtGui.QApplication(sys.argv)
    frm = Login('')
    frm.show()
    sys.exit(app.exec_())
 