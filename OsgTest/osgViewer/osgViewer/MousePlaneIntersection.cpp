#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgGA/GUIEventHandler>
#include <osg/ShapeDrawable>
#include <osg/positionattitudetransform>
#include <osgManipulator/Projector>

#include "osgTools.h"

class CInputHandler : public osgGA::GUIEventHandler
{
public:
    CInputHandler( osg::PositionAttitudeTransform* pPatSphere )
    {
        m_rPatSphere = pPatSphere;
        osg::Plane plane( osg::Vec3( 0, 0, 1 ), osg::Vec3( 0, 0, 0 ) );

        m_rPlaneProj = new osgManipulator::PlaneProjector;
        m_rPlaneProj->setPlane( plane );
    }

    virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* pObject, osg::NodeVisitor* pNodeVisitor )
    {
        osgViewer::View* view = dynamic_cast<osgViewer::View*>( &aa );
        if ( !view ) return false;

        if ( ea.getEventType() == osgGA::GUIEventAdapter::DRAG )
        {
            osgManipulator::PointerInfo pi;
            pi.reset();
            pi.setCamera( view->getCamera() );
            pi.setMousePosition( ea.getX(), ea.getY() );
            osg::Vec3d v;
            m_rPlaneProj->project( pi, v );

            m_rPatSphere->setPosition( v );

            return true;
        }
        return false;
    }

private:
    osg::ref_ptr<osgManipulator::PlaneProjector> m_rPlaneProj;
    osg::ref_ptr<osg::PositionAttitudeTransform> m_rPatSphere;
};

osg::Node* CreateScene( osg::PositionAttitudeTransform* pPat )
{
    osg::Group* pGroup = new osg::Group;

    osg::Geode* pGeode = new osg::Geode;
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box( osg::Vec3( 0.0f, 0.0f, -0.5f ), 100.0f, 100.0f, 1.0f ) ) );
    pGroup->addChild( pGeode );

    osg::Geode* pGeodeSphere = new osg::Geode;
    pGeodeSphere->addDrawable( new osg::ShapeDrawable( new osg::Sphere( osg::Vec3( 0.0f, 0.0f, 0.0f ), 5.0f ) ) );
    pPat->addChild( pGeodeSphere );
    pGroup->addChild( pPat );

    return pGroup;
}

int main( int argc, char* argv[] )
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    rViewer->setUpViewInWindow( 32, 32, 512, 512 );

    osg::ref_ptr<osg::PositionAttitudeTransform> rPat = new osg::PositionAttitudeTransform;

    osg::Group* root = new osg::Group;
    root->addChild( CreateScene( rPat.get() ) );

    osg::Camera* hudCamera = osgTools::createHUDCamera( 0, 800, 0, 600 );
    osg::Geode* info = new osg::Geode();
    osgText::Text* infoText = osgTools::createText( osg::Vec3( 10, 100, 0 ), "", 15 );
    info->addDrawable( infoText );
    hudCamera->addChild( info );
    root->addChild( hudCamera );

    osgTools::PositonHandler* ph = new osgTools::PositonHandler();
    ph->infoText = infoText;
    rViewer->addEventHandler( ph );

    // add the handler to the viewer
    rViewer->addEventHandler( new CInputHandler( rPat.get() ) );

    // set the scene-graph data the viewer will render
    rViewer->setSceneData( root );

    // execute main loop
    return rViewer->run();
}