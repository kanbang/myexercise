#ifndef OTHER_H
#define OTHER_H
#include <QDialog>
class QLabel;
class QLineEdit;
class QDialogButtonBox;
class QComboBox;
class CanShuDialog : public QDialog
{
	Q_OBJECT;
public:
	CanShuDialog(QWidget *parent =0);
	virtual ~CanShuDialog();

	//下面的这一系列是需要传给主对话框的，所以用公用成员
	QComboBox* drivemethodItem;//掘进方式的下拉选择按钮
	QComboBox* isLijinZaojinCB;
	QLineEdit *lenthLineEdit;	//巷道长度输入
	QLineEdit *areaLineEdit;		//巷道断面输入
	QLineEdit *gasLineEdit;     //瓦斯涌出量输入
	QLineEdit *zhayaoLineEdit;  //炸药量的输入
	//QLineEdit *C_CH4LineEdit;	//最高允许瓦斯浓度
	//QLineEdit *C_jin_CH4LineEdit;//进风流中瓦斯浓度的输入
	QLineEdit *chan_GLineEdit;   //产尘量的输入
	QLineEdit *zuigao_GLineEdit; //最高允许产尘量
	QLineEdit *ji_GLineEdit;	 //进风流中基底产尘量
private:
	QLabel *lenthLabel;			//显示“巷道长度”
	QLabel *areaLabel;			//显示“断面面积”
	QLabel *gasLabel;			//显示“瓦斯涌出量”
	QLabel *zhayaoLabel;
	//QLabel *C_CH4Label;
	//QLabel *C_jin_CH4Label;
	QLabel *chan_GLabel;
	QLabel *zuigao_GLabel;
	QLabel *ji_GLabel;
	QLabel *drivmethodLabel;	//显示“掘进方式”
	QLabel *isLijinZaojinLabel;
	QDialogButtonBox *buttonBox;//确定和取消的选择按钮
};
#endif