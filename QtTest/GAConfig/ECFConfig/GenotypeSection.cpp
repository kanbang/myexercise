#include "GenotypeSection.h"
#include "GenotypeParams.h"

GenotypeSection::GenotypeSection( QWidget* parent ) : ConfigableWidget( parent )
{
    genosCombox = new QComboBox;
    genosCombox->addItem( tr( "Binary" ) );
    genosCombox->addItem( tr( "BitString" ) );
    genosCombox->addItem( tr( "FloatingPoint" ) );
    genosCombox->addItem( tr( "Permutation" ) );
    genosCombox->setCurrentIndex( 0 );

    pagesWidget = new QStackedWidget;
    pagesWidget->addWidget( new BinaryParams );
    pagesWidget->addWidget( new BitStringParams );
    pagesWidget->addWidget( new FloatingPointParams );
    pagesWidget->addWidget( new PermutationParams );

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget( genosCombox );
    mainLayout->addWidget( pagesWidget );
    setLayout( mainLayout );

    connect( genosCombox, SIGNAL( activated( int ) ),
             pagesWidget, SLOT( setCurrentIndex( int ) ) );
}

TiXmlNode* GenotypeSection::writeConfNode()
{
    TiXmlElement* node = new TiXmlElement( "Genotype" );

    ConfigableWidget* cw = ( ConfigableWidget* )pagesWidget->currentWidget();
    node->LinkEndChild( cw->writeConfNode() );

    /*for(int i=0;i<pagesWidget->count();i++)
    {
    	ConfigableWidget* cw = (ConfigableWidget*)pagesWidget->widget(i);
    	node->LinkEndChild(cw->writeConfNode());
    }*/
    return node;
}