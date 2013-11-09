#include "StdAfx.h"

#include "CADDrawHelper.h"
#include "LayerHelper.h"
#include "ConvexHullHelper.h"

// �����߿�
static void SeLineWidth_Helper( AcDbEntity* pEnt )
{
    if( pEnt->isKindOf( AcDbPolyline::desc() ) )
    {
        AcDbPolyline* pPline = AcDbPolyline::cast( pEnt );
        /*
        int count = pPline->numVerts();
        for(int i=0;i<count;i++)
        {
        	pPline->setWidthsAt(i, width, width);
        }
        */
        pPline->setConstantWidth( 0 );
    }
    pEnt->setLineWeight( AcDb::kLnWtByLwDefault ); // Ĭ��
    //pEnt->setLineWeight(kLnWtByLayer); // ���
    // ò��ʹ��0��Ч
}

static void PrintPt( long id, bool se, const AcGePoint3d& pt )
{
    /*if(IsInRange(pt))
    {*/
    acutPrintf( _T( "\nid=%ld, %s, %.15f, %.15f" ), id, ( se ? _T( "ʼ��" ) : _T( "ĩ��" ) ), pt.x, pt.y );
    //}
}

static void GetPts_Helper( AcDbEntity* pEnt, AcGePoint3dArray& pts )
{
    /*if(pEnt->isKindOf(AcDbPolyline::desc()))
    {
    	AcDbPolyline* pPline = AcDbPolyline::cast(pEnt);
    	int count = pPline->numVerts();
    	for(int i=0;i<count;i++)
    	{
    		AcGePoint3d pt;
    		pPline->getPointAt(i, pt);
    		if(!pts.contains(pt)) pts.append(pt);
    	}
    }
    else */if( pEnt->isKindOf( AcDbLine::desc() ) )
    {
        AcDbLine* pLine = AcDbLine::cast( pEnt );
        AcGePoint3d spt = pLine->startPoint();
        AcGePoint3d ept = pLine->endPoint();
        /*pLine->getStartPoint(spt);
        pLine->getEndPoint(ept);*/
        if( spt == ept ) return;

        long id = pLine->objectId().asOldId();
        /*double L = (spt - ept).length();

        if(L < 1)
        {
        	PrintPt(id, true, spt);
        	PrintPt(id, false, ept);
        	acutPrintf(_T("\n����:%.15f"), L);
        }*/

        //PrintPt(id, true, spt);
        //PrintPt(id, false, ept);

        if( !pts.contains( spt ) )
        {
            //if(L<1) acutPrintf(_T("\n��֧%dʼ����ӵ�����"), id);
            pts.append( spt );
        }
        else
        {
            //if(L<1) acutPrintf(_T("\nid=%ld, ��ʼ��%.15f, %.15f���ڼ�����"), id, spt.x, spt.y);
        }
        if( !pts.contains( ept ) )
        {
            //if(L<1) acutPrintf(_T("\nid=%dĩ����ӵ�����"), id);
            pts.append( ept );
        }
        else
        {
            //if(L<1) acutPrintf(_T("\nid=%ld��ĩ��%.15f, %.15f���ڼ�����"), id, ept.x, ept.y);
        }
    }
}

static void GetAllPtsOnLayer( AcGePoint3dArray& pts, const CString& layerName )
{
    AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();

    AcDbBlockTable* pBlkTbl;
    pDB->getSymbolTable( pBlkTbl, AcDb::kForRead );

    AcDbBlockTableRecord* pBlkTblRcd;
    pBlkTbl->getAt( ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead );
    pBlkTbl->close();

    AcDbBlockTableRecordIterator* pBlkTblRcdItr;
    pBlkTblRcd->newIterator( pBlkTblRcdItr );
    for ( pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step() )
    {
        AcDbEntity* pEnt;
        if( Acad::eOk != pBlkTblRcdItr->getEntity( pEnt, AcDb::kForRead ) ) continue;

        if( layerName.CompareNoCase( pEnt->layer() ) == 0 ) // ֻѡ��ָ��ͼ���ϵ�����ֱ��
        {
            // ��ȡ������
            GetPts_Helper( pEnt, pts );
        }
        pEnt->close();
    }
    delete pBlkTblRcdItr;

    pBlkTblRcd->close();

}

static void DeleteAllEntsOnLayer( const CString& layerName )
{
    AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();
    AcDbBlockTable* pBlkTbl;
    pDB->getSymbolTable( pBlkTbl, AcDb::kForRead );

    AcDbBlockTableRecord* pBlkTblRcd;
    pBlkTbl->getAt( ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead );
    pBlkTbl->close();

    AcDbBlockTableRecordIterator* pBlkTblRcdItr;
    pBlkTblRcd->newIterator( pBlkTblRcdItr );
    for ( pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step() )
    {
        AcDbEntity* pEnt;
        if( Acad::eOk != pBlkTblRcdItr->getEntity( pEnt, AcDb::kForWrite ) ) continue;

        if( layerName.CompareNoCase( pEnt->layer() ) == 0 )
        {
            pEnt->erase( true );
        }
        pEnt->close();
    }
    delete pBlkTblRcdItr;

    pBlkTblRcd->close();
}

static void ZeroLineWidth_Helper( const CString& layerName )
{
    AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();
    AcDbBlockTable* pBlkTbl;
    pDB->getSymbolTable( pBlkTbl, AcDb::kForRead );

    AcDbBlockTableRecord* pBlkTblRcd;
    pBlkTbl->getAt( ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead );
    pBlkTbl->close();

    AcDbBlockTableRecordIterator* pBlkTblRcdItr;
    pBlkTblRcd->newIterator( pBlkTblRcdItr );
    for ( pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step() )
    {
        AcDbEntity* pEnt;
        if( Acad::eOk != pBlkTblRcdItr->getEntity( pEnt, AcDb::kForWrite ) ) continue;

        if( layerName.CompareNoCase( pEnt->layer() ) == 0 ) // ֻѡ��ָ��ͼ��
        {
            //acutPrintf(_T("\n��%dͼԪ����ͼ��:%s"), i++, pEnt->layer());
            // ���ÿ��
            SeLineWidth_Helper( pEnt );
        }
        pEnt->close();
    }
    delete pBlkTblRcdItr;

    pBlkTblRcd->close();
}

static void ExecuteZoomCommand2( const AcGePoint3d& minPt, const AcGePoint3d& maxPt )
{
    /*
     * ���������(��)
     * ע��acedCommand������modeless dialog���޷�ʹ��
     * �ο�: "Code Differences under the Application Execution Context" topic in ARX docs
     */
    // ����cad���ZOOM-->W (���ŵ�ͼԪ�Ĵ��ڷ�Χ)
    acedCommand( RTSTR, _T( "ZOOM" ), RTSTR, _T( "W" ), RT3DPOINT, asDblArray( minPt ), RT3DPOINT, asDblArray( maxPt ), 0 );
    //acedCommand(RTSTR, _T("ZOOM"), RTSTR, _T("A"), 0);
}

static void ZoomWindow( const AcGePoint3d& minPt, const AcGePoint3d& maxPt, double radius )
{
    AcGeVector3d v = maxPt - minPt;
    v.normalize();
    double c = 1 + 0.618;

    ExecuteZoomCommand2( minPt - v * radius * c, maxPt + v * radius * c );
}

static void FixLinePoint( AcDbEntity* pEnt, const AcGePoint3d& pt1, const AcGePoint3d& pt2 )
{
    if( pEnt->isKindOf( AcDbLine::desc() ) )
    {
        AcDbLine* pLine = AcDbLine::cast( pEnt );
        AcGePoint3d spt, ept;
        pLine->getStartPoint( spt );
        pLine->getEndPoint( ept );

        if( ( pt1 != spt ) && ( pt2 == ept ) )
        {
            //acutPrintf(_T("\n�ҵ�ֱ�ߣ�ֱ�ߵ�ĩ�����p1"));
            pLine->setEndPoint( pt1 ); // ȫ������p1��
        }
        else if( ( pt2 == spt ) && ( pt1 != ept ) )
        {
            //acutPrintf(_T("\n�ҵ�ֱ�ߣ�ֱ�ߵ�ʼ�����p1"));
            pLine->setStartPoint( pt1 );
        }
    }
}

static void FixLinePoints_Helper( const CString& layerName, const AcGePoint3d& pt1, const AcGePoint3d& pt2 )
{
    AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();
    AcDbBlockTable* pBlkTbl;
    pDB->getSymbolTable( pBlkTbl, AcDb::kForRead );

    AcDbBlockTableRecord* pBlkTblRcd;
    pBlkTbl->getAt( ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead );
    pBlkTbl->close();

    AcDbBlockTableRecordIterator* pBlkTblRcdItr;
    pBlkTblRcd->newIterator( pBlkTblRcdItr );
    for ( pBlkTblRcdItr->start(); !pBlkTblRcdItr->done(); pBlkTblRcdItr->step() )
    {
        AcDbEntity* pEnt;
        if( Acad::eOk != pBlkTblRcdItr->getEntity( pEnt, AcDb::kForWrite ) ) continue;

        if( layerName.CompareNoCase( pEnt->layer() ) == 0 ) // ֻѡ��ָ��ͼ��
        {
            FixLinePoint( pEnt, pt1, pt2 );
        }
        pEnt->close();
    }
    delete pBlkTblRcdItr;

    pBlkTblRcd->close();
}

struct SEPoint
{
    bool operator==( const SEPoint& sept )
    {
        return ( ( sept.spt == spt && sept.ept == ept ) ||
                 ( sept.ept == spt && sept.spt == ept ) );
    }
    AcGePoint3d spt;
    AcGePoint3d ept;
};

static bool GetSEPt( AcDbEntity* pEnt, AcGePoint3d& spt, AcGePoint3d& ept )
{
    if( !pEnt->isKindOf( AcDbLine::desc() ) ) return false;
    AcDbLine* pLine = AcDbLine::cast( pEnt );
    pLine->getStartPoint( spt );
    pLine->getEndPoint( ept );
    return true;
}

// �����غ�ֱ��
static bool FindSuperpositionLine_Helper( const CString& layerName, AcGePoint3d& spt, AcGePoint3d& ept )
{
    AcArray<SEPoint> ses;

    AcDbDatabase* pDB = acdbHostApplicationServices()->workingDatabase();
    AcDbBlockTable* pBlkTbl;
    pDB->getSymbolTable( pBlkTbl, AcDb::kForRead );

    AcDbBlockTableRecord* pBlkTblRcd;
    pBlkTbl->getAt( ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead );
    pBlkTbl->close();

    bool ret = false;
    AcDbBlockTableRecordIterator* pBlkTblRcdItr;
    pBlkTblRcd->newIterator( pBlkTblRcdItr );
    for ( pBlkTblRcdItr->start(); !pBlkTblRcdItr->done() && !ret; pBlkTblRcdItr->step() )
    {
        AcDbEntity* pEnt;
        if( Acad::eOk != pBlkTblRcdItr->getEntity( pEnt, AcDb::kForWrite ) ) continue;

        if( layerName.CompareNoCase( pEnt->layer() ) == 0 ) // ֻѡ��ָ��ͼ��
        {
            if( GetSEPt( pEnt, spt, ept ) )
            {
                SEPoint se;
                se.spt = spt;
                se.ept = ept;
                if( ses.contains( se ) )
                {
                    ret = true;
                }
                else
                {
                    ses.append( se );
                }
            }
        }
        pEnt->close();
    }
    delete pBlkTblRcdItr;

    pBlkTblRcd->close();

    return ret;
}

void ZeroLineWidth()
{
    CString srcLayer = _T( "���" ); // Դͼ��
    CString msg;
    msg.Format( _T( "\n�����%s��ͼ���ϵ����ж���ߺ�ֱ�ߵ��߿�..." ), srcLayer );
//	acutPrintf(_T("\n�����%s��ͼ���ϵ����ж���ߺ�ֱ�ߵ��߿�..."), srcLayer);
    AfxMessageBox( msg );
    ZeroLineWidth_Helper( srcLayer );
}

void DrawLinePoints()
{
    // ��ȡ��ǰͼ������
    CString curLayer = LayerHelper::GetCurrentLayerName();
    acutPrintf( _T( "\n��ǰͼ��:��%s��" ), curLayer );

    CString srcLayer = _T( "���" ); // Դͼ��
    acutPrintf( _T( "\n��ȡ��%s��ͼ���ϵ�����ֱ�ߵĵ�����" ), srcLayer );
    AcGePoint3dArray pts;
    GetAllPtsOnLayer( pts, srcLayer );
    if( pts.isEmpty() ) return;

    CString targetLayer = _T( "��ʱ" );
    acutPrintf( _T( "\n�½���%s��ͼ�㣻���ͼ����ڣ���ɾ��ͼ���ϵ�����ͼ��..." ), targetLayer );
    LayerHelper::AddLayer( targetLayer ); // ��������򲻽���
    DeleteAllEntsOnLayer( targetLayer );

    // ���½�ͼ������Ϊ��ǰͼ��
    LayerHelper::SetCurrentLayer( targetLayer );

    double radius = 10;
    int len = pts.length();
    for( int i = 0; i < len; i++ )
    {
        DrawEllipse( pts[i], radius, radius );
    }
    LayerHelper::SetCurrentLayer( curLayer );

    AcGePoint3d pt1, pt2;
    ClosestPoint_3D( pts, pt1, pt2 );

    acutPrintf( _T( "\n����������:(%.15f, %.15f)-->(%.15f, %.15f)" ), pt1.x, pt1.y, pt2.x, pt2.y );
    acutPrintf( _T( "\n�����Ծ���:%.15f\n" ), ( pt2 - pt1 ).length() );

    ZoomWindow( pt1, pt2, radius );
}

void FixLinePoints()
{
    CString srcLayer = _T( "���" ); // Դͼ��
    acutPrintf( _T( "\n��ȡ��%s��ͼ���ϵ�����ֱ�ߵĵ�����" ), srcLayer );
    AcGePoint3dArray pts;
    GetAllPtsOnLayer( pts, srcLayer );
    if( pts.isEmpty() ) return;

    AcGePoint3d pt1, pt2;
    ClosestPoint_3D( pts, pt1, pt2 );

    ZoomWindow( pt1, pt2, 10 );

    AcGeVector3d v = pt2 - pt1;
    CString msg;
    msg.Format( _T( "�����Ծ���:%.15f\n�Ƿ�����???" ), v.length() );
    if( IDYES == AfxMessageBox( msg, MB_YESNO ) )
    {
        FixLinePoints_Helper( srcLayer, pt1, pt2 );
    }
}

void FindSuperpositionLine()
{
    CString srcLayer = _T( "���" ); // Դͼ��
    acutPrintf( _T( "\n��ȡ��%s��ͼ���ϵ�����ֱ�ߵĵ�����" ), srcLayer );

    AcGePoint3d spt, ept;
    if( FindSuperpositionLine_Helper( srcLayer, spt, ept ) )
    {
        acutPrintf( _T( "\n�غ�ֱ�߶˵�����:(%.15f, %.15f)-->(%.15f, %.15f)\n" ), spt.x, spt.y, ept.x, ept.y );
        ZoomWindow( spt, ept, 10 );
    }
    else
    {
        CString msg;
        msg.Format( _T( "��%s��ͼ����û���غϵ�ֱ��" ), srcLayer );
        AfxMessageBox( msg );
    }
}
