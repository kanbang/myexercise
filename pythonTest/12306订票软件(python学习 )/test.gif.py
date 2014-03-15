from PyQt4 import QtGui, Qt, QtCore
import sys


class Notifier(QtGui.QDialog):
    def __init__(self, parent=None):
        super(Notifier, self).__init__(parent)
        gif = QtGui.QMovie()
        gif.setFileName("code.webp")
        labelRemember = QtGui.QLabel(self)
        labelRemember.setMovie(gif)
        gif.start()
        #gif.setFileName("1.gif")
        gif2 = QtGui.QMovie()
        gif2.setFileName("1.gif")
        labelRemember.setMovie(gif2)
        gif2.start()
        #labelRemember.setMovie(gif)
        #gif.start()
        labelRemember.setGeometry(QtCore.QRect(0, 0, 400, 400))


if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    notifier = Notifier()
    notifier.show()
    sys.exit(app.exec_())

