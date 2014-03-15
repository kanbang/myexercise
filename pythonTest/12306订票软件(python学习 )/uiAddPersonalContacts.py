from PyQt4 import QtCore, QtGui, Qt
from datetime import *
import alertWindow


class AddPersonalContactsPanel(QtGui.QWidget):
    def __init__(self, ctr, parent=None):
        super(AddPersonalContactsPanel, self).__init__(parent)
        self.ctr = ctr
        username = QtGui.QLabel('姓名:')
        self.usernameInput = QtGui.QLineEdit()
        self.usernameInput.setStyleSheet("QLineEdit{padding:5px;}")
        self.usernameInput.setValidator(QtGui.QRegExpValidator(Qt.QRegExp("[\u4e00-\u9fa5]{2,5}"), self))

        self.resize(400, 400)
        sex = QtGui.QLabel('性别:')
        self.sexMan = QtGui.QRadioButton()
        self.sexMan.setChecked(True)
        sexManText = QtGui.QLabel(' 男')
        self.sexMan.setCursor(QtCore.Qt.PointingHandCursor)

        self.sexWoman = QtGui.QRadioButton()
        self.sexWoman.setCursor(QtCore.Qt.PointingHandCursor)
        sexWomanText = QtGui.QLabel('女')

        sexLayout = QtGui.QHBoxLayout()
        sexLayout.addWidget(self.sexMan)
        sexLayout.addWidget(sexManText)
        sexLayout.addWidget(self.sexWoman)
        sexLayout.addWidget(sexWomanText)
        sexLayout.addStretch()

        birthDay = QtGui.QLabel('出生日期：')
        self.birthDaySelect = QtGui.QDateEdit()
        self.birthDaySelect.setDisplayFormat(("yyyy.MM.dd"))
        self.birthDaySelect.setStyleSheet("QLineEdit{padding:4px;}")

        cardType = QtGui.QLabel('证件类型：')
        self.cardTypeSelect = QtGui.QComboBox()
        self.cardTypeSelect.addItem('二代身份证', '1')
        self.cardTypeSelect.addItem('一代身份证', '2')
        self.cardTypeSelect.addItem('港澳通行证', 'C')
        self.cardTypeSelect.addItem('台湾通行证', 'G')
        self.cardTypeSelect.addItem('护照', 'B')
        self.cardTypeSelect.setStyleSheet("QLineEdit{padding:4px;}")

        cardNo = QtGui.QLabel('证件号码：')
        self.cardNoInput = QtGui.QLineEdit()
        self.cardNoInput.setStyleSheet("QLineEdit{padding:4px;}")

        passengerType = QtGui.QLabel('旅客类型：')
        self.passengerTypeSelect = QtGui.QComboBox()
        self.passengerTypeSelect.addItem('成人', '1')
        self.passengerTypeSelect.addItem('儿童', '2')
        self.passengerTypeSelect.addItem('学生', '3')
        self.passengerTypeSelect.addItem('伤残军人', '4')

        mobileNo = QtGui.QLabel('手机号码：')
        self.mobileNoInput = QtGui.QLineEdit()
        self.mobileNoInput.setStyleSheet("QLineEdit{padding:4px;}")
        self.mobileNoInput.setValidator(QtGui.QRegExpValidator(Qt.QRegExp("[0-9]{11}"), self))

        submit = QtGui.QPushButton('提交')
        submit.setCursor(QtCore.Qt.PointingHandCursor)
        submit.setStyleSheet("QPushButton{margin-top:30px;height:50px}")
        submit.setMaximumSize(QtCore.QSize(200, 65))
        self.connect(submit, QtCore.SIGNAL("clicked()"), self.submit)

        layout = QtGui.QGridLayout()
        layout.addWidget(username, 0, 0, 1, 1)
        layout.addWidget(self.usernameInput, 0, 1, 1, 1)
        layout.addWidget(sex, 1, 0, 1, 1)
        layout.addLayout(sexLayout, 1, 1, 1, 1)
        layout.addWidget(birthDay, 2, 0, 1, 1)
        layout.addWidget(self.birthDaySelect, 2, 1, 1, 1)

        layout.addWidget(cardType, 3, 0, 1, 1)
        layout.addWidget(self.cardTypeSelect, 3, 1, 1, 1)

        layout.addWidget(cardNo, 4, 0, 1, 1)
        layout.addWidget(self.cardNoInput, 4, 1, 1, 1)

        layout.addWidget(passengerType, 5, 0, 1, 1)
        layout.addWidget(self.passengerTypeSelect, 5, 1, 1, 1)

        layout.addWidget(mobileNo, 6, 0, 1, 1)
        layout.addWidget(self.mobileNoInput, 6, 1, 1, 1)
        layout.addWidget(submit, 7, 1, 1, 2)

        vLayout = QtGui.QVBoxLayout()
        vLayout.addLayout(layout)
        vLayout.addStretch()
        self.setLayout(vLayout)

    def alert(self, text_):
        alertWindow.AlertWindow(text=text_).exec_()

    def submit(self):
        username = self.usernameInput.text()
        sex = 'M' if self.sexMan.isChecked() else 'F'
        birthDay = [int(i) for i in (self.birthDaySelect.text()).split('.')]
        dateObject = date(birthDay[0], birthDay[1], birthDay[2])
        passengerType = self.passengerTypeSelect.itemData(self.passengerTypeSelect.currentIndex())
        mobileNo = self.mobileNoInput.text()
        cardType = self.cardTypeSelect.itemData(self.cardTypeSelect.currentIndex())
        cardNo = self.cardNoInput.text()

        if len(username) == 0:
            self.alert('姓名为空')
            return

        elif len(cardNo) == 0:
            self.alert('证件号码为空')
            return

        elif not len(mobileNo) == 11:
            self.alert('手机号码不正确')
            return

        elif int(cardType) == 2:
            if not len(cardNo) == 18:
                self.alert('证件号码不正确')
                return

        data = {
            'name': username,
            'sex_code': sex,
            'country_code': 'CN',
            'card_type': cardType,
            'card_no': cardNo,
            'passenger_type': passengerType,
            'mobile_no': mobileNo,
            'born_date': dateObject.isoformat(),
        }

        if self.ctr.addPersonalContact(data):
            self.alert('添加成功')

        else:
            self.alert('添加失败')


