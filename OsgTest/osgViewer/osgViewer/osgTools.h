#ifndef OSG_TOOLS_H
#define OSG_TOOLS_H

#include "osgAll.h"
//#include "osgViewDirection.h"

#include <boost/signals2.hpp>
#include <boost/bind.hpp>

enum osgViewDirection
{
	VD_ALL   =   0,  // 默认视图
	VD_FRONT =   1,  // 正视图
	VD_TOP   =   2,  // 俯视图
	VD_SIDE  =   3   // 侧视图
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
	* 一个2d的漫游器
	* 直接从OrbitManipulator派生而来,去掉了OrbitManipulator自带的一些功能
	* 去掉的内容包括：
	*	1) 鼠标左键旋转
	*	2) 鼠标右键缩放
	*	3) 旋转(平移操作不需要旋转)
	*	4) 甩出动画效果
	*
	* 实现思路：
	osgGA::CameraManipulator初始默认的坐标系(xoy,在构造函数中可以看到)
	eye    : (0,0,0)
	center : (0,0,-1)
	up     : (0,1,0)

	osgGA::CameraManipulator::computeHomePosition()函数计算漫游器的位置
	该函数调用了setHomePosition()函数,来更新_homeEye, _homeCenter, _homeUp
	setHomePosition(boundingSphere.center() + osg::Vec3d(0.0,-dist,0.0f),
	boundingSphere.center(),
	osg::Vec3d(0.0f,0.0f,1.0f),
	_autoComputeHomePosition);
	从这行代码可以看出osg将观察坐标系修改成了xoz
	eye    :  (0,-1,0)
	center :  (0,0,0)
	up     :  (0,0,1)

	因此从xoy   -->  xoz 存在一个旋转变换，即_rotation(绕x轴旋转+90度)
	_rotation变量的计算体现在osgGA::StandardManipulator::setTransformation()中
	该函数计算了_rotation
	_rotation = rotation_matrix.getRotate().inverse();  (StandardManipulator.cpp第134行)

	这个2d漫游器变换成其他的平面坐标系，需要进行额外的变换，即
	xoz  -->  xoz
	xoz  -->  xoy
	xoz  -->  yoz
	xoz  -->  ...
	这个变换的实现封装在getUpVectorRotate()函数中
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
		// 特殊处理空格键
		virtual bool handleKeyDown ( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );
		// 屏蔽鼠标左键
		virtual bool performMovementLeftMouseButton( const double eventTimeDelta, const double dx, const double dy );
		// 屏蔽鼠标右键
		virtual bool performMovementRightMouseButton( const double eventTimeDelta, const double dx, const double dy );

	private:
		osg::Quat getUpVectorRotate();
		osgViewDirection m_vd;
	};

	// 从osgCookbook书上摘录的，也可以实现2维的控制
	// 尚未实现完成，后续有时间再完善(目前没用上该类)
	// 还需要结合osgGA::StandradManipulator和osgGA::OrbitManipulator类的部分来完成
	// 来源: <<osg cook book>> --> chapter4 --> "Manipulating the top, front, and side views"
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
		/* 掩码标记
		* 当node的掩码和visitor的掩码进行&运算,如果等于0,则节点不会被处理
		* 例如:
		*	node->setNodeMask(0x1);
		*
		*   osgUtil::IntersectionVisitor iv( intersector.get() );
		*   iv.setTraversalMask(~PICK_MASK);
		*
		* 详见: osg::Node::NodeMask, osg::Node::accept(), osg::NodeVisitor::validNodeMask()
		* 参考: <<osg3.0 beginner guide>> --> chapter9 -->"Time for action – clicking and selecting geometries"
		*/
		PICK_MASK = 1 << 19
	};

	/*
	* pick辅助类
	* 参考实现:
	*	1) osgEarth/Picker.cpp
	*	2) osg/examples/PickHandler
	*	3) <<osg3.0 beginner guide>> --> chapter9 --> "Time for action – clicking and selecting geometries"
	*	4) <<osg cook book>>里的PickHandler
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
		// 排除掩码为~traversalMask的节点
		unsigned traversalMask;
	};

	extern osg::Geometry* buildSelectBoxGeom( const osg::Vec3& corner, float width, float height );

	// 查找节点的所有子节点中是否包含_toFindNode节点
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

	// 查找节点中的叶子节点
	class FindGeodeNodeVisitor : public osg::NodeVisitor
	{
	public:
		FindGeodeNodeVisitor();
		osg::Geode* getFindResult();
		virtual	void apply ( osg::Geode& geode );
	private:
		osg::Geode* findGeode;
	};

	/* 从osgManipulator例子摘录的(不完善，还需要修改)
	 * 目前可以使用AutoTransform达到同样的效果
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