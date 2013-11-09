#include "stdafx.h"
#include "QuestionNaireDlg.h"

#include "DBFieldNames.h"

#define ORIGIN_TITLE _T("人工询问&现场调查")

static CString GetClausePath( const CString& dbPath, const CString& regulationTableName, int clauseNum )
{
    TCHAR szPath[_MAX_PATH] = {0}, szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFile[_MAX_FNAME], szExt[_MAX_EXT];
    _tsplitpath( ( LPCTSTR )dbPath, szDrive, szDir, szFile, szExt ); // 拆分路径
    _tcscat_s( szPath, szDrive ); // 合并盘符
    _tcscat_s( szPath, szDir ); // 合并目录

    CString clauseFile;
    clauseFile.Format( _T( "%s\\%d.rtf" ), regulationTableName, clauseNum );
    _tcscat_s( szPath, clauseFile ); // 条款编号对应的rtf文件

    return szPath;
}

static DWORD CALLBACK MyStreamInCallback( DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb )
{
    CFile* pFile = ( CFile* ) dwCookie;
    *pcb = pFile->Read( pbBuff, cb );
    return 0;
}

static void ReadRTFData( CRichEditCtrl& contentRichEdit, const CString& rtfFilePath )
{
    // The file from which to load the contents of the rich edit control.
    CFile cFile( rtfFilePath, CFile::modeRead );
    EDITSTREAM es;
    es.dwCookie = ( DWORD ) &cFile;
    es.pfnCallback = MyStreamInCallback;

    long size = contentRichEdit.StreamIn( SF_RTF, es );
}

IMPLEMENT_DYNAMIC( QuestionNaireDlg, CDialog )

QuestionNaireDlg::QuestionNaireDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( QuestionNaireDlg::IDD, pParent )
    , m_yes_or_no( FALSE )
{

}

QuestionNaireDlg::~QuestionNaireDlg()
{
}

void QuestionNaireDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_CLAUSE_CONTENT, m_contentRichEdit );
    DDX_Radio( pDX, IDC_NO_RADIO, m_yes_or_no );
}

BEGIN_MESSAGE_MAP( QuestionNaireDlg, CDialog )
    ON_BN_CLICKED( IDC_PREVIOUS_BTN, &QuestionNaireDlg::OnBnClickedPreviousBtn )
    ON_BN_CLICKED( IDC_NEXT_BTN, &QuestionNaireDlg::OnBnClickedNextBtn )
    ON_BN_CLICKED( IDC_YES_RADIO, &QuestionNaireDlg::OnBnClickedYesRadio )
    ON_BN_CLICKED( IDC_NO_RADIO, &QuestionNaireDlg::OnBnClickedNoRadio )
END_MESSAGE_MAP()


BOOL QuestionNaireDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // 设置标题
    SetWindowText( ORIGIN_TITLE );

    //m_contentRichEdit.SetBackgroundColor(FALSE, RGB(192,192, 192));

    //m_contentRichEdit.SetWordWrapMode(WBF_WORDWRAP | WBF_WORDBREAK);
    m_contentRichEdit.SetTargetDevice( NULL, 0 );

    m_pos = 0;
    GetDlgItem( IDC_PREVIOUS_BTN )->EnableWindow( FALSE );
    fillRichEditContent( getCurrentClause() );

    return TRUE;
}

void QuestionNaireDlg::OnBnClickedPreviousBtn()
{
    if( m_pos > 0 )
    {
        GetDlgItem( IDC_NEXT_BTN )->EnableWindow( TRUE );
        m_pos--;
    }
    if( m_pos == 0 )
    {
        GetDlgItem( IDC_PREVIOUS_BTN )->EnableWindow( FALSE );
    }
    fillRichEditContent( getCurrentClause() );
}

void QuestionNaireDlg::OnBnClickedNextBtn()
{
    int len = m_qnrs.length();
    if( m_pos < len - 1 )
    {
        GetDlgItem( IDC_PREVIOUS_BTN )->EnableWindow( TRUE );
        m_pos++;
    }
    if( m_pos == len - 1 )
    {
        GetDlgItem( IDC_NEXT_BTN )->EnableWindow( FALSE );
    }
    fillRichEditContent( getCurrentClause() );
}

void QuestionNaireDlg::OnBnClickedYesRadio()
{
    UpdateData( TRUE );

    setQuestionNaireResult( m_pos, m_yes_or_no ); // my_yes = 0
}

void QuestionNaireDlg::OnBnClickedNoRadio()
{
    UpdateData( TRUE );

    setQuestionNaireResult( m_pos, m_yes_or_no );
}

void QuestionNaireDlg::addClause( int clause )
{
    bool ret = false;
    int len = m_qnrs.length();
    for( int i = 0; i < len; i++ )
    {
        if( m_qnrs[i].clause == clause )
        {
            ret = true;
            break;
        }
    }
    if( !ret )
    {
        QuestionNaireResult qnr = {clause, 1};
        m_qnrs.append( qnr );
    }
}

int QuestionNaireDlg::getResult( int clause ) const
{
    int yes_or_no = 1;
    int len = m_qnrs.length();
    for( int i = 0; i < len; i++ )
    {
        if( m_qnrs[i].clause == clause )
        {
            yes_or_no = m_qnrs[i].yes_or_no;
            break;
        }
    }
    return yes_or_no;
}

int QuestionNaireDlg::getCurrentClause() const
{
    return m_qnrs[m_pos].clause;
}

void QuestionNaireDlg::fillRichEditContent( int clause )
{
    // 设置标题
    CString title;
    title.AppendFormat( _T( "%s--《煤矿安全规程》第%d条" ), ORIGIN_TITLE, clause );
    SetWindowText( title );

    // 清空
    m_contentRichEdit.SetSel( 0, -1 );
    m_contentRichEdit.Clear();

    CDaoDatabase db;
    try
    {
        db.Open( m_regulateDB );

        CDaoRecordset rs( &db );
        CString sql;
        sql.Format( _T( "select * from %s where %s=%d" ), m_regulationTable, CLAUSE_NUM_FIELD, clause );
        rs.Open( dbOpenDynaset, sql, 0 );

        rs.MoveFirst();
        while( !rs.IsEOF() )
        {
            // 获取rtf文件名称(例如100.rtf)
            COleVariant v1;
            rs.GetFieldValue( CLAUSE_CONTENT_FIELD , v1 );

            // 目前没有使用v1变量，直接使用条款编号构造路径
            ReadRTFData( m_contentRichEdit, GetClausePath( m_regulateDB, m_regulationTable, clause ) );

            m_yes_or_no = getQuestionNaireResult( clause ); // 从本地记录中读取调查结果

            rs.MoveNext () ;
        }

        rs.Close();
        db.Close();
    }
    catch( CDaoException* e )
    {
        e->ReportError();
        e->Delete();

        if( db.IsOpen() ) db.Close();
    }

    //m_contentRichEdit.SetSel(0, 1);

    UpdateData( FALSE );
}

void QuestionNaireDlg::setQuestionNaireResult( int pos, int yes_or_no )
{
    m_qnrs[pos].yes_or_no = yes_or_no;
}

int QuestionNaireDlg::getQuestionNaireResult( int clause )
{
    int yes_no = -1;
    int len = m_qnrs.length();
    for( int i = 0; i < len; i++ )
    {
        if( m_qnrs[i].clause == clause )
        {
            yes_no = m_qnrs[i].yes_or_no;
            break;
        }
    }
    return yes_no;
}
