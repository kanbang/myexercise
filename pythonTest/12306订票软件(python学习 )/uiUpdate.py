from PyQt4 import QtCore, QtGui


class uiUpdate(QtGui.QWidget):
    def __init__(self, ctr, parent=None):
        super(uiUpdate, self).__init__(parent)
        self.ctr = ctr

    def build(self):
        self.txt = QtGui.QLabel('2010年1月1日更新')
        self.button = QtGui.QPushButton('检查更新')
        layout = QtGui.QVBoxLayout()
        layout.setSpacing(10)
        layout.setMargin(0)
        layout.addStretch()
        layout.addWidget(self.txt)
        layout.addWidget(self.button)
        layout.addStretch()
        layout.setAlignment(QtCore.Qt.AlignCenter | QtCore.Qt.AlignVCenter)
        self.setLayout(layout)

    def checkUpdate(self):
        if self.ctr.checkUpdate():
            self.txt.setText('有新版本更新')
            self.button.setText('点击更新')
        else:
            self.txt.setText('已是最新版本')

