// ColorCombox.cpp : implementation file
//

#include "stdafx.h"

#include "ColorCombox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorCombox

CColorCombox::CColorCombox()
{

}

CColorCombox::~CColorCombox()
{

}

BEGIN_MESSAGE_MAP( CColorCombox, CComboBox )
    //{{AFX_MSG_MAP(CColorCombox)
    ON_CONTROL_REFLECT( CBN_SELENDOK, OnSelendok )
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorCombox message handlers

void CColorCombox::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
    //��֤�Ƿ�Ϊ��Ͽ�ؼ�
    ASSERT( lpDrawItemStruct->CtlType == ODT_COMBOBOX );
    CDC dc ;
    dc.Attach( lpDrawItemStruct->hDC );

    //��ȡ��Ŀ����
    CRect itemRC ( lpDrawItemStruct->rcItem );
    //������ʾ��ɫ������
    CRect clrRC = itemRC;
    //�����ı�����
    CRect textRC = itemRC;
    //��ȡϵͳ�ı���ɫ
    COLORREF clrText = GetSysColor( COLOR_WINDOWTEXT );
    //ѡ��ʱ���ı���ɫ
    COLORREF clrSelected = GetSysColor( COLOR_HIGHLIGHT );
    //��ȡ���ڱ�����ɫ
    COLORREF clrNormal = GetSysColor( COLOR_WINDOW );
    //��ȡ��ǰ��Ŀ����
    int nIndex = lpDrawItemStruct->itemID;
    //�ж���Ŀ״̬
    int nState = lpDrawItemStruct->itemState;
    if( nState & ODS_SELECTED )	//����ѡ��״̬
    {
        dc.SetTextColor( ( 0x00FFFFFF & ~( clrText ) ) );		//�ı���ɫȡ��
        dc.SetBkColor( clrSelected );						//�����ı�������ɫ
        dc.FillSolidRect( &clrRC, clrSelected );			//�����Ŀ����Ϊ����Ч��
    }
    else
    {
        dc.SetTextColor( clrText );						//�����������ı���ɫ
        dc.SetBkColor( clrNormal );						//�����������ı�������ɫ
        dc.FillSolidRect( &clrRC, clrNormal );
    }
    if( nState & ODS_FOCUS )								//�����Ŀ��ȡ���㣬���ƽ�������
    {
        dc.DrawFocusRect( &itemRC );
    }

    //�����ı�����
    int nclrWidth = itemRC.Width() / 4;
    textRC.left = nclrWidth + 55;

    //������ɫ��ʾ����
    clrRC.DeflateRect( 2, 2 );
    clrRC.right = nclrWidth + 50;


    //������ɫ�ı����������ɫ����
    if ( nIndex != -1 )	//��Ŀ��Ϊ��
    {
        //��ȡ��Ŀ��ɫ
        COLORREF clrItem = GetItemData( nIndex );
        dc.SetBkMode( TRANSPARENT );
        //��ȡ�ı�
        CString szText;
        GetLBText( nIndex, szText );
        //����ı�
        dc.DrawText( szText, textRC, DT_LEFT | DT_VCENTER | DT_SINGLELINE );
        dc.FillSolidRect( &clrRC, clrItem );
        //�����ɫ
        dc.FrameRect( &clrRC, &CBrush( RGB( 0, 0, 0 ) ) );
    }
    dc.Detach();
}

int CColorCombox::AddItem( LPCTSTR lpszText, COLORREF clrValue )
{
    int nIndex = AddString( lpszText );
    SetItemData( nIndex, clrValue );
    return nIndex;
}

COLORREF CColorCombox::GetCurColor()
{
    int nIndex = GetCurSel();
    if ( nIndex != -1 )
    {
        return GetItemData( nIndex );
    }
    else
        return -1;
}

void CColorCombox::OnSelendok()
{
    CString lpCaption;
    GetLBText( this->GetCurSel(), lpCaption );
    if( lpCaption.CompareNoCase( _T( "����" ) ) == 0 )
    {
        CColorDialog crDlg( RGB( 255, 0, 0 ), CC_FULLOPEN );
        if( crDlg.DoModal() == IDOK )
        {
            SetItemData( this->GetCurSel(), crDlg.GetColor() );
            Invalidate();
        }
    }
}