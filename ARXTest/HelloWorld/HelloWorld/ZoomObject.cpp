#include "stdafx.h"

/* 全局函数(实现在ZoomHelper.cpp中) */
extern void ExecuteZoomCommand1( const AcGePoint3d& minPt, const AcGePoint3d& maxPt );
extern void ExecuteZoomCommand2( const AcGePoint3d& minPt, const AcGePoint3d& maxPt );
//extern void ZommWindowUseCom(const AcGePoint3d& minPt, const AcGePoint3d& maxPt);
extern void ZoomWindowUseView( const AcGePoint3d& minPt, const AcGePoint3d& maxPt );
// 无效
extern void ZoomWindowUseGSView( const AcGePoint3d& minPt, const AcGePoint3d& maxPt );

static bool IsValidExtent( const AcDbExtents& ext )
{
    AcGeVector3d v = ext.maxPoint() - ext.minPoint();
    //acutPrintf(_T("\nextents:x=%.3f,%y=%.3f,z=%.3f\n"), v.x, v.y, v.z);
    return ( v.x >= 0 && v.y >= 0 && v.z >= 0 ); // x,y,z的差值必须>=0
}

static void ZoomEntity_Helper( const AcGePoint3d& minPt, const AcGePoint3d& maxPt )
{
    //acutPrintf(_T("\n最小点:(%.3f,%.3f,%.3f)\t最大点:(%.3f,%.3f,%.3f)\n"), minPt.x, minPt.y, minPt.z, maxPt.x, maxPt.y, maxPt.z);

    AcGeVector3d v = maxPt - minPt;

    // 沿着对角线由内向外放大(最小点变小，最大点变大)
    double c = 0.618; // 黄金比例值

    // 1) 使用sendStringToExecute方法
    // 缺点:在命令行显示一些提示字符串，有点烦人(例如: "命令：zoom w 指定第一个位置 ... ")
    //ExecuteZoomCommand1(minPt-c*v, maxPt+c*v);

    // 2) 使用acedCommand方法
    // 缺点: 在modelss dialog中无法使用(具体的说是在application context下无法执行)
    // 参见：arxdoc.chm->Advanced Topics->The Multiple Document Interface
    //        ->Application Execution Context
    //        ->Code Differences under the Application Execution Context
    //ExecuteZoomCommand2(minPt-c*v, maxPt+c*v);

    // 3) 使用com
    // 缺点: 显示一些空的命令提示(例如"命令: ")
    //ZommWindowUseCom(minPt-c*v, maxPt+c*v);

    // 4) 使用view
    // 缺点：如果在缩放的时候，对图元进行了修改，例如修改颜色，并不会马上更新
    //        只有当前焦点在cad的绘图窗口时，才会更新
    // 参见：<<ObjectARX开发实例教程-20090826>>中的"4.4 视图"小节
    ZoomWindowUseView( minPt - c * v, maxPt + c * v );
    acedGetAcadFrame()->SetFocus(); // 切换焦点(解决图形修改的问题)

    // 5) 使用AcGsView(失败)
    // AcGsView只能用于3d模式
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
        acutPrintf( _T( "\n1) exten无效" ) );
    }
    else if( !IsValidExtent( extents ) )
    {
        acutPrintf( _T( "\n2) exten无效" ) );
    }
    else
    {
        ZoomEntity_Helper( extents.minPoint(), extents.maxPoint() );
    }
}