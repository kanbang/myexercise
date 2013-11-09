#pragma once

#include "ConfigableWidget.h"

class RegistrySection : public QWidget
{
	Q_OBJECT
public:
	RegistrySection(QWidget* parent=0);
	virtual TiXmlNode* writeConfNode();

private:
	void createPopulationParams(QFormLayout *formLayout);
	void createOperatorsParams(QFormLayout *formLayout);
	void createTerminationConditionsParams(QFormLayout *formLayout);
	void createLoggingAndCheckpointingParams(QFormLayout *formLayout);

private:
	QSpinBox* randomizerSeedSpinBox;
	QSpinBox* populationSizeSpinBox;
	//QSpinBox* populationDemesSpinBox;
	//QSpinBox* migrationFreqSpinBox;
	//QSpinBox* migrationNumberSpinBox;

	QDoubleSpinBox* mutationIndprobSpinBox;
	//QComboBox* mutationGenotypesCombox;
	//QDoubleSpinBox* mutationProtectedSpinBox;
	//QComboBox* crossoverGenotypesCombox;
	//QDoubleSpinBox* crossoverProtectedSpinBox;

	QCheckBox* termStagnationCheckBox;
	QSpinBox* termStagnationSpinBox;
	QCheckBox* termMaxgenCheckBox;
	QSpinBox* termMaxgenSpinBox;
	QCheckBox* termMaxtimeCheckBox;
	QSpinBox* termMaxtimeSpinBox;
	QCheckBox* termFitnessvalCheckBox;
	QDoubleSpinBox* termFitnessvalSpinBox;

	QSpinBox* logLevelSpinBox;
	QLineEdit* logFileNameEdit;
	QSpinBox* logFrequencySpinBox;
	QLineEdit* milestoneFilenameEdit;
	QSpinBox* milestoneIntervalSpinBox;
};
