#include <iostream>

//#define TIXML_USE_STL
#include <tinyxml.h>

using namespace std;

void build_simple_doc( )
{
    // Make xml: <?xml ..><Hello>World</Hello>
    TiXmlDocument doc;
    //TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    TiXmlElement* element = new TiXmlElement( "Hello" );
    //element->LinkEndChild( new TiXmlText( "World" ) );
    //doc.LinkEndChild( decl );
    doc.LinkEndChild( element );

    TiXmlElement* childElement = new TiXmlElement( "you" );
    //childElement->LinkEndChild(new TiXmlText( "World" ));
    childElement->LinkEndChild( new TiXmlText( "" ) );
    element->LinkEndChild( childElement );

    doc.SaveFile( "madeByHand.xml" );
}

int main()
{
    build_simple_doc();
    cout << "hello world" << endl;
    return 0;
}