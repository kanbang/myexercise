#include "canshuDialog.h"
#include "mainDialog.h"
#include <QtGui/QtGui>

class CDialogBuildin;
CanShuDialog::CanShuDialog( QWidget *parent ) : QDialog( parent )
{
	QGridLayout* CSGridLayout = new QGridLayout();  

	lenthLabel = new QLabel(tr("Lenth:"));
	areaLabel = new QLabel(tr("Are:"));
	gasLabel = new QLabel(tr("wasiyongchuliang:"));
	zhayaoLabel = new QLabel(tr("zhayaoliang:"));
	drivmethodLabel = new QLabel(tr("juejinfangshi:"));
	//C_CH4Label = new QLabel(tr("zuigao:"));
	//C_jin_CH4Label = new QLabel(tr("jinfengch4:"));
	chan_GLabel = new QLabel(tr("chanchenliang:"));
	zuigao_GLabel = new QLabel(tr("zuigaoyunxuchanchen:"));
	ji_GLabel = new QLabel(tr("jinfenghanchen:"));
	isLijinZaojinLabel = new QLabel(tr("shifou:"));
	//C_CH4LineEdit = new QLineEdit("0");
	//C_jin_CH4LineEdit = new QLineEdit("0");
	chan_GLineEdit = new QLineEdit("0");
	zuigao_GLineEdit = new QLineEdit("0");
	ji_GLineEdit = new QLineEdit("0");
	lenthLineEdit = new QLineEdit("0");
	areaLineEdit = new QLineEdit("0");
	gasLineEdit = new QLineEdit("0");
	zhayaoLineEdit = new QLineEdit("0");

	drivemethodItem = new QComboBox;
	isLijinZaojinCB = new QComboBox;
	QStringList juejinfangshi = QStringList()
		<< tr("meihangjuejin")
		<< tr("yanhangjuejin");
	drivemethodItem->addItems(juejinfangshi);
	drivemethodItem->setCurrentIndex(0);
	//drivemethodItem->setEditable(true);

	QStringList islijin = QStringList()
		<< tr("Yes") << tr("No");
	isLijinZaojinCB->addItems(islijin);
	isLijinZaojinCB->setCurrentIndex(1);

	CSGridLayout->addWidget(lenthLabel,0,0,1,1);  
	CSGridLayout->addWidget(lenthLineEdit,0,1,1,1); 
	CSGridLayout->addWidget(areaLabel,1,0,1,1);  
	CSGridLayout->addWidget(areaLineEdit,1,1,1,1);  
	CSGridLayout->addWidget(gasLabel,2,0,1,1);  
	CSGridLayout->addWidget(gasLineEdit,2,1,1,1);
	CSGridLayout->addWidget(zhayaoLabel,3,0,1,1);
	CSGridLayout->addWidget(zhayaoLineEdit,3,1,1,1);
	/*CSGridLayout->addWidget(C_CH4Label,4,0,1,1);
	CSGridLayout->addWidget(C_CH4LineEdit,4,1,1,1);
	CSGridLayout->addWidget(C_jin_CH4Label,5,0,1,1);
	CSGridLayout->addWidget(C_jin_CH4LineEdit,5,1,1,1);*/
	CSGridLayout->addWidget(chan_GLabel,4,0,1,1);
	CSGridLayout->addWidget(chan_GLineEdit,4,1,1,1);
	CSGridLayout->addWidget(zuigao_GLabel,5,0,1,1);
	CSGridLayout->addWidget(zuigao_GLineEdit,5,1,1,1);
	CSGridLayout->addWidget(ji_GLabel,6,0,1,1);
	CSGridLayout->addWidget(ji_GLineEdit,6,1,1,1);
	CSGridLayout->addWidget(drivmethodLabel,7,0,1,1); 
	CSGridLayout->addWidget(drivemethodItem,7,1,1,1); 
	CSGridLayout->addWidget(isLijinZaojinLabel,8,0,1,1);
	CSGridLayout->addWidget(isLijinZaojinCB,8,1,1,1);

	buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	
	CSGridLayout->addWidget(buttonBox,9,1,1,1); 

	setLayout(CSGridLayout); 
	setWindowTitle(tr("jibencanshudeshuru"));  
	resize(300,400);  
}

CanShuDialog::~CanShuDialog()  
{  

}  