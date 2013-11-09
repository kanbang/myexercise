#include "MyDragger.h"
#include <osgManipulator/Command>

#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osg/LineWidth>
#include <osg/Material>

MyDragger::MyDragger() : osgManipulator::Dragger(), _checkForNodeInNodePath( true )
{
    _projector = new osgManipulator::LineProjector;
    setColor( osg::Vec4( 0.0f, 1.0f, 0.0f, 1.0f ) );
    setPickColor( osg::Vec4( 1.0f, 1.0f, 0.0f, 1.0f ) );
}

MyDragger::MyDragger( const osg::Vec3d& s, const osg::Vec3d& e ) : osgManipulator::Dragger(), _checkForNodeInNodePath( true )
{
    _projector = new osgManipulator::LineProjector( s, e );
    setColor( osg::Vec4( 0.0f, 1.0f, 0.0f, 1.0f ) );
    setPickColor( osg::Vec4( 1.0f, 1.0f, 0.0f, 1.0f ) );
}

MyDragger::~MyDragger()
{
}

bool MyDragger::handle( const osgManipulator::PointerInfo& pointer, const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
    // Check if the dragger node is in the nodepath.
    if ( _checkForNodeInNodePath )
    {
        if ( !pointer.contains( this ) ) return false;
    }

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

        if ( _projector->project( pointer, _startProjectedPoint ) )
        {
            // Generate the motion command.
            osg::ref_ptr<osgManipulator::TranslateInLineCommand> cmd =
                new osgManipulator::TranslateInLineCommand( _projector->getLineStart(),
                        _projector->getLineEnd() );
            cmd->setStage( osgManipulator::MotionCommand::START );
            cmd->setLocalToWorldAndWorldToLocal( _projector->getLocalToWorld(), _projector->getWorldToLocal() );

            // Dispatch command.
            dispatch( *cmd );

            // Set color to pick color.
            osgManipulator::setMaterialColor( _pickColor, *this );

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
            osg::ref_ptr<osgManipulator::TranslateInLineCommand> cmd =
                new osgManipulator::TranslateInLineCommand( _projector->getLineStart(),
                        _projector->getLineEnd() );
            cmd->setStage( osgManipulator::MotionCommand::MOVE );
            cmd->setLocalToWorldAndWorldToLocal( _projector->getLocalToWorld(), _projector->getWorldToLocal() );
            cmd->setTranslation( projectedPoint - _startProjectedPoint );

            // Dispatch command.
            dispatch( *cmd );

            aa.requestRedraw();
        }
        return true;
    }

    // Pick finish.
    case ( osgGA::GUIEventAdapter::RELEASE ):
    {
        osg::Vec3d projectedPoint;
        if ( _projector->project( pointer, projectedPoint ) )
        {
            osg::ref_ptr<osgManipulator::TranslateInLineCommand> cmd =
                new osgManipulator::TranslateInLineCommand( _projector->getLineStart(),
                        _projector->getLineEnd() );

            cmd->setStage( osgManipulator::MotionCommand::FINISH );
            cmd->setLocalToWorldAndWorldToLocal( _projector->getLocalToWorld(), _projector->getWorldToLocal() );

            // Dispatch command.
            dispatch( *cmd );

            // Reset color.
            osgManipulator::setMaterialColor( _color, *this );

            aa.requestRedraw();
        }

        return true;
    }
    default:
        return false;
    }
}

void MyDragger::setupDefaultGeometry()
{
    // Get the line length and direction.
    osg::Vec3 lineDir = _projector->getLineEnd() - _projector->getLineStart();
    float lineLength = lineDir.length();
    lineDir.normalize();

    osg::Geode* geode = new osg::Geode;
    // Create a left cone.
    {
        osg::Cone* cone = new osg::Cone ( _projector->getLineStart(), 0.025f * lineLength, 0.10f * lineLength );
        osg::Quat rotation;
        rotation.makeRotate( lineDir, osg::Vec3( 0.0f, 0.0f, 1.0f ) );
        cone->setRotation( rotation );

        geode->addDrawable( new osg::ShapeDrawable( cone ) );
    }

    // Create a right cone.
    {
        osg::Cone* cone = new osg::Cone ( _projector->getLineEnd(), 0.025f * lineLength, 0.10f * lineLength );
        osg::Quat rotation;
        rotation.makeRotate( osg::Vec3( 0.0f, 0.0f, 1.0f ), lineDir );
        cone->setRotation( rotation );

        geode->addDrawable( new osg::ShapeDrawable( cone ) );
    }

    // Create an invisible cylinder for picking the line.
    {
        osg::Cylinder* cylinder = new osg::Cylinder ( ( _projector->getLineStart() + _projector->getLineEnd() ) / 2, 0.015f * lineLength, lineLength );
        osg::Quat rotation;
        rotation.makeRotate( osg::Vec3( 0.0f, 0.0f, 1.0f ), lineDir );
        cylinder->setRotation( rotation );
        osg::Drawable* cylinderGeom = new osg::ShapeDrawable( cylinder );

        osgManipulator::setDrawableToAlwaysCull( *cylinderGeom );

        geode->addDrawable( cylinderGeom );
    }

    osg::Geode* lineGeode = new osg::Geode;
    // Create a line.
    {
        osg::Geometry* geometry = new osg::Geometry();

        osg::Vec3Array* vertices = new osg::Vec3Array( 2 );
        ( *vertices )[0] = _projector->getLineStart();
        ( *vertices )[1] = _projector->getLineEnd();

        geometry->setVertexArray( vertices );
        geometry->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::LINES, 0, 2 ) );

        lineGeode->addDrawable( geometry );
    }

    // Turn of lighting for line and set line width.
    lineGeode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
    osg::LineWidth* linewidth = new osg::LineWidth();
    linewidth->setWidth( 2.0f );
    lineGeode->getOrCreateStateSet()->setAttributeAndModes( linewidth, osg::StateAttribute::ON );

    // Add line and cones to the scene.
    addChild( lineGeode );
    addChild( geode );
}
