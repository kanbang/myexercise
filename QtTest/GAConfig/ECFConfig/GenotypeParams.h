#pragma once

#include "ConfigableWidget.h"

class GenotypeParams : public ConfigableWidget
{
	Q_OBJECT
public:
	GenotypeParams(QWidget* parent=0);
};

class BinaryParams : public GenotypeParams
{
	Q_OBJECT
public:
	BinaryParams(QWidget* parent=0);
	virtual TiXmlNode* writeConfNode();

private:
	QLineEdit* lboundLineEdit;
	QLineEdit* uboundLineEdit;
	QSpinBox* precisionSpineBox;
	QSpinBox* dimensionSpineBox;
	QCheckBox* roundCheckBox;
	QDoubleSpinBox* mutSimpleSpinBox;
	QDoubleSpinBox* mutMixSpinBox;
	QDoubleSpinBox* crxOnePointSpinBox;
	QDoubleSpinBox* crxTwoPointSpinBox;
	QDoubleSpinBox* crxUniformSpinBox;
	QDoubleSpinBox* crxSegmentedSpinBox;
	QDoubleSpinBox* crxReducedsurrogateSpinBox;
};


class BitStringParams : public GenotypeParams
{
	Q_OBJECT
public:
	BitStringParams(QWidget* parent=0);
	virtual TiXmlNode* writeConfNode();

private:
	QSpinBox* sizeSpinBox;
	QCheckBox* crxOnepointCheckBox;
	QDoubleSpinBox* crxOnepointSpinBox;
	QCheckBox* crxUniformCheckBox;
	QDoubleSpinBox* crxUniformSpinBox;
	QCheckBox* mutSimpleCheckBox;
	QDoubleSpinBox* mutSimpleSpinBox;
	QCheckBox* mutMixCheckBox;
	QDoubleSpinBox* mutMixSpinBox;
};

class FloatingPointParams : public GenotypeParams
{
	Q_OBJECT
public:
	FloatingPointParams(QWidget* parent=0);
	virtual TiXmlNode* writeConfNode();

private:
	QLineEdit* lboundLineEdit;
	QLineEdit* uboundLineEdit;
	QSpinBox* precisionSpineBox;
	QDoubleSpinBox* mutSimpleSpinBox;
	QDoubleSpinBox* crxOnePointSpinBox;
	QDoubleSpinBox* crxArithmeticSpinBox;
};

class PermutationParams : public GenotypeParams
{
	Q_OBJECT
public:
	PermutationParams(QWidget* parent=0);
	virtual TiXmlNode* writeConfNode();

private:
	QSpinBox* sizeSpintBox;		
	QDoubleSpinBox* crxOXSpinBox;
	QDoubleSpinBox* crxPBXSpinBox;
	QDoubleSpinBox* crxPMXSpinBox;
	QDoubleSpinBox* mutToggleSpinBox;
	QDoubleSpinBox* mutInvSpinBox;
	QDoubleSpinBox* mutInsSpinBox;
};