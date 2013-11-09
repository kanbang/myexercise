#pragma once

#include "ConfigableWidget.h"

QT_BEGIN_NAMESPACE
class QComboBox;
class QStackedWidget;
QT_END_NAMESPACE

class AlgoSection : public ConfigableWidget
{
	Q_OBJECT
public:
	AlgoSection(QWidget* parent=0);
	virtual TiXmlNode* writeConfNode();

private:
	QComboBox* algosCombox;
	QStackedWidget* pagesWidget;
};
