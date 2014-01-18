#ifndef DECISION_H
#define DECISION_H
#include <cmath>
#include <qstring.h>
#include <QApplication>
#include <QObject>
#include <QtSql>

const double t=25;
const double Ctan=0.0002;
const double K_CH4 = 1.5;
const double kongqi_rho = 1.2;
const double ksai_on = 1;
const double ksai_jo = 0.025;
const double wz_Value_L = 10;
class Decision
{
public:
	Decision();
	virtual ~Decision();
	double feliang_Q();
	double chen_Q();
	double wa_Q();
	double xu_Q();
	double hunhe_Q();
	double yansuan_Q();
	double fengzu_R();
	QString display();
	double Caculate();
	double fengtongxishu();
	double loufeng_P();
	double fengjiQH();
	void fengjixuanxing();
	void WriteData(QString stl);
	QString chouchuDisplay(QString strDisplay);
	QString yaruDisplay(QString strDisplay);
	QString hangdaoDisplay(QString strDisplay);
//private:
	double zhayaoliang_Value_A;
	double CO_Value_b;
	double are_Value_S;
	double lenth_Value_L;
	double Yloufengxishu_Value_P;
	double Cloufengxishu_Value_P;
	double Yfengliang_Value_Q;
	double Cfengliang_Value_Q;
	double xu_Value_Q;
	double wa_Value_Q;
	double chen_Value_Q;
	double Yfengzu_Value_R;
	double Cfengzu_Value_R;
	double Yzhijing_Value_D;
	double Czhijing_Value_D;
	double wasiyongchu_Value;
	//double C_jin_CH4;
	//double C_CH4;
	double chanchen_G;
	double zuigao_G;
	double ji_G;
	double Ybanjing_alpha;
	double Cbanjing_alpha;
	double Cksai_be;
	double Yksai_be;
	double ksai_in;
	double Ybeita;
	double Cbeita;

	double YfengtongLth;
	double YfengtongArea;
	double Yfengtongjieshu_n;
	double Yfengtong_K;
	double Yfengtong_enta;
	double Yevery_Value_R;
	
	QString Ystlfengtongleixing;
	QString YstljiekouMethod;
	QString Ystlfengjileixing;
	QString Yfengji;

	double CfengtongLth;
	double CfengtongArea;
	double Cfengtongjieshu_n;
	double Cfengtong_K;
	double Cfengtong_enta;
	double Cevery_Value_R;
	QString stldriveMethod;
	QString tongfengMethod;
	QString Cstlfengtongleixing;
	QString Cstlrukouyuanjiao;
	QString CstljiekouMethod;
	QString Cstlfengjileixing;
	QString Cfengji;
private:
	double Cfengji_Value_Q;
	double Cfengji_Value_H;
	double Yfengji_Value_Q;
	double Yfengji_Value_H;
	int fengtongleixingFlag;
	double Yju_Value_L;
	double Cju_Value_L;
	//QString strDisplay;

};
#endif