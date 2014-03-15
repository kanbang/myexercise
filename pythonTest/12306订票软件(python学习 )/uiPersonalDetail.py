from PyQt4 import QtCore, QtGui


class PersonalDetailPanel(QtGui.QWidget):
    def __init__(self, parent=None):
        super(PersonalDetailPanel, self).__init__(parent)

    def build(self, personalData):
        headerLabels = ['用户名:', '姓名:', '生日:', '姓别:', '证件类型:', '证件号码:', '手机号码:', '旅客类型:']
        personalData = [personalData['username'], personalData['realname'], personalData['birthday'],
                        personalData['sex'], personalData['cardType'], personalData['cardNo'], personalData['mobileNo'],
                        personalData['passengerType']]
        gLayout = QtGui.QGridLayout()

        for i in range(len(headerLabels)):
            label1 = QtGui.QLabel(headerLabels[i])
            label1.setAlignment(QtCore.Qt.AlignRight | QtCore.Qt.AlignVCenter)
            label1.setStyleSheet('QLabel{padding:10px;}')
            label2 = QtGui.QLabel(personalData[i])
            gLayout.addWidget(label1, i, 0, 1, 1)
            gLayout.addWidget(label2, i, 1, 1, 1)

        hLayout = QtGui.QHBoxLayout()
        hLayout.addLayout(gLayout)
        hLayout.addStretch()
        vLayout = QtGui.QVBoxLayout()
        vLayout.addLayout(hLayout)
        vLayout.addStretch()
        #removeLayout
        self.setLayout(vLayout)

