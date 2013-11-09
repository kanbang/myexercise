#pragma once

#include <QDialog>

#include "AlgoSection.h"
#include "RegistrySection.h"
#include "GenotypeSection.h"

class GAConfigDialog : public QDialog
{
    Q_OBJECT

public:
    GAConfigDialog();

private slots:
	void writeConfigFile();

private:
	AlgoSection* algoSection;
	RegistrySection* regSection;
	GenotypeSection* genoSection;
};