#include <osgViewer/Viewer>
#include <osgGA/GUIEventHandler>
#include <osg/ShapeDrawable>
#include <osg/positionattitudetransform>

#include "osgTools.h"
#include "MyDragger.h"

using namespace std;
using namespace osgManipulator;

class CDraggerPoint: public osgManipulator::Translate2DDragger
{
public:
    virtual void setupDefaultGeometry()
    {
        osg::ref_ptr< osg::Geometry   > pGeom = new osg::Geometry();
        osg::ref_ptr< osg::Geode    > pGeoSphere = new osg::Geode();
        osg::ref_ptr< osg::Vec3Array > pVertices = new osg::Vec3Array( 3 );
        int t = 2;
        ( *pVertices )[ 0].set( 0, 0 + t, 0 );
        ( *pVertices )[ 1].set( 0, 0 + t, -2 );
        ( *pVertices )[ 2].set( 2, 0 + t, 0 );
        pGeom->setVertexArray( pVertices.get() );

        osg::ref_ptr< osg::DrawElementsUInt> pTri = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLES , 0 );
        pTri->push_back( 0 );
        pTri->push_back( 1 );
        pTri->push_back( 2 );
        pGeom->addPrimitiveSet( pTri.get() );

        pGeoSphere->addDrawable( pGeom.get() );
        addChild( pGeoSphere.get() );
    }

    virtual bool handle( const PointerInfo& pointer, const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
    {
        // Check if the dragger node is in the nodepath.
        if ( !pointer.contains( this ) ) return false;

        switch ( ea.getEventType() )
        {
            // Pick start.
        case ( osgGA::GUIEventAdapter::PUSH ):
        {
            // Get the LocalToWorld matrix for this node and set it for the projector.
            osg::NodePath nodePathToRoot;
            computeNodePathToRoot( *this, nodePathToRoot );
            osg::Matrix localToWorld = osg::computeLocalToWorld( nodePathToRoot );
            _projector->setLocalToWorld( localToWorld );

            //设置点移动平面,点即鼠标与物体交点(世界坐标系),法线为//视线方向
            osg::Plane* pplnPTPush = new osg::Plane( pointer.getEyeDir(),  pointer.getLocalIntersectPoint()/** _projector->getLocalToWorld()*/ );
            _projector->setPlane( *pplnPTPush );

            if ( _projector->project( pointer, _startProjectedPoint ) )
            {
                // Generate the motion command.
                osg::ref_ptr<TranslateInPlaneCommand> cmd = new TranslateInPlaneCommand( _projector->getPlane() );

                cmd->setStage( MotionCommand::START );
                cmd->setReferencePoint( _startProjectedPoint );
                cmd->setLocalToWorldAndWorldToLocal( _projector->getLocalToWorld(), _projector->getWorldToLocal() );

                dispatch( *cmd );

                ////在该点处添加一个球
                //osg::ref_ptr< osg::Shape> sphere = new osg::Sphere( _startProjectedPoint* _projector->getLocalToWorld(), 0.08f);
                //osg::ref_ptr<osg::ShapeDrawable> sphereDrawable = new osg::ShapeDrawable(sphere);
                //osg::ref_ptr<osg::Geode> sphereGeode = new osg::Geode();
                //sphereGeode->addDrawable(sphereDrawable);

                //g_pRoot->addChild( sphereGeode);


                // Set color to pick color.
                setMaterialColor( _pickColor, *this );
                getOrCreateStateSet()->setAttributeAndModes( _polygonOffset.get(), osg::StateAttribute::ON );

                aa.requestRedraw();
            }
            return true;
        }

        // Pick move.
        case ( osgGA::GUIEventAdapter::DRAG ):
        {

            osg::Vec3d projectedPoint;
            if ( _projector->project( pointer, projectedPoint ) )
            {
                // Generate the motion command.
                osg::ref_ptr<TranslateInPlaneCommand> cmd = new TranslateInPlaneCommand( _projector->getPlane() );

                cmd->setStage( MotionCommand::MOVE );
                cmd->setLocalToWorldAndWorldToLocal( _projector->getLocalToWorld(), _projector->getWorldToLocal() );
                cmd->setTranslation( projectedPoint - _startProjectedPoint );
                cmd->setReferencePoint( _startProjectedPoint );

                // Dispatch command.
                dispatch( *cmd );

                /*	osg::ref_ptr< osg::Shape> sphere = new osg::Sphere( projectedPoint* _projector->getLocalToWorld(), 0.08f);
                	osg::ref_ptr<osg::ShapeDrawable> sphereDrawable = new osg::ShapeDrawable(sphere);
                	osg::ref_ptr<osg::Geode> sphereGeode = new osg::Geode();
                	sphereGeode->addDrawable(sphereDrawable);

                	g_pRoot->addChild( sphereGeode);*/

                aa.requestRedraw();
            }
            return true;
        }

        // Pick finish.
        case ( osgGA::GUIEventAdapter::RELEASE ):
        {

            osg::ref_ptr<TranslateInPlaneCommand> cmd = new TranslateInPlaneCommand( _projector->getPlane() );

            cmd->setStage( MotionCommand::FINISH );
            cmd->setReferencePoint( _startProjectedPoint );
            cmd->setLocalToWorldAndWorldToLocal( _projector->getLocalToWorld(), _projector->getWorldToLocal() );

            // Dispatch command.
            dispatch( *cmd );

            // Reset color.
            setMaterialColor( _color, *this );
            getOrCreateStateSet()->removeAttribute( _polygonOffset.get() );

            aa.requestRedraw();

            return true;
        }
        default:
            return false;
        }
    }

    osg::ref_ptr<osg::Group> g_pRoot;

};

osgManipulator::Dragger* createDragger( const std::string& name )
{
    osgManipulator::Dragger* dragger = 0;
    if ( "TabPlaneDragger" == name )
    {
        osgManipulator::TabPlaneDragger* d = new osgManipulator::TabPlaneDragger();
        d->setupDefaultGeometry();
        dragger = d;
    }
    else if ( "TabPlaneTrackballDragger" == name )
    {
        osgManipulator::TabPlaneTrackballDragger* d = new osgManipulator::TabPlaneTrackballDragger();
        d->setupDefaultGeometry();
        dragger = d;
    }
    else if ( "TabBoxTrackballDragger" == name )
    {
        osgManipulator::TabBoxTrackballDragger* d = new osgManipulator::TabBoxTrackballDragger();
        d->setupDefaultGeometry();
        dragger = d;
    }
    else if ( "TrackballDragger" == name )
    {
        osgManipulator::TrackballDragger* d = new osgManipulator::TrackballDragger();
        d->setupDefaultGeometry();
        dragger = d;
    }
    else if ( "Translate1DDragger" == name )
    {
        osgManipulator::Translate1DDragger* d = new osgManipulator::Translate1DDragger();
        d->setupDefaultGeometry();
        dragger = d;
    }
    else if ( "Translate2DDragger" == name )
    {
        osgManipulator::Translate2DDragger* d = new osgManipulator::Translate2DDragger();
        d->setupDefaultGeometry();
        dragger = d;
    }
    else if ( "TranslateAxisDragger" == name )
    {
        osgManipulator::TranslateAxisDragger* d = new osgManipulator::TranslateAxisDragger();
        d->setupDefaultGeometry();
        dragger = d;
    }
    else if ( "TabBoxDragger" == name )
    {
        osgManipulator::TabBoxDragger* d = new osgManipulator::TabBoxDragger();
        d->setupDefaultGeometry();
        dragger = d;
    }
    else
    {
        CDraggerPoint* d = new  CDraggerPoint();
        d->setupDefaultGeometry();
        dragger = d;
    }

    return dragger;
}

// The DraggerContainer node is used to fix the dragger's size on the screen
class DraggerContainer : public osg::Group
{
public:
    DraggerContainer() : _draggerSize( 240.0f ), _active( true ) {}

    DraggerContainer( const DraggerContainer& copy, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY )
        :   osg::Group( copy, copyop ),
            _dragger( copy._dragger ), _draggerSize( copy._draggerSize ), _active( copy._active )
    {}

    META_Node( osgManipulator, DraggerContainer );

    void setDragger( osgManipulator::Dragger* dragger )
    {
        _dragger = dragger;
        if ( !containsNode( dragger ) ) addChild( dragger );
    }

    osgManipulator::Dragger* getDragger()
    {
        return _dragger.get();
    }
    const osgManipulator::Dragger* getDragger() const
    {
        return _dragger.get();
    }

    void setDraggerSize( float size )
    {
        _draggerSize = size;
    }
    float getDraggerSize() const
    {
        return _draggerSize;
    }

    void setActive( bool b )
    {
        _active = b;
    }
    bool getActive() const
    {
        return _active;
    }

    void traverse( osg::NodeVisitor& nv )
    {
        if ( _dragger.valid() )
        {
            if ( _active && nv.getVisitorType() == osg::NodeVisitor::CULL_VISITOR )
            {
                osgUtil::CullVisitor* cv = static_cast<osgUtil::CullVisitor*>( &nv );

                float pixelSize = cv->pixelSize( _dragger->getBound().center(), 0.48f );
                if ( pixelSize != _draggerSize )
                {
                    float pixelScale = pixelSize > 0.0f ? _draggerSize / pixelSize : 1.0f;
                    osg::Vec3d scaleFactor( pixelScale, pixelScale, pixelScale );

                    osg::Vec3 trans = _dragger->getMatrix().getTrans();
                    _dragger->setMatrix( osg::Matrix::scale( scaleFactor ) * osg::Matrix::translate( trans ) );
                }
            }
        }
        osg::Group::traverse( nv );
    }

protected:
    osg::ref_ptr<osgManipulator::Dragger> _dragger;
    float _draggerSize;
    bool _active;
};

osg::Node* addDraggerToScene( osg::Node* scene, const std::string& name, bool fixedSizeInScreen )
{
    scene->getOrCreateStateSet()->setMode( GL_NORMALIZE, osg::StateAttribute::ON );

    osg::MatrixTransform* selection = new osg::MatrixTransform;
    selection->addChild( scene );

    osgManipulator::Dragger* dragger = createDragger( name );

    osg::Group* root = new osg::Group;
    root->addChild( selection );

    if ( fixedSizeInScreen )
    {
        DraggerContainer* draggerContainer = new DraggerContainer;
        draggerContainer->setDragger( dragger );
        root->addChild( draggerContainer );
    }
    else
        root->addChild( dragger );

    float scale = scene->getBound().radius() * 1.6;
    dragger->setMatrix( osg::Matrix::scale( scale, scale, scale ) *
                        osg::Matrix::translate( scene->getBound().center() ) );

    dragger->addTransformUpdating( selection );

    // we want the dragger to handle it's own events automatically
    dragger->setHandleEvents( true );

    // if we don't set an activation key or mod mask then any mouse click on
    // the dragger will activate it, however if do define either of ActivationModKeyMask or
    // and ActivationKeyEvent then you'll have to press either than mod key or the specified key to
    // be able to activate the dragger when you mouse click on it.  Please note the follow allows
    // activation if either the ctrl key or the 'a' key is pressed and held down.
    dragger->setActivationModKeyMask( osgGA::GUIEventAdapter::MODKEY_CTRL );
    dragger->setActivationKeyEvent( 'a' );

    return root;
}

class CInputHandler : public osgGA::GUIEventHandler
{
public:
    CInputHandler( osg::PositionAttitudeTransform* pPatSphere )
    {
        m_rPatSphere = pPatSphere;
    }

    virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* pObject, osg::NodeVisitor* pNodeVisitor )
    {
        osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>( &aa );
        if ( !pViewer )
        {
            return false;
        }

        if ( ea.getEventType() == osgGA::GUIEventAdapter::DRAG )
        {
            osgViewer::ViewerBase::Cameras cams;
            pViewer->getCameras( cams );

            float x = ea.getXnormalized();
            float y = ea.getYnormalized();

            osgUtil::LineSegmentIntersector* picker = new osgUtil::LineSegmentIntersector( osgUtil::Intersector::PROJECTION, x, y );
            osgUtil::IntersectionVisitor iv( picker );
            cams[0]->accept( iv );

            if ( picker->containsIntersections() )
            {
                osgUtil::LineSegmentIntersector::Intersection intersection = picker->getFirstIntersection();
                osg::Vec3 v = intersection.getWorldIntersectPoint();
                m_rPatSphere->setPosition( v );
            }

            return true; // return true, event handled
        }

        return false;
    }

private:
    osg::ref_ptr<osg::PositionAttitudeTransform> m_rPatSphere;
};

class FixedSizeNode : public osg::MatrixTransform
{
public:
    FixedSizeNode()
    {
        setNumChildrenRequiringEventTraversal( getNumChildrenRequiringEventTraversal() + 1 );

    }
    void traverse( osg::NodeVisitor& nv );
    virtual osg::BoundingSphere computeBound () const;

    osg::ref_ptr<osg::PositionAttitudeTransform> m_rPatSphere;

};

void FixedSizeNode::traverse( osg::NodeVisitor& nv )
{
    int a = nv.getVisitorType();
    if ( nv.getVisitorType() == osg::NodeVisitor::EVENT_VISITOR )
    {
        osgGA::EventVisitor* ev = dynamic_cast<osgGA::EventVisitor*>( &nv );
        if ( ev )
        {
            for( osgGA::EventQueue::Events::iterator itr = ev->getEvents().begin();
                    itr != ev->getEvents().end();
                    ++itr )
            {
                osgGA::GUIEventAdapter& ea = *( itr->get() );
                osgGA::GUIActionAdapter& aa = *( ev->getActionAdapter() );

                osgViewer::View* view = dynamic_cast<osgViewer::View*>( &aa );

                if ( ea.getEventType() == osgGA::GUIEventAdapter::PUSH )
                {
                    osgUtil::LineSegmentIntersector* picker = new osgUtil::LineSegmentIntersector( osgUtil::Intersector::PROJECTION, ea.getX(), ea.getY() );
                    osgUtil::IntersectionVisitor iv( picker );
                    view->getCamera()->accept( iv );

                    if ( picker->containsIntersections() )
                    {
                        osgUtil::LineSegmentIntersector::Intersection intersection = picker->getFirstIntersection();
                        osg::Vec3 v = intersection.getWorldIntersectPoint();
                        m_rPatSphere->setPosition( v );
                    }
                }
            }
        }
    }

    osg::MatrixTransform::traverse( nv );
}

osg::BoundingSphere FixedSizeNode::computeBound() const
{
    return osg::MatrixTransform::computeBound();
}


osg::Node* CreateScene( osg::PositionAttitudeTransform* pPat )
{
    // create a group to contain our floor and sphere
    osg::Group* pGroup = new osg::Group;

    // create floor
    osg::Geode* pGeode = new osg::Geode;
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box( osg::Vec3( 0.0f, 0.0f, -0.5f ), 100.0f, 100.0f, 1.0f ) ) );
    pGroup->addChild( pGeode );

    // create sphere
    osg::Geode* pGeodeSphere = new osg::Geode;
    pGeodeSphere->addDrawable( new osg::ShapeDrawable( new osg::Sphere( osg::Vec3( 0.0f, 0.0f, 0.0f ), 5.0f ) ) );
    pPat->addChild( pGeodeSphere );
    //pGroup->addChild( pPat );
    pGroup->addChild( addDraggerToScene( pPat, "CDraggerPoint", false ) );

    //FixedSizeNode* fs = new FixedSizeNode;
    //fs->m_rPatSphere = pPat;

    //osg::Vec3 spt(100,100,100), ept(150,150,150);
    //float lineLength = 50;
    //osg::Cylinder* cylinder = new osg::Cylinder ((spt+ept)/2, 0.5 * lineLength, lineLength);
    //osg::Drawable* cylinderGeom = new osg::ShapeDrawable(cylinder);
    //osg::Geode* geode = new osg::Geode;
    //geode->addDrawable(cylinderGeom);
    //fs->addChild(geode);
    //fs->setDataVariance(osg::Object::DYNAMIC);
    //fs->getOrCreateStateSet()->setDataVariance(osg::Object::DYNAMIC);

    //pGroup->addChild(fs);
    //pGroup->addChild(pPat);

    return pGroup;
}

int main( int argc, char* argv[] )
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    rViewer->setUpViewInWindow( 32, 32, 512, 512 );

    osg::ref_ptr<osg::PositionAttitudeTransform> rPat = new osg::PositionAttitudeTransform;

    // add the handler to the viewer
    //rViewer->addEventHandler( new CInputHandler(rPat.get()) );

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

    // set the scene-graph data the viewer will render
    rViewer->setSceneData( root );

    return rViewer->run();
    return 0;
}