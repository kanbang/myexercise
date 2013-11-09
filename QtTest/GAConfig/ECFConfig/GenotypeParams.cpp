#include "GenotypeParams.h"
#include <cfloat>
#include <climits>

GenotypeParams::GenotypeParams( QWidget* parent ) : ConfigableWidget( parent )
{

}

BinaryParams::BinaryParams( QWidget* parent ) : GenotypeParams( parent )
{
    lboundLineEdit = new QLineEdit;
    lboundLineEdit->setValidator( new QDoubleValidator( -1 * DBL_MIN, DBL_MAX, 3, lboundLineEdit ) );
    lboundLineEdit->setText( "-5" );

    uboundLineEdit = new QLineEdit;
    uboundLineEdit->setValidator( new QDoubleValidator( -1 * DBL_MIN, DBL_MAX, 3, uboundLineEdit ) );
    uboundLineEdit->setText( "5" );

    precisionSpineBox = new QSpinBox;
    precisionSpineBox->setRange( 1, 8 );
    precisionSpineBox->setSingleStep( 1 );
    precisionSpineBox->setValue( 3 );

    dimensionSpineBox = new QSpinBox;
    dimensionSpineBox->setRange( 1, INT_MAX );
    dimensionSpineBox->setSingleStep( 1 );
    dimensionSpineBox->setValue( 1 );

    roundCheckBox = new QCheckBox();

    mutSimpleSpinBox = new QDoubleSpinBox;
    mutSimpleSpinBox->setRange( 0, 1 );
    mutSimpleSpinBox->setSingleStep( 0.01 );
    mutSimpleSpinBox->setValue( 1 );

    mutMixSpinBox = new QDoubleSpinBox;
    mutMixSpinBox->setRange( 0, 1 );
    mutMixSpinBox->setSingleStep( 0.01 );
    mutMixSpinBox->setValue( 0 );

    crxOnePointSpinBox = new QDoubleSpinBox;
    crxOnePointSpinBox->setRange( 0, 1 );
    crxOnePointSpinBox->setSingleStep( 0.01 );
    crxOnePointSpinBox->setValue( 1 );

    crxTwoPointSpinBox = new QDoubleSpinBox;
    crxTwoPointSpinBox->setRange( 0, 1 );
    crxTwoPointSpinBox->setSingleStep( 0.01 );
    crxTwoPointSpinBox->setValue( 0 );

    crxUniformSpinBox = new QDoubleSpinBox;
    crxUniformSpinBox->setRange( 0, 1 );
    crxUniformSpinBox->setSingleStep( 0.01 );
    crxUniformSpinBox->setValue( 0 );

    crxSegmentedSpinBox = new QDoubleSpinBox;
    crxSegmentedSpinBox->setRange( 0, 1 );
    crxSegmentedSpinBox->setSingleStep( 0.01 );
    crxSegmentedSpinBox->setValue( 0 );

    crxReducedsurrogateSpinBox = new QDoubleSpinBox;
    crxReducedsurrogateSpinBox->setRange( 0, 1 );
    crxReducedsurrogateSpinBox->setSingleStep( 0.01 );
    crxReducedsurrogateSpinBox->setValue( 0 );

    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow( tr( "lbound" ), lboundLineEdit );
    formLayout->addRow( tr( "ubound" ), uboundLineEdit );
    formLayout->addRow( tr( "precision" ), precisionSpineBox );
    formLayout->addRow( tr( "dimension" ), dimensionSpineBox );
    formLayout->addRow( tr( "rounding" ), roundCheckBox );
    formLayout->addRow( tr( "mut.simple" ), mutSimpleSpinBox );
    formLayout->addRow( tr( "mut.mix" ), mutMixSpinBox );
    formLayout->addRow( tr( "crx.onepoint" ), crxOnePointSpinBox );
    formLayout->addRow( tr( "crx.twopoint" ), crxTwoPointSpinBox );
    formLayout->addRow( tr( "crx.uniform" ), crxUniformSpinBox );
    formLayout->addRow( tr( "crx.segmented" ), crxSegmentedSpinBox );
    formLayout->addRow( tr( "crx.reducedsurrogate" ), crxReducedsurrogateSpinBox );


    setLayout( formLayout );
}

TiXmlNode* BinaryParams::writeConfNode()
{
    TiXmlElement* node = new TiXmlElement( "Binary" );
    node->LinkEndChild( WriteDoubleEntry( "lbound", lboundLineEdit->text().toDouble() ) );
    node->LinkEndChild( WriteDoubleEntry( "ubound", uboundLineEdit->text().toDouble() ) );
    node->LinkEndChild( WriteIntEntry( "precision", precisionSpineBox->value() ) );
    node->LinkEndChild( WriteIntEntry( "dimension", dimensionSpineBox->value() ) );
    if( roundCheckBox->checkState() == Qt::Checked )
    {
        node->LinkEndChild( WriteIntEntry( "rounding", 1 ) );
    }
    else
    {
        node->LinkEndChild( WriteIntEntry( "rounding", 0 ) );
    }

    node->LinkEndChild( WriteDoubleEntry( "mut.simple", mutSimpleSpinBox->value() ) );
    node->LinkEndChild( WriteDoubleEntry( "mut.mix", mutMixSpinBox->value() ) );
    node->LinkEndChild( WriteDoubleEntry( "crx.onepoint", crxOnePointSpinBox->value() ) );
    node->LinkEndChild( WriteDoubleEntry( "crx.uniform", crxUniformSpinBox->value() ) );

    return node;
}


BitStringParams::BitStringParams( QWidget* parent ) : GenotypeParams( parent )
{
    sizeSpinBox = new QSpinBox;
    sizeSpinBox->setRange( 1, INT_MAX );
    sizeSpinBox->setSingleStep( 1 );
    sizeSpinBox->setValue( 20 );

    crxOnepointCheckBox = new QCheckBox( tr( "crx.onepoint" ) );
    crxOnepointSpinBox = new QDoubleSpinBox;
    crxOnepointSpinBox->setRange( 0.0, 1.0 );
    crxOnepointSpinBox->setSingleStep( 0.01 );
    crxOnepointSpinBox->setValue( 1.0 );

    crxUniformCheckBox = new QCheckBox( tr( "crx.uniform" ) );
    crxUniformSpinBox = new QDoubleSpinBox;
    crxUniformSpinBox->setRange( 0.0, 1.0 );
    crxUniformSpinBox->setSingleStep( 0.01 );
    crxUniformSpinBox->setValue( 0 );

    mutSimpleCheckBox = new QCheckBox( tr( "mut.simple" ) );
    mutSimpleSpinBox = new QDoubleSpinBox;
    mutSimpleSpinBox->setRange( 0.0, 1.0 );
    mutSimpleSpinBox->setSingleStep( 0.01 );
    mutSimpleSpinBox->setValue( 1.0 );

    mutMixCheckBox = new QCheckBox( tr( "mut.mix" ) );
    mutMixSpinBox = new QDoubleSpinBox;
    mutMixSpinBox->setRange( 0.0, 1.0 );
    mutMixSpinBox->setSingleStep( 0.01 );
    mutMixSpinBox->setValue( 0 );

    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow( tr( "size" ), sizeSpinBox );
    formLayout->addRow( crxOnepointCheckBox, crxOnepointSpinBox );
    formLayout->addRow( crxUniformCheckBox, crxUniformSpinBox );
    formLayout->addRow( mutSimpleCheckBox, mutSimpleSpinBox );
    formLayout->addRow( mutMixCheckBox, mutMixSpinBox );

    setLayout( formLayout );
}

TiXmlNode* BitStringParams::writeConfNode()
{
    TiXmlElement* node = new TiXmlElement( "BitString" );
    node->LinkEndChild( WriteIntEntry( "size", sizeSpinBox->value() ) );
    if( crxOnepointCheckBox->checkState() == Qt::Checked )
    {
        node->LinkEndChild( WriteDoubleEntry( "crx.onepoint", crxOnepointSpinBox->value() ) );
    }

    if( crxUniformCheckBox->checkState() == Qt::Checked )
    {
        node->LinkEndChild( WriteDoubleEntry( "crx.uniform", crxUniformSpinBox->value() ) );
    }

    if( mutSimpleCheckBox->checkState() == Qt::Checked )
    {
        node->LinkEndChild( WriteDoubleEntry( "mut.simple", mutSimpleSpinBox->value() ) );
    }

    if( mutMixCheckBox->checkState() == Qt::Checked )
    {
        node->LinkEndChild( WriteDoubleEntry( "mut.mix", mutMixSpinBox->value() ) );
    }

    return node;
}

FloatingPointParams::FloatingPointParams( QWidget* parent ) : GenotypeParams( parent )
{
    lboundLineEdit = new QLineEdit;
    lboundLineEdit->setValidator( new QDoubleValidator( -1 * DBL_MIN, DBL_MAX, 3, lboundLineEdit ) );
    lboundLineEdit->setText( "-50" );

    uboundLineEdit = new QLineEdit;
    uboundLineEdit->setValidator( new QDoubleValidator( -1 * DBL_MIN, DBL_MAX, 3, uboundLineEdit ) );
    uboundLineEdit->setText( "50" );

    precisionSpineBox = new QSpinBox;
    precisionSpineBox->setRange( 1, 8 );
    precisionSpineBox->setSingleStep( 1 );
    precisionSpineBox->setValue( 3 );

    mutSimpleSpinBox = new QDoubleSpinBox;
    mutSimpleSpinBox->setRange( 0, 1 );
    mutSimpleSpinBox->setSingleStep( 0.01 );
    mutSimpleSpinBox->setValue( 1 );

    crxArithmeticSpinBox = new QDoubleSpinBox;
    crxArithmeticSpinBox->setRange( 0, 1 );
    crxArithmeticSpinBox->setSingleStep( 0.01 );
    crxArithmeticSpinBox->setValue( 1 );

    crxOnePointSpinBox = new QDoubleSpinBox;
    crxOnePointSpinBox->setRange( 0, 1 );
    crxOnePointSpinBox->setSingleStep( 0.01 );
    crxOnePointSpinBox->setValue( 0 );

    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow( tr( "lbound" ), lboundLineEdit );
    formLayout->addRow( tr( "ubound" ), uboundLineEdit );
    formLayout->addRow( tr( "precision" ), precisionSpineBox );
    formLayout->addRow( tr( "mut.simple" ), mutSimpleSpinBox );
    formLayout->addRow( tr( "crx.arithmetic" ), crxArithmeticSpinBox );
    formLayout->addRow( tr( "crx.onepoint" ), crxOnePointSpinBox );

    setLayout( formLayout );
}

TiXmlNode* FloatingPointParams::writeConfNode()
{
    TiXmlElement* node = new TiXmlElement( "FloatingPoint" );
    node->LinkEndChild( WriteDoubleEntry( "lbound", lboundLineEdit->text().toDouble() ) );
    node->LinkEndChild( WriteDoubleEntry( "ubound", uboundLineEdit->text().toDouble() ) );
    node->LinkEndChild( WriteIntEntry( "precision", precisionSpineBox->value() ) );

    node->LinkEndChild( WriteDoubleEntry( "mut.simple", mutSimpleSpinBox->value() ) );
    node->LinkEndChild( WriteDoubleEntry( "crx.arithmetic", crxArithmeticSpinBox->value() ) );
    node->LinkEndChild( WriteDoubleEntry( "crx.onepoint", crxOnePointSpinBox->value() ) );

    return node;
}

PermutationParams::PermutationParams( QWidget* parent ) : GenotypeParams( parent )
{
    sizeSpintBox = new QSpinBox;
    sizeSpintBox->setRange( 1, INT_MAX );
    sizeSpintBox->setSingleStep( 1 );
    sizeSpintBox->setValue( 30 );

    crxOXSpinBox = new QDoubleSpinBox;
    crxOXSpinBox->setRange( 0, 1 );
    crxOXSpinBox->setSingleStep( 0.01 );
    crxOXSpinBox->setValue( 1 );

    crxPBXSpinBox = new QDoubleSpinBox;
    crxPBXSpinBox->setRange( 0, 1 );
    crxPBXSpinBox->setSingleStep( 0.01 );
    crxPBXSpinBox->setValue( 0 );

    crxPMXSpinBox = new QDoubleSpinBox;
    crxPMXSpinBox->setRange( 0, 1 );
    crxPMXSpinBox->setSingleStep( 0.01 );
    crxPMXSpinBox->setValue( 0 );

    mutToggleSpinBox = new QDoubleSpinBox;
    mutToggleSpinBox->setRange( 0, 1 );
    mutToggleSpinBox->setSingleStep( 0.01 );
    mutToggleSpinBox->setValue( 1 );

    mutInvSpinBox = new QDoubleSpinBox;
    mutInvSpinBox->setRange( 0, 1 );
    mutInvSpinBox->setSingleStep( 0.01 );
    mutInvSpinBox->setValue( 0 );

    mutInsSpinBox = new QDoubleSpinBox;
    mutInsSpinBox->setRange( 0, 1 );
    mutInsSpinBox->setSingleStep( 0.01 );
    mutInsSpinBox->setValue( 0 );

    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow( tr( "size" ), sizeSpintBox );
    formLayout->addRow( tr( "crx.OX" ), crxOXSpinBox );
    formLayout->addRow( tr( "crx.PBX" ), crxPBXSpinBox );
    formLayout->addRow( tr( "crx.PMX" ), crxPMXSpinBox );
    formLayout->addRow( tr( "mut.toggle" ), mutToggleSpinBox );
    formLayout->addRow( tr( "mut.inv" ), mutInvSpinBox );
    formLayout->addRow( tr( "mut.ins" ), mutInsSpinBox );

    setLayout( formLayout );
}

TiXmlNode* PermutationParams::writeConfNode()
{
    TiXmlElement* node = new TiXmlElement( "Permutation" );
    node->LinkEndChild( WriteIntEntry( "size", sizeSpintBox->value() ) );
    node->LinkEndChild( WriteDoubleEntry( "crx.OX", crxOXSpinBox->value() ) );
    node->LinkEndChild( WriteDoubleEntry( "crx.PBX", crxPBXSpinBox->value() ) );
    node->LinkEndChild( WriteDoubleEntry( "crx.PMX", crxPMXSpinBox->value() ) );
    node->LinkEndChild( WriteDoubleEntry( "mut.toggle", mutToggleSpinBox->value() ) );
    node->LinkEndChild( WriteDoubleEntry( "mut.inv", mutInvSpinBox->value() ) );
    node->LinkEndChild( WriteDoubleEntry( "mut.ins", mutInsSpinBox->value() ) );

    return node;
}