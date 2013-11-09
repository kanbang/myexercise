#include "stdafx.h"

#include "DoubleLine.h"

#include "../MineGE/LinkedGE.h"
#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

#include "GambitParamHelper.h"

#include <stdlib.h>
#include <time.h>

static void GetEdgeSEPts( const AcDbObjectIdArray& objIds, AcGePoint3dArray& spts, AcGePoint3dArray& epts )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) ) continue;

        LinkedGE* pEdge = LinkedGE::cast( pObj );
        if( pEdge == 0 ) continue;

        AcGePoint3d spt, ept;
        pEdge->getSEPoint( spt, ept );

        spts.append( spt );
        epts.append( ept );
    }
    actrTransactionManager->endTransaction();
}

static void GetEdgeWidths( const AcDbObjectIdArray& objIds, AcGeDoubleArray& widths )
{
    //srand ( time(NULL) );

    GambitParam gp;
    GambitParamHelper::ReadGambitParam( gp );

//	double width = 8;
    double width = gp.width;

    // 后续再修改
    // 考虑在属性数据中增加字段"巷道宽度", 读取该数据作为巷道实际宽度
    int n = objIds.length();
    for( int i = 0; i < n; i++ )
    {
        //double w = 20 + rand()%20;
        double w = width;
        widths.append( w );
    }

    //widths[0] = 100;
    //widths[1] = 40;
}

void BuildLineArray( const AcDbObjectIdArray& objIds, AcDbVoidPtrArray& lines )
{
    AcGePoint3dArray spts, epts;
    GetEdgeSEPts( objIds, spts, epts );

    AcGeDoubleArray widths;
    GetEdgeWidths( objIds, widths );

    // 获取一些属性数据,例如风速等
    // 后续添加
    // ...

    // 创建DoubleLine数组
    int n = spts.length();
    for( int i = 0; i < n; i++ )
    {
        bool isWorkSurface = ArxUtilHelper::IsEqualType( _T( "WorkSurface" ), objIds[i] );
        DoubleLine* pLine = new DoubleLine( spts[i], epts[i], widths[i], isWorkSurface );
        lines.append( ( void* )pLine );
    }
}