#include "stdafx.h"
#include "DrawManagerDlg.h"

#include "../MineGE/DrawHelper.h"
#include "../MineGEDraw/MineGEDrawSystem.h"

IMPLEMENT_DYNAMIC( DrawManagerDlg, CDialog )

DrawManagerDlg::DrawManagerDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( DrawManagerDlg::IDD, pParent )
{

}

DrawManagerDlg::~DrawManagerDlg()
{
}

void DrawManagerDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_TYPE_COMBO, m_typeCombo );
    DDX_Control( pDX, IDC_DRAW_COMBO, m_drawCombo );
}


BEGIN_MESSAGE_MAP( DrawManagerDlg, CDialog )
    ON_BN_CLICKED( IDC_CFG_BTN, &DrawManagerDlg::OnBnClickedCfgBtn )
    ON_CBN_SELCHANGE( IDC_TYPE_COMBO, &DrawManagerDlg::OnCbnSelchangeTypeCombo )
    ON_BN_CLICKED( IDC_SET_BTN, &DrawManagerDlg::OnBnClickedSetBtn )
END_MESSAGE_MAP()


// DrawManagerDlg ��Ϣ�������
BOOL DrawManagerDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    AcStringArray allGETypes;
    DrawHelper::GetAllRegGETypesForDraw( allGETypes );
    if( allGETypes.isEmpty() ) return TRUE;

    int len = allGETypes.length();
    for( int i = 0; i < len; i++ )
    {
        m_typeCombo.AddString( allGETypes[i].kACharPtr() );
    }
    m_typeCombo.SetCurSel( 0 ); // �趨��ǰѡ��Ϊ��һ����Ŀ

    // ����ѡ����µ�ǰdraw
    AcStringArray drawList;
    MineGEDrawSystem::GetInstance()->getAllGEDrawsByGEType( allGETypes[0].kACharPtr(), drawList );
    if( drawList.isEmpty() ) return TRUE;

    len = drawList.length();
    for( int i = 0; i < len; i++ )
    {
        m_drawCombo.AddString( drawList[i].kACharPtr() );
    }
    // ���õ�ǰ��ʾ��draw
    m_drawCombo.SetCurSel( findDrawIndex( allGETypes[0].kACharPtr(), drawList ) );

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void DrawManagerDlg::OnBnClickedCfgBtn()
{
    this->EnableWindow( FALSE );
    DrawHelper::ConfigDraw( getSelType(), getSelDraw() );
    this->EnableWindow( TRUE );
    //MessageBox(_T("���ӻ�Ч�������޸ĳɹ�"));
}

void DrawManagerDlg::OnCbnSelchangeTypeCombo()
{
    // ���draw��Ͽ����������
    clearDrawComboContent();

    CString selType = getSelType();
    AcStringArray drawList;
    MineGEDrawSystem::GetInstance()->getAllGEDrawsByGEType( selType, drawList );
    if( drawList.isEmpty() ) return;

    int len = drawList.length();
    for( int i = 0; i < len; i++ )
    {
        m_drawCombo.AddString( drawList[i].kACharPtr() );
    }
    // ���õ�ǰ��ʾ��draw
    m_drawCombo.SetCurSel( findDrawIndex( selType, drawList ) );
}

void DrawManagerDlg::clearDrawComboContent()
{
    m_drawCombo.ResetContent();
}

CString DrawManagerDlg::getSelType()
{
    int sel = m_typeCombo.GetCurSel();
    CString selType;
    m_typeCombo.GetLBText( sel, selType );
    return selType;
}

CString DrawManagerDlg::getSelDraw()
{
    int sel = m_drawCombo.GetCurSel();
    CString selDraw;
    m_drawCombo.GetLBText( sel, selDraw );
    return selDraw;
}

int DrawManagerDlg::findDrawIndex( const CString& geType, const AcStringArray& drawList )
{
    CString draw;
    DrawHelper::GetCurrentDraw( geType, draw );
    return drawList.find( draw );
}

void DrawManagerDlg::OnBnClickedSetBtn()
{
    this->EnableWindow( FALSE );
    DrawHelper::SwitchDraw( getSelType(), getSelDraw() );
    this->EnableWindow( TRUE );
    MessageBox( _T( "�л����ӻ�Ч���ɹ�" ) );
}
