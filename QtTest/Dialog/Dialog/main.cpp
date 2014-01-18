#include <QtGui/QApplication>  
#include <QtCore/QTextCodec> 
//#include <QLibraryInfo>
#include <QTranslator>
//#include <QLocale>
#include "mainDialog.h"  

int main(int argc, char *argv[])  
{  
	QApplication app(argc, argv);

	//QApplication::addLibraryPath("./plugins");

	QTranslator translator1,translator2;
	translator1.load(":/qt_zh_CN");
	translator2.load(":/dialog_zh");
	app.installTranslator(&translator1);
	app.installTranslator(&translator2);

	//QTextCodec* pTextCodec = QTextCodec::codecForLocale();  
	//QTextCodec::setCodecForTr(pTextCodec);  

	CDialogBuildin dlg;  
	dlg.show();

	return app.exec();  
}  