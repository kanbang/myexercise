#include "hunhetongfeng.h"
#include <QtGui/QtGui> 
Hunhetongfeng::Hunhetongfeng( QWidget *parent ):QDialog(parent)
{
	creatYRForm();
	creatCCForm();

	btnBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	connect(btnBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(btnBox, SIGNAL(rejected()), this, SLOT(reject()));

	QHBoxLayout *submainLayout = new QHBoxLayout;
	submainLayout->addWidget(YRGroupBox);
	submainLayout->addWidget(CCGroupBox);
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(submainLayout);
	mainLayout->addWidget(btnBox);
	setLayout(mainLayout);
	setWindowTitle(tr("fengtongdecanshu"));  
	resize(160,150);  

}
Hunhetongfeng::~Hunhetongfeng()
{

}

void Hunhetongfeng::creatYRForm()
{
	YRGroupBox = new QGroupBox(tr("yarushicanshu"));
	QFormLayout* YRFormLayout = new QFormLayout();  
	YxuanzefengtongCB = new QComboBox;
	YxuanzebeitaCB = new QComboBox;
	YzhijingCB = new QComboBox;
	YfengtongLthEdit = new QLineEdit("0");
	YfengtongjietouNEdit = new QLineEdit("0");
	YeveryREdit = new QLineEdit("0");

	YfengtongleixingLabel = new QLabel(tr("fengtongleixingshi:"));
	YxuanzebeitaLabel = new QLabel(tr("xuanzejiaodu:"));
	YfengtongLthLabel = new QLabel(tr("fengtongchangdu:"));
	YfengtongjietouLabel = new QLabel(tr("fengtongjietoushu:"));
	YeveryRLabel = new QLabel(tr("everyR:"));
	YzhijingLabel = new QLabel(tr("zhijingdexuanze"));

	QStringList Yfengtongleixing = QStringList()
		<<tr("dianji")
		<< tr("yingzhifengtong")
		<< tr("rouxingfengtong");
	YxuanzefengtongCB->addItems(Yfengtongleixing);

	QStringList Yxuanzebeita = QStringList()
		<<"20" << "40" << "60" << "80" << "90" << "100" << "120";
	YxuanzebeitaCB->addItems(Yxuanzebeita);
	YxuanzebeitaCB->setEditable(true);
	YxuanzebeitaCB->setCurrentIndex(4);


	QStringList Yzhijings = QStringList()
		<<"200" << "300" << "400" << "500"
		<< "600" << "700" << "800" << "900" << "1000";
	YzhijingCB->addItems(Yzhijings);
	YzhijingCB->setEditable(true);
	YzhijingCB->setCurrentIndex(3);

	connect(YxuanzefengtongCB, SIGNAL(activated(int)), this, SLOT(onYRChanged(int)));

	YRFormLayout->addRow(YfengtongleixingLabel,YxuanzefengtongCB);
	YRFormLayout->addRow(YxuanzebeitaLabel,YxuanzebeitaCB);
	YRFormLayout->addRow(YfengtongLthLabel,YfengtongLthEdit);
	YRFormLayout->addRow(YfengtongjietouLabel,YfengtongjietouNEdit);
	YRFormLayout->addRow(YeveryRLabel,YeveryREdit);
	YRFormLayout->addRow(YzhijingLabel,YzhijingCB);

	YRGroupBox->setLayout(YRFormLayout);
}

void Hunhetongfeng::creatCCForm()
{
	CCGroupBox = new QGroupBox(tr("chouchucanshu"));
	QFormLayout* CCFormLayout = new QFormLayout();
	CxuanzefengtongCB = new QComboBox;
	CxuanzebeitaCB = new QComboBox;
	CrukouCB = new QComboBox;
	CzhijingCB = new QComboBox;
	CfengtongLthEdit = new QLineEdit("0");
	CfengtongjietouNEdit = new QLineEdit("0");
	CeveryREdit = new QLineEdit("0");

	CfengtongleixingLabel = new QLabel(tr("fengtongleixingshi:"));
	CxuanzebeitaLabel = new QLabel(tr("xuanzejiaodu:"));
	CrukouLabel = new QLabel(tr("rukoushifouyuanjiao:"));
	CfengtongLthLabel = new QLabel(tr("fengtongchangdu:"));
	CfengtongjietouLabel = new QLabel(tr("fengtongjietoushu:"));
	CeveryRLabel = new QLabel(tr("everyR:"));
	CzhijingLabel = new QLabel(tr("zhijingdexuanze"));


	QStringList Cfengtongleixing = QStringList()
		<<tr("dianji")
		<< tr("yingzhifengtong")
		<< tr("rouxingfengtong");
	CxuanzefengtongCB->addItems(Cfengtongleixing);
	CxuanzefengtongCB->setCurrentIndex(0);

	QStringList Cxuanzebeita = QStringList()
		<<"20" << "40" << "60" << "80" << "90" << "100" << "120";
	CxuanzebeitaCB->addItems(Cxuanzebeita);
	CxuanzebeitaCB->setEditable(true);
	CxuanzebeitaCB->setCurrentIndex(4);

	QStringList Cxuanzeyuanjiao = QStringList()
		<< tr("wanquanxiuyuan")
		<<tr("buxiuyuand zhijiao");
	CrukouCB->addItems(Cxuanzeyuanjiao);
	CrukouCB->setCurrentIndex(0);

	QStringList Czhijings = QStringList()
		<<"200" << "300" << "400" << "500"
		<< "600" << "700" << "800" << "900" << "1000";
	CzhijingCB->addItems(Czhijings);
	CzhijingCB->setEditable(true);
	CzhijingCB->setCurrentIndex(3);

	connect(CxuanzefengtongCB, SIGNAL(activated(int)), this, SLOT(onCCChanged(int)));

	CCFormLayout->addRow(CfengtongleixingLabel,CxuanzefengtongCB);
	CCFormLayout->addRow(CxuanzebeitaLabel,CxuanzebeitaCB);
	CCFormLayout->addRow(CrukouLabel,CrukouCB);
	CCFormLayout->addRow(CfengtongLthLabel,CfengtongLthEdit);
	CCFormLayout->addRow(CfengtongjietouLabel,CfengtongjietouNEdit);
	CCFormLayout->addRow(CeveryRLabel,CeveryREdit);
	CCFormLayout->addRow(CzhijingLabel,CzhijingCB);

	CCGroupBox->setLayout(CCFormLayout);
}

void Hunhetongfeng::onCCChanged( int index )
{
	QStringList items;
	if(index!=0)
	{
		if(index == 1 )
		{
			items <<tr("yingzhifalanjiacaoshen")<<tr("yingzhijiajiaozhidianquan");
			CeveryREdit->setEnabled(true);
		}
		if(index == 2)
		{
			items <<tr("rouxingchajie") << tr("rouxingluoquanfanbian");
			CeveryREdit->setEnabled(false);
		}
		bool ok;
		QString item = QInputDialog::getItem(this, tr("xuanzelejiekou"),
			tr("jiekou:"), items, 0, false, &ok);
		if (ok && !item.isEmpty())
		{
			CstljiekouMethod = item;
		}

	}
	else
	{
		QMessageBox::warning(this, tr("Message"), tr("xuanzecunwu"), QMessageBox::Ok); 
	}
}
void Hunhetongfeng::onYRChanged( int index )
{
	QStringList items;
	if(index!=0)
	{
		if(index == 1 )
		{
			items <<tr("yingzhifalanjiacaoshen")<<tr("yingzhijiajiaozhidianquan");
			YeveryREdit->setEnabled(true);
		}
		if(index == 2)
		{
			items <<tr("rouxingchajie") << tr("rouxingluoquanfanbian");
			YeveryREdit->setEnabled(false);
		}
		bool ok;
		QString item = QInputDialog::getItem(this, tr("xuanzelejiekou"),
			tr("jiekou:"), items, 0, false, &ok);
		if (ok && !item.isEmpty())
		{
			YstljiekouMethod = item;
		}

	}
	else
	{
		QMessageBox::warning(this, tr("Message"), tr("xuanzecunwu"), QMessageBox::Ok); 
	}
}