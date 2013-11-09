#pragma once

#include <QtGui>
#include <tinyxml.h>

class ConfigableWidget : public QWidget
{
	Q_OBJECT
public:
	ConfigableWidget(QWidget* parent = 0);
	virtual TiXmlNode* writeConfNode() = 0;
};

extern TiXmlNode* WriteStringEntry( const char* name, const char* value );
extern TiXmlNode* WriteIntEntry( const char* name, int value );
extern TiXmlNode* WriteDoubleEntry( const char* name, double value );
