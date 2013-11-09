#include "RegistrySection.h"

#include <climits>
#include <cfloat>

void RegistrySection::createPopulationParams( QFormLayout* formLayout )
{
    randomizerSeedSpinBox = new QSpinBox;
    randomizerSeedSpinBox->setRange( 0, INT_MAX );
    randomizerSeedSpinBox->setSingleStep( 1 );
    randomizerSeedSpinBox->setValue( 0 );

    populationSizeSpinBox = new QSpinBox;
    populationSizeSpinBox->setRange( 1, INT_MAX );
    populationSizeSpinBox->setSingleStep( 1 );
    populationSizeSpinBox->setValue( 100 );

    //populationDemesSpinBox = new QSpinBox;
    //populationDemesSpinBox->setRange(1, 1000);
    //populationDemesSpinBox->setSingleStep(1);
    //populationDemesSpinBox->setValue(1);

    // migration相关的参数只有population.demes>1才有效
    // demes表示一个子种群
    // migration.freq表示每个多少代进行一次交换
    // migration.number表示demes交换的个体个数
    // 因此这2个参数与种群个数和进化代数有关，应该关联消息事件(signal/slot)
    //migrationFreqSpinBox = new QSpinBox;
    //migrationFreqSpinBox->setRange(1, 1000);  // 最大值与进化代数有关
    //migrationFreqSpinBox->setSingleStep(1);
    //migrationFreqSpinBox->setValue(10);

    //migrationNumberSpinBox = new QSpinBox;
    //migrationNumberSpinBox->setRange(1, 1000); // 最大值与种群个数有关
    //migrationNumberSpinBox->setSingleStep(1);
    //migrationNumberSpinBox->setValue(5);

    formLayout->addRow( tr( "randomizer.seed" ), randomizerSeedSpinBox );
    formLayout->addRow( tr( "population.size" ), populationSizeSpinBox );
    //formLayout->addRow(tr("population.demes"), populationDemesSpinBox);
    //formLayout->addRow(tr("migration.freq"), migrationFreqSpinBox);
    //formLayout->addRow(tr("migration.number"), migrationNumberSpinBox);
}

void RegistrySection::createOperatorsParams( QFormLayout* formLayout )
{
    mutationIndprobSpinBox = new QDoubleSpinBox;
    mutationIndprobSpinBox->setRange( 0.0, 1.0 );
    mutationIndprobSpinBox->setSingleStep( 0.01 );
    mutationIndprobSpinBox->setValue( 0.3 );

    //mutationGenotypesCombox = new QComboBox;
    //mutationGenotypesCombox->addItem(tr("random"));
    //mutationGenotypesCombox->addItem(tr("all"));
    //mutationGenotypesCombox->setCurrentIndex(0);

    //mutation.protected
    //QDoubleSpinBox*
    // 待定

    //QComboBox* crossoverGenotypesCombox = new QComboBox;
    //crossoverGenotypesCombox->addItem(tr("random"));
    //crossoverGenotypesCombox->addItem(tr("all"));
    //crossoverGenotypesCombox->setCurrentIndex(0);

    //crossover.protected
    //QDoubleSpinBox*
    // 待定

    formLayout->addRow( tr( "mutation.indprob" ), mutationIndprobSpinBox );
    //formLayout->addRow(tr("mutation.genotypes"), mutationGenotypesCombox);
    //formLayout->addRow(tr("mutation.protected"), mutationProtectedSpinBox);
    //formLayout->addRow(tr("crossover.genotypes"), crossoverGenotypesCombox);
    //formLayout->addRow(tr("crossover.protected"), crossoverProtectedSpinBox);
}

void RegistrySection::createTerminationConditionsParams( QFormLayout* formLayout )
{
    termStagnationCheckBox = new QCheckBox( tr( "term.stagnation" ) );
    termStagnationCheckBox->setCheckState( Qt::Checked );

    termStagnationSpinBox = new QSpinBox;
    termStagnationSpinBox->setRange( 0, INT_MAX );
    termStagnationSpinBox->setSingleStep( 1 );
    termStagnationSpinBox->setValue( 100 );
    termStagnationSpinBox->setToolTip( tr( "max number of consecutive generations without improvement (default: 5000 / pop_size)" ) );

    termMaxgenCheckBox = new QCheckBox( tr( "term.maxgen" ) );

    termMaxgenSpinBox = new QSpinBox;
    termMaxgenSpinBox->setRange( 0, INT_MAX );
    termMaxgenSpinBox->setSingleStep( 1 );
    termMaxgenSpinBox->setValue( 10 );

    termMaxtimeCheckBox = new QCheckBox( tr( "term.maxtime" ) );

    termMaxtimeSpinBox = new QSpinBox;
    termMaxtimeSpinBox->setRange( 0, INT_MAX );
    termMaxtimeSpinBox->setSingleStep( 1 );
    termMaxtimeSpinBox->setValue( 10 );
    termMaxtimeSpinBox->setToolTip( tr( "unit: second" ) );

    termFitnessvalCheckBox = new QCheckBox( tr( "term.fitnessval" ) );
    termFitnessvalSpinBox = new QDoubleSpinBox;
    termFitnessvalSpinBox->setRange( 0, DBL_MAX );
    termFitnessvalSpinBox->setSingleStep( 0.01 );
    termFitnessvalSpinBox->setValue( 1.0 );

    formLayout->addRow( termStagnationCheckBox, termStagnationSpinBox );
    formLayout->addRow( termMaxgenCheckBox, termMaxgenSpinBox );
    formLayout->addRow( termMaxtimeCheckBox, termMaxtimeSpinBox );
    formLayout->addRow( termFitnessvalCheckBox, termFitnessvalSpinBox );
}

void RegistrySection::createLoggingAndCheckpointingParams( QFormLayout* formLayout )
{
    logLevelSpinBox = new QSpinBox;
    logLevelSpinBox->setRange( 1, 5 );
    logLevelSpinBox->setSingleStep( 1 );
    logLevelSpinBox->setValue( 3 );

    logFileNameEdit = new QLineEdit;
    logFileNameEdit->setText( "log.txt" );
    logFileNameEdit->setToolTip( tr( "file type is txt format" ) );

    logFrequencySpinBox = new QSpinBox;
    logFrequencySpinBox->setRange( 1, 100 );
    logFrequencySpinBox->setSingleStep( 1 );
    logFrequencySpinBox->setValue( 1 );

    milestoneFilenameEdit = new QLineEdit;
    milestoneFilenameEdit->setText( "out.txt" );
    milestoneFilenameEdit->setToolTip( tr( "file type is txt format" ) );

    milestoneIntervalSpinBox = new QSpinBox;
    milestoneIntervalSpinBox->setRange( 0, 100 );
    milestoneIntervalSpinBox->setSingleStep( 1 );
    milestoneIntervalSpinBox->setValue( 0 );
    milestoneIntervalSpinBox->setToolTip( tr( "0: save only at the end" ) );

    formLayout->addRow( tr( "log.level" ), logLevelSpinBox );
    formLayout->addRow( tr( "log.filename" ), logFileNameEdit );
    formLayout->addRow( tr( "log.frequency" ), logFrequencySpinBox );
    formLayout->addRow( tr( "milestone.filename" ), milestoneFilenameEdit );
    formLayout->addRow( tr( "milestone.interval" ), milestoneIntervalSpinBox );
}

RegistrySection::RegistrySection( QWidget* parent ) : QWidget( parent )
{
    QFormLayout* formLayout = new QFormLayout;
    createPopulationParams( formLayout );
    createOperatorsParams( formLayout );
    createTerminationConditionsParams( formLayout );
    createLoggingAndCheckpointingParams( formLayout );
    setLayout( formLayout );
}

TiXmlNode* RegistrySection::writeConfNode()
{
    TiXmlElement* node = new TiXmlElement( "Registry" );
    node->LinkEndChild( WriteIntEntry( "randomizer.seed", randomizerSeedSpinBox->value() ) );
    node->LinkEndChild( WriteIntEntry( "population.size", populationSizeSpinBox->value() ) );
    //node->LinkEndChild(WriteIntEntry("population.demes", populationDemesSpinBox->value()));
    //node->LinkEndChild(WriteDoubleEntry("migration.freq", migrationFreqSpinBox->value()));
    //node->LinkEndChild(WriteIntEntry("migration.number", migrationNumberSpinBox->value()));

    node->LinkEndChild( WriteDoubleEntry( "mutation.indprob", mutationIndprobSpinBox->value() ) );
    //node->LinkEndChild(WriteIntEntry("mutation.genotypes", mutationGenotypesCombox->currentText()));
    //node->LinkEndChild(WriteDoubleEntry("mutation.protected", mutationProtectedSpinBox->value()));
    //node->LinkEndChild(WriteIntEntry("crossover.genotypes", crossoverGenotypesCombox->currentText()));
    //node->LinkEndChild(WriteDoubleEntry("crossover.protected", crossoverProtectedSpinBox->value()));

    if( termStagnationCheckBox->checkState() == Qt::Checked )
    {
        node->LinkEndChild( WriteIntEntry( "term.stagnation", termStagnationSpinBox->value() ) );
    }

    if( termMaxgenCheckBox->checkState() == Qt::Checked )
    {
        node->LinkEndChild( WriteIntEntry( "term.maxgen", termMaxgenSpinBox->value() ) );
    }

    if( termMaxtimeCheckBox->checkState() == Qt::Checked )
    {
        node->LinkEndChild( WriteIntEntry( "term.maxtime", termMaxtimeSpinBox->value() ) );
    }

    if( termFitnessvalCheckBox->checkState() == Qt::Checked )
    {
        node->LinkEndChild( WriteDoubleEntry( "term.fitnessval", termFitnessvalSpinBox->value() ) );
    }

    node->LinkEndChild( WriteIntEntry( "log.level", logLevelSpinBox->value() ) );
    node->LinkEndChild( WriteStringEntry( "log.filename", logFileNameEdit->text().toAscii() ) );
    node->LinkEndChild( WriteIntEntry( "log.frequency", logFrequencySpinBox->value() ) );
    node->LinkEndChild( WriteStringEntry( "milestone.filename", milestoneFilenameEdit->text().toAscii() ) );
    node->LinkEndChild( WriteIntEntry( "milestone.interval", milestoneIntervalSpinBox->value() ) );

    return node;
}