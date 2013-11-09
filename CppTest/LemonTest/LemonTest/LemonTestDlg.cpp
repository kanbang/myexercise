
// LemonTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LemonTest.h"
#include "LemonTestDlg.h"

#undef OUT
#undef IN

#include <lemon/list_graph.h>
#include <lemon/adaptors.h>
#include <lemon/dfs.h>
#include <lemon/bfs.h>
#include <lemon/dijkstra.h>
#include <lemon/connectivity.h>

using namespace lemon;
typedef ListDigraph Digraph;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// �Ի�������
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange( CDataExchange* pDX );  // DDX/DDV ֧��

// ʵ��
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog( CAboutDlg::IDD )
{
}

void CAboutDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
}

BEGIN_MESSAGE_MAP( CAboutDlg, CDialog )
END_MESSAGE_MAP()


// CLemonTestDlg �Ի���




CLemonTestDlg::CLemonTestDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( CLemonTestDlg::IDD, pParent )
    , m_xx( 0 )
{
    m_hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );
}

void CLemonTestDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Text( pDX, IDC_EDIT1, m_xx );
}

BEGIN_MESSAGE_MAP( CLemonTestDlg, CDialog )
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED( IDC_BUTTON1, &CLemonTestDlg::OnBnClickedButton1 )
    ON_BN_CLICKED( IDC_BUTTON2, &CLemonTestDlg::OnBnClickedButton2 )
    ON_BN_CLICKED( IDC_BUTTON3, &CLemonTestDlg::OnBnClickedButton3 )
    ON_BN_CLICKED( IDC_BUTTON4, &CLemonTestDlg::OnBnClickedButton4 )
    ON_BN_CLICKED( IDC_BUTTON5, &CLemonTestDlg::OnBnClickedButton5 )
    ON_BN_CLICKED( IDC_BUTTON6, &CLemonTestDlg::OnBnClickedButton6 )
    ON_BN_CLICKED( IDC_BUTTON7, &CLemonTestDlg::OnBnClickedButton7 )
    ON_BN_CLICKED( IDC_BUTTON8, &CLemonTestDlg::OnBnClickedButton8 )
    ON_BN_CLICKED( IDC_BUTTON9, &CLemonTestDlg::OnBnClickedButton9 )
    ON_BN_CLICKED( IDC_BUTTON10, &CLemonTestDlg::OnBnClickedButton10 )
    ON_BN_CLICKED( IDC_BUTTON11, &CLemonTestDlg::OnBnClickedButton11 )
    ON_WM_CLOSE()
    ON_BN_CLICKED( IDC_BUTTON12, &CLemonTestDlg::OnBnClickedButton12 )
    ON_BN_CLICKED( IDC_BUTTON13, &CLemonTestDlg::OnBnClickedButton13 )
    ON_CBN_SELCHANGE( IDC_COMBO1, &CLemonTestDlg::OnCbnSelchangeCombo1 )
END_MESSAGE_MAP()


// CLemonTestDlg ��Ϣ��������

BOOL CLemonTestDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // ��������...���˵������ӵ�ϵͳ�˵��С�

    // IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
    ASSERT( ( IDM_ABOUTBOX & 0xFFF0 ) == IDM_ABOUTBOX );
    ASSERT( IDM_ABOUTBOX < 0xF000 );

    CMenu* pSysMenu = GetSystemMenu( FALSE );
    if ( pSysMenu != NULL )
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString( IDS_ABOUTBOX );
        ASSERT( bNameValid );
        if ( !strAboutMenu.IsEmpty() )
        {
            pSysMenu->AppendMenu( MF_SEPARATOR );
            pSysMenu->AppendMenu( MF_STRING, IDM_ABOUTBOX, strAboutMenu );
        }
    }

    // ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
    //  ִ�д˲���
    SetIcon( m_hIcon, TRUE );			// ���ô�ͼ��
    SetIcon( m_hIcon, FALSE );		// ����Сͼ��

    // TODO: �ڴ����Ӷ���ĳ�ʼ������

    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CLemonTestDlg::OnSysCommand( UINT nID, LPARAM lParam )
{
    if ( ( nID & 0xFFF0 ) == IDM_ABOUTBOX )
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand( nID, lParam );
    }
}

// �����Ի���������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLemonTestDlg::OnPaint()
{
    if ( IsIconic() )
    {
        CPaintDC dc( this ); // ���ڻ��Ƶ��豸������

        SendMessage( WM_ICONERASEBKGND, reinterpret_cast<WPARAM>( dc.GetSafeHdc() ), 0 );

        // ʹͼ���ڹ����������о���
        int cxIcon = GetSystemMetrics( SM_CXICON );
        int cyIcon = GetSystemMetrics( SM_CYICON );
        CRect rect;
        GetClientRect( &rect );
        int x = ( rect.Width() - cxIcon + 1 ) / 2;
        int y = ( rect.Height() - cyIcon + 1 ) / 2;

        // ����ͼ��
        dc.DrawIcon( x, y, m_hIcon );
    }
    else
    {
        CDialog::OnPaint();
    }
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CLemonTestDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>( m_hIcon );
}

void CLemonTestDlg::OnBnClickedButton1()
{
    Digraph dg;
    Digraph::Node v1 = dg.addNode();
    Digraph::Node v2 = dg.addNode();
    Digraph::Node v3 = dg.addNode();
    Digraph::Node v4 = dg.addNode();
    Digraph::Arc e1 = dg.addArc( v1, v2 );
    Digraph::Arc e2 = dg.addArc( v1, v3 );
    Digraph::Arc e3 = dg.addArc( v2, v3 );
    Digraph::Arc e4 = dg.addArc( v2, v4 );
    Digraph::Arc e5 = dg.addArc( v3, v4 );

    typedef Digraph::ArcMap<CString> EdgeNameMap;
    EdgeNameMap eName( dg );
    eName[e1] = _T( "e1" );
    eName[e2] = _T( "e2" );
    eName[e3] = _T( "e3" );
    eName[e4] = _T( "e4" );
    eName[e5] = _T( "e5" );

    typedef Digraph::NodeMap<CString> NodeNameMap;
    NodeNameMap nName( dg );
    nName[v1] = _T( "v1" );
    nName[v2] = _T( "v2" );
    nName[v3] = _T( "v3" );
    nName[v4] = _T( "v4" );

    bool ret = true;
    typedef Path<Digraph> DPath;
    DPath pp;
    Dfs<Digraph> aDfs( dg );
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // lemon��run(s,t)ʵ����bug
    // ֻ��ͨ��run(s)�ķ����õ�dsf tree
    // Ȼ���ٵõ�path��������Ч�ʾͱȽϲ�
    // ��Ҫ��������ͼ
    // bfsҲ�����
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //Bfs<Digraph> bDfs(dg);
    //bool ret = aDfs.run(v1, v4);
    aDfs.run( v1 );
    //bool ret = bDfs.run(v1, v4);
    if( ret )
    {
        if( !aDfs.reached( v4 ) ) return;
        pp = aDfs.path( v4 );
        CString msg;
        //pp = bDfs.path(v3);
        msg.Format( _T( "\n%s->%s��·��:" ), nName[v1], nName[v4] );
        for( DPath::ArcIt e( pp ); e != INVALID; ++e )
        {
            DPath::Arc a = e;
            msg.AppendFormat( _T( "%s->" ), eName[a] );
        }
        msg.AppendFormat( _T( "\n" ) );

        MessageBox( msg );
    }
}
typedef ListDigraph Digraph;

void CLemonTestDlg::OnBnClickedButton2()
{
    Digraph dg;
    Digraph::Node v1 = dg.addNode();
    Digraph::Node v2 = dg.addNode();
    Digraph::Node v3 = dg.addNode();
    Digraph::Node v4 = dg.addNode();
    Digraph::Arc e1 = dg.addArc( v1, v2 );
    Digraph::Arc e2 = dg.addArc( v1, v3 );
    Digraph::Arc e3 = dg.addArc( v2, v3 );
    Digraph::Arc e4 = dg.addArc( v2, v4 );
    Digraph::Arc e5 = dg.addArc( v3, v4 );

    typedef Digraph::ArcMap<CString> EdgeNameMap;
    EdgeNameMap eName( dg );
    eName[e1] = _T( "e1" );
    eName[e2] = _T( "e2" );
    eName[e3] = _T( "e3" );
    eName[e4] = _T( "e4" );
    eName[e5] = _T( "e5" );

    typedef Digraph::NodeMap<CString> NodeNameMap;
    NodeNameMap nName( dg );
    nName[v1] = _T( "v1" );
    nName[v2] = _T( "v2" );
    nName[v3] = _T( "v3" );
    nName[v4] = _T( "v4" );

    typedef Undirector<Digraph> Graph;
    Graph g( dg ); // ת��������ͼ
    typedef Path<Undirector<Digraph>> UPath;
    UPath pp;
    Dfs<Graph> aDfs( g );
    // 3��д��
    //aDfs.run(v1);

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // lemon��run(s,t)ʵ����bug
    // ֻ��ͨ��run(s)�ķ����õ�dsf tree
    // Ȼ���ٵõ�path��������Ч�ʾͱȽϲ�
    // ��Ҫ��������ͼ
    // bfsҲ�����
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // Ŀǰ���޸�(�ӹ���������һ��Сpatch��Ȼ���lemon��Դ�����±�����һ��)
    bool ret = aDfs.run( v1, v4 );
    //bool ret = dfs(g).path(pp).run(v1, v4);
    if( ret )
    {
        if( !aDfs.reached( v4 ) ) return;
        pp = aDfs.path( v4 );
        CString msg;
        msg.Format( _T( "\n%s->%s��·��:" ), nName[v1], nName[v4] );
        for( UPath::ArcIt e( pp ); e != INVALID; ++e )
        {
            UPath::Arc a = e;
            //Digraph::Arc b = a;
            msg.AppendFormat( _T( "%s(%s, %s)->" ), eName[a], nName[dg.source( a )], nName[dg.target( a )] );
            //msg.AppendFormat(_T("%s(%s, %s)->"), eName[a], nName[dg.source(b)], nName[dg.target(b)]);
            //msg.AppendFormat(_T("%s(%s, %s)->"), eName[a], nName[g.source(a)], nName[g.target(a)]);
//			msg.AppendFormat(_T("%s(%s, %s)->"), eName[b], nName[g.source(b)], nName[g.target(b)]); // error!
        }
        msg.AppendFormat( _T( "\n" ) );
        MessageBox( msg );
    }
}

void CLemonTestDlg::OnBnClickedButton3()
{
    Digraph dg;
    typedef Digraph::NodeMap<CString> NodeNameMap;
    NodeNameMap name( dg );
    Digraph::Node v1 = dg.addNode();
    Digraph::Node v2 = dg.addNode();
    Digraph::Node v3 = dg.addNode();
    dg.addArc( v1, v2 );
    dg.addArc( v2, v3 );

    name[v1] = _T( "v1" );
    name[v2] = _T( "v2" );
    name[v3] = _T( "v3" );

    CString msg;
    for( Digraph::NodeIt n( dg ); n != INVALID; ++n )
    {
        msg.AppendFormat( _T( "%s\n" ), name[n] );
    }

    MessageBox( msg );
}

void CLemonTestDlg::OnBnClickedButton4()
{
    Digraph dg;
    Digraph::Node v1 = dg.addNode();
    Digraph::Node v2 = dg.addNode();
    Digraph::Node v3 = dg.addNode();
    Digraph::Node v4 = dg.addNode();
    Digraph::Arc e1 = dg.addArc( v1, v2 );
    Digraph::Arc e2 = dg.addArc( v1, v3 );
    Digraph::Arc e3 = dg.addArc( v2, v3 );
    Digraph::Arc e4 = dg.addArc( v2, v4 );
    Digraph::Arc e5 = dg.addArc( v3, v4 );

    typedef Digraph::ArcMap<CString> EdgeNameMap;
    EdgeNameMap eName( dg );
    eName[e1] = _T( "e1" );
    eName[e2] = _T( "e2" );
    eName[e3] = _T( "e3" );
    eName[e4] = _T( "e4" );
    eName[e5] = _T( "e5" );

    typedef Digraph::ArcMap<double> Weight;
    Weight w( dg );
    w[e1] = 1;
    w[e2] = 1;
    w[e3] = 1;
    w[e4] = 1;
    w[e5] = 1;

    typedef Digraph::NodeMap<CString> NodeNameMap;
    NodeNameMap nName( dg );
    nName[v1] = _T( "v1" );
    nName[v2] = _T( "v2" );
    nName[v3] = _T( "v3" );
    nName[v4] = _T( "v4" );

    // Dijkstra���·���㷨
    Dijkstra<Digraph, Weight> dk( dg, w );

    //dk.run(v4);
    dk.run( v1 );
    if( !dk.reached( v4 ) ) return;

    typedef Path<Digraph> DPath;
    DPath p1 = dk.path( v4 );

    CString msg;
    msg.Format( _T( "\n%s->%s��·��:" ), nName[v1], nName[v4] );
    for( DPath::ArcIt e( p1 ); e != INVALID; ++e )
    {
        msg.AppendFormat( _T( "%s(%s, %s)->" ), eName[e], nName[dg.source( e )], nName[dg.target( e )] );
    }
    msg.AppendFormat( _T( "\n" ) );
    MessageBox( msg );
}

static bool ReadWordInstallPath( CString& path )
{
    long lRet = 0;
    CString strRegOffice = _T( "Software\\Microsoft\\Office\\11.0\\Word\\InstallRoot" ); // office03�汾
    HKEY   hResult;
    lRet = ::RegOpenKeyEx( HKEY_LOCAL_MACHINE, strRegOffice, 0, KEY_QUERY_VALUE, &hResult );
    if( lRet == ERROR_SUCCESS )
    {
        DWORD      WordType =  REG_SZ;
        DWORD      WordData =  100;
        LPTSTR     KeyByte[1024];

        lRet = ::RegQueryValueEx( hResult, ( _T( "Path" ) ), NULL, &WordType, ( LPBYTE )KeyByte, &WordData );
        if( lRet == ERROR_SUCCESS )
        {
            path = ( LPTSTR )KeyByte;
            path.Append( _T( "word.exe" ) );
        }

        RegCloseKey( hResult );
    }
    return ( lRet == ERROR_SUCCESS );
}

void CLemonTestDlg::OnBnClickedButton5()
{
    CString path;
    if( ReadWordInstallPath( path ) )
    {
        CString msg;
        msg.Format( _T( "��ȡword��װ·���ɹ�\n%s" ), path );
        MessageBox( msg );
    }
    else
    {
        MessageBox( _T( "��ȡ·��ʧ��" ) );
    }

    CString filePath = _T( "C:\\Users\\anheihb03dlj\\Desktop\\��ú��ȫ��̡�103����������㷽��.doc" );
    ShellExecute( NULL, _T( "open" ), filePath, NULL, NULL, SW_HIDE/*SW_SHOWNORMAL*/ );
}

static CString GetPathDir( const CString& path )
{
    // ����2��Ҳ����ʹ��cstring�ķ���
    return path.Left( path.ReverseFind( _T( '\\' ) ) );
}

void CLemonTestDlg::OnBnClickedButton6()
{
    CString path = _T( "C:\\Users\\anheihb03dlj\\Desktop\\����ģ���޸�.txt" );
    MessageBox( GetPathDir( path ) );
}

static void SplitCString( const CString& content, const CString& token, CStringArray& cc )
{
    int curPos = 0;
    CString str = content.Tokenize( token, curPos );
    while ( str != _T( "" ) )
    {
        cc.Add( str );
        str = content.Tokenize( token, curPos );
    };
}

void CLemonTestDlg::OnBnClickedButton7()
{
    CStdioFile inFile;
    if( !inFile.Open( _T( "D:\\xx.txt" ), CFile::modeRead | CFile::typeText ) ) return;

    // ��ȡͼԪ���ͺ��ֶ�����
    CString geType;
    CStringArray fields;

    CString strLine;
    inFile.ReadString( strLine );
    geType = strLine.Trim();

    // ��ȡ�ֶ�
    CString strLine2;
    inFile.ReadString( strLine2 );
    SplitCString( strLine2, _T( " \t" ), fields );
}

// GUID�ַ���ǰ׺
#define GUID_PREFIX _T("JLDATA")

// GUID�ַ�����ʽ
#define GUID_FORMAT _T("{%08lX-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}")

// ����GUID�ַ���
static bool NewGUID( CString& strGUID )
{
    GUID m_guid;
    bool ret = ( S_OK == ::CoCreateGuid( &m_guid ) );
    if( ret )
    {
        strGUID.Format( _T( "%s_" ), GUID_PREFIX ); // ����ǰ׺
        // ׷��GUID�ַ���
        strGUID.AppendFormat( GUID_FORMAT,
                              m_guid.Data1, m_guid.Data2, m_guid.Data3,
                              m_guid.Data4[0], m_guid.Data4[1], m_guid.Data4[2],
                              m_guid.Data4[3], m_guid.Data4[4], m_guid.Data4[5],
                              m_guid.Data4[6], m_guid.Data4[7] );
    }
    return ret;
}

void CLemonTestDlg::OnBnClickedButton8()
{
    CString strGUID;
    NewGUID( strGUID );
    MessageBox( strGUID );
}

void CLemonTestDlg::OnBnClickedButton9()
{
    Digraph dg;
    Digraph::Node v1 = dg.addNode();
    Digraph::Node v2 = dg.addNode();
    Digraph::Node v3 = dg.addNode();
    Digraph::Node v4 = dg.addNode();
    Digraph::Arc e1 = dg.addArc( v1, v2 );
    Digraph::Arc e2 = dg.addArc( v1, v3 );
    Digraph::Arc e3 = dg.addArc( v2, v3 );
    Digraph::Arc e4 = dg.addArc( v2, v4 );
    Digraph::Arc e5 = dg.addArc( v3, v4 );

    typedef Digraph::ArcMap<CString> EdgeNameMap;
    EdgeNameMap eName( dg );
    eName[e1] = _T( "e1" );
    eName[e2] = _T( "e2" );
    eName[e3] = _T( "e3" );
    eName[e4] = _T( "e4" );
    eName[e5] = _T( "e5" );

    typedef Digraph::NodeMap<CString> NodeNameMap;
    NodeNameMap nName( dg );
    nName[v1] = _T( "v1" );
    nName[v2] = _T( "v2" );
    nName[v3] = _T( "v3" );
    nName[v4] = _T( "v4" );

    Dfs<Digraph> aDfs( dg );
    aDfs.run( v1 );

    typedef ReverseDigraph<Digraph> RDiagraph;
    RDiagraph rdg( dg );
    Dfs<RDiagraph> bDfs( rdg );
    bDfs.run( v4 );

    CString msg;
    for( Digraph::NodeIt n( dg ); n != INVALID; ++n )
    {
        msg.AppendFormat( _T( "%s: %d %d\n" ), nName[n], aDfs.dist( n ), bDfs.dist( n ) );
    }

    MessageBox( msg );
}

void CLemonTestDlg::OnBnClickedButton10()
{
    Digraph dg;
    Digraph::Node v1 = dg.addNode();
    Digraph::Node v2 = dg.addNode();
    Digraph::Node v3 = dg.addNode();
    Digraph::Node v4 = dg.addNode();
    Digraph::Arc e1 = dg.addArc( v1, v2 ); // v1->v2
    Digraph::Arc e2 = dg.addArc( v3, v1 ); // v1->v3
    Digraph::Arc e3 = dg.addArc( v3, v2 ); // v3->v2
    Digraph::Arc e4 = dg.addArc( v2, v4 ); // v2->v4
    Digraph::Arc e5 = dg.addArc( v4, v3 ); // v3->v4

    typedef Digraph::ArcMap<CString> EdgeNameMap;
    EdgeNameMap eName( dg );
    eName[e1] = _T( "e1" );
    eName[e2] = _T( "e2" );
    eName[e3] = _T( "e3" );
    eName[e4] = _T( "e4" );
    eName[e5] = _T( "e5" );

    typedef Digraph::NodeMap<CString> NodeNameMap;
    NodeNameMap nName( dg );
    nName[v1] = _T( "v1" );
    nName[v2] = _T( "v2" );
    nName[v3] = _T( "v3" );
    nName[v4] = _T( "v4" );

    typedef Undirector<Digraph> UGraph;
    UGraph ug( dg );

    Dfs<UGraph> aDfs( ug );
    if( aDfs.run( v4, v1 ) )
    {
        typedef Path<UGraph> UPath;
        UPath p = aDfs.path( v1 );

        CString msg;
        for( UPath::ArcIt e( p ); e != INVALID; ++e )
        {
            Digraph::Node u = ug.source( e );
            Digraph::Node v = ug.target( e );
            UGraph::Arc ue = e;    // ArcIt������Arc
            Digraph::Arc ee = ue; // UGraph::Arc��ת����Digraph::Arc
            msg.AppendFormat( _T( "��֧:%s(%s, %s)\n" ), eName[ee], nName[u], nName[v] );
        }

        MessageBox( msg );
    }
    else
    {
        MessageBox( _T( "���ɴ�" ) );
    }
}

BOOL WCharToMByte( const wchar_t* wch, char*& ch )
{
    int size = WideCharToMultiByte( CP_ACP, 0, wch, -1, NULL, 0, NULL, NULL );
    /*char* */
    ch = new char[size + 1];
    BOOL ret = WideCharToMultiByte( CP_ACP, 0, wch, -1, ch, size, NULL, NULL );
    if( !ret )
    {
        delete []ch;
        ch = 0;
    }
    return ret;
}

void CLemonTestDlg::OnBnClickedButton11()
{
    wchar_t* wch = _T( "��������" );
    char* ch;
    if( !WCharToMByte( wch, ch ) )
    {
        MessageBox( _T( "ת��ʧ��" ) );
    }
    else
    {
        //AfxMessageBoxA()
    }
    //int size=WideCharToMultiByte(CP_ACP,0,wch,-1,NULL,0,NULL,NULL);
    //char *ch=new char[size+1];
    //if(!WideCharToMultiByte(CP_ACP,0,wch,-1,ch,size,NULL,NULL))
    //{
    //	MessageBox(_T("ת��ʧ��"));
    //	//return false;
    //}
    //else
    //{

    //}
}

void CLemonTestDlg::OnClose()
{
    //MessageBox(_T("close"));

    CDialog::OnClose();
}

void CLemonTestDlg::OnCancel()
{
    //MessageBox(_T("onCancel"));

    CDialog::OnCancel();
}

void CLemonTestDlg::OnBnClickedButton12()
{
    CRect rect;
    GetDlgItem( IDC_RECT_PIC )->GetWindowRect( &rect );

    CString msg;
    msg.Format( _T( "left:%d, right:%d, top:%d, bottom:%d, width:%d, height:%d" ), rect.left, rect.right, rect.top, rect.bottom, rect.Width(), rect.Height() );

    ScreenToClient( &rect );
    msg.AppendFormat( _T( "\n[C]left:%d, right:%d, top:%d, bottom:%d, width:%d, height:%d" ), rect.left, rect.right, rect.top, rect.bottom, rect.Width(), rect.Height() );

    CRect dlgRect;
    GetWindowRect( &dlgRect );
    msg.AppendFormat( _T( "\n\nleft:%d, right:%d, top:%d, bottom:%d, width:%d, height:%d" ), dlgRect.left, dlgRect.right, dlgRect.top, dlgRect.bottom, dlgRect.Width(), dlgRect.Height() );

    //ScreenToClient(&dlgRect);
    //msg.AppendFormat(_T("\n[C]left:%d, right:%d, top:%d, bottom:%d, width:%d, height:%d"), dlgRect.left, dlgRect.right, dlgRect.top, dlgRect.bottom, dlgRect.Width(), dlgRect.Height());

    //MessageBox(msg);

    dlgRect.bottom += 20;
    rect.bottom += 100;
    SetWindowPos( NULL, dlgRect.left, dlgRect.top, dlgRect.Width(), dlgRect.Height(), SWP_NOMOVE | SWP_NOZORDER );
    GetDlgItem( IDC_RECT_PIC )->SetWindowPos( NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOMOVE | SWP_NOZORDER );
}

void CLemonTestDlg::OnBnClickedButton13()
{
    Digraph dg;
    Digraph::Node v1 = dg.addNode();
    Digraph::Node v2 = dg.addNode();
    Digraph::Node v3 = dg.addNode();
    Digraph::Node v4 = dg.addNode();
    Digraph::Arc e1 = dg.addArc( v1, v2 ); // v1->v2
    Digraph::Arc e2 = dg.addArc( v1, v3 ); // v1->v3
    Digraph::Arc e3 = dg.addArc( v3, v2 ); // v3->v2
    Digraph::Arc e4 = dg.addArc( v2, v4 ); // v2->v4
    Digraph::Arc e5 = dg.addArc( v3, v4 ); // v3->v4

    typedef Digraph::NodeMap<CString> NodeNameMap;
    NodeNameMap nName( dg );
    nName[v1] = _T( "v1" );
    nName[v2] = _T( "v2" );
    nName[v3] = _T( "v3" );
    nName[v4] = _T( "v4" );

    typedef Digraph::NodeMap<int> NodeOrder;
    NodeOrder order( dg );
    topologicalSort( dg, order );

    CString msg;
    for( Digraph::NodeIt n( dg ); n != INVALID; ++n )
    {
        msg.AppendFormat( _T( "%s order=%d\n" ), nName[n], order[n] );
    }
    MessageBox( msg );
}

void CLemonTestDlg::OnCbnSelchangeCombo1()
{
    // TODO: �ڴ����ӿؼ�֪ͨ�����������
}