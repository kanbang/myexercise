#pragma once

#include "ConfigableWidget.h"

class AlgoParams : public ConfigableWidget
{
	Q_OBJECT
public:
	AlgoParams(QWidget *parent = 0);
};

class SteadyStateTournamentParams : public AlgoParams
{
	Q_OBJECT
public:
	SteadyStateTournamentParams(QWidget *parent = 0);
	virtual TiXmlNode* writeConfNode();

private:
	QSpinBox* tsizeSpinBox;
};

class RouletteWheelParams : public AlgoParams
{
	Q_OBJECT
public:
	RouletteWheelParams(QWidget *parent = 0);
	virtual TiXmlNode* writeConfNode();

private:
	QDoubleSpinBox* crxprobSpinBox;
	QDoubleSpinBox* selpressureSpinBox;
};

class EliminationParams : public AlgoParams
{
	Q_OBJECT
public:
	EliminationParams(QWidget *parent = 0);
	virtual TiXmlNode* writeConfNode();

private:
	QDoubleSpinBox* gengapSpinBox;
	QDoubleSpinBox* selpressureSpinBox;
};