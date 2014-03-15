from PyQt4 import QtGui, Qt, QtCore


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


class LabelBtn(QtGui.QLabel):
    def __init__(self, ID):
        super(LabelBtn, self).__init__()
        self.setMouseTracking(True)
        self.ID = ID

    def mouseReleaseEvent(self, event):  #注:
        #鼠标点击事件
        self.parent().btnHandle(self.ID)


class AlertWindow(QtGui.QDialog):
    def __init__(self, ctr=None, text=None, parent=None):
        super(AlertWindow, self).__init__(parent)
        self.ctr = ctr
        self.setMouseTracking(True)
        self.resize(400, 120)

        self.text = QtGui.QLabel()
        if text:
            self.setText(text)
        self.activateWindow()

        self.setWindowFlags(Qt.Qt.FramelessWindowHint)

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

        displayWidgetLayout = QtGui.QVBoxLayout()

        displayWidgetLayout.addWidget(self.text)
        displayWidgetLayout.setAlignment(QtCore.Qt.AlignCenter | QtCore.Qt.AlignVCenter)
        displayWidget.setLayout(displayWidgetLayout)
        #窗口居中显示
        desktop = QtGui.QApplication.desktop()
        width = desktop.width()
        height = desktop.height()
        self.move((width - self.width()) / 2, (height - self.height()) / 2)

        self.closeButton = LabelBtn(2)              #定义关闭按钮 ID:2
        self.closeButton.setParent(self)
        self.resizeEvent()
        self.closeButton.setStyleSheet("LabelBtn{background:url(images/close.png); }")
        self.closeButton.setCursor(QtCore.Qt.PointingHandCursor)

        layout = QtGui.QVBoxLayout()
        layout.addWidget(topBar)
        layout.addWidget(displayWidget)
        layout.setSpacing(0)
        layout.setMargin(0)
        self.setLayout(layout)
        self.setStyleSheet(("QDialog{padding:20px; background-color:#FFFFFF;border:2px solid #f64a6b; }"))

    def resizeEvent(self, event=None):
        self.closeButton.setGeometry(self.width() - 20, 7, 14, 14)

    def setText(self, text):
        self.text.setText(text)

    def btnHandle(self, ID):

        if ID == 2:
            #关闭
            self.close()

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

