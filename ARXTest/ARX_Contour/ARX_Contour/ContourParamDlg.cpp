#include "stdafx.h"
#include "ContourParamDlg.h"

#include "ContourParamHelper.h"
#include "../ArxHelper/HelperClass.h"
#include "../ContourGE/ContourLine.h"
#include "../ContourGE/ContourLabel.h"
#include "../ContourGE/ContourFill.h"

IMPLEMENT_DYNAMIC( ContourParamDlg, CDialog )

ContourParamDlg::ContourParamDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( ContourParamDlg::IDD, pParent )
    , m_textHeight( 0 )
    , m_bSmooth( FALSE )
{

}

ContourParamDlg::~ContourParamDlg()
{
}

void ContourParamDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Text( pDX, IDC_CONTOUR_TEXT_HEIGHT, m_textHeight );
    DDX_Check( pDX, IDC_CONTOUR_SMOOTH, m_bSmooth );
}


BEGIN_MESSAGE_MAP( ContourParamDlg, CDialog )
    ON_BN_CLICKED( IDOK, &ContourParamDlg::OnBnClickedOk )
END_MESSAGE_MAP()

BOOL ContourParamDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    bool bSmooth;
    ContourParamHelper::ReadParams( bSmooth, m_textHeight );
    m_bSmooth = ( bSmooth ? 1 : 0 );

    UpdateData( FALSE );

    return TRUE;
}

static void SmootContourLines( bool bSmooth )
{
    AcDbObjectIdArray objIds;
    ArxDataTool::GetEntsByType( _T( "ContourLine" ), objIds );
    if( objIds.isEmpty() ) return;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    for( int i = 0; i < objIds.length(); i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForWrite ) ) continue;

        ContourLine* pContour = ContourLine::cast( pObj );
        pContour->smooth( bSmooth );
    }
    actrTransactionManager->endTransaction();
}

static void SmootContourLabels( bool bSmooth )
{
    AcDbObjectIdArray objIds;
    ArxDataTool::GetEntsByType( _T( "ContourLabel" ), objIds );
    if( objIds.isEmpty() ) return;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    for( int i = 0; i < objIds.length(); i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForWrite ) ) continue;

        ContourLabel* pContour = ContourLabel::cast( pObj );
        pContour->smooth( bSmooth );
        pContour->update();
    }
    actrTransactionManager->endTransaction();
}

static void SmootContourFills( bool bSmooth )
{
    AcDbObjectIdArray objIds;
    ArxDataTool::GetEntsByType( _T( "ContourFill" ), objIds );
    if( objIds.isEmpty() ) return;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    for( int i = 0; i < objIds.length(); i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForWrite ) ) continue;

        ContourFill* pFill = ContourFill::cast( pObj );
        pFill->smooth( bSmooth );
    }
    actrTransactionManager->endTransaction();
}

static void SetContourLabelTextHeight( double textHeight )
{
    AcDbObjectIdArray objIds;
    ArxDataTool::GetEntsByType( _T( "ContourLabel" ), objIds );
    if( objIds.isEmpty() ) return;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    for( int i = 0; i < objIds.length(); i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForWrite ) ) continue;

        ContourLabel* pContour = ContourLabel::cast( pObj );
        pContour->setTextHeight( textHeight );
    }
    actrTransactionManager->endTransaction();
}

void ContourParamDlg::OnBnClickedOk()
{
    if( UpdateData( TRUE ) )
    {
        ContourParamHelper::WriteParams( m_bSmooth != 0, m_textHeight );

        SmootContourLines( m_bSmooth != 0 );
        SmootContourLabels( m_bSmooth != 0 );
        SetContourLabelTextHeight( m_textHeight );
        SmootContourFills( m_bSmooth != 0 );

        OnOK();
    }
}
