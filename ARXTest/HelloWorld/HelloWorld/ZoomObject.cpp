#include "stdafx.h"

/* ȫ�ֺ���(ʵ����ZoomHelper.cpp��) */
extern void ExecuteZoomCommand1( const AcGePoint3d& minPt, const AcGePoint3d& maxPt );
extern void ExecuteZoomCommand2( const AcGePoint3d& minPt, const AcGePoint3d& maxPt );
//extern void ZommWindowUseCom(const AcGePoint3d& minPt, const AcGePoint3d& maxPt);
extern void ZoomWindowUseView( const AcGePoint3d& minPt, const AcGePoint3d& maxPt );
// ��Ч
extern void ZoomWindowUseGSView( const AcGePoint3d& minPt, const AcGePoint3d& maxPt );

static bool IsValidExtent( const AcDbExtents& ext )
{
    AcGeVector3d v = ext.maxPoint() - ext.minPoint();
    //acutPrintf(_T("\nextents:x=%.3f,%y=%.3f,z=%.3f\n"), v.x, v.y, v.z);
    return ( v.x >= 0 && v.y >= 0 && v.z >= 0 ); // x,y,z�Ĳ�ֵ����>=0
}

static void ZoomEntity_Helper( const AcGePoint3d& minPt, const AcGePoint3d& maxPt )
{
    //acutPrintf(_T("\n��С��:(%.3f,%.3f,%.3f)\t����:(%.3f,%.3f,%.3f)\n"), minPt.x, minPt.y, minPt.z, maxPt.x, maxPt.y, maxPt.z);

    AcGeVector3d v = maxPt - minPt;

    // ���ŶԽ�����������Ŵ�(��С���С��������)
    double c = 0.618; // �ƽ����ֵ

    // 1) ʹ��sendStringToExecute����
    // ȱ��:����������ʾһЩ��ʾ�ַ������е㷳��(����: "���zoom w ָ����һ��λ�� ... ")
    //ExecuteZoomCommand1(minPt-c*v, maxPt+c*v);

    // 2) ʹ��acedCommand����
    // ȱ��: ��modelss dialog���޷�ʹ��(�����˵����application context���޷�ִ��)
    // �μ���arxdoc.chm->Advanced Topics->The Multiple Document Interface
    //        ->Application Execution Context
    //        ->Code Differences under the Application Execution Context
    //ExecuteZoomCommand2(minPt-c*v, maxPt+c*v);

    // 3) ʹ��com
    // ȱ��: ��ʾһЩ�յ�������ʾ(����"����: ")
    //ZommWindowUseCom(minPt-c*v, maxPt+c*v);

    // 4) ʹ��view
    // ȱ�㣺��������ŵ�ʱ�򣬶�ͼԪ�������޸ģ������޸���ɫ�����������ϸ���
    //        ֻ�е�ǰ������cad�Ļ�ͼ����ʱ���Ż����
    // �μ���<<ObjectARX����ʵ���̳�-20090826>>�е�"4.4 ��ͼ"С��
    ZoomWindowUseView( minPt - c * v, maxPt + c * v );
    acedGetAcadFrame()->SetFocus(); // �л�����(���ͼ���޸ĵ�����)

    // 5) ʹ��AcGsView(ʧ��)
    // AcGsViewֻ������3dģʽ
    //ZoomWindowUseGSView(minPt-c*v, maxPt+c*v);
}

void ZoomToEntity( const AcDbObjectId& objId )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForRead ) )
    {
        actrTransactionManager->abortTransaction();
        return;
    }

    //AcApDocument
    //AcApDocManager
    //AcDbBlockTableRecord
    AcDbEntity* pEnt = AcDbEntity::cast( pObj );
    if( pEnt == 0 )
    {
        actrTransactionManager->abortTransaction();
        return;
    }

    AcDbExtents extents;
    bool ret = ( Acad::eOk == pEnt->getGeomExtents( extents ) );

    actrTransactionManager->endTransaction();

    if( !ret )
    {
        acutPrintf( _T( "\n1) exten��Ч" ) );
    }
    else if( !IsValidExtent( extents ) )
    {
        acutPrintf( _T( "\n2) exten��Ч" ) );
    }
    else
    {
        ZoomEntity_Helper( extents.minPoint(), extents.maxPoint() );
    }
}