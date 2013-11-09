#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osg/PositionAttitudeTransform>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/MatrixTransform>
#include <osg/PolygonMode>
#include <osgGA/OrbitManipulator>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgFX/Scribe>

#include <osgGA/GUIEventHandler>

#include <osgUtil/Optimizer>

#include <iostream>

#include "CommonFunctions"

class BoxPicker : public osgGA::GUIEventHandler
{
public:
    BoxPicker()
    {
        x = 0.0f;
        y = 0.0f;
        OK = false;
    }

    osg::observer_ptr<osg::Geometry> geometry;

    virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
    {

        bool doit = false;

        osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>( &aa );
        if ( !viewer )
        {
            return false;
        }

        if( ea.getEventType() == osgGA::GUIEventAdapter::PUSH )
        {
            x = ea.getXnormalized();
            y = ea.getYnormalized();

            x_pick = ea.getX();
            y_pick = ea.getY();

            OK = true;

        }

        if ( ea.getEventType() == osgGA::GUIEventAdapter::DRAG )
        {
            if ( OK )
            {
                float end_x = ea.getXnormalized();
                float end_y = ea.getYnormalized();

                if ( geometry.valid() )
                {
                    osg::Vec3Array* vertex = new osg::Vec3Array( 4 );
                    vertex->push_back( osg::Vec3( x, 0, y ) );
                    vertex->push_back( osg::Vec3( x, 0, end_y ) );
                    vertex->push_back( osg::Vec3( end_x, 0, end_y ) );
                    vertex->push_back( osg::Vec3( end_x, 0, y ) );

                    geometry->setVertexArray( vertex );
                    geometry->dirtyDisplayList();

                    //std::cout<<x<<" "<<y<<" "<<end_x<<" "<<end_y<<std::endl;
                }
                if ( ea.getModKeyMask()&osgGA::GUIEventAdapter::MODKEY_LEFT_SHIFT )
                {
                    doit = true;
                }
            }
        }

        if ( ea.getEventType() == osgGA::GUIEventAdapter::RELEASE )
        {
            OK = false;

            float pick_x = ea.getX();
            float pick_y = ea.getY();

            float xMin, xMax, yMin, yMax;

            xMin = osg::minimum( x_pick, pick_x );
            xMax = osg::maximum( x_pick, pick_x );
            yMin = osg::minimum( y_pick, pick_y );
            yMax = osg::maximum( y_pick, pick_y );

            osg::ref_ptr<osgUtil::PolytopeIntersector> intersector =
                new osgUtil::PolytopeIntersector( osgUtil::Intersector::WINDOW, xMin, yMin, xMax, yMax );

            osgUtil::IntersectionVisitor iv( intersector.get() );
            viewer->getCamera()->accept( iv );

            if ( intersector->containsIntersections() )
            {
                std::cout << "OK" << std::endl;
                /*
                for(osgUtil::PolytopeIntersector::Intersections::iterator
                        hitr = intersector->getIntersections().begin();
                        hitr != intersector->getIntersections().end();
                        ++hitr)
                {

                        osg::NodePath np = hitr->nodePath;
                        for (osg::NodePath::iterator itr = np.begin(); itr != np.end(); itr++)
                        {
                                if (!(*itr)->getName().empty())
                                {
                                        std::cout<<(*itr)->getName()<<std::endl;
                                }
                        }

                        break;
                }
                */
            }

            if ( geometry.valid() )
            {
                osg::Vec3Array* vertex = new osg::Vec3Array( 4 );
                vertex->push_back( osg::Vec3( 0, 0, 0 ) );
                vertex->push_back( osg::Vec3( 0, 0, 0 ) );
                vertex->push_back( osg::Vec3( 0, 0, 0 ) );
                vertex->push_back( osg::Vec3( 0, 0, 0 ) );
                geometry->setVertexArray( vertex );
                geometry->dirtyDisplayList();
            }
        }

        return doit;
    }

    float x, y;
    float x_pick, y_pick;

    bool OK;
};

/** create quad at specified position. */
osg::Geometry* createSquare( const osg::Vec3& corner, const osg::Vec3& width, const osg::Vec3& height, osg::Image* image = NULL )
{
    // set up the Geometry.
    osg::Geometry* geom = new osg::Geometry;

    osg::Vec3Array* coords = new osg::Vec3Array( 4 );
    ( *coords )[0] = corner;
    ( *coords )[1] = corner + width;
    ( *coords )[2] = corner + width + height;
    ( *coords )[3] = corner + height;


    geom->setVertexArray( coords );

    osg::Vec3Array* norms = new osg::Vec3Array( 1 );
    ( *norms )[0] = width ^ height;
    ( *norms )[0].normalize();

    geom->setNormalArray( norms );
    geom->setNormalBinding( osg::Geometry::BIND_OVERALL );

    osg::Vec2Array* tcoords = new osg::Vec2Array( 4 );
    ( *tcoords )[0].set( 0.0f, 0.0f );
    ( *tcoords )[1].set( 1.0f, 0.0f );
    ( *tcoords )[2].set( 1.0f, 1.0f );
    ( *tcoords )[3].set( 0.0f, 1.0f );
    geom->setTexCoordArray( 0, tcoords );

    geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, 4 ) );

    if ( image )
    {
        osg::StateSet* stateset = new osg::StateSet;
        osg::Texture2D* texture = new osg::Texture2D;
        texture->setImage( image );
        stateset->setTextureAttributeAndModes( 0, texture, osg::StateAttribute::ON );
        geom->setStateSet( stateset );
    }

    return geom;
}

osg::Geometry* createAxis( const osg::Vec3& corner, float axisize )
{
    // set up the Geometry.
    osg::Geometry* geom = new osg::Geometry;

    osg::Vec3Array* coords = new osg::Vec3Array;
    coords->push_back( corner );
    coords->push_back( corner + osg::Vec3( axisize, 0, 0 ) );
    coords->push_back( corner );
    coords->push_back( corner + osg::Vec3( 0, axisize, 0 ) );
    coords->push_back( corner );
    coords->push_back( corner + osg::Vec3( 0, 0, axisize ) );

    //(*coords)[0] = corner;
    //(*coords)[1] = corner+osg::Vec3(axisize,0,0);
    //(*coords)[2] = corner;
    //(*coords)[3] = corner+osg::Vec3(0,axisize,0);
    //(*coords)[4] = corner;
    //(*coords)[5] = corner+osg::Vec3(0,0,axisize);

    geom->setVertexArray( coords );

    osg::Vec4 x_color( 0.0f, 1.0f, 1.0f, 1.0f );
    osg::Vec4 y_color( 0.0f, 1.0f, 1.0f, 1.0f );
    osg::Vec4 z_color( 1.0f, 0.0f, 0.0f, 1.0f );

    osg::Vec4Array* color = new osg::Vec4Array;
    color->push_back( x_color );
    //color->push_back(x_color);
    //color->push_back(y_color);
    //color->push_back(y_color);
    //color->push_back(z_color);
    //color->push_back(z_color);
    //(*color)[0] = x_color;
    //(*color)[1] = x_color;
    //(*color)[2] = y_color;
    //(*color)[3] = y_color;
    //(*color)[4] = z_color;
    //(*color)[5] = z_color;

    geom->setColorArray( color );
    geom->setColorBinding( osg::Geometry::BIND_OVERALL );

    geom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, 6 ) );

    //osg::StateSet* stateset = new osg::StateSet;
    //geom->setStateSet(stateset);
    //osg::StateSet* stateset = geom->getOrCreateStateSet();

    //osg::LineWidth* linewidth = new osg::LineWidth();
    //linewidth->setWidth(4.0f);
    //stateset->setAttributeAndModes(linewidth,osg::StateAttribute::ON);
    //stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

    return geom;
}

osg::Geometry* createSelectBox( const osg::Vec3& corner, float width, float height )
{
    osg::Geometry* geo = new osg::Geometry;
    //geo->setDataVariance(osg::Object::DYNAMIC);
    //geo->setUseDisplayList(false);
    osg::Vec3Array* vertex = new osg::Vec3Array;
    vertex->push_back( corner );
    vertex->push_back( corner + osg::Vec3( width, 0, 0 ) );
    vertex->push_back( corner + osg::Vec3( width, height, 0 ) );
    vertex->push_back( corner + osg::Vec3( 0, height, 0 ) );
    geo->setVertexArray( vertex );

    osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back( osg::Vec4( 1.0f, 1.0, 0.8f, 0.2f ) );
    geo->setColorArray( colors );
    geo->setColorBinding( osg::Geometry::BIND_OVERALL );

    //osg::Vec3Array* normal = new osg::Vec3Array(1);
    //normal->push_back(osg::Vec3(0,0,1));
    //geo->setNormalArray(normal);
    //geo->setNormalBinding(osg::Geometry::BIND_OVERALL);

    geo->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, 4 ) );
    return geo;
}

int main( int argc, char* argv[] )
{
    osg::ArgumentParser argument( &argc, argv );

    osg::Node* model1 = osgDB::readNodeFile( "axes.osgt" );
    model1->setName( "COW1" );
    osg::MatrixTransform* mt1 = new osg::MatrixTransform;
    mt1->setMatrix( osg::Matrix::translate( osg::Vec3( 10, 0, 0 ) ) );
    mt1->addChild( model1 );

    osg::Node* model2 = osgDB::readNodeFile( "axes.osgt" );
    model2->setName( "COW2" );

    osg::Geometry* selboxGeom = createSelectBox( osg::Vec3( 150, 150, 0 ), 100, 100 );
    osg::Geode* selBox = new osg::Geode;
    selBox->addDrawable( selboxGeom );

    osg::StateSet* stateset = selBox->getOrCreateStateSet();
    //stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    //osg::ref_ptr<osg::PolygonMode> polyMode = new osg::PolygonMode;
    //polyMode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
    //stateset->setAttributeAndModes(polyMode.get());

    // 启用混合，设置透明.
    // 关闭深度检测，使不透明的多边形遮挡它后面的透明物体
    stateset->setMode( GL_BLEND, osg::StateAttribute::ON );
    stateset->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
    stateset->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );
    //stateset->setRenderBinDetails(11, "RenderBin");

    osg::Geode* hello = new osg::Geode;
    hello->addDrawable( osgCookBook::createText( osg::Vec3( 200, 200, 0 ), "hello wolrd", 20 ) );

    osg::Geode* axis = new osg::Geode();
    axis->addDrawable( createAxis( osg::Vec3( 100, 100, 0.0f ), 200 ) );
    //axis->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

    osg::PositionAttitudeTransform* axis2 = new osg::PositionAttitudeTransform();
    axis2->addChild( osgDB::readNodeFile( "axes.osgt" ) );
    axis2->setPosition( osg::Vec3( 150, 150, 0 ) );
    axis2->setScale( osg::Vec3d( 300, 300, 300 ) );

    osg::Camera* camera = osgCookBook::createHUDCamera( 0, 1024, 0, 768 );
    camera->addChild( selBox );
    camera->addChild( hello );
    //camera->addChild(axis2);
    camera->addChild( axis );

    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild( camera );
    //root->addChild(mt1);
    //      root->addChild(model2);
    //root->addChild(selBox);
    //root->addChild(axis);

    osgViewer::Viewer viewer;

    //osg::ref_ptr<BoxPicker> picker = new BoxPicker;
    //picker->geometry = selboxGeom;
    //viewer.addEventHandler(picker.get());
    viewer.setSceneData( root.get() );
    //viewer.setCameraManipulator(new osgGA::OrbitManipulator);
    viewer.run();
    return 0;
}