#include "StdAfx.h"
#include "DLJJoint.h"

Adesk::UInt32 DLJJoint::kCurrentVersionNumber = 1 ;

ACRX_DXF_DEFINE_MEMBERS (
    DLJJoint, AcDbEntity,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation, DLJJOINT,
    HELLOWORLDAPP
    | Product Desc:     A description for your object
    | Company:          Your company name
    | WEB Address:      Your company WEB site address
)

    DLJJoint::DLJJoint () : AcDbEntity ()
{
    acutPrintf( _T( "\n构造..." ) );
}

DLJJoint::~DLJJoint ()
{
    acutPrintf( _T( "\n析构..." ) );
}

//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus DLJJoint::dwgOutFields ( AcDbDwgFiler* pFiler ) const
{
    assertReadEnabled () ;
    //----- Save parent class information first.
    Acad::ErrorStatus es = AcDbEntity::dwgOutFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be saved first
    if ( ( es = pFiler->writeUInt32 ( DLJJoint::kCurrentVersionNumber ) ) != Acad::eOk )
        return ( es ) ;

    acutPrintf( _T( "\nDLJJoint::dwgOutFields()..." ) );
    /*
     * 上面的代码是向导自动添加的
     * 首先需要调用基类AcDbEntity的序列化函数
     * 然后再编写与自定义实体相关的数据操作
     */
    pFiler->writeItem( m_pt );
    pFiler->writeItem( m_radius );

    return ( pFiler->filerStatus () ) ;
}

Acad::ErrorStatus DLJJoint::dwgInFields ( AcDbDwgFiler* pFiler )
{
    assertWriteEnabled () ;
    //----- Read parent class information first.
    Acad::ErrorStatus es = AcDbEntity::dwgInFields ( pFiler ) ;
    if ( es != Acad::eOk )
        return ( es ) ;
    //----- Object version number needs to be read first
    Adesk::UInt32 version = 0 ;
    if ( ( es = pFiler->readUInt32 ( &version ) ) != Acad::eOk )
        return ( es ) ;
    if ( version > DLJJoint::kCurrentVersionNumber )
        return ( Acad::eMakeMeProxy ) ;
    //- Uncomment the 2 following lines if your current object implementation cannot
    //- support previous version of that object.
    //if ( version < DLJJoint::kCurrentVersionNumber )
    //	return (Acad::eMakeMeProxy) ;

    acutPrintf( _T( "\nDLJJoint::dwgInFields()..." ) );
    pFiler->readItem( &m_pt );
    pFiler->readItem( &m_radius );

    return ( pFiler->filerStatus () ) ;
}

// 辅助方法--绘制圆
static void DrawCircle( AcGiWorldDraw* mode, const AcGePoint3d& insertPt, double radius )
{
    mode->geometry().circle( insertPt, radius, AcGeVector3d::kZAxis );
}

#define PI 3.1415926535897932384626433832795

// 辅助方法--绘制十字
static void DrawCross( AcGiWorldDraw* mode, const AcGePoint3d& insertPt, double radius )
{
    // 定义一个x轴的标准向量(1,0,0)
    AcGeVector3d v( AcGeVector3d::kXAxis );
    // 向量长度放大(或缩小)
    v *= radius;

    for( int i = 0; i < 4; i++ )
    {
        AcGePoint3dArray pts;
        pts.append( insertPt );
        pts.append( insertPt + v );

        // 计算得到两点坐标，绘制一条直线
        // 类似MFC的lineTo()方法
        mode->geometry().worldLine( pts.asArrayPtr() );

        // 逆时针旋转90°
        v.rotateBy( PI / 2, AcGeVector3d::kZAxis );
    }
}

//AcRxObject* DLJJoint::clone() const
//{
//	//acutPrintf(_T("\nid:%d clone()..."), objectId());
//	return AcDbEntity::clone();
//}

//-----------------------------------------------------------------------------
//----- AcDbEntity protocols
Adesk::Boolean DLJJoint::subWorldDraw ( AcGiWorldDraw* mode )
{
    assertReadEnabled () ;

    acutPrintf( _T( "\nid:%d subWorldDraw..." ), objectId() );
    AcDbHandle h = objectId().handle();
    // 1) 绘制圆
    DrawCircle( mode, m_pt, m_radius );

    // 2) 绘制十字，即绘制4条夹角为90°的直线
    DrawCross( mode, m_pt, m_radius );

    //return (AcDbEntity::subWorldDraw (mode)) ;
    return Adesk::kTrue;
}


//Acad::ErrorStatus DLJJoint::subTransformBy(const AcGeMatrix3d & xform)
//{
//	assertWriteEnabled () ;
//
//	acutPrintf(_T("\nid:%d subTransformBy..."),objectId());
//	//Acad::ErrorStatus retCode =AcDbEntity::subTransformBy (xform) ;
//	/*
//	 * 当执行cad命令移动、旋转、缩放时，cad会传入一个变换矩阵xform
//	 * 图形变换的实质就是坐标，向量通过变换矩阵xform进行计算
//	 * 得到变换后的新坐标以及新向量
//	 * 注：请参考计算机图形学的资料
//	 */
//
//	/*
//	 * 最简单的方法就是构造一个cad内置的图形实体执行transform操作
//	 * 然后获取更新后的参数
//	 */
//	// 1) 构造一个圆
//	AcDbCircle circle(m_pt, AcGeVector3d::kZAxis, m_radius);
//	// 2) 圆调用transformBy()方法进行变换
//	circle.transformBy(xform);
//	//3) 获取更新后的参数
//	m_pt = circle.center();     // 获取变换后的圆心坐标
//	m_radius = circle.radius(); // 获取变换后的圆半径
//
//	return (Acad::eOk) ;
//}
//
//Acad::ErrorStatus DLJJoint::subGetOsnapPoints(AcDb::OsnapMode osnapMode, Adesk::GsMarker gsSelectionMark, const AcGePoint3d & pickPoint, const AcGePoint3d & lastPoint, const AcGeMatrix3d & viewXform, AcGePoint3dArray & snapPoints, AcDbIntArray & geomIds) const
//{
//	assertReadEnabled();
//
//	//Acad::ErrorStatus retCode =AcDbEntity::subGetOsnapPoints (osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds) ;
//
//	/*
//	 * cad的捕捉点有多种类型，
//	 * 当前的可视化效果只考虑"圆心"捕捉点
//	 * 即鼠标移动到图形附近，只有"圆心"捕捉点会响应
//	 */
//	if(osnapMode == AcDb::kOsModeCen) // 只捕捉圆心
//	{
//		snapPoints.append(m_pt);
//	}
//
//	return (Acad::eOk) ;
//}
//
//Acad::ErrorStatus DLJJoint::subGetGripPoints(AcGePoint3dArray & gripPoints, AcDbIntArray & osnapModes, AcDbIntArray & geomIds) const
//{
//	assertReadEnabled();
//
//	//Acad::ErrorStatus retCode =AcDbEntity::subGetGripPoints (gripPoints, osnapModes, geomIds) ;
//	/*
//	 * 选中图形，显示夹点
//	 * 本示例显示2个夹点，用于移动图形和放大图形
//	 */
//	gripPoints.append(m_pt);                  // 圆心作为夹点
//
//	AcGeVector3d v(AcGeVector3d::kXAxis);
//	v *= m_radius;
//
//	gripPoints.append(m_pt + v);             // +x轴的端点作为夹点
//
//	//v.rotateBy(PI/2, AcGeVector3d::kZAxis);  // 逆时针旋转90°
//	//gripPoints.append(m_pt + v);             // +y轴的端点作为夹点
//
//	//v.rotateBy(PI/2, AcGeVector3d::kZAxis);  // 逆时针旋转90°
//	//gripPoints.append(m_pt + v);             // -x轴的端点作为夹点
//
//	//v.rotateBy(PI/2, AcGeVector3d::kZAxis);  // 逆时针旋转90°
//	//gripPoints.append(m_pt + v);              // -y轴的端点作为夹点
//
//	return (Acad::eOk) ;
//}
//
//Acad::ErrorStatus DLJJoint::subMoveGripPointsAt(const AcDbIntArray & indices, const AcGeVector3d & offset)
//{
//	assertWriteEnabled () ;
//
//	//Acad::ErrorStatus retCode =AcDbEntity::subMoveGripPointsAt (indices, offset) ;
//
//	// 夹点按照subGetGripPoints()方法中定义的顺序排列
//	int len = indices.length();
//	for(int i=0;i<len;i++)
//	{
//		int idx = indices.at(i);
//		if(idx == 0)
//		{
//			// 当前夹点是圆心，移动图形
//			m_pt += offset;       // 对圆心执行偏移变换
//		}
//		if(idx == 1)
//		{
//			// 缩放圆
//			// 1) 计算x轴的端点坐标
//			AcGeVector3d v(AcGeVector3d::kXAxis);
//			AcGePoint3d pt = m_pt + v*m_radius;
//
//			// 2) 进行坐标偏移计算
//			pt += offset;
//
//			// 3) 计算新坐标与圆心之间的长度，并作为圆半径
//			// 坐标相减，得到一个向量，然后得到向量长度
//			m_radius = (pt - m_pt).length();
//		}
//	}
//
//	return (Acad::eOk) ;
//}
