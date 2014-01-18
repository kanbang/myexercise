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
	QTextEdit*      pTextEdit_display;  //主窗口文字显示
	QPushButton*    pushBtn_ventilateWays; //通风方式选择按钮
	QPushButton*    pushBtn_hairType;      //风筒类型选择按钮
	QPushButton*    pushBtn_ensure;			//字体编辑按钮
	QPushButton*    pushBtn_basicPara;		//基本参数输入的入口按钮
	QPushButton*    pushBtn_jisuan;		//决策按钮，按下这个按钮可以进入计算系统，进行计算，决策
	QPushButton*    pushBtn_close;			//关闭按钮
	QPushButton*    pushBtn_Cfengji;			//清除数据重新输入
	QPushButton*	pushBtn_Yfengji;		//选择风筒直径
	QString			stlLeixing ;
	QString			CstlLeixing;
	QString			YstlLeixing;
	QString			stlislijin;
	//int				currunt;

private slots:  
	void    doPushBtn();				//槽函数，按钮的动作
};

#endif