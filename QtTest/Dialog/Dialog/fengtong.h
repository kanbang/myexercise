#ifndef FENG_TONG_H
#define FENG_TONG_H
#include <QDialog>
class QLabel;
class QDialogButtonBox;
class QComboBox;
class QLineEdit;
class Fengtong : public QDialog
{
	Q_OBJECT;
public:
	Fengtong(QWidget *parent =0);
	virtual ~Fengtong();

	QComboBox* xuanzefengtongCB;
	QComboBox* xuanzebeitaCB;
	QComboBox* rukouCB;
	QComboBox* zhijingCB;
	QLineEdit* fengtongLthEdit;
	QLineEdit* fengtongAreaEdit;
	QLineEdit* fengtongjietouNEdit;
	QLineEdit* everyREdit;
	QString stljiekouMethod;
public slots:
	void onChanged(int index);
private:
	QLabel* fengtongleixingLabel;
	QLabel* xuanzebeitaLabel;
	QLabel* rukouLabel;
	QLabel* fengtongLthLabel;
	QLabel* fengtongAreaLabel;
	QLabel* fengtongjietouLabel;
	QLabel* everyRLabel;
	QLabel* zhijingLabel;
	QDialogButtonBox *btnBox;//确定和取消的选择按钮
};
#endif