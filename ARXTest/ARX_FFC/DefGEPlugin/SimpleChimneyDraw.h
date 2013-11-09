#include "dbmain.h"

#include "../MineGEDraw/MineGEDraw.h"

class SimpleChimneyDraw : public MineGEDraw {

public:
	ACRX_DECLARE_MEMBERS(SimpleChimneyDraw) ;

public:
	SimpleChimneyDraw () ;
	virtual ~SimpleChimneyDraw () ;

	virtual void setAllExtraParamsToDefault();
	virtual void configExtraParams();
	virtual void updateExtraParams();

	virtual void readKeyParam(DrawParamReader& reader);
	virtual void writeKeyParam(DrawParamWriter& writer);

	virtual void readExtraParam(DrawParamReader& reader);
	virtual void writeExtraParam(DrawParamWriter& writer);

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode) ;
	virtual Acad::ErrorStatus subTransformBy( const AcGeMatrix3d & xform );
	virtual Acad::ErrorStatus subGetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;

	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;

	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;

private:
	void drawSegment(AcGiWorldDraw* mode, const AcGePoint3d& spt, const AcGePoint3d& ept);

private:
	// key param
	AcGePoint3dArray m_pts;

	// extra param
	double m_length;               // ��Ͳ����
	double m_width;                // ��Ͳ���
	double m_space;                // ���ڷ�Ͳ֮��ļ��
	double m_lineWidth; 
} ;

#ifdef DEFGEPLUGIN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SimpleChimneyDraw)
#endif
