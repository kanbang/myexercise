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
    acutPrintf( _T( "\n����..." ) );
}

DLJJoint::~DLJJoint ()
{
    acutPrintf( _T( "\n����..." ) );
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
     * ����Ĵ��������Զ���ӵ�
     * ������Ҫ���û���AcDbEntity�����л�����
     * Ȼ���ٱ�д���Զ���ʵ����ص����ݲ���
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

// ��������--����Բ
static void DrawCircle( AcGiWorldDraw* mode, const AcGePoint3d& insertPt, double radius )
{
    mode->geometry().circle( insertPt, radius, AcGeVector3d::kZAxis );
}

#define PI 3.1415926535897932384626433832795

// ��������--����ʮ��
static void DrawCross( AcGiWorldDraw* mode, const AcGePoint3d& insertPt, double radius )
{
    // ����һ��x��ı�׼����(1,0,0)
    AcGeVector3d v( AcGeVector3d::kXAxis );
    // �������ȷŴ�(����С)
    v *= radius;

    for( int i = 0; i < 4; i++ )
    {
        AcGePoint3dArray pts;
        pts.append( insertPt );
        pts.append( insertPt + v );

        // ����õ��������꣬����һ��ֱ��
        // ����MFC��lineTo()����
        mode->geometry().worldLine( pts.asArrayPtr() );

        // ��ʱ����ת90��
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
    // 1) ����Բ
    DrawCircle( mode, m_pt, m_radius );

    // 2) ����ʮ�֣�������4���н�Ϊ90���ֱ��
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
//	 * ��ִ��cad�����ƶ�����ת������ʱ��cad�ᴫ��һ���任����xform
//	 * ͼ�α任��ʵ�ʾ������꣬����ͨ���任����xform���м���
//	 * �õ��任����������Լ�������
//	 * ע����ο������ͼ��ѧ������
//	 */
//
//	/*
//	 * ��򵥵ķ������ǹ���һ��cad���õ�ͼ��ʵ��ִ��transform����
//	 * Ȼ���ȡ���º�Ĳ���
//	 */
//	// 1) ����һ��Բ
//	AcDbCircle circle(m_pt, AcGeVector3d::kZAxis, m_radius);
//	// 2) Բ����transformBy()�������б任
//	circle.transformBy(xform);
//	//3) ��ȡ���º�Ĳ���
//	m_pt = circle.center();     // ��ȡ�任���Բ������
//	m_radius = circle.radius(); // ��ȡ�任���Բ�뾶
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
//	 * cad�Ĳ�׽���ж������ͣ�
//	 * ��ǰ�Ŀ��ӻ�Ч��ֻ����"Բ��"��׽��
//	 * ������ƶ���ͼ�θ�����ֻ��"Բ��"��׽�����Ӧ
//	 */
//	if(osnapMode == AcDb::kOsModeCen) // ֻ��׽Բ��
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
//	 * ѡ��ͼ�Σ���ʾ�е�
//	 * ��ʾ����ʾ2���е㣬�����ƶ�ͼ�κͷŴ�ͼ��
//	 */
//	gripPoints.append(m_pt);                  // Բ����Ϊ�е�
//
//	AcGeVector3d v(AcGeVector3d::kXAxis);
//	v *= m_radius;
//
//	gripPoints.append(m_pt + v);             // +x��Ķ˵���Ϊ�е�
//
//	//v.rotateBy(PI/2, AcGeVector3d::kZAxis);  // ��ʱ����ת90��
//	//gripPoints.append(m_pt + v);             // +y��Ķ˵���Ϊ�е�
//
//	//v.rotateBy(PI/2, AcGeVector3d::kZAxis);  // ��ʱ����ת90��
//	//gripPoints.append(m_pt + v);             // -x��Ķ˵���Ϊ�е�
//
//	//v.rotateBy(PI/2, AcGeVector3d::kZAxis);  // ��ʱ����ת90��
//	//gripPoints.append(m_pt + v);              // -y��Ķ˵���Ϊ�е�
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
//	// �е㰴��subGetGripPoints()�����ж����˳������
//	int len = indices.length();
//	for(int i=0;i<len;i++)
//	{
//		int idx = indices.at(i);
//		if(idx == 0)
//		{
//			// ��ǰ�е���Բ�ģ��ƶ�ͼ��
//			m_pt += offset;       // ��Բ��ִ��ƫ�Ʊ任
//		}
//		if(idx == 1)
//		{
//			// ����Բ
//			// 1) ����x��Ķ˵�����
//			AcGeVector3d v(AcGeVector3d::kXAxis);
//			AcGePoint3d pt = m_pt + v*m_radius;
//
//			// 2) ��������ƫ�Ƽ���
//			pt += offset;
//
//			// 3) ������������Բ��֮��ĳ��ȣ�����ΪԲ�뾶
//			// ����������õ�һ��������Ȼ��õ���������
//			m_radius = (pt - m_pt).length();
//		}
//	}
//
//	return (Acad::eOk) ;
//}
