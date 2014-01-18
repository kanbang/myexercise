#ifndef DIALOG_H 
#define DIALOG_H

#include "canshuDialog.h" 
#include "fengtong.h"
#include <QWidget>

class QTextEdit;  
class QPushButton;  
class CDialogBuildin : public QWidget  
{  
	Q_OBJECT  

public:  
	CDialogBuildin(QWidget *parent = 0);  
	virtual ~CDialogBuildin(); 

private:  
	QTextEdit*      pTextEdit_display;  //������������ʾ
	QPushButton*    pushBtn_ventilateWays; //ͨ�緽ʽѡ��ť
	QPushButton*    pushBtn_hairType;      //��Ͳ����ѡ��ť
	QPushButton*    pushBtn_ensure;			//����༭��ť
	QPushButton*    pushBtn_basicPara;		//���������������ڰ�ť
	QPushButton*    pushBtn_jisuan;		//���߰�ť�����������ť���Խ������ϵͳ�����м��㣬����
	QPushButton*    pushBtn_close;			//�رհ�ť
	QPushButton*    pushBtn_Cfengji;			//���������������
	QPushButton*	pushBtn_Yfengji;		//ѡ���Ͳֱ��
	QString			stlLeixing ;
	QString			CstlLeixing;
	QString			YstlLeixing;
	QString			stlislijin;
	//int				currunt;

private slots:  
	void    doPushBtn();				//�ۺ�������ť�Ķ���
};

#endif