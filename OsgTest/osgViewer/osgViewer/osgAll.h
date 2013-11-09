#ifndef OSG_ALL_H
#define OSG_ALL_H

#include <osg/Node>
#include <osg/Group>
#include <osg/Switch>
#include <osg/Texture>
#include <osg/Camera>
#include <osg/Depth>
#include <osg/ClearNode>
#include <osg/Texture2D>
#include <osg/PolygonMode>
#include <osg/Projection>
#include <osg/LineWidth>
#include <osg/PolygonMode>
#include <osg/ShapeDrawable>
#include <osgText/Font>
#include <osgText/Text>
#include <osg/io_utils>
#include <osg/CullFace>
#include <osg/AnimationPath>
#include <osg/AutoTransform>
#include <osg/ComputeBoundsVisitor>
#include <osg/CoordinateSystemNode>
#include <osg/PositionAttitudeTransform>

#include <osgViewer/View>
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgDB/ReadFile>
//#include <osgQt/GraphicsWindowQt>

#include <osgFX/Scribe>
#include <osgFX/Effect>
#include <osgFX/Outline>

#include <osgGA/GUIEventHandler>
#include <osgGA/TerrainManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>

#include <osgManipulator/AntiSquish>
#include <osgManipulator/TabBoxDragger>
#include <osgManipulator/TabBoxTrackballDragger>
#include <osgManipulator/TabPlaneDragger>
#include <osgManipulator/TabPlaneTrackballDragger>
#include <osgManipulator/TrackballDragger>
#include <osgManipulator/Translate1DDragger>
#include <osgManipulator/Translate2DDragger>
#include <osgManipulator/TranslateAxisDragger>

#include <osgUtil/Optimizer>
#include <osgUtil/LineSegmentIntersector>
#include <osgUtil/PolytopeIntersector>

#include <vector>

typedef std::vector<osg::Node*> osgNodeArray;

#endif // OSG_ALL_H