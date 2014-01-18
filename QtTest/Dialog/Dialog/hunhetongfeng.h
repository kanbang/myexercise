#ifndef HUNHE_TONGFENG_H
#define HUNHE_TONGFENG_H
#include <QDialog>
#include <QtGui/QtGui>

class QLabel;
class QDialogButtonBox;
class QComboBox;
class QLineEdit;
class Hunhetongfeng : public QDialog
{
	Q_OBJECT;
public:
	Hunhetongfeng(QWidget *parent =0);
	virtual ~Hunhetongfeng();

	QComboBox* YxuanzefengtongCB;
	QComboBox* YxuanzebeitaCB;
	QComboBox* YrukouCB;
	QComboBox* YzhijingCB;
	QLineEdit* YfengtongLthEdit;
	QLineEdit* YfengtongAreaEdit;
	QLineEdit* YfengtongjietouNEdit;
	QLineEdit* YeveryREdit;
	QString YstljiekouMethod;

	QComboBox* CxuanzefengtongCB;
	QComboBox* CxuanzebeitaCB;
	QComboBox* CrukouCB;
	QComboBox* CzhijingCB;
	QLineEdit* CfengtongLthEdit;
	QLineEdit* CfengtongAreaEdit;
	QLineEdit* CfengtongjietouNEdit;
	QLineEdit* CeveryREdit;
	QString CstljiekouMethod;

	void creatYRForm();
	void creatCCForm();
public slots:
	void onYRChanged(int index);
	void onCCChanged(int index);
private:
	QLabel* YfengtongleixingLabel;
	QLabel* YxuanzebeitaLabel;
	QLabel* YrukouLabel;
	QLabel* YfengtongLthLabel;
	QLabel* YfengtongAreaLabel;
	QLabel* YfengtongjietouLabel;
	QLabel* YeveryRLabel;
	QLabel* YzhijingLabel;

	QLabel* CfengtongleixingLabel;
	QLabel* CxuanzebeitaLabel;
	QLabel* CrukouLabel;
	QLabel* CfengtongLthLabel;
	QLabel* CfengtongAreaLabel;
	QLabel* CfengtongjietouLabel;
	QLabel* CeveryRLabel;
	QLabel* CzhijingLabel;

	QGroupBox* YRGroupBox;
	QGroupBox* CCGroupBox;

	QDialogButtonBox *btnBox;//确定和取消的选择按钮
};
#endif