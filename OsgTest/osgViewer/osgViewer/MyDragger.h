#ifndef MYDRAGGER_H
#define MYDRAGGER_H

#include <osgManipulator/Dragger>
#include <osgManipulator/Projector>

/**
 * Dragger for performing 1D translation.
 */
class  MyDragger : public osgManipulator::Dragger
{
    public:

        MyDragger();

        //META_OSGMANIPULATOR_Object(osgManipulator,MyDragger)

        MyDragger(const osg::Vec3d& s, const osg::Vec3d& e);

        /** Handle pick events on dragger and generate TranslateInLine commands. */
		virtual bool handle(const osgManipulator::PointerInfo& pi, const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);

        /** Setup default geometry for dragger. */
        void setupDefaultGeometry();

        /** Set/Get color for dragger. */
        inline void setColor(const osg::Vec4& color) { _color = color; setMaterialColor(_color,*this); }
        inline const osg::Vec4& getColor() const { return _color; }

        /** Set/Get pick color for dragger. Pick color is color of the dragger when picked.
            It gives a visual feedback to show that the dragger has been picked. */
        inline void setPickColor(const osg::Vec4& color) { _pickColor = color; }
        inline const osg::Vec4& getPickColor() const { return _pickColor; }

        inline void setCheckForNodeInNodePath(bool onOff) { _checkForNodeInNodePath = onOff; }

    protected:

        virtual ~MyDragger();

		osg::ref_ptr< osgManipulator::LineProjector >   _projector;
        osg::Vec3d                      _startProjectedPoint;

        osg::Vec4                       _color;
        osg::Vec4                       _pickColor;

        bool                            _checkForNodeInNodePath;
};

#endif // MYDRAGGER_H
