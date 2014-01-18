#include "mainDialog.h"
#include <QtGui/QtGui>  
#include <QColor>
#include "decision.h"
#include "hunhetongfeng.h"
#include <QListWidgetItem>

Decision* decision = new Decision;
CDialogBuildin::CDialogBuildin(QWidget *parent)  
: QWidget(parent)  
{  
	pTextEdit_display = new QTextEdit(tr("Welcome to the system!")); 
	pTextEdit_display->setReadOnly(true); 
	pTextEdit_display->setFont(QFont( "Cursive", 11 , QFont::Normal ));

	QGridLayout* pGridLayout = new QGridLayout();  

	pushBtn_basicPara = new QPushButton(tr("Base Parameter"));	   
	pushBtn_ventilateWays = new QPushButton(tr("tongfengfangfa"));  
	pushBtn_hairType = new QPushButton(tr("fengtongcanshu")); 
	pushBtn_Yfengji = new QPushButton(tr("yarufengjixuanxing"));
	pushBtn_jisuan = new QPushButton(tr("jisuan")); 
	pushBtn_Cfengji = new QPushButton(tr("chouchufengjixuanxing"));
	pushBtn_ensure = new QPushButton(tr("quedingfangan"));
	pushBtn_close = new QPushButton(tr("Exit"));

	pGridLayout->setColumnStretch(1, 1);
	pGridLayout->setColumnMinimumWidth(1, 300);//设置最小显示，也就是默认窗口的大小
	pGridLayout->addWidget(pushBtn_basicPara,0,4,1,1); 
	pGridLayout->addWidget(pushBtn_ventilateWays,1,4,1,1);  
	pGridLayout->addWidget(pushBtn_hairType,2,4,1,1); 
	pGridLayout->addWidget(pushBtn_jisuan,3,4,1,1); 
	pGridLayout->addWidget(pushBtn_Yfengji,4,4,1,1);
	pGridLayout->addWidget(pushBtn_Cfengji,5,4,1,1);
	pGridLayout->addWidget(pushBtn_ensure,6,4,1,1);
	pGridLayout->addWidget(pushBtn_close,7,4,1,1);  
	pGridLayout->addWidget(pTextEdit_display,0,0,8,4); 

	connect(pushBtn_basicPara,SIGNAL(clicked()),this,SLOT(doPushBtn()));
	connect(pushBtn_ventilateWays,SIGNAL(clicked()),this,SLOT(doPushBtn()));  
	connect(pushBtn_hairType,SIGNAL(clicked()),this,SLOT(doPushBtn()));  
	connect(pushBtn_Yfengji,SIGNAL(clicked()),this,SLOT(doPushBtn()));
	connect(pushBtn_jisuan,SIGNAL(clicked()),this,SLOT(doPushBtn())); 
	connect(pushBtn_Cfengji,SIGNAL(clicked()),this,SLOT(doPushBtn()));
	connect(pushBtn_close,SIGNAL(clicked()),this,SLOT(close()));  
	connect(pushBtn_ensure,SIGNAL(clicked()),this,SLOT(doPushBtn()));

	setLayout(pGridLayout);  
	setWindowTitle(tr("title"));  
	resize(350,400); 
}  

CDialogBuildin::~CDialogBuildin()  
{  

}  

void CDialogBuildin::doPushBtn()  
{ 
	QPushButton* btn = qobject_cast<QPushButton*>(sender());  //用来判断是按的哪个按钮
	if(btn == pushBtn_ventilateWays)  
	{  
		QStringList items;
		items << tr("chouchushitongfeng") 
			<<tr("yarutongfeng")<< tr("hunheshitongfeng"); 

		bool ok;
		QString item = QInputDialog::getItem(this, tr("xuanzetongfengfangfa"),
			tr("tongfengfangfa:"), items, 1, false, &ok);//弹出的窗口
		if (ok && !item.isEmpty())
		{
			decision->tongfengMethod = item;	

			if(item == tr("chouchushitongfeng"))
			{
				pushBtn_Yfengji->setEnabled(false);
				pushBtn_Cfengji->setEnabled(true);
			}

			if(item == tr("yarutongfeng"))
			{
				pushBtn_Cfengji->setEnabled(false);
				pushBtn_Yfengji->setEnabled(true);
			}

			if(item == tr("hunheshitongfeng"))
			{
				pushBtn_Yfengji->setEnabled(true);
				pushBtn_Cfengji->setEnabled(true);
			}
		}
	}  
	else if(btn == pushBtn_hairType)  
	{  
		if(decision->tongfengMethod == tr("chouchushitongfeng") || decision->tongfengMethod == tr("yarutongfeng"))
		{
			Fengtong fengtong(this);
			if(decision->tongfengMethod == tr("chouchushitongfeng"))
			{
				fengtong.rukouCB->setEnabled(true);
			}
			else
			{
				fengtong.rukouCB->setEnabled(false);
			}
			if(fengtong.exec() == 1)
			{
				stlLeixing = fengtong.xuanzefengtongCB->currentText();
				if(stlLeixing == tr("dianji"))
				{
					if(decision->tongfengMethod==tr("chouchushitongfeng"))
					{
						decision->Cstlfengtongleixing = tr("meiyouxuanzefengtongleixing");
					}
					if(decision->tongfengMethod == tr("yarutongfeng"))
					{
						decision->Ystlfengtongleixing = tr("meiyouxuanzefengtongleixing");
					}
				}
				else
				{
					if(decision->tongfengMethod==tr("chouchushitongfeng"))
					{
						decision->Cstlfengtongleixing = stlLeixing;
					}
					if(decision->tongfengMethod == tr("yarutongfeng"))
					{
						decision->Ystlfengtongleixing = stlLeixing;
					}
				}
				if(decision->tongfengMethod==tr("chouchushitongfeng"))
				{
					decision->Cstlrukouyuanjiao = fengtong.rukouCB->currentText();
					decision->Cbeita = fengtong.xuanzebeitaCB->currentText().toDouble();
					decision->CfengtongLth = fengtong.fengtongLthEdit->text().toDouble();
					decision->Cfengtongjieshu_n = fengtong.fengtongjietouNEdit->text().toDouble();
					decision->CstljiekouMethod = fengtong.stljiekouMethod;
					decision->Cevery_Value_R = fengtong.everyREdit->text().toDouble();
					decision->Czhijing_Value_D = fengtong.zhijingCB->currentText().toDouble();
				}

				if(decision->tongfengMethod==tr("yarutongfeng"))
				{
					decision->Ybeita = fengtong.xuanzebeitaCB->currentText().toDouble();
					decision->YfengtongLth = fengtong.fengtongLthEdit->text().toDouble();
					decision->Yfengtongjieshu_n = fengtong.fengtongjietouNEdit->text().toDouble();
					decision->YstljiekouMethod = fengtong.stljiekouMethod;
					decision->Yevery_Value_R = fengtong.everyREdit->text().toDouble();
					decision->Yzhijing_Value_D = fengtong.zhijingCB->currentText().toDouble();
				}
			}
		}
		else if(decision->tongfengMethod == tr("hunheshitongfeng"))
		{
			Hunhetongfeng hunhe(this);
			if(hunhe.exec() == 1)
			{

				CstlLeixing = hunhe.CxuanzefengtongCB->currentText();
				YstlLeixing = hunhe.YxuanzefengtongCB->currentText();
				if(CstlLeixing == tr("dianji"))
				{
					decision->Cstlfengtongleixing = tr("meiyouxuanzefengtongleixing");
				}
				else
				{
					decision->Cstlfengtongleixing = hunhe.CxuanzefengtongCB->currentText();
				}
				if(YstlLeixing == tr("dianji"))
				{
					decision->Ystlfengtongleixing = tr("meiyouxuanzefengtongleixing");
				}
				else
				{
					decision->Ystlfengtongleixing = hunhe.YxuanzefengtongCB->currentText();
				}
				decision->Cstlrukouyuanjiao = hunhe.CrukouCB->currentText();
				decision->Cbeita = hunhe.CxuanzebeitaCB->currentText().toDouble();
				decision->CfengtongLth = hunhe.CfengtongLthEdit->text().toDouble();
				decision->Cfengtongjieshu_n = hunhe.CfengtongjietouNEdit->text().toDouble();
				decision->CstljiekouMethod = hunhe.CstljiekouMethod;
				decision->Cevery_Value_R = hunhe.CeveryREdit->text().toDouble();
				decision->Czhijing_Value_D = hunhe.CzhijingCB->currentText().toDouble();

				decision->Ybeita = hunhe.YxuanzebeitaCB->currentText().toDouble();
				decision->YfengtongLth = hunhe.YfengtongLthEdit->text().toDouble();
				decision->Yfengtongjieshu_n = hunhe.YfengtongjietouNEdit->text().toDouble();
				decision->YstljiekouMethod = hunhe.YstljiekouMethod;
				decision->Yevery_Value_R = hunhe.YeveryREdit->text().toDouble();
				decision->Yzhijing_Value_D = hunhe.YzhijingCB->currentText().toDouble();

			}
		}
		else
		{
			QMessageBox fangfamsgBox;
			fangfamsgBox.setText(QObject::tr("haimeiyouxuanzetongfeng!"));
			fangfamsgBox.setWindowTitle(QObject::tr("tishi"));
			fangfamsgBox.exec();
		}

	} 

	else if(btn == pushBtn_basicPara)  
	{  
		// 基本参数 
		decision->Yfengji=QString();
		decision->Cfengji=QString();
		CanShuDialog canshuDialog(this);
		if(canshuDialog.exec() == 1)
		{
			QString stl = pTextEdit_display->toPlainText();

			QString stlLenth = canshuDialog.lenthLineEdit->text();
			decision->lenth_Value_L = stlLenth.toDouble();
			QString stlAre = canshuDialog.areaLineEdit->text();
			decision->are_Value_S = stlAre.toDouble();
			QString stlGas = canshuDialog.gasLineEdit->text();
			decision->wasiyongchu_Value = stlGas.toDouble();
			QString stlZhayao = canshuDialog.zhayaoLineEdit->text();
			decision->zhayaoliang_Value_A = stlZhayao.toDouble();
			//QString stlCH4 = canshuDialog.C_CH4LineEdit->text();
			//decision->C_CH4 = stlCH4.toDouble();
			//QString stljinCH4 = canshuDialog.C_jin_CH4LineEdit->text();
			//decision->C_jin_CH4 = stljinCH4.toDouble();
			QString stlchan = canshuDialog.chan_GLineEdit->text();
			decision->chanchen_G = stlchan.toDouble();
			QString stlzuigao = canshuDialog.zuigao_GLineEdit->text();
			decision->zuigao_G = stlzuigao.toDouble();
			QString stlji = canshuDialog.ji_GLineEdit->text();
			decision->ji_G = stlji.toDouble();
			QString stlDrivMe = canshuDialog.drivemethodItem->currentText();
			decision->stldriveMethod = stlDrivMe;
			stlislijin = canshuDialog.isLijinZaojinCB->currentText();
			if(stlDrivMe == tr("meihangjuejin"))
			{
				decision->CO_Value_b = 100;
			}

			else if(stlDrivMe == tr("yanhangjuejin"))
			{
				decision->CO_Value_b = 40;
				/*decision->wasiyongchu_Value = 0.0;*/
			}

			QString stlJYDisplay;
			if(stlislijin == tr("Yes"))
			{
				stlJYDisplay.append(tr("jianyixuanze600-1000mmyingzhi"));
				//currunt =4;
			}
			else
			{
				if(decision->lenth_Value_L <= 200 && decision->lenth_Value_L >0)
				{
					stlJYDisplay.append(tr("jianyi400mm"));
				}
				else if(decision->lenth_Value_L >200 && decision->lenth_Value_L <=500)
				{
					stlJYDisplay.append(tr("jianyi500mm"));
				}
				else if (decision->lenth_Value_L>500 && decision->lenth_Value_L <=1000)
				{
					stlJYDisplay.append(tr("jianyi800-1000mm"));
					//currunt = 5;
				}
				else
				{
					stlJYDisplay.append(tr("Welcome to the system!"));
					//currunt = 0;
				}
			}
			pTextEdit_display->setText(stlJYDisplay);
		}

	}  
	else if(btn == pushBtn_jisuan)  
	{  
		// 计算 
		/*if((decision->Cstlfengtongleixing.isEmpty() && decision->Ystlfengtongleixing.isEmpty())\
			||(decision->Cstlfengtongleixing == tr("meiyouxuanzefengtongleixing") && decision->Ystlfengtongleixing == tr("meiyouxuanzefengtongleixing"))\
			||(decision->Cstlfengtongleixing.isEmpty()&&decision->Ystlfengtongleixing == tr("meiyouxuanzefengtongleixing"))\
			||(decision->Ystlfengtongleixing.isEmpty()&&decision->Cstlfengtongleixing == tr("meiyouxuanzefengtongleixing")))
		{
			QMessageBox ftmsgBox;
			ftmsgBox.setWindowTitle(QObject::tr("tishi"));
			ftmsgBox.setText(QObject::tr("meoyouxuanzefengtongleixing"));
			ftmsgBox.exec();
		}
		else
		{*/
			decision->Caculate();
			QString str = decision->display();
			pTextEdit_display->setText(str);
		//}
	}  

	else if(btn == pushBtn_Yfengji)
	{
		QStringList items;
		if(decision->Yfengji != QObject::tr("meiyoukexuan\n"))
		{
			if(decision->Yfengji.isEmpty())
			{
				QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
				db.setDatabaseName("Fengji.db");
				if (!db.open())  
				{  
					QMessageBox::warning(this, tr("Message"), tr("dabukai"), QMessageBox::Ok);
				}
				QSqlQuery q; 
				q.exec("select * from Chouchu");
				while (q.next())
				{    
					//QString name = q.value(0).toString(); 
					items << q.value(0).toString();
				}
				q.exec("select * from Yaru where Leixing not like 'JBT%'");
				while (q.next())
				{    
					//QString name = q.value(0).toString(); 
					items << q.value(0).toString();
				}
				db.close();
			}
			else
			{
				items=decision->Yfengji.split("\r\n");
			}
		}
		else
		{
			QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
			db.setDatabaseName("Fengji.db");
			if (!db.open())  
			{  
				QMessageBox::warning(this, tr("Message"), tr("dabukai"), QMessageBox::Ok);
			} 
			QSqlQuery q; 
			q.exec("select * from Yaru");
			while (q.next())
			{    
				QString name = q.value(0).toString(); 
				/*int FLmin = q.value(1).toInt();
				int FLmax =q.value(2).toInt();
				int FYmin = q.value(3).toInt();
				int FYmax = q.value(4).toInt();*/
				items << q.value(0).toString();
			}
			db.close();
		}
		bool ok;
		QString item = QInputDialog::getItem(this, tr("xuanzefengjileixing"),
			tr("fengjileixing:"), items, 0, true, &ok);
		if (ok && !item.isEmpty())
		{
			decision->Ystlfengjileixing = item;
		}
	}

	else if(btn == pushBtn_Cfengji)
	{
		QStringList items;
//本来这是下拉的，我想用表格式的，改了一点，还没有该回来
		QWidget* widget = new QWidget;  
		QListWidget* listWidget = new QListWidget;  
		QVBoxLayout* layout = new QVBoxLayout;  
		int i=0;

		if(decision->Cfengji != QObject::tr("meiyoukexuan\n"))
		{
			if(decision->Cfengji.isEmpty())
			{
				QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
				db.setDatabaseName("Fengji.db");
				if (!db.open())  
				{  
					QMessageBox::warning(this, tr("Message"), tr("dabukai"), QMessageBox::Ok);
				}
				QSqlQuery q; 
				q.exec("select * from Chouchu");
				while (q.next())
				{    
					//QString name = q.value(0).toString(); 
					items << q.value(0).toString();
					
					QListWidgetItem* lst2 = new QListWidgetItem(q.value(0).toString(), listWidget);  
					listWidget->insertItem(i, lst2);  
					i++;
				}
				q.exec("select * from Yaru where Leixing not like 'JBT%'");
				while (q.next())
				{    
					//QString name = q.value(0).toString(); 
					items << q.value(0).toString();

					QListWidgetItem* lst2 = new QListWidgetItem(q.value(0).toString(), listWidget);  
					listWidget->insertItem(i, lst2);  
					i++;
				}
				db.close();
			}
			else
			{
				items=decision->Cfengji.split("\r\n");
			}

		}
		else
		{

			QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
			db.setDatabaseName("Fengji.db");
			if (!db.open())  
			{  
				QMessageBox::warning(this, tr("Message"), tr("dabukai"), QMessageBox::Ok);
			}
			QSqlQuery q; 
			q.exec("select * from Chouchu");
			while (q.next())
			{    
				QString name = q.value(0).toString(); 
				items << q.value(0).toString();

				QListWidgetItem* lst2 = new QListWidgetItem(q.value(0).toString(), listWidget);  
				listWidget->insertItem(i, lst2);  
				i++;
			}
			db.close();
		}
		/*bool ok;
		QString item = QInputDialog::getItem(this, tr("xuanzefengjileixing"),
			tr("fengjileixing:"), items, 0, true, &ok);
		if (ok && !item.isEmpty())
		{
			decision->Cstlfengjileixing = item;
		}*/
		
		//listWidget->show();  
		layout->addWidget(listWidget);  
		widget->setLayout(layout);  
		widget->show();  
	}
	else if(btn == pushBtn_ensure)  
	{  

	}
}