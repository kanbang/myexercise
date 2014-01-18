#include "fengtong.h"
#include <QtGui/QtGui>

class CDialogBuildin;
Fengtong::Fengtong( QWidget *parent ) : QDialog( parent )
{
	QGridLayout* FTGridLayout = new QGridLayout();  
	xuanzefengtongCB = new QComboBox;
	xuanzebeitaCB = new QComboBox;
	rukouCB = new QComboBox;
	zhijingCB = new QComboBox;
	fengtongLthEdit = new QLineEdit("0");
	fengtongjietouNEdit = new QLineEdit("0");
	everyREdit = new QLineEdit("0");

	fengtongleixingLabel = new QLabel(tr("fengtongleixingshi:"));
	xuanzebeitaLabel = new QLabel(tr("xuanzejiaodu:"));
	rukouLabel = new QLabel(tr("rukoushifouyuanjiao:"));
	fengtongLthLabel = new QLabel(tr("fengtongchangdu:"));
	fengtongjietouLabel = new QLabel(tr("fengtongjietoushu:"));
	everyRLabel = new QLabel(tr("everyR:"));
	zhijingLabel = new QLabel(tr("zhijingdexuanze"));

	QStringList fengtongleixing = QStringList()
		<<tr("dianji")
		<< tr("yingzhifengtong")
		<< tr("rouxingfengtong");
	xuanzefengtongCB->addItems(fengtongleixing);
	xuanzefengtongCB->setCurrentIndex(0);
	//xuanzefengtongCB->setEditable(true);
	QStringList xuanzebeita = QStringList()
		<<"20" << "40" << "60" << "80" << "90" << "100" << "120";
	xuanzebeitaCB->addItems(xuanzebeita);
	xuanzebeitaCB->setEditable(true);
	xuanzebeitaCB->setCurrentIndex(4);
	QStringList xuanzeyuanjiao = QStringList()
		<< tr("wanquanxiuyuan")
		<<tr("buxiuyuand zhijiao");
	rukouCB->addItems(xuanzeyuanjiao);
	rukouCB->setCurrentIndex(0);

	QStringList zhijings = QStringList()
		<<"200" << "300" << "400" << "500"
		<< "600" << "700" << "800" << "900" << "1000";
	zhijingCB->addItems(zhijings);
	zhijingCB->setEditable(true);
	zhijingCB->setCurrentIndex(3);

	connect(xuanzefengtongCB, SIGNAL(activated(int)), this, SLOT(onChanged(int)));
	//connect(xuanzefengtongCB, SIGNAL(currentIndexChanged(int)), this, SLOT(onChanged(int)));
	FTGridLayout->addWidget(fengtongleixingLabel,0,0,1,1);
	FTGridLayout->addWidget(xuanzefengtongCB,0,1,1,1); 
	FTGridLayout->addWidget(xuanzebeitaLabel,1,0,1,1);
	FTGridLayout->addWidget(xuanzebeitaCB,1,1,1,1);
	FTGridLayout->addWidget(rukouLabel,2,0,1,1);
	FTGridLayout->addWidget(rukouCB,2,1,1,1);
	FTGridLayout->addWidget(fengtongLthLabel,3,0,1,1);
	FTGridLayout->addWidget(fengtongLthEdit,3,1,1,1);
	FTGridLayout->addWidget(fengtongjietouLabel,4,0,1,1);
	FTGridLayout->addWidget(fengtongjietouNEdit,4,1,1,1);
	FTGridLayout->addWidget(everyRLabel,5,0,1,1);
	FTGridLayout->addWidget(everyREdit,5,1,1,1);
	FTGridLayout->addWidget(zhijingLabel,6,0,1,1);
	FTGridLayout->addWidget(zhijingCB,6,1,1,1);

	btnBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	connect(btnBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(btnBox, SIGNAL(rejected()), this, SLOT(reject()));

	FTGridLayout->addWidget(btnBox,8,1,1,1); 

	setLayout(FTGridLayout); 
	setWindowTitle(tr("fengtongdecanshu"));  
	resize(80,150);  
}

Fengtong::~Fengtong()  
{  

} 

void Fengtong::onChanged( int index )
{
	QStringList items;
	if(index!=0)
	{
		if(index == 1 )
		{
			items <<tr("yingzhifalanjiacaoshen")<<tr("yingzhijiajiaozhidianquan");
			everyREdit->setEnabled(true);
		}
		if(index == 2)
		{
			items <<tr("rouxingchajie") << tr("rouxingluoquanfanbian");
			everyREdit->setEnabled(false);
		}
		bool ok;
		QString item = QInputDialog::getItem(this, tr("xuanzelejiekou"),
			tr("jiekou:"), items, 0, false, &ok);
		if (ok && !item.isEmpty())
		{
			stljiekouMethod = item;
		}
		
	}
	else
	{
		QMessageBox::warning(this, tr("Message"), tr("xuanzecunwu"), QMessageBox::Ok); 
	}
}