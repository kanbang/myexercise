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

	//�������һϵ������Ҫ�������Ի���ģ������ù��ó�Ա
	QComboBox* drivemethodItem;//�����ʽ������ѡ��ť
	QComboBox* isLijinZaojinCB;
	QLineEdit *lenthLineEdit;	//�����������
	QLineEdit *areaLineEdit;		//�����������
	QLineEdit *gasLineEdit;     //��˹ӿ��������
	QLineEdit *zhayaoLineEdit;  //ըҩ��������
	//QLineEdit *C_CH4LineEdit;	//���������˹Ũ��
	//QLineEdit *C_jin_CH4LineEdit;//����������˹Ũ�ȵ�����
	QLineEdit *chan_GLineEdit;   //������������
	QLineEdit *zuigao_GLineEdit; //������������
	QLineEdit *ji_GLineEdit;	 //�������л��ײ�����
private:
	QLabel *lenthLabel;			//��ʾ��������ȡ�
	QLabel *areaLabel;			//��ʾ�����������
	QLabel *gasLabel;			//��ʾ����˹ӿ������
	QLabel *zhayaoLabel;
	//QLabel *C_CH4Label;
	//QLabel *C_jin_CH4Label;
	QLabel *chan_GLabel;
	QLabel *zuigao_GLabel;
	QLabel *ji_GLabel;
	QLabel *drivmethodLabel;	//��ʾ�������ʽ��
	QLabel *isLijinZaojinLabel;
	QDialogButtonBox *buttonBox;//ȷ����ȡ����ѡ��ť
};
#endif