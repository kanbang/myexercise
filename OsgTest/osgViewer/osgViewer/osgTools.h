#ifndef OSG_TOOLS_H
#define OSG_TOOLS_H

#include "osgAll.h"
//#include "osgViewDirection.h"

#include <boost/signals2.hpp>
#include <boost/bind.hpp>

enum osgViewDirection
{
	VD_ALL   =   0,  // Ĭ����ͼ
	VD_FRONT =   1,  // ����ͼ
	VD_TOP   =   2,  // ����ͼ
	VD_SIDE  =   3   // ����ͼ
};

namespace osgTools
{
	extern osg::AnimationPathCallback* createAnimationPathCallback( float radius, float time );
	extern osg::Camera* createRTTCamera( osg::Camera::BufferComponent buffer, osg::Texture* tex, bool isAbsolute = false );
	extern osg::Camera* createHUDCamera( double left, double right, double bottom, double top );
	extern osg::Geode* createScreenQuad( float width, float height, float scale = 1.0f );
	extern osgText::Text* createText( const osg::Vec3& pos, const std::string& content, float size );
	extern osg::Camera* createCamera( int x, int y, int w, int h );

	extern float randomValue( float min, float max );
	extern osg::Vec3 randomVector( float min, float max );
	extern osg::Matrix randomMatrix( float min, float max );
	extern void setupOsgLog( osg::NotifySeverity severity, const std::string& logfile );
	extern osg::Matrix computeMVPW( osg::Camera* camera );
	extern void adjustHudCameraViewport( osg::Camera* camera, osg::Viewport* vp );

	/**
	* һ��2d��������
	* ֱ�Ӵ�OrbitManipulator��������,ȥ����OrbitManipulator�Դ���һЩ����
	* ȥ�������ݰ�����
	*	1) ��������ת
	*	2) ����Ҽ�����
	*	3) ��ת(ƽ�Ʋ�������Ҫ��ת)
	*	4) ˦������Ч��
	*
	* ʵ��˼·��
	osgGA::CameraManipulator��ʼĬ�ϵ�����ϵ(xoy,�ڹ��캯���п��Կ���)
	eye    : (0,0,0)
	center : (0,0,-1)
	up     : (0,1,0)

	osgGA::CameraManipulator::computeHomePosition()����������������λ��
	�ú���������setHomePosition()����,������_homeEye, _homeCenter, _homeUp
	setHomePosition(boundingSphere.center() + osg::Vec3d(0.0,-dist,0.0f),
	boundingSphere.center(),
	osg::Vec3d(0.0f,0.0f,1.0f),
	_autoComputeHomePosition);
	�����д�����Կ���osg���۲�����ϵ�޸ĳ���xoz
	eye    :  (0,-1,0)
	center :  (0,0,0)
	up     :  (0,0,1)

	��˴�xoy   -->  xoz ����һ����ת�任����_rotation(��x����ת+90��)
	_rotation�����ļ���������osgGA::StandardManipulator::setTransformation()��
	�ú���������_rotation
	_rotation = rotation_matrix.getRotate().inverse();  (StandardManipulator.cpp��134��)

	���2d�������任��������ƽ������ϵ����Ҫ���ж���ı任����
	xoz  -->  xoz
	xoz  -->  xoy
	xoz  -->  yoz
	xoz  -->  ...
	����任��ʵ�ַ�װ��getUpVectorRotate()������
	_rotation *= getUpVectorRotate();
	*/
	class TwoDimManipulator : public osgGA::OrbitManipulator
	{
		typedef osgGA::OrbitManipulator inherited;

	public:
		TwoDimManipulator( int flags = DEFAULT_SETTINGS );
		TwoDimManipulator( const TwoDimManipulator& tm,
			const osg::CopyOp& copyOp = osg::CopyOp::SHALLOW_COPY );

		void setViewDirection( osgViewDirection vd );
		osgViewDirection getViewDirection() const;
		void zoomOut();
		void zoomIn();

		virtual void setTransformation ( const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up );

	protected:
		// ���⴦��ո��
		virtual bool handleKeyDown ( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );
		// ����������
		virtual bool performMovementLeftMouseButton( const double eventTimeDelta, const double dx, const double dy );
		// ��������Ҽ�
		virtual bool performMovementRightMouseButton( const double eventTimeDelta, const double dx, const double dy );

	private:
		osg::Quat getUpVectorRotate();
		osgViewDirection m_vd;
	};

	// ��osgCookbook����ժ¼�ģ�Ҳ����ʵ��2ά�Ŀ���
	// ��δʵ����ɣ�������ʱ��������(Ŀǰû���ϸ���)
	// ����Ҫ���osgGA::StandradManipulator��osgGA::OrbitManipulator��Ĳ��������
	// ��Դ: <<osg cook book>> --> chapter4 --> "Manipulating the top, front, and side views"
	class AuxiliaryViewUpdater : public osgGA::GUIEventHandler
	{
	public:
		AuxiliaryViewUpdater();
		virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );

		void panModel( const float dx, const float dy, const float dz = 0.0f );
		void zoomModel( const float dy, bool pushForwardIfNeeded = true );

	protected:
		osg::Vec3d _center;
		double _distance;
		double _wheelZoomFactor;
		float _offsetX, _offsetY;
		float _lastDragX, _lastDragY;
	};

	class PositonHandler  : public osgGA::GUIEventHandler
	{
	public:
		PositonHandler();
		virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );

		osg::observer_ptr<osgText::Text> infoText;
	};

	enum Mask
	{
		/* ������
		* ��node�������visitor���������&����,�������0,��ڵ㲻�ᱻ����
		* ����:
		*	node->setNodeMask(0x1);
		*
		*   osgUtil::IntersectionVisitor iv( intersector.get() );
		*   iv.setTraversalMask(~PICK_MASK);
		*
		* ���: osg::Node::NodeMask, osg::Node::accept(), osg::NodeVisitor::validNodeMask()
		* �ο�: <<osg3.0 beginner guide>> --> chapter9 -->"Time for action �C clicking and selecting geometries"
		*/
		PICK_MASK = 1 << 19
	};

	/*
	* pick������
	* �ο�ʵ��:
	*	1) osgEarth/Picker.cpp
	*	2) osg/examples/PickHandler
	*	3) <<osg3.0 beginner guide>> --> chapter9 --> "Time for action �C clicking and selecting geometries"
	*	4) <<osg cook book>>���PickHandler
	*/
	class PickHelper
	{
	public:
		struct Hit
		{
			osg::Node* node;
			osg::Group* parent;
		};
		typedef std::vector<PickHelper::Hit> Hits;
		static void PicksToNodes(Hits& picks, osgNodeArray& nodes);

	public:
		PickHelper( osgViewer::View* view, unsigned traversalMask = ~PICK_MASK );
		bool selectWidnow( double x1, double y1, double x2, double y2, PickHelper::Hits& picks );
		//void addNodePicker( PickHelper::Hit& pick );
		//void addNodePickers( PickHelper::Hits& picks );
		//void removeNodePicker( osg::Node* node );
		//void removeAllPickers();
		//void getSelectedNodes( osgNodeArray& nodes );

	private:
		osgViewer::View* view;
		// �ų�����Ϊ~traversalMask�Ľڵ�
		unsigned traversalMask;
	};

	extern osg::Geometry* buildSelectBoxGeom( const osg::Vec3& corner, float width, float height );

	// ���ҽڵ�������ӽڵ����Ƿ����_toFindNode�ڵ�
	class FindChildNodeVisitor : public osg::NodeVisitor
	{
	public:
		FindChildNodeVisitor(osg::Node* _toFindNode);
		bool getFindResult();
		virtual	void apply ( osg::Node& node );
	private:
		osg::Node* toFindNode;
		bool bFind;
	};

	// ���ҽڵ��е�Ҷ�ӽڵ�
	class FindGeodeNodeVisitor : public osg::NodeVisitor
	{
	public:
		FindGeodeNodeVisitor();
		osg::Geode* getFindResult();
		virtual	void apply ( osg::Geode& geode );
	private:
		osg::Geode* findGeode;
	};

	/* ��osgManipulator����ժ¼��(�����ƣ�����Ҫ�޸�)
	 * Ŀǰ����ʹ��AutoTransform�ﵽͬ����Ч��
		osg::MatrixTransform* mt = new osg::MatrixTransform;
		mt->addChild(geode);

		osgTools::FixedSizeNode* fsNode = new osgTools::FixedSizeNode;
		fsNode->setMatrixTransform(mt);
		fsNode->setFixedSize(getGripBoxSize());
		fsNode->setActive(true);
		gripLayer->addChild(fsNode);
	*/
	class FixedSizeNode : public osg::Group
	{
	public:
		FixedSizeNode();
		FixedSizeNode(const FixedSizeNode& copy, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);

		void setMatrixTransform(osg::MatrixTransform* mt);
		osg::MatrixTransform* getMatrixTransform();
		const osg::MatrixTransform* getMatrixTransform() const;
		void setFixedSize(float size);
		float getFixedSize() const;
		void setActive(bool b);
		bool getActive() const;
		void traverse(osg::NodeVisitor& nv);

	protected:
		osg::ref_ptr<osg::MatrixTransform> _mt;
		osg::Vec3 _center;
		osg::Vec3 _trans;
		osg::Quat _quat;
		float _fixedSize;
		bool _active;
	};

	extern osg::Node* addDraggerToNode(osg::Node* node, const std::string& name, bool fixedSizeInScreen=true);
	extern void setDrawableToAlwaysCull(osg::Drawable& drawable);
	extern osg::Node* creatAutoTransform(const osg::Vec3& pos, osg::Node* node);
}

#endif // OSG_TOOLS_H