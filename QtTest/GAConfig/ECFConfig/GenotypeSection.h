#pragma once

#include "ConfigableWidget.h"

QT_BEGIN_NAMESPACE
class QStackedWidget;
QT_END_NAMESPACE

class GenotypeSection : public ConfigableWidget
{
	Q_OBJECT
public:
	GenotypeSection(QWidget* parent = 0);
	virtual TiXmlNode* writeConfNode();

private:
	QComboBox* genosCombox;
	QStackedWidget* pagesWidget;
};
