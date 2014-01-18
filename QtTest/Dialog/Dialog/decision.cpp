#include "decision.h"
#include "mainDialog.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

Decision::Decision()
{
	are_Value_S =0;
	lenth_Value_L =0;
	zhayaoliang_Value_A=0;
	CO_Value_b=0;
	Yloufengxishu_Value_P=0;
	Cloufengxishu_Value_P=0;
	Yfengliang_Value_Q=0;
	Cfengliang_Value_Q=0;
	xu_Value_Q=0;
	wa_Value_Q=0;
	chen_Value_Q=0;
	Yfengzu_Value_R=0;
	Yzhijing_Value_D=0;
	Cfengzu_Value_R=0;
	Czhijing_Value_D=0;
	wasiyongchu_Value=0;
	//C_jin_CH4=0;
	//C_CH4=0;
	chanchen_G=0;
	zuigao_G=0;
	ji_G=0;
	Ybanjing_alpha=0;
	Cbanjing_alpha=0;
	Cksai_be=0;
	Yksai_be=0;
	ksai_in=0;
	Ybeita=0;
	YfengtongLth=0;
	YfengtongArea=0;
	Yfengtongjieshu_n=0;
	Yfengtong_K=0;
	Yfengtong_enta=0;
	Yevery_Value_R = 0;
	Yfengji_Value_Q = 0;
	Yfengji_Value_H = 0;
	Yfengji = QString();

	Cbeita=0;
	CfengtongLth=0;
	CfengtongArea=0;
	Cfengtongjieshu_n=0;
	Cfengtong_K=0;
	Cfengtong_enta=0;
	Cevery_Value_R = 0;
	Cfengji_Value_Q = 0;
	Cfengji_Value_H = 0;
	Cfengji = QString();
	fengtongleixingFlag = 0;
}

Decision::~Decision()
{

}
double Decision::Caculate()
{
	if(tongfengMethod == QObject::tr("chouchushitongfeng"))
	{
		Cju_Value_L = 1.5*sqrt(are_Value_S);
		if(Cstlrukouyuanjiao == QObject::tr("wanquanxiuyuan"))
		{
			ksai_in = 0.1;
		}
		else
		{
			ksai_in = 0.55;
		}
		Cksai_be = 0.0001133*Cbeita*Cbeita+0.00354*Cbeita+0.03097;
		CfengtongArea = 3.1415926 * Czhijing_Value_D * Czhijing_Value_D * 0.001 *0.001/4;
	}
	if(tongfengMethod == QObject::tr("yarutongfeng"))
	{
		Yju_Value_L = 4*sqrt(are_Value_S);
		Yksai_be = 0.0001133*Ybeita*Ybeita+0.00354*Ybeita+0.03097;
		YfengtongArea = 3.1415926 * Yzhijing_Value_D * Yzhijing_Value_D * 0.001 *0.001/4;
	}
	if(tongfengMethod == QObject::tr("hunheshitongfeng"))
	{
		Cju_Value_L = 1.4*sqrt(are_Value_S);
		Yju_Value_L = 4*sqrt(are_Value_S);
		if(Cstlrukouyuanjiao == QObject::tr("wanquanxiuyuan"))
		{
			ksai_in = 0.1;
		}
		else
		{
			ksai_in = 0.55;
		}
		Cksai_be = 0.0001133*Cbeita*Cbeita+0.00354*Cbeita+0.03097;
		CfengtongArea = 3.1415926 * Czhijing_Value_D * Czhijing_Value_D * 0.001 *0.001/4;
		Yksai_be = 0.0001133*Ybeita*Ybeita+0.00354*Ybeita+0.03097;
		YfengtongArea = 3.1415926 * Yzhijing_Value_D * Yzhijing_Value_D * 0.001 *0.001/4;
	}
	fengtongxishu();
	loufeng_P();
	xu_Q();
	chen_Q();
	wa_Q();
	hunhe_Q();
	feliang_Q();
	fengzu_R();
	fengjiQH();
	fengjixuanxing();
	return 0;
}
double Decision::yansuan_Q()
{
	if(stldriveMethod == QObject::tr("yanhangjuejin"))
	{
		if (Cfengliang_Value_Q <= 9*are_Value_S)
		{
			Cfengliang_Value_Q =9*are_Value_S;
		}
	}
	if(stldriveMethod == QObject::tr("meihangjuejin"))
	{
		if(15*are_Value_S >= Cfengliang_Value_Q)
		{
			Cfengliang_Value_Q = 15*are_Value_S;
		}
	}
	if(Cfengliang_Value_Q >= 240*are_Value_S)
	{
		Cfengliang_Value_Q = 240*are_Value_S;
	}

	if(stldriveMethod == QObject::tr("yanhangjuejin"))
	{
		if (Yfengliang_Value_Q <= 9*are_Value_S)
		{
			Yfengliang_Value_Q =9*are_Value_S;
		}
	}
	if(stldriveMethod == QObject::tr("meihangjuejin"))
	{
		if(15*are_Value_S >= Yfengliang_Value_Q)
		{
			Yfengliang_Value_Q = 15*are_Value_S;
		}
	}
	if(Yfengliang_Value_Q >= 240*are_Value_S)
	{
		Yfengliang_Value_Q = 240*are_Value_S;
	}
	return 0;
}
double Decision::feliang_Q()
{
	if(xu_Value_Q>=Cfengliang_Value_Q)
	{
		Cfengliang_Value_Q = xu_Value_Q;
	}
	if(wa_Value_Q>=Cfengliang_Value_Q)
	{
		Cfengliang_Value_Q = wa_Value_Q;
	}
	if(chen_Value_Q>=Cfengliang_Value_Q)
	{
		Cfengliang_Value_Q =chen_Value_Q;
	}

	if(xu_Value_Q>=Yfengliang_Value_Q)
	{
		Yfengliang_Value_Q = xu_Value_Q;
	}
	if(wa_Value_Q>=Yfengliang_Value_Q)
	{
		Yfengliang_Value_Q = wa_Value_Q;
	}
	if(chen_Value_Q>=Yfengliang_Value_Q)
	{
		Yfengliang_Value_Q =chen_Value_Q;
	}

	yansuan_Q();

	return 0;
}

double Decision::fengzu_R()
{
	if(tongfengMethod == QObject::tr("yarutongfeng"))
	{
		if(Yzhijing_Value_D!=0&&YfengtongArea!=0)
		{
			double d=Yzhijing_Value_D*0.001;
			double R_fr = 6.5*Ybanjing_alpha*YfengtongLth/(pow(d,5));
			double R_jo = Yfengtongjieshu_n*ksai_jo*kongqi_rho/(2*YfengtongArea*YfengtongArea);
			double R_be = Yksai_be*kongqi_rho/(2*YfengtongArea*YfengtongArea);
			double R_on = ksai_on*kongqi_rho/(2*YfengtongArea*YfengtongArea);
			Yfengzu_Value_R = R_fr + R_jo + R_be + R_on;
		}
	}

	if(tongfengMethod == QObject::tr("chouchushitongfeng"))
	{
		if(Czhijing_Value_D!=0&&CfengtongArea!=0)
		{
			double d=Czhijing_Value_D*0.001;
			double R_fr = 6.5*Cbanjing_alpha*CfengtongLth/(pow(d,5));
			double R_jo = Cfengtongjieshu_n*ksai_jo*kongqi_rho/(2*CfengtongArea*CfengtongArea);
			double R_be = Cksai_be*kongqi_rho/(2*CfengtongArea*CfengtongArea);
			double R_in = ksai_in*kongqi_rho/(2*CfengtongArea*CfengtongArea);
			Cfengzu_Value_R = R_fr + R_jo + R_be + R_in;
		}
	}

	if(tongfengMethod == QObject::tr("hunheshitongfeng"))
	{
		if(Yzhijing_Value_D!=0&&YfengtongArea!=0)
		{
			double Yd=Yzhijing_Value_D*0.001;
			double YR_fr = 6.5*Ybanjing_alpha*YfengtongLth/(pow(Yd,5));
			double YR_jo = Yfengtongjieshu_n*ksai_jo*kongqi_rho/(2*YfengtongArea*YfengtongArea);
			double YR_be = Yksai_be*kongqi_rho/(2*YfengtongArea*YfengtongArea);
			double R_on = ksai_on*kongqi_rho/(2*YfengtongArea*YfengtongArea);

			double Cd=Czhijing_Value_D*0.001;
			double CR_fr = 6.5*Cbanjing_alpha*CfengtongLth/(pow(Cd,5));
			double CR_jo = Cfengtongjieshu_n*ksai_jo*kongqi_rho/(2*CfengtongArea*CfengtongArea);
			double CR_be = Cksai_be*kongqi_rho/(2*CfengtongArea*CfengtongArea);
			double R_in = ksai_in*kongqi_rho/(2*CfengtongArea*CfengtongArea);
			Yfengzu_Value_R = YR_fr + YR_jo + YR_be + R_on;
			Cfengzu_Value_R = CR_fr+ CR_jo + CR_be + R_in;
		}

	}
	return 0;
}

double Decision::hunhe_Q()
{
	if (QObject::tr("chouchushitongfeng") == tongfengMethod)
	{
		double pao_L=15 + zhayaoliang_Value_A/5;
		double a = (zhayaoliang_Value_A*CO_Value_b*are_Value_S*pao_L)/Ctan;
		Cfengliang_Value_Q = (0.254/t)*pow(a,1.0/2);
	}
	if (QObject::tr("yarutongfeng") == tongfengMethod)
	{
		double a = (zhayaoliang_Value_A*CO_Value_b*are_Value_S*are_Value_S*lenth_Value_L*lenth_Value_L)/(Yloufengxishu_Value_P*Yloufengxishu_Value_P*Ctan);
		Yfengliang_Value_Q = (0.465/t) *pow(a,1.0/3);
	}
	if (QObject::tr("hunheshitongfeng")== tongfengMethod)
	{
		if(are_Value_S !=0)
		{
			double L_P = 400 * zhayaoliang_Value_A /are_Value_S;
			double a = (zhayaoliang_Value_A*CO_Value_b*are_Value_S*are_Value_S*L_P*L_P)/Ctan;
			if(CfengtongLth > YfengtongLth)
			{
				Yfengliang_Value_Q = (0.465/t) *pow(a,1.0/3);
				if(xu_Value_Q>chen_Value_Q && xu_Value_Q >wa_Value_Q)
				{
					Cfengliang_Value_Q = xu_Value_Q+Yfengliang_Value_Q;
				}
				if(wa_Value_Q > chen_Value_Q)
				{
					Cfengliang_Value_Q = Yfengliang_Value_Q +0.5*60*are_Value_S;
				}
				if(chen_Value_Q>=wa_Value_Q)
				{
					Cfengliang_Value_Q = Yfengliang_Value_Q +0.15*60*are_Value_S;
				}
			}

			if(CfengtongLth < YfengtongLth)
			{
				Cfengliang_Value_Q = (0.465/t) *pow(a,1.0/3);
				if(xu_Value_Q>chen_Value_Q && xu_Value_Q >wa_Value_Q)
				{
					Yfengliang_Value_Q = xu_Value_Q+Cfengliang_Value_Q;
				}
				if(wa_Value_Q > chen_Value_Q)
				{
					Yfengliang_Value_Q = Cfengliang_Value_Q +0.5*60*are_Value_S;
				}
				if(chen_Value_Q>=wa_Value_Q)
				{
					Yfengliang_Value_Q = Cfengliang_Value_Q +0.15*60*are_Value_S;
				}
			}
		}
	}
	return 0;
}

double Decision::chen_Q()
{
	if(zuigao_G-ji_G != 0)
	{
		chen_Value_Q = chanchen_G /(zuigao_G-ji_G);
	}
	return 0;

}

double Decision::wa_Q()
{
	/*if(C_CH4-C_jin_CH4 != 0)
	{
	wa_Value_Q = 100*(100*K_CH4*wasiyongchu_Value)/(C_CH4-C_jin_CH4);
	}*/
	wa_Value_Q = 100 * wasiyongchu_Value * K_CH4;
	return 0;
}


double Decision::xu_Q()
{
	xu_Value_Q = 25*zhayaoliang_Value_A;
	return 0;
}

double Decision::fengtongxishu()
{
	if(Ystlfengtongleixing == QObject::tr("yingzhifengtong"))
	{
		fengtongleixingFlag = 0;
		if (YstljiekouMethod == QObject::tr("yingzhifalanjiacaoshen"))
		{
			Yfengtong_K = 0.0025;
		}
		if (YstljiekouMethod == QObject::tr("yingzhijiajiaozhidianquan"))
		{
			Yfengtong_K = 0.002;
		}
		if(Yzhijing_Value_D == 200)
		{
			Ybanjing_alpha = 49;
		}
		else if(Yzhijing_Value_D == 300)
		{
			Ybanjing_alpha = 44.1;
		}
		else if(Yzhijing_Value_D == 400)
		{
			Ybanjing_alpha = 39.2;
		}
		else if(Yzhijing_Value_D == 500)
		{
			Ybanjing_alpha = 34.3;
		}
		else if(Yzhijing_Value_D == 600)
		{
			Ybanjing_alpha = 29.4;
		}
		else if(Yzhijing_Value_D == 800)
		{
			Ybanjing_alpha =24.5;
		}
	}

	if(Cstlfengtongleixing == QObject::tr("rouxingfengtong"))
	{
		fengtongleixingFlag = 1;
		if (CstljiekouMethod == QObject::tr("rouxingchajie"))
		{
			Cfengtong_enta = 0.015;
		}
		if (CstljiekouMethod == QObject::tr("rouxingluoquanfanbian"))
		{
			Cfengtong_enta = 0.005;
		}
		if(Czhijing_Value_D == 300)
		{
			Cbanjing_alpha = 53;
		}
		else if(Czhijing_Value_D == 400)
		{
			Cbanjing_alpha = 49;
		}
		else if(Czhijing_Value_D == 500)
		{
			Cbanjing_alpha = 45;
		}
		else if(Czhijing_Value_D == 600)
		{
			Cbanjing_alpha = 41;
		}
		else if(Czhijing_Value_D == 700)
		{
			Cbanjing_alpha =38;
		}
		else if(Czhijing_Value_D == 800)
		{
			Cbanjing_alpha =32;
		}
		else if(Czhijing_Value_D == 900)
		{
			Cbanjing_alpha = 30;
		}
		else if(Czhijing_Value_D == 1000)
		{
			Cbanjing_alpha = 29;
		}
	}
	if(Cstlfengtongleixing == QObject::tr("yingzhifengtong"))
	{
		fengtongleixingFlag = 0;
		if (CstljiekouMethod == QObject::tr("yingzhifalanjiacaoshen"))
		{
			Cfengtong_K = 0.0025;
		}
		if (CstljiekouMethod == QObject::tr("yingzhijiajiaozhidianquan"))
		{
			Cfengtong_K = 0.002;
		}
		if(Czhijing_Value_D == 200)
		{
			Cbanjing_alpha = 49;
		}
		else if(Czhijing_Value_D == 300)
		{
			Cbanjing_alpha = 44.1;
		}
		else if(Czhijing_Value_D == 400)
		{
			Cbanjing_alpha = 39.2;
		}
		else if(Czhijing_Value_D == 500)
		{
			Cbanjing_alpha = 34.3;
		}
		else if(Czhijing_Value_D == 600)
		{
			Cbanjing_alpha = 29.4;
		}
		else if(Czhijing_Value_D == 800)
		{
			Cbanjing_alpha =24.5;
		}
	}
	if(Ystlfengtongleixing == QObject::tr("rouxingfengtong"))
	{
		fengtongleixingFlag = 1;
		if (YstljiekouMethod == QObject::tr("rouxingchajie"))
		{
			Yfengtong_enta = 0.015;
		}
		if (YstljiekouMethod == QObject::tr("rouxingluoquanfanbian"))
		{
			Yfengtong_enta = 0.005;
		}
		if(Yzhijing_Value_D == 300)
		{
			Ybanjing_alpha = 53;
		}
		else if(Yzhijing_Value_D == 400)
		{
			Ybanjing_alpha = 49;
		}
		else if(Yzhijing_Value_D == 500)
		{
			Ybanjing_alpha = 45;
		}
		else if(Yzhijing_Value_D == 600)
		{
			Ybanjing_alpha = 41;
		}
		else if(Yzhijing_Value_D == 700)
		{
			Ybanjing_alpha =38;
		}
		else if(Yzhijing_Value_D == 800)
		{
			Ybanjing_alpha =32;
		}
		else if(Yzhijing_Value_D == 900)
		{
			Ybanjing_alpha = 30;
		}
		else if(Yzhijing_Value_D == 1000)
		{
			Ybanjing_alpha = 29;
		}
	}

	Ybanjing_alpha = Ybanjing_alpha*0.0001;
	Cbanjing_alpha = Cbanjing_alpha*0.0001;
	return 0;
}
double Decision::loufeng_P()
{
	if (Ystlfengtongleixing == QObject::tr("yingzhifengtong"))
	{
		double a = 1+Yfengtong_K*Yzhijing_Value_D*0.001*Yfengtongjieshu_n*pow(Yevery_Value_R*YfengtongLth,1.0/2)/3;
		Yloufengxishu_Value_P = pow(a,2);
	}
	else
	{
		if(1-Yfengtongjieshu_n*Yfengtong_enta != 0)
		{
			Yloufengxishu_Value_P = 1.0/(1-Yfengtongjieshu_n*Yfengtong_enta);
		}

	}

	if (Cstlfengtongleixing == QObject::tr("yingzhifengtong"))
	{
		double a = 1+Cfengtong_K*Czhijing_Value_D*0.001*Cfengtongjieshu_n*pow(Cevery_Value_R*CfengtongLth,1.0/2)/3;
		Cloufengxishu_Value_P = pow(a,2);
	}
	else
	{
		if(1-Cfengtongjieshu_n*Cfengtong_enta != 0)
		{
			Cloufengxishu_Value_P = 1.0/(1-Cfengtongjieshu_n*Cfengtong_enta);
		}

	}
	return 0;
}

double Decision::fengjiQH()
{
	Yfengji_Value_Q = Yloufengxishu_Value_P*Yfengliang_Value_Q;
	Yfengji_Value_H = Yfengzu_Value_R*Yfengji_Value_Q*Yfengliang_Value_Q/3600;
	Cfengji_Value_Q = Cloufengxishu_Value_P*Cfengliang_Value_Q;
	Cfengji_Value_H = Cfengzu_Value_R*Cfengji_Value_Q*Cfengliang_Value_Q/3600;
	return 0;
}

void Decision::fengjixuanxing()
{
	Yfengji = QString();
	Cfengji = QString();
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("Fengji.db");
	if (!db.open())
	{  
		QMessageBox fangfamsgBox;
		fangfamsgBox.setText(QObject::tr("dabukai"));
		//fangfamsgBox.setWindowTitle(QObject::tr("tishi"));
		fangfamsgBox.exec();
	}
	QSqlQuery q; 
	if(tongfengMethod == QObject::tr("chouchushitongfeng"))
	{
		q.exec("select * from Chouchu");
		while (q.next())
		{    
			QString name = q.value(0).toString(); 
			int FLmin = q.value(1).toInt();
			int FLmax =q.value(2).toInt();
			int FYmin = q.value(3).toInt();
			int FYmax = q.value(4).toInt();
			if(FLmin<=Cfengji_Value_Q && FLmax>=Cfengji_Value_Q && FYmin<=Cfengji_Value_H && FYmax>=Cfengji_Value_H)
			{
				Cfengji.append(name);
				Cfengji.append("\r\n");
			}
			//qDebug() << name << FLmin<<FLmax<<FYmin<<FYmax<<endl;  
		}
		if(Cfengji.isEmpty())
		{
			Cfengji=(QObject::tr("meiyoukexuan\n"));
		}
	}
	if(tongfengMethod == QObject::tr("yarutongfeng"))
	{
		q.exec("select * from Yaru");
		while (q.next())
		{    
			QString name = q.value(0).toString(); 
			int FLmin = q.value(1).toInt();
			int FLmax =q.value(2).toInt();
			int FYmin = q.value(3).toInt();
			int FYmax = q.value(4).toInt();
			if(FLmin<=Yfengji_Value_Q && FLmax>=Yfengji_Value_Q && FYmin<=Yfengji_Value_H && FYmax>=Yfengji_Value_H)
			{
				Yfengji.append(name);
				Yfengji.append("\r\n");
			}
			//qDebug() << name << FLmin<<FLmax<<FYmin<<FYmax<<endl;  
		}
		if(Yfengji.isEmpty())
		{
			Yfengji=(QObject::tr("meiyoukexuan\n"));
		}
	}
	if(tongfengMethod == QObject::tr("hunheshitongfeng"))
	{
		q.exec("select * from Yaru");
		while (q.next())
		{    
			QString name = q.value(0).toString(); 
			int FLmin = q.value(1).toInt();
			int FLmax =q.value(2).toInt();
			int FYmin = q.value(3).toInt();
			int FYmax = q.value(4).toInt();
			if(FLmin<=Yfengji_Value_Q && FLmax>=Yfengji_Value_Q && FYmin<=Yfengji_Value_H && FYmax>=Yfengji_Value_H)
			{
				Yfengji.append(name);
				Yfengji.append("\r\n");
			}
			//qDebug() << name << FLmin<<FLmax<<FYmin<<FYmax<<endl;  
		}
		q.exec("select * from Chouchu");
		while (q.next())
		{    
			QString name = q.value(0).toString(); 
			int FLmin = q.value(1).toInt();
			int FLmax =q.value(2).toInt();
			int FYmin = q.value(3).toInt();
			int FYmax = q.value(4).toInt();
			if(FLmin<=Cfengji_Value_Q && FLmax>=Cfengji_Value_Q && FYmin<=Cfengji_Value_H && FYmax>=Cfengji_Value_H)
			{
				Cfengji.append(name);
				Cfengji.append("\r\n");
			}
			//qDebug() << name << FLmin<<FLmax<<FYmin<<FYmax<<endl;  
		}
		if(Cfengji.isEmpty())
		{
			Cfengji=(QObject::tr("meiyoukexuan\n"));
		}
		if(Yfengji.isEmpty())
		{
			Yfengji=(QObject::tr("meiyoukexuan\n"));
		}
	}
	db.close();
}

void Decision::WriteData( QString stl )
{

	QMessageBox mb(QObject::tr("tishi"),
		QObject::tr("shifouxierudaobaogao"),
		QMessageBox::Question,
		QMessageBox::Yes | QMessageBox::Default,
		QMessageBox::No | QMessageBox::Escape,
		QMessageBox::NoButton);
	if (mb.exec() == QMessageBox::Yes) 
	{
		QFile fi("report.txt");
		if(fi.open(QIODevice::WriteOnly))
		{
			QTextStream stream(&fi);
			stream << stl;
			fi.close();
		}
		else
		{
			QMessageBox xzmsgBox;
			xzmsgBox.setWindowTitle(QObject::tr("jinggao"));
			xzmsgBox.setText(QObject::tr("Erro."));
			xzmsgBox.exec();
		}
	}
}

QString Decision::hangdaoDisplay(QString strDisplay)
{
	/*strDisplay.append(QObject::tr("shurudejibenxinxi:"));
	strDisplay.append("\r\n");*/
	strDisplay.append(QObject::tr("hangdaochangdu:"));
	strDisplay.append(QString::number(lenth_Value_L));
	strDisplay.append("\r\n");
	strDisplay.append(QObject::tr("hangdaomianji:"));
	strDisplay.append(QString::number(are_Value_S));
	strDisplay.append("\r\n");
	strDisplay.append(QObject::tr("wasiyongchuliang:"));
	strDisplay.append(QString::number(wasiyongchu_Value));
	strDisplay.append("\r\n");
	strDisplay.append(QObject::tr("juejinfangshi:"));
	strDisplay.append(stldriveMethod);
	strDisplay.append("\r\n");
	strDisplay.append(QObject::tr("tongfengfangshi:"));
	strDisplay.append(tongfengMethod);
	strDisplay.append("\r\n");
	return strDisplay;
}
QString Decision::yaruDisplay(QString strDisplay)
{
	//hangdaoDisplay(strDisplay);
	strDisplay.append(QObject::tr("jushananzhuangzaili"));
	strDisplay.append(QString::number(wz_Value_L));
	strDisplay.append(QObject::tr("jinfeng"));
	strDisplay.append("\r\n");
	strDisplay.append(QObject::tr("fengtongmouduan:"));
	strDisplay.append(QString::number(Yju_Value_L));
	strDisplay.append("\r\n");
	strDisplay.append(QObject::tr("fengtongleixing:"));
	strDisplay.append(Ystlfengtongleixing);
	strDisplay.append("\r\n");
	strDisplay.append(QObject::tr("fengtongzhijing:"));
	strDisplay.append(QString::number(Yzhijing_Value_D));
	strDisplay.append("\r\n");
	strDisplay.append(QObject::tr("fengtongchangdu:"));
	strDisplay.append(QString::number(YfengtongLth));
	strDisplay.append("\r\n");
	strDisplay.append(QObject::tr("fengtongduanmianji:"));
	strDisplay.append(QString::number(YfengtongArea));
	strDisplay.append("\r\n");
	strDisplay.append(QObject::tr("fengliang:"));
	strDisplay.append(QString::number(Yfengliang_Value_Q));
	strDisplay.append("\r\n");
	strDisplay.append(QObject::tr("fengzu:"));
	strDisplay.append(QString::number(Yfengzu_Value_R));
	strDisplay.append("\r\n");
	strDisplay.append(QObject::tr("jushangongzuofengliang:"));
	strDisplay.append(QString::number(Yfengji_Value_Q));
	strDisplay.append("\r\n");
	strDisplay.append(QObject::tr("jushangongzuofengya:"));
	strDisplay.append(QString::number(Yfengji_Value_H));
	strDisplay.append("\r\n");
	strDisplay.append(QObject::tr("jiyixuanzejushanxinghaowei:"));
	strDisplay.append(Yfengji);
	strDisplay.append("\r\n");
	return strDisplay;
}

QString Decision::chouchuDisplay(QString strDisplay)
{
	//hangdaoDisplay(strDisplay);
	strDisplay.append(QObject::tr("jushananzhuangzaili"));
	strDisplay.append(QString::number(wz_Value_L));
	strDisplay.append(QObject::tr("yiwaidehuifengce"));
	strDisplay.append("\r\n");
	strDisplay.append(QObject::tr("fengtongmouduan:"));
	strDisplay.append(QString::number(Cju_Value_L));
	strDisplay.append("\r\n");
	strDisplay.append(QObject::tr("fengtongleixing:"));
	strDisplay.append(Cstlfengtongleixing);
	strDisplay.append("\r\n");
	strDisplay.append(QObject::tr("fengtongzhijing:"));
	strDisplay.append(QString::number(Czhijing_Value_D));
	strDisplay.append("\r\n");
	strDisplay.append(QObject::tr("fengtongchangdu:"));
	strDisplay.append(QString::number(CfengtongLth));
	strDisplay.append("\r\n");
	strDisplay.append(QObject::tr("fengtongduanmianji:"));
	strDisplay.append(QString::number(CfengtongArea));
	strDisplay.append("\r\n");
	strDisplay.append(QObject::tr("fengliang:"));
	strDisplay.append(QString::number(Cfengliang_Value_Q));
	strDisplay.append("\r\n");
	strDisplay.append(QObject::tr("fengzu:"));
	strDisplay.append(QString::number(Cfengzu_Value_R));
	strDisplay.append("\r\n");
	strDisplay.append(QObject::tr("jushangongzuofengliang:"));
	strDisplay.append(QString::number(Cfengji_Value_Q));
	strDisplay.append("\r\n");
	strDisplay.append(QObject::tr("jushangongzuofengya:"));
	strDisplay.append(QString::number(Cfengji_Value_H));
	strDisplay.append("\r\n");
	strDisplay.append(QObject::tr("jiyixuanzejushanxinghaowei:"));
	strDisplay.append(Cfengji);
	strDisplay.append("\r\n");
	return strDisplay;
}
QString Decision::display()
{
	//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
	QString qstl;
	QString stlreturn;
	QString Wrt;

	Wrt.append(QObject::tr("meikezhayaochansheng:"));
	Wrt.append(QString::number(CO_Value_b));
	Wrt.append("\r\n");
	Wrt.append(QObject::tr("chanchengliang:"));
	Wrt.append(QString::number(chanchen_G));
	Wrt.append("\r\n");
	Wrt.append(QObject::tr("zuigaowasi:"));
	Wrt.append(QString::number(zuigao_G));
	Wrt.append("\r\n");
	Wrt.append(QObject::tr("jinfengchen:"));
	Wrt.append(QString::number(ji_G));
	Wrt.append("\r\n");
	Wrt.append(QObject::tr("paiyanfengliang:"));
	Wrt.append(QString::number(xu_Value_Q));
	Wrt.append("\r\n");
	Wrt.append(QObject::tr("wasifengliang:"));
	Wrt.append(QString::number(wa_Value_Q));
	Wrt.append("\r\n");
	Wrt.append(QObject::tr("kuangchen:"));
	Wrt.append(QString::number(chen_Value_Q));
	Wrt.append("\r\n");
	stlreturn.append(hangdaoDisplay(qstl));
	//Wrt.append(stlreturn);

	if(tongfengMethod == QObject::tr("yarutongfeng"))
	{
		qstl = QString();
		Wrt.append(QObject::tr("loufengxishu:"));
		Wrt.append(QString::number(Yloufengxishu_Value_P));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtongmocaizulialph:"));
		Wrt.append(QString::number(Ybanjing_alpha));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtongzulixishuksai_be:"));
		Wrt.append(QString::number(Yksai_be));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("beita:"));
		Wrt.append(QString::number(Ybeita));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtongchangdu:"));
		Wrt.append(QString::number(YfengtongLth));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtongmianji:"));
		Wrt.append(QString::number(YfengtongArea));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtongjieshu_n:"));
		Wrt.append(QString::number(Yfengtongjieshu_n));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("jinshulianjiefangshi:"));
		Wrt.append(YstljiekouMethod);
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtong_K:"));
		Wrt.append(QString::number(Yfengtong_K));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtong_enta:"));
		Wrt.append(QString::number(Yfengtong_enta));
		Wrt.append("\r\n");
		if(fengtongleixingFlag == 0)
		{
			Wrt.append(QObject::tr("every_Value_R:"));
			Wrt.append(QString::number(Yevery_Value_R));
			Wrt.append("\r\n");
		}
		stlreturn.append(yaruDisplay(qstl));
		Wrt.append(stlreturn);
	}

	if(tongfengMethod == QObject::tr("chouchushitongfeng"))
	{
		qstl=QString();	
		Wrt.append(QObject::tr("loufengxishu:"));
		Wrt.append(QString::number(Cloufengxishu_Value_P));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtongmocaizulialph:"));
		Wrt.append(QString::number(Cbanjing_alpha));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtongzulixishuksai_be:"));
		Wrt.append(QString::number(Cksai_be));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("beita:"));
		Wrt.append(QString::number(Cbeita));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("ksai_in:"));
		Wrt.append(QString::number(ksai_in));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtongchangdu:"));
		Wrt.append(QString::number(CfengtongLth));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtongmianji:"));
		Wrt.append(QString::number(CfengtongArea));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtongjieshu_n:"));
		Wrt.append(QString::number(Cfengtongjieshu_n));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("jinshulianjiefangshi:"));
		Wrt.append(CstljiekouMethod);
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtong_K:"));
		Wrt.append(QString::number(Cfengtong_K));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("shifouyuanjiao:"));
		Wrt.append(Cstlrukouyuanjiao);
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtong_enta:"));
		Wrt.append(QString::number(Cfengtong_enta));
		Wrt.append("\r\n");
		if(fengtongleixingFlag == 0)
		{
			Wrt.append(QObject::tr("every_Value_R:"));
			Wrt.append(QString::number(Cevery_Value_R));
			Wrt.append("\r\n");
		}
		stlreturn.append(chouchuDisplay(qstl));
		Wrt.append(stlreturn);
	}

	if(tongfengMethod == QObject::tr("hunheshitongfeng"))
	{
		qstl = QString();
		qstl.append("\r\n");
		qstl.append(QObject::tr("yarushi:"));
		qstl.append("\r\n");
		//Wrt.append(qstl);
		stlreturn.append(yaruDisplay(qstl));
		Wrt.append(stlreturn);
		Wrt.append(QObject::tr("loufengxishu:"));
		Wrt.append(QString::number(Yloufengxishu_Value_P));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtongmocaizulialph:"));
		Wrt.append(QString::number(Ybanjing_alpha));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtongzulixishuksai_be:"));
		Wrt.append(QString::number(Yksai_be));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("beita:"));
		Wrt.append(QString::number(Ybeita));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtongchangdu:"));
		Wrt.append(QString::number(YfengtongLth));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtongmianji:"));
		Wrt.append(QString::number(YfengtongArea));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtongjieshu_n:"));
		Wrt.append(QString::number(Yfengtongjieshu_n));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("jinshulianjiefangshi:"));
		Wrt.append(YstljiekouMethod);
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtong_K:"));
		Wrt.append(QString::number(Yfengtong_K));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtong_enta:"));
		Wrt.append(QString::number(Yfengtong_enta));
		Wrt.append("\r\n");

		Wrt.append(QObject::tr("every_Value_R:"));
		Wrt.append(QString::number(Yevery_Value_R));
		Wrt.append("\r\n");
		qstl = QString();
		qstl.append("\r\n");
		qstl.append(QObject::tr("chouchushi:"));
		qstl.append("\r\n");
		//Wrt.append(qstl);
		QString s;
		s=chouchuDisplay(qstl);
		stlreturn.append(s);
		Wrt.append(s);
		Wrt.append(QObject::tr("loufengxishu:"));
		Wrt.append(QString::number(Cloufengxishu_Value_P));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtongmocaizulialph:"));
		Wrt.append(QString::number(Cbanjing_alpha));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtongzulixishuksai_be:"));
		Wrt.append(QString::number(Cksai_be));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("beita:"));
		Wrt.append(QString::number(Cbeita));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("ksai_in:"));
		Wrt.append(QString::number(ksai_in));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtongchangdu:"));
		Wrt.append(QString::number(CfengtongLth));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtongmianji:"));
		Wrt.append(QString::number(CfengtongArea));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtongjieshu_n:"));
		Wrt.append(QString::number(Cfengtongjieshu_n));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("jinshulianjiefangshi:"));
		Wrt.append(CstljiekouMethod);
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtong_K:"));
		Wrt.append(QString::number(Cfengtong_K));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("shifouyuanjiao:"));
		Wrt.append(Cstlrukouyuanjiao);
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("fengtong_enta:"));
		Wrt.append(QString::number(Cfengtong_enta));
		Wrt.append("\r\n");
		Wrt.append(QObject::tr("every_Value_R:"));
		Wrt.append(QString::number(Cevery_Value_R));
		Wrt.append("\r\n");
	}
	WriteData(Wrt);

	return stlreturn;
}