#include "stdafx.h"
#include "TestDlg4.h"

#include <fstream>
// 需要包含<fstream>才能使用
#include "AcFStream.h"

IMPLEMENT_DYNAMIC( TestDlg4, CDialog )

TestDlg4::TestDlg4( CWnd* pParent /*=NULL*/ )
    : CDialog( TestDlg4::IDD, pParent )
{

}

TestDlg4::~TestDlg4()
{
}

void TestDlg4::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_LIST1, m_list );
}


BEGIN_MESSAGE_MAP( TestDlg4, CDialog )
END_MESSAGE_MAP()


// TestDlg4 消息处理程序

static void BuildHeadColumns( CListCtrl& m_list, const AcStringArray fields )
{
    int len = fields.length();

    CRect rect;
    m_list.GetClientRect( &rect );

    int n = len;
    if( n > 10 ) n = 10;
    double dw = rect.Width() / ( n + 1 );

    m_list.InsertColumn( 0, _T( "编号" ), LVCFMT_LEFT, dw );
    for( int i = 0; i < len; i++ )
    {
        m_list.InsertColumn( i + 1, fields[i].kACharPtr(), LVCFMT_LEFT, dw );
    }
}
static void SetItem( CListCtrl& m_list, int k, const AcStringArray& data )
{
    int n = data.length();
    for( int i = 0; i < n; i++ )
    {
        m_list.SetItemText( k, i + 1, data[i].kACharPtr() );
    }
}

static void AddItem( CListCtrl& m_list, int k, const AcStringArray& data )
{
    // 插入新的数据
    m_list.InsertItem( k, _T( "xx" ) );

    // 增加编号
    CString num;
    num.Format( _T( "%d" ), k + 1 );
    m_list.SetItemText( k, 0, num );

    // 修改数据
    SetItem( m_list, k, data );
}

static void FillDatas( CListCtrl& m_list, int n, const AcStringArray& datas )
{
    //首先删除所有行
    m_list.DeleteAllItems();

    int m = datas.length() / n;
    if( datas.length() - m * n > 0 ) m--;

    for( int i = 0; i < m; i++ )
    {
        AcStringArray item_datas;

        for( int j = n * i; j < n * ( i + 1 ); j++ )
        {
            item_datas.append( datas[j] );
        }

        AddItem( m_list, i, item_datas );
    }
}

static void ReadDataFromFile( const CString& filepath, AcStringArray& fields, AcStringArray& datas )
{
    AcIfstream inFile( filepath );
    if( !inFile ) return;

    // 读取列数和行数
    int n = 0, m = 0;
    inFile >> n >> m;

    // 读取字段
    for( int i = 0; i < n; i++ )
    {
        TCHAR str[100];
        inFile >> str;
        fields.append( str );
    }

    // 读取数据
    for( int j = 0; j < m; j++ )
    {
        for( int i = 0; i < n; i++ )
        {
            TCHAR str[100];
            inFile >> str;
            datas.append( str );
        }
    }
    inFile.close();
}

BOOL TestDlg4::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化

    CString filepath = _T( "D:\\mvssdata.txt" );
    AcStringArray fields;
    AcStringArray datas;
    ReadDataFromFile( filepath, fields, datas );

    BuildHeadColumns( m_list, fields );

    FillDatas( m_list, fields.length(), datas );

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}
