#include <QApplication>
#include <QTextCodec>
#include <iostream>
using namespace std;

#include "GAConfigDialog.h"

int main( int argc, char* argv[] )
{
    QApplication app( argc, argv );

    QTranslator translator;
    translator.load( "ECFConfig_zh" );
    //QString locale = QLocale::system().name();
    //translator.load(QString("ECFConfig_") + locale);  // ����ϵͳlocale����qm�����ļ�
    app.installTranslator( &translator );

    //QTextCodec *codec = QTextCodec::codecForName("System");    //��ȡϵͳ����
    //QTextCodec::setCodecForLocale(codec);
    //QTextCodec::setCodecForCStrings(codec);
    //QTextCodec::setCodecForTr(codec);

    GAConfigDialog w;
    w.show();

    return app.exec();
}