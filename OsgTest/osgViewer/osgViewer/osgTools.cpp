#include "osgTools.h"
//#include "osgGeo.h"
#include <fstream>
#include <algorithm>
#include <sstream>
#include <set>

//#include <JLBase/RS_Console.h>

namespace osgTools
{

    osg::ref_ptr<osgText::Font> g_font = osgText::readFontFile( "fonts/arial.ttf" );

    osg::AnimationPathCallback* createAnimationPathCallback( float radius, float time )
    {
        osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath;
        path->setLoopMode( osg::AnimationPath::LOOP );

        unsigned int numSamples = 32;
        float delta_yaw = 2.0f * osg::PI / ( ( float )numSamples - 1.0f );
        float delta_time = time / ( float )numSamples;
        for ( unsigned int i = 0; i < numSamples; ++i )
        {
            float yaw = delta_yaw * ( float )i;
            osg::Vec3 pos( sinf( yaw )*radius, cosf( yaw )*radius, 0.0f );
            osg::Quat rot( -yaw, osg::Z_AXIS );
            path->insert( delta_time * ( float )i, osg::AnimationPath::ControlPoint( pos, rot ) );
        }

        osg::ref_ptr<osg::AnimationPathCallback> apcb = new osg::AnimationPathCallback;
        apcb->setAnimationPath( path.get() );
        return apcb.release();
    }

    osg::Camera* createRTTCamera( osg::Camera::BufferComponent buffer, osg::Texture* tex, bool isAbsolute )
    {
        osg::ref_ptr<osg::Camera> camera = new osg::Camera;
        camera->setClearColor( osg::Vec4() );
        camera->setClearMask( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        camera->setRenderTargetImplementation( osg::Camera::FRAME_BUFFER_OBJECT );
        camera->setRenderOrder( osg::Camera::PRE_RENDER );
        if ( tex )
        {
            tex->setFilter( osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR );
            tex->setFilter( osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR );
            camera->setViewport( 0, 0, tex->getTextureWidth(), tex->getTextureHeight() );
            camera->attach( buffer, tex );
        }

        if ( isAbsolute )
        {
            camera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
            camera->setProjectionMatrix( osg::Matrix::ortho2D( 0.0, 1.0, 0.0, 1.0 ) );
            camera->setViewMatrix( osg::Matrix::identity() );
            camera->addChild( createScreenQuad( 1.0f, 1.0f ) );
        }
        return camera.release();
    }

    osg::Camera* createHUDCamera( double left, double right, double bottom, double top )
    {
        osg::ref_ptr<osg::Camera> camera = new osg::Camera;
        camera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
        camera->setClearMask( GL_DEPTH_BUFFER_BIT );
        camera->setRenderOrder( osg::Camera::POST_RENDER );
        camera->setAllowEventFocus( false );
        camera->setProjectionMatrix( osg::Matrix::ortho2D( left, right, bottom, top ) );
        camera->setViewMatrix( osg::Matrix::identity() );
        camera->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
        return camera.release();
    }
    osg::Camera* createCamera( int x, int y, int w, int h )
    {
        osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
        osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
        traits->windowDecoration = false;
        traits->x = x;
        traits->y = y;
        traits->width = w;
        traits->height = h;
        traits->doubleBuffer = true;
        traits->alpha = ds->getMinimumNumAlphaBits();
        traits->stencil = ds->getMinimumNumStencilBits();
        traits->sampleBuffers = ds->getMultiSamples();
        traits->samples = ds->getNumMultiSamples();

        osg::ref_ptr<osg::Camera> camera = new osg::Camera;
        //camera->setGraphicsContext( new osgQt::GraphicsWindowQt( traits.get() ) );
        camera->setClearColor( osg::Vec4( 0.2, 0.2, 0.6, 1.0 ) );
        camera->setViewport( new osg::Viewport( 0, 0, traits->width, traits->height ) );
        camera->setProjectionMatrixAsPerspective(
            30.0f, static_cast<double>( traits->width ) / static_cast<double>( traits->height ), 1.0f, 10000.0f );
        return camera.release();
    }

    osg::Geode* createScreenQuad( float width, float height, float scale )
    {
        osg::Geometry* geom = osg::createTexturedQuadGeometry(
                                  osg::Vec3(), osg::Vec3( width, 0.0f, 0.0f ), osg::Vec3( 0.0f, height, 0.0f ),
                                  0.0f, 0.0f, width * scale, height * scale );
        osg::ref_ptr<osg::Geode> quad = new osg::Geode;
        quad->addDrawable( geom );

        int values = osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED;
        quad->getOrCreateStateSet()->setAttribute(
            new osg::PolygonMode( osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL ), values );
        quad->getOrCreateStateSet()->setMode( GL_LIGHTING, values );
        return quad.release();
    }

    osgText::Text* createText( const osg::Vec3& pos, const std::string& content, float size )
    {
        osg::ref_ptr<osgText::Text> text = new osgText::Text;
        text->setDataVariance( osg::Object::DYNAMIC );
        text->setFont( g_font.get() );
        text->setCharacterSize( size );
        text->setAxisAlignment( osgText::TextBase::XY_PLANE );
        text->setPosition( pos );
        text->setText( content );
        return text.release();
    }

    float randomValue( float min, float max )
    {
        return ( min + ( float )rand() / ( RAND_MAX + 1.0f ) * ( max - min ) );
    }

    osg::Vec3 randomVector( float min, float max )
    {
        return osg::Vec3( randomValue( min, max ),
                          randomValue( min, max ),
                          randomValue( min, max ) );
    }

    osg::Matrix randomMatrix( float min, float max )
    {
        osg::Vec3 rot = randomVector( -osg::PI, osg::PI );
        osg::Vec3 pos = randomVector( min, max );
        return osg::Matrix::rotate( rot[0], osg::X_AXIS, rot[1], osg::Y_AXIS, rot[2], osg::Z_AXIS ) *
               osg::Matrix::translate( pos );
    }

    void adjustHudCameraViewport( osg::Camera* camera, osg::Viewport* vp )
    {
        if( camera && camera->getReferenceFrame() == osg::Transform::ABSOLUTE_RF )
        {
            camera->setProjectionMatrix( osg::Matrix::ortho2D( vp->x(), vp->width(), vp->y(), vp->height() ) );
            camera->setViewport( vp );
        }
    }

    TwoDimManipulator::TwoDimManipulator( int flags )
    {
        _allowThrow = false;
        _thrown = false;
        _verticalAxisFixed = false;
        m_vd = VD_TOP;
    }

    TwoDimManipulator::TwoDimManipulator( const TwoDimManipulator& tm, const osg::CopyOp& copyOp )
        : inherited( tm, copyOp )
    {
        _allowThrow = tm._allowThrow;
        _thrown = tm._thrown;
        _verticalAxisFixed = tm._verticalAxisFixed;
        m_vd = tm.m_vd;
    }

    osg::Quat TwoDimManipulator::getUpVectorRotate()
    {
        osg::Quat new_rotate;
        switch( m_vd )
        {
        case VD_ALL:
            new_rotate = osg::Quat( osg::PI_4, osg::Z_AXIS );
            break;
        case VD_FRONT:
            break;
        case VD_TOP:
            new_rotate = osg::Quat( -osg::PI_2, osg::X_AXIS );
            break;
        case VD_SIDE:
            new_rotate = osg::Quat( osg::PI_2, osg::Z_AXIS );
            break;
        }
        return new_rotate;
    }

    void TwoDimManipulator::setViewDirection( osgViewDirection vd )
    {
        m_vd = vd;

        osg::Vec3d oldCenter = _center;
        double oldDistance = _distance;

        // 计算旋转角度
        setTransformation( _homeEye, _homeCenter, _homeUp );

        _center = oldCenter;
        _distance = oldDistance;
    }

    osgViewDirection TwoDimManipulator::getViewDirection() const
    {
        return m_vd;
    }

    void TwoDimManipulator::zoomOut()
    {
        zoomModel( +_wheelZoomFactor, true );
    }

    void TwoDimManipulator::zoomIn()
    {
        zoomModel( -_wheelZoomFactor, true );
    }

    void TwoDimManipulator::setTransformation( const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up )
    {
        // 父类的setTransformation方法会计算xoy坐标系到xoz坐标系的旋转变换(_rotation)
        // _rotation = ....;
        inherited::setTransformation( eye, center, up );
        _rotation *= getUpVectorRotate();
    }

    bool TwoDimManipulator::performMovementLeftMouseButton( const double eventTimeDelta, const double dx, const double dy )
    {
        if( m_vd == VD_ALL )
            return inherited::performMovementLeftMouseButton( eventTimeDelta, dx, dy );
        else
            return false;
    }

    bool TwoDimManipulator::performMovementRightMouseButton( const double eventTimeDelta, const double dx, const double dy )
    {
        return false;
    }

    bool TwoDimManipulator::handleKeyDown( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us )
    {
        osgViewer::View* view = dynamic_cast<osgViewer::View*>( &us );
        if ( !view ) return false;

        if( ea.getKey() == osgGA::GUIEventAdapter::KEY_Space )
        {
            setNode( view->getSceneData() );
        }
        return inherited::handleKeyDown( ea, us );
    }

    AuxiliaryViewUpdater::AuxiliaryViewUpdater()
        : _distance( -1.0 ), _offsetX( 0.0f ), _offsetY( 0.0f ),
          _lastDragX( -1.0f ), _lastDragY( -1.0f ), _wheelZoomFactor( 0.1 )
    {}

    bool AuxiliaryViewUpdater::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
    {
        osgViewer::View* view = dynamic_cast<osgViewer::View*>( &aa );
        if ( view )
        {
            switch ( ea.getEventType() )
            {
            case( osgGA::GUIEventAdapter::SCROLL ):
            {
                osgGA::GUIEventAdapter::ScrollingMotion sm = ea.getScrollingMotion();
                switch( sm )
                {
                    // mouse scroll up event
                case( osgGA::GUIEventAdapter::SCROLL_UP ):
                {
                    // perform zoom
                    zoomModel( _wheelZoomFactor, true );
                }
                break;

                // mouse scroll down event
                case( osgGA::GUIEventAdapter::SCROLL_DOWN ):
                {
                    // perform zoom
                    zoomModel( -_wheelZoomFactor, true );
                }
                break;
                }
            }

            break;

            case( osgGA::GUIEventAdapter::DRAG ):
            {
                if ( _lastDragX > 0.0f && _lastDragY > 0.0f )
                {
                    if ( ea.getButtonMask() == osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON )
                    {
                        _offsetX += ea.getX() - _lastDragX;
                        _offsetY += ea.getY() - _lastDragY;

                        float scale = -0.3f * _distance;
                        panModel( _offsetX * scale, _offsetY * scale );
                    }
                }
                _lastDragX = ea.getX();
                _lastDragY = ea.getY();
            }
            break;

            case( osgGA::GUIEventAdapter::FRAME ):
            {
                if ( view->getCamera() )
                {
                    osg::Vec3d eye, center, up;
                    view->getCamera()->getViewMatrixAsLookAt( eye, center, up );

                    osg::Vec3d lookDir = center - eye;
                    lookDir.normalize();
                    osg::Vec3d side = lookDir ^ up;
                    side.normalize();

                    const osg::BoundingSphere& bs = view->getSceneData()->getBound();
                    if ( _distance < 0.0 ) _distance = bs.radius() * 3.0;
                    center = bs.center();

                    center -= ( side * _offsetX + up * _offsetY ) * 0.1;
                    view->getCamera()->setViewMatrixAsLookAt( center - lookDir * _distance, center, up );
                }
            }
            break;
            }
        }
        return false;
    }

    void AuxiliaryViewUpdater::panModel( const float dx, const float dy, const float dz )
    {
        osg::Vec3d dv( dx, dy, dz );
        _center += dv;
    }

    void AuxiliaryViewUpdater::zoomModel( const float dy, bool pushForwardIfNeeded )
    {
        // scale
        float scale = 1.0f + dy;
        _distance *= scale;
    }

    class LogFileHandler : public osg::NotifyHandler
    {
    public:
        LogFileHandler( const std::string& file )
        {
            _log.open( file.c_str() );
        }
        virtual ~LogFileHandler()
        {
            _log.close();
        }

        virtual void notify( osg::NotifySeverity severity, const char* msg )
        {
            _log << msg;
        }

    protected:
        std::ofstream _log;
    };

    void setupOsgLog( osg::NotifySeverity severity, const std::string& logfile )
    {
        osg::setNotifyLevel( osg::DEBUG_INFO );
        osg::setNotifyHandler( new LogFileHandler( logfile ) );
    }

    osg::Matrix computeMVPW( osg::Camera* camera )
    {
        osg::Matrix matrix;

        if ( camera->getViewport() )
        {
            matrix.set( camera->getViewMatrix() * camera->getProjectionMatrix() );
            matrix.postMult( camera->getViewport()->computeWindowMatrix() );
        }
        return matrix;
    }

    void RemoveNodePicker_Helper( osg::Node* node )
    {
        if( !node ) return;
        osg::ref_ptr<osgFX::Effect> effect = dynamic_cast<osgFX::Effect*>( node );
        if( effect && effect->getNumChildren() > 0 )
        {
            osg::ref_ptr<osg::Node> child_node = effect->getChild( 0 );
            effect->removeChildren( 0, effect->getNumChildren() );

            osg::Node::ParentList parentList = effect->getParents();
            for( osg::Node::ParentList::iterator itr = parentList.begin(); itr != parentList.end(); ++itr )
            {
                ( *itr )->replaceChild( effect.get(), child_node.get() );
            }
        }
    }

    class RemovePickerVisitor : public osg::NodeVisitor
    {
    public:
        RemovePickerVisitor() : NodeVisitor( NodeVisitor::TRAVERSE_ALL_CHILDREN )
        {
        }
        virtual	void apply ( osg::Node& node )
        {
            traverse( node );
            RemoveNodePicker_Helper( &node );
        }
    };

    class GetPickersVisitor : public osg::NodeVisitor
    {
    public:
        GetPickersVisitor( osgNodeArray& _nodes )
            : NodeVisitor( NodeVisitor::TRAVERSE_ALL_CHILDREN ), nodes( _nodes )
        {
        }
        virtual	void apply ( osg::Node& node )
        {
            traverse( node );
            osg::ref_ptr<osgFX::Effect> effect = dynamic_cast<osgFX::Effect*>( &node );
            if( effect && effect->getNumChildren() > 0 )
            {
                nodes.push_back( effect->getChild( 0 ) );
            }
        }

    private:
        osgNodeArray& nodes;
    };

    PickHelper::PickHelper( osgViewer::View* m_view, unsigned _traversalMask )
        : view( m_view ), traversalMask( _traversalMask )
    {
    }

    bool PickHelper::selectWidnow( double x1, double y1, double x2, double y2, PickHelper::Hits& picks )
    {
        double xMin, yMin, xMax, yMax;
        xMin = ( std::min )( x1, x2 );
        xMax = ( std::max )( x1, x2 );
        yMin = ( std::min )( y1, y2 );
        yMax = ( std::max )( y1, y2 );

        osg::ref_ptr<osgUtil::PolytopeIntersector> intersector =
            new osgUtil::PolytopeIntersector( osgUtil::Intersector::WINDOW, xMin, yMin, xMax, yMax );

        osgUtil::IntersectionVisitor iv( intersector.get() );
        iv.setTraversalMask( traversalMask ); // 排除包含PICK_MASK掩码的节点
        view->getCamera()->accept( iv );

        bool ret = intersector->containsIntersections();
        if ( ret )
        {
            // 排除重复的节点

            typedef std::set<osg::Node*> IntersetNodeSet;
            IntersetNodeSet intersect_nodes;
            std::pair<IntersetNodeSet::iterator, bool> ret;

            for( osgUtil::PolytopeIntersector::Intersections::iterator
                    hitr = intersector->getIntersections().begin();
                    hitr != intersector->getIntersections().end();
                    ++hitr )
            {
                osg::NodePath& np = hitr->nodePath;

                PickHelper::Hit pick;
                pick.node = ( np.size() >= 1 ) ? np[np.size() - 1] : 0;
                pick.parent = ( np.size() >= 2 ) ? dynamic_cast<osg::Group*>( np[np.size() - 2] ) : 0;

                // 添加失败,则表示有重复
                ret = intersect_nodes.insert( pick.node );
                if( ret.second )
                {
                    picks.push_back( pick );
                }
            }
        }
        return ret;
    }

    //void PickHelper::removeNodePicker( osg::Node* node )
    //{
    //	RemoveNodePicker_Helper( node );
    //}

    //void PickHelper::removeAllPickers()
    //{
    //	// 遍历整个场景图,查找包含高亮特效的节点并删除高亮特效
    //	RemovePickerVisitor rpv;
    //	view->getSceneData()->accept( rpv );
    //}

    //void PickHelper::addNodePicker( PickHelper::Hit& pick )
    //{
    //	osg::Node* node = pick.node;
    //	osg::Group* parent = pick.parent;

    //	//用一种特效高亮显示被选中物体
    //	if ( parent && node )
    //	{
    //		osg::ref_ptr<osgFX::Outline> parentEffect = dynamic_cast<osgFX::Outline*>( parent );
    //		//osg::ref_ptr<osgFX::Scribe> parentEffect = dynamic_cast<osgFX::Scribe*>(parent);
    //		if ( !parentEffect )
    //		{
    //			osg::ref_ptr<osgFX::Outline> effect = new osgFX::Outline();
    //			//osg::ref_ptr<osgFX::Scribe> effect = new osgFX::Scribe();
    //			effect->setWidth( 4 );
    //			effect->setColor( osg::Vec4( 1.0f, 0.0f, 0.0f, 1.0f ) );
    //			effect->addChild( node );
    //			parent->replaceChild( node, effect.get() );
    //		}
    //	}
    //}

    //void PickHelper::addNodePickers( PickHelper::Hits& picks )
    //{
    //	typedef PickHelper::Hits::iterator PickIterator;
    //	for( PickIterator itr = picks.begin(); itr != picks.end(); ++itr )
    //	{
    //		addNodePicker( *itr );
    //	}
    //}

    //void PickHelper::getSelectedNodes( osgNodeArray& nodes )
    //{
    //	GetPickersVisitor gpv( nodes );
    //	view->getSceneData()->accept( gpv );
    //}

    void PickHelper::PicksToNodes( Hits& picks, osgNodeArray& nodes )
    {
        for( int i = 0; i < picks.size(); i++ )
        {
            nodes.push_back( picks[i].node );
        }
    }

    osg::Geometry* buildSelectBoxGeom( const osg::Vec3& corner, float width, float height )
    {
        osg::Geometry* selBoxGeom = new osg::Geometry;
        //selBoxGeom->setDataVariance(osg::Object::DYNAMIC);
        //selBoxGeom->setUseDisplayList(false);

        osg::Vec3Array* vertex = new osg::Vec3Array;
        vertex->push_back( corner );
        vertex->push_back( corner + osg::Vec3( width, 0, 0 ) );
        vertex->push_back( corner + osg::Vec3( width, height, 0 ) );
        vertex->push_back( corner + osg::Vec3( 0, height, 0 ) );
        selBoxGeom->setVertexArray( vertex );

        osg::Vec4Array* colors = new osg::Vec4Array;
        // 浅蓝色--(0.0f,0.498f,1.0f,0.196f)
        // 浅绿色--(0.0f,1.0f,0.247f,0.196f)
        osg::Vec4 c1( 0.0f, 0.498f, 1.0f, 0.196f );
        osg::Vec4 c2( 0.0f, 1.0f, 0.247f, 0.196f );
        colors->push_back( width > 0 ? c1 : c2 );

        selBoxGeom->setColorArray( colors );
        selBoxGeom->setColorBinding( osg::Geometry::BIND_OVERALL );

        //osg::Vec3Array* normal = new osg::Vec3Array(1);
        //normal->push_back(osg::Vec3(0,0,1));
        //selBoxGeom->setNormalArray(normal);
        //selBoxGeom->setNormalBinding(osg::Geometry::BIND_OVERALL);

        selBoxGeom->addPrimitiveSet( new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, 4 ) );

        return selBoxGeom;
    }

    PositonHandler::PositonHandler()
    {
    }

    std::string osgVecToString( const osg::Vec3& pt )
    {
        std::ostringstream ss;
        ss << "(" << pt.x() << ", " << pt.y() << ", " << pt.z() << ")";
        return ss.str();
    }

    bool PositonHandler::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
    {
        osgViewer::View* view = dynamic_cast<osgViewer::View*>( &aa );
        if ( !view ) return false;

        osg::Vec3 pt( ea.getX(), ea.getY(), 0 );
        pt = pt * osg::Matrix::inverse( osgTools::computeMVPW( view->getCamera() ) );

        osg::Vec3 eye, center, up;
        view->getCamera()->getViewMatrixAsLookAt( eye, center, up, 1000 );

        double left, right, bottom, top, zNera, zFar;
        view->getCamera()->getProjectionMatrixAsFrustum( left, right, bottom, top, zNera, zFar );

        //osg::Group* gp = view->getSceneData()->asGroup()->getChild(4)->asGroup()->getChild(2)->asGroup();

        osg::Vec3 node_center = view->getSceneData()->getBound().center();
        std::ostringstream ss;
        ss /*<< "preview count:" << gp->getNumChildren() <<"\n"*/
                << "Mouse: " << osgVecToString( osg::Vec3( ea.getX(), ea.getY(), 0 ) ) << "\n"
                << "World: " << osgVecToString( pt ) << "\n"
                << "(xMin, xMax) = " << osgVecToString( osg::Vec3( ea.getXmin(), ea.getXmax(), 0 ) ) << "  "
                << "(yMin, yMax) = " << osgVecToString( osg::Vec3( ea.getYmin(), ea.getYmax(), 0 ) ) << "\n"
                << "Event Type: " << ea.getEventType() << ", Button: " << ea.getButton() << " Button Mask:" << ea.getButtonMask() << "\n"
                << "eye: " << osgVecToString( eye ) << "  center: " << osgVecToString( center ) << "  up: " << osgVecToString( up ) << "\n"
                << "scene center: " << osgVecToString( node_center ) << "\n"
                << "left:" << left << " right:" << right << " bottom:" << bottom << " top:" << top << " zNear:" << zNera << " zFar:" << zFar
                << "\n";

        infoText->setText( ss.str() );

        return false;
    }


    FindChildNodeVisitor::FindChildNodeVisitor( osg::Node* _toFindNode )
        : NodeVisitor( NodeVisitor::TRAVERSE_ALL_CHILDREN ), toFindNode( _toFindNode ), bFind( false )
    {
    }

    bool FindChildNodeVisitor::getFindResult()
    {
        return bFind;
    }

    void FindChildNodeVisitor::apply ( osg::Node& node )
    {
        if( toFindNode == &node )
        {
            bFind = true;
        }
        else
        {
            traverse( node );
        }
    }

    FindGeodeNodeVisitor::FindGeodeNodeVisitor()
        : NodeVisitor( NodeVisitor::TRAVERSE_ALL_CHILDREN ), findGeode( 0 )
    {
    }

    osg::Geode* FindGeodeNodeVisitor::getFindResult()
    {
        return findGeode;
    }

    void FindGeodeNodeVisitor::apply( osg::Geode& geode )
    {
        findGeode = &geode;
    }

    FixedSizeNode::FixedSizeNode() : _fixedSize( 240.0f ), _active( true ) {}

    FixedSizeNode::FixedSizeNode( const FixedSizeNode& copy, const osg::CopyOp& copyop )
        :   osg::Group( copy, copyop ),
            _mt( copy._mt ), _fixedSize( copy._fixedSize ), _active( copy._active )
    {}

    void FixedSizeNode::setMatrixTransform( osg::MatrixTransform* mt )
    {
        _mt = mt;
        _center = _mt->getBound().center();
        _trans = _mt->getMatrix().getTrans();
        _quat = _mt->getMatrix().getRotate();
        if ( !containsNode( mt ) ) addChild( mt );
    }

    osg::MatrixTransform* FixedSizeNode::getMatrixTransform()
    {
        return _mt.get();
    }

    const osg::MatrixTransform* FixedSizeNode::getMatrixTransform() const
    {
        return _mt.get();
    }

    void FixedSizeNode::setFixedSize( float size )
    {
        _fixedSize = size;
    }

    float FixedSizeNode::getFixedSize() const
    {
        return _fixedSize;
    }

    void FixedSizeNode::setActive( bool b )
    {
        _active = b;
    }

    bool FixedSizeNode::getActive() const
    {
        return _active;
    }

    void FixedSizeNode::traverse( osg::NodeVisitor& nv )
    {
        if ( _mt.valid() )
        {
            if ( _active && nv.getVisitorType() == osg::NodeVisitor::CULL_VISITOR )
            {
                osgUtil::CullVisitor* cv = static_cast<osgUtil::CullVisitor*>( &nv );

                float pixelSize = cv->pixelSize( _center, 0.48f );
                if ( pixelSize != _fixedSize )
                {
                    float pixelScale = pixelSize > 0.0f ? _fixedSize / pixelSize : 1.0f;
                    osg::Vec3d scaleFactor( pixelScale, pixelScale, pixelScale );

                    osg::Vec3 offset = _center - _center * osg::Matrix::scale( scaleFactor );
                    _mt->setMatrix( osg::Matrix::scale( scaleFactor ) *
                                    osg::Matrix::rotate( _quat ) *
                                    osg::Matrix::translate( _trans + offset ) );
                }
            }
        }
        osg::Group::traverse( nv );
    }

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
        else
        {
            osgManipulator::TabBoxDragger* d = new osgManipulator::TabBoxDragger();
            d->setupDefaultGeometry();
            dragger = d;
        }



        return dragger;
    }

    osg::Node* addDraggerToNode( osg::Node* node, const std::string& name, bool fixedSizeInScreen )
    {
        node->getOrCreateStateSet()->setMode( GL_NORMALIZE, osg::StateAttribute::ON );

        osg::MatrixTransform* selection = new osg::MatrixTransform;
        selection->addChild( node );

        osgManipulator::Dragger* dragger = createDragger( name );

        osg::Group* root = new osg::Group;
        root->addChild( selection );

        if ( fixedSizeInScreen )
        {
            FixedSizeNode* fsNode = new FixedSizeNode;
            fsNode->setMatrixTransform( dragger );
            root->addChild( fsNode );
        }
        else
            root->addChild( dragger );

        float scale = node->getBound().radius() * 1.2;
        dragger->setMatrix( osg::Matrix::scale( scale, scale, scale ) *
                            osg::Matrix::translate( node->getBound().center() ) );

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

    class ForceCullCallback : public osg::Drawable::CullCallback
    {
    public:
        virtual bool cull( osg::NodeVisitor*, osg::Drawable*, osg::State* ) const
        {
            return true;
        }
    };

    void setDrawableToAlwaysCull( osg::Drawable& drawable )
    {
        ForceCullCallback* cullCB = new ForceCullCallback;
        drawable.setCullCallback ( cullCB );
    }

    osg::Node* creatAutoTransform( const osg::Vec3& pos, osg::Node* node )
    {
        osg::AutoTransform* at = new osg::AutoTransform;
        at->setPosition( pos );
        //at->setPivotPoint(pos * scaleFactor);
        at->setAutoScaleToScreen( true );
        at->addChild( node );

        //osgManipulator::AntiSquish* as = new osgManipulator::AntiSquish;
        //as->setPivot(pos);
        //as->addChild(at);

        return at;
    }
}