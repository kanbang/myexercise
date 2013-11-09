#include <QApplication>
#include <QVBoxLayout>
#include <QtGui>
#include "qtPainter.h"

int main(int argc, char *argv[])
{
	QApplication app(argc,argv);
	QWidget *widget=new QWidget;
	widget->resize(800,600);

	TestWidget *test=new TestWidget;
	//QTextEdit *tex=new QTextEdit;
	QVBoxLayout* vbox = new QVBoxLayout;

	vbox->addWidget(test);
	//vbox->addWidget(tex);

	widget->setLayout(vbox);

	widget->show();
	return app.exec();
}
