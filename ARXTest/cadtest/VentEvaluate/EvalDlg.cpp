#include "stdafx.h"

#include "EvalDlg.h"
#include "DBFieldNames.h"
#include "IniFile.h"
#include "EvalHelper.h"
#include "FilePathHelper.h"

#include "../EvalBase/EvalResult.h"
#include "../EvalBase/EvalService.h"
#include "../DaoHelper/TableCreator.h"
#include "../DaoHelper/DataWriter.h"
using namespace PJ;

#include "../MineGE/DataListHelper.h"
#include "../MineGE/DataHelper.h"
#include "../VentNetworkAlgo/VNAlgoHelper.h"

/* ȫ�ֺ���(ʵ����QuestionNaire.cpp) */
extern void QuestionNaire( const CString& regulateDB, const CString& regulationTable,
                           const CString& evalResultDBPath, const CString& caclResultTable );

// ��ΪlistCtrl�ı���ͷ(header)
const static TCHAR* REGULATION_INFO[] =
{
    ID,
    NAME_FIELD,
    VERSION_FIELD,
    PUBLISHER_FIELD,
    LINK_TABLE_FIELD
};

static void GetClauseNums( const CString& dbName, const CString& tblName, AcDbIntArray& allClauses )
{
    CDaoDatabase db;
    try
    {
        db.Open( dbName );

        CDaoRecordset rs( &db );
        CString sql;
        sql.Format( _T( "select * from %s" ), tblName );  // ��ȡ��̱�--tablename
        rs.Open( dbOpenDynaset, sql, 0 );

        rs.MoveFirst () ;
        while( !rs.IsEOF() )
        {
            COleVariant v;
            rs.GetFieldValue( CLAUSE_NUM_FIELD , v ); // ��ȡ������
            allClauses.append( v.intVal );

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

        allClauses.removeAll(); // �����쳣�����
    }
}

static bool WriteCoalMineInfo( CIniFile& iniFile )
{
    // "����Ϣ"���ݶ���
    AcDbObjectId objId;
    if( !SingleDataObjectHelper::GetObjectId( COAL_MINE_SECTION, objId ) )
    {
        CString msg;
        msg.Format( _T( "��%s�����ݶ��󲻴���" ), COAL_MINE_SECTION );
        AfxMessageBox( msg );
        return false;
    }

    // "����"
    CString v1;
    if( !DataHelper::GetPropertyData( objId, COAL_MINE_NAME_KEY, v1 ) ) return false;

    // "�������Ź�˾"
    CString v2;
    if( !DataHelper::GetPropertyData( objId, COMPANY_NAME_KEY, v2 ) ) return false;

    // "ͨ�����ϵ��"
    // ���Ϊ��(δ��������)��Ĭ��Ϊ1.15
    CString v3;
    if( !DataHelper::GetPropertyData( objId, COAL_MINE_AIR_FLOW_COEFF_KEY, v3 ) ) return false;

    // "ͨ��ϵ��"
    // ���Ϊ��(δ��������)��Ĭ��Ϊ1.15
    CString v4;
    if( !DataHelper::GetPropertyData( objId, COAL_MINE_AIR_FLOW_COEFF2_KEY, v4 ) ) return false;

    if( v1.GetLength() == 0 ) v1 = _T( "xxx��" );
    if( v2.GetLength() == 0 ) v2 = _T( "xxx���Ź�˾" );
    if( v3.GetLength() == 0 ) v3 = _T( "1.15" );
    if( v4.GetLength() == 0 ) v4 = _T( "1.15" );

    iniFile.SetKeyValue( COAL_MINE_SECTION, COAL_MINE_NAME_KEY, v1 );
    iniFile.SetKeyValue( COAL_MINE_SECTION, COMPANY_NAME_KEY, v2 );
    iniFile.SetKeyValue( COAL_MINE_SECTION, COAL_MINE_AIR_FLOW_COEFF_KEY, v3 );
    iniFile.SetKeyValue( COAL_MINE_SECTION, COAL_MINE_AIR_FLOW_COEFF2_KEY, v4 );

    return true;
}

static bool WriteEvalProjectInfo( CIniFile& iniFile )
{
    AcDbObjectId objId;
    if( !SingleDataObjectHelper::GetObjectId( EVAL_PROJECT_INFO_SECTION, objId ) )
    {
        CString msg;
        msg.Format( _T( "��%s�����ݶ��󲻴���" ), EVAL_PROJECT_INFO_SECTION );
        AfxMessageBox( msg );
        return false;
    }

    // ���������ݡ�
    CString v1;
    if( !DataHelper::GetPropertyData( objId, EVAL_REGULATION_KEY, v1 ) ) return false;

    // "�ƶ����۷�����ʼʱ��"
    CString v2;
    if( !DataHelper::GetPropertyData( objId, PREPARE_SOLUTION_BEGIN_KEY, v2 ) ) return false;

    // "�ƶ����۷�������ʱ��"
    CString v3;
    if( !DataHelper::GetPropertyData( objId, PREPARE_SOLUTION_END_KEY, v3 ) ) return false;

    // "�ռ��������Ͽ�ʼʱ��"
    CString v4;
    if( !DataHelper::GetPropertyData( objId, PREPARE_INFO_BEGIN_KEY, v4 ) ) return false;

    // "�ռ��������Ͻ���ʱ��"
    CString v5;
    if( !DataHelper::GetPropertyData( objId, PREPARE_INFO_END_KEY, v5 ) ) return false;

    // "ǩ�����ۺ�ͬ��ʼʱ��"
    CString v6;
    if( !DataHelper::GetPropertyData( objId, PREPARE_CONTRACT_BEGIN_KEY, v6 ) ) return false;

    // "ǩ�����ۺ�ͬ����ʱ��"
    CString v7;
    if( !DataHelper::GetPropertyData( objId, PREPARE_CONTRACT_END_KEY, v7 ) ) return false;

    // "���������᳹��ʼʱ��"
    CString v8;
    if( !DataHelper::GetPropertyData( objId, SURVEY_LEARN_BEGIN_KEY, v8 ) ) return false;

    // "���������᳹����ʱ��"
    CString v9;
    if( !DataHelper::GetPropertyData( objId, SURVEY_LEARN_END_KEY, v9 ) ) return false;

    // "��չ���۹�����ʼʱ��"
    CString v10;
    if( !DataHelper::GetPropertyData( objId, WORK_DO_BEGIN_KEY, v10 ) ) return false;

    // "��չ���۹�������ʱ��"
    CString v11;
    if( !DataHelper::GetPropertyData( objId, WORK_DO_END_KEY, v11 ) ) return false;

    // "�������۱��濪ʼʱ��"
    CString v12;
    if( !DataHelper::GetPropertyData( objId, WORK_REPORT_BEGIN_KEY, v12 ) ) return false;

    // "�������۱������ʱ��"
    CString v13;
    if( !DataHelper::GetPropertyData( objId, WORK_REPORT_END_KEY, v13 ) ) return false;

    if( v1.GetLength() == 0 ) v1 = _T( "ú��ȫ���" );

    iniFile.SetKeyValue( EVAL_PROJECT_INFO_SECTION, EVAL_REGULATION_KEY, v1 );
    iniFile.SetKeyValue( EVAL_PROJECT_INFO_SECTION, PREPARE_SOLUTION_BEGIN_KEY, String2Time( v2 ) );
    iniFile.SetKeyValue( EVAL_PROJECT_INFO_SECTION, PREPARE_SOLUTION_END_KEY, String2Time( v3 ) );
    iniFile.SetKeyValue( EVAL_PROJECT_INFO_SECTION, PREPARE_INFO_BEGIN_KEY, String2Time( v4 ) );
    iniFile.SetKeyValue( EVAL_PROJECT_INFO_SECTION, PREPARE_INFO_END_KEY, String2Time( v5 ) );
    iniFile.SetKeyValue( EVAL_PROJECT_INFO_SECTION, PREPARE_CONTRACT_BEGIN_KEY, String2Time( v6 ) );
    iniFile.SetKeyValue( EVAL_PROJECT_INFO_SECTION, PREPARE_CONTRACT_END_KEY, String2Time( v7 ) );
    iniFile.SetKeyValue( EVAL_PROJECT_INFO_SECTION, SURVEY_LEARN_BEGIN_KEY, String2Time( v8 ) );
    iniFile.SetKeyValue( EVAL_PROJECT_INFO_SECTION, SURVEY_LEARN_END_KEY, String2Time( v9 ) );
    iniFile.SetKeyValue( EVAL_PROJECT_INFO_SECTION, WORK_DO_BEGIN_KEY, String2Time( v10 ) );
    iniFile.SetKeyValue( EVAL_PROJECT_INFO_SECTION, WORK_DO_END_KEY, String2Time( v11 ) );
    iniFile.SetKeyValue( EVAL_PROJECT_INFO_SECTION, WORK_REPORT_BEGIN_KEY, String2Time( v12 ) );
    iniFile.SetKeyValue( EVAL_PROJECT_INFO_SECTION, WORK_REPORT_END_KEY, String2Time( v13 ) );

    return true;
}

static bool WriteExtraData( CIniFile& iniFile )
{
    // 1��д�����Ϣ
    if( !WriteCoalMineInfo( iniFile ) ) return false;

    // 2��д��������Ŀ��Ϣ
    if( !WriteEvalProjectInfo( iniFile ) ) return false;

    return true;
}

IMPLEMENT_DYNAMIC( EvalDlg, CDialog )

EvalDlg::EvalDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( EvalDlg::IDD, pParent )
    , m_regulateDB( _T( "" ) )
    , m_outputReport( _T( "" ) )
{

}

EvalDlg::~EvalDlg()
{
}

void EvalDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Text( pDX, IDC_EVAL_DB_TEMPLATE_PATH, m_regulateDB );
    DDX_Control( pDX, IDC_EVAL_REGULATION_LIST, m_list );
    DDX_Text( pDX, IDC_EVAL_OUTPUT_EDIT, m_outputReport );
}


BEGIN_MESSAGE_MAP( EvalDlg, CDialog )
    ON_BN_CLICKED( IDC_EVAL_DB_TEMPLATE_PATH_SEL_BUTTON, &EvalDlg::OnBnClickedEvalDbSelBtn )
    ON_BN_CLICKED( IDC_EVAL_OUTPUT_BUTTON, &EvalDlg::OnBnClickedEvalOutputButton )
    ON_BN_CLICKED( IDC_EVAL_DO_BTN, &EvalDlg::OnBnClickedEvalDoBtn )
END_MESSAGE_MAP()


// EvalDlg ��Ϣ�������

void EvalDlg::OnBnClickedEvalDbSelBtn()
{
    CFileDialog openDialog(
        TRUE,
        _T( "mdb" ),
        NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
        _T( "Access2003���ݿ�(*.mdb)|*.mdb||" ) );

    if( IDOK == openDialog.DoModal() )
    {
        m_regulateDB = openDialog.GetPathName();
        UpdateData( FALSE );        // ��ʾ�ڱ༭����

        readFromDB(); // ��ȡ���ݿ�
    }
}

void EvalDlg::OnBnClickedEvalOutputButton()
{
    CFileDialog SaveDialog( FALSE, _T( "*doc" ), NULL, OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T( "Word2003�ĵ�(*.doc)|*.doc||" ) );

    if( IDOK == SaveDialog.DoModal() )
    {
        m_outputReport = SaveDialog.GetPathName();
        UpdateData( FALSE ); // ��ʾ���·��
    }
}

void EvalDlg::OnBnClickedEvalDoBtn()
{
    UpdateData( TRUE );

    if( !checkInput() ) return; // �������ʧ��
    if( !checkOutput() ) return; // ������ʧ��

    CString dir = GetPathDir( m_regulateDB );
    if( dir.CompareNoCase( GetPathDir( m_outputReport ) ) == 0 )
    {
        MessageBox( _T( "�����桿���۱��治������������۱������ݿ�����·��\n\t��ѡ������·��!!!" ) );
        m_outputReport = _T( "" );
        UpdateData( FALSE );
        return;
    }

    if( !ConnectivityHelper::DAG() ) // ����dag--�����޻�ͼ
    {
        CString msg;
        msg.Format( _T( "ͨ��ϵͳ�д��ڵ����·\n��ʹ�á�ͨ����״���ۡ��˵��еġ������顿���߽��м��" ) );
        MessageBox( msg );
        return;
    }

    // �������������ݿ���ͬһĿ¼�µ���ʱ�ļ�
    cleanTempFile( dir );

    // ѯ��
    if( IDYES != MessageBox( _T( "�Ƿ�ִ������" ), _T( "��ʾ" ), MB_YESNO ) ) return;

    // ִ������
    doNormalEvalWithoutUnit( dir );

    // ���ɱ����õ�һЩ�м������ļ�
    if( generateReport( dir ) )
    {
        CString msg;
        msg.Format( _T( "������ɣ���һ���������۱���!!!" ) );
        msg.AppendFormat( _T( "\n��ʾ:" ) );
        msg.AppendFormat( _T( "\n\t1) ���ɱ���ǰ�뱣�沢�ر��Ѵ򿪵�word�ĵ�" ) );
        msg.AppendFormat( _T( "\n\t2) ���word����\"��ȫ����\"�Ի�����ѡ��\"���ú�\"" ) );
        MessageBox( msg );

        // �ڹ����Ϣ���ݿ�����Ŀ¼�´����۱���ģ��
        // ���۱���ģ�����vba���룬�Զ����ɱ���
        CString filePath;
        filePath.Format( _T( "%s\\%s.doc" ), dir, EVAL_REPORT_TEMPLATE_FILE_NAME );
        OpenWordDocument( filePath );

        MessageBox( _T( "�������۱���ɹ�!" ) );

        // �����ɵı���
        OpenWordDocument( m_outputReport );
    }
    else
    {
        MessageBox( _T( "�������۱���ʧ��!" ) );
    }

    // �����ʱ�ļ�
    cleanTempFile( dir );

    OnOK();
}

// �ж��Ƿ���Ч��mdb�ļ�
static bool IsValidMDB( const CString& path )
{
    bool ret = false;
    if( IsFileExist( path ) )
    {
        TCHAR ext[_MAX_EXT];
        _tsplitpath( path, NULL, NULL, NULL, ext );
        //acutPrintf(_T("\n��׺:%s"), ext);
        if( CString( ext ).CompareNoCase( _T( ".mdb" ) ) == 0 )
        {
            ret = true;
        }
    }
    return ret;
}

BOOL EvalDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    initListCtrl();

    if( IsValidMDB( m_regulateDB ) )
    {
        readFromDB();
    }

    return TRUE;
}

void EvalDlg::initListCtrl()
{
    m_list.SetExtendedStyle( m_list.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
    LVCOLUMN lvcol;
    lvcol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
    lvcol.fmt = LVCFMT_CENTER;
    lvcol.iSubItem = 1;

    // ���������һ��(��"��Ӧ�����ݿ��")
    const int len = sizeof( REGULATION_INFO ) / sizeof( REGULATION_INFO[0] );

    assert( len > 1 );

    CRect rect;
    m_list.GetClientRect( &rect );
    lvcol.cx = rect.Width() / ( len - 1 );

    for( int i = 0; i < len - 1; i++ )
    {
        lvcol.pszText = const_cast<LPTSTR>( REGULATION_INFO[i] );
        m_list.InsertColumn( i, &lvcol );
    }
}

void EvalDlg::readFromDB()
{
    m_tables.removeAll();
    //this->m_list.DeleteAllItems();

    // ��ʼ��ȡ���ݣ������뵽listctrl
    const int len = sizeof( REGULATION_INFO ) / sizeof( REGULATION_INFO[0] );
    UINT nIndex = 0 ;

    CDaoDatabase db;
    try
    {
        db.Open( m_regulateDB );

        CDaoRecordset rs( &db );
        CString sql;
        sql.Format( _T( "select * from %s" ), REGULATION_INFO_MAIN_TABLE_NAME ); // ��ȡregulationInfo��
        rs.Open( dbOpenDynaset, sql, 0 );

        rs.MoveFirst () ;
        while( !rs.IsEOF() )
        {
            // ���б�ؼ���Ӽ�¼
            CString idStr;
            idStr.Format( _T( "%d" ), nIndex + 1 );
            this->m_list.InsertItem( nIndex, idStr );

            for( int i = 0; i < len ; i++ )
            {
                if( i == 0 )
                {
                    m_list.SetItemText( nIndex, 0, idStr );
                }
                else
                {
                    COleVariant v;
                    rs.GetFieldValue( REGULATION_INFO[i] , v );
                    if( i == len - 1 ) // ���������һ��, ��¼��m_tables��
                    {
                        m_tables.append( ( LPCTSTR )v.bstrVal );
                    }
                    else
                    {
                        this->m_list.SetItemText( nIndex, i, ( LPCTSTR )v.bstrVal );
                    }
                }
            }
            // �Ƶ���һ����¼
            rs.MoveNext () ;
            nIndex++ ;
        }

        rs.Close();
        db.Close();
    }
    catch( CDaoException* e )
    {
        e->ReportError();
        e->Delete();

        if( db.IsOpen() ) db.Close();

        // �쳣����
        m_tables.removeAll();
        m_regulateDB = _T( "" );
        UpdateData( FALSE ); // ����
    }

    // ѡ�е�1��
    m_list.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED );
}

CString EvalDlg::getSelRegulationTableName()
{
    int row = 0;
    for( int i = 0; i < m_list.GetItemCount(); i++ )
    {
        if( LVIS_SELECTED == m_list.GetItemState( i, LVIS_SELECTED ) )
        {
            row = i;
            break;
        }
    }
//	return m_list.GetItemText(row, 4); // ��ȡtable name
    return m_tables[row].kACharPtr();
}

void EvalDlg::doNormalEvalWithoutUnit( const CString& outputDirPath )
{
    // �ռ���ѡ��Ĺ��������������
    CString regulationName = getSelRegulationTableName();
    if( regulationName.GetLength() == 0 ) return ;

    EvalService* pEvalService = EvalService::CreateInstance();

    // ���ز��
    // Ŀǰ��ʱ��ô����
    // �����ٿ��ǲ������...
    pEvalService->loadPlugin( BuildPath( GetAppPathDir(), _T( "CMSR2010.arx" ) ) );

    // �������۽�����ݿ�·��
    // �����ݿ��뱨�����ͬһ��Ŀ¼��
    CString evalResultDBPath;
    evalResultDBPath.Format( _T( "%s\\%s.mdb" ), outputDirPath, EVAL_RESULT_DATA_BASE_NAME );
    pEvalService->setEvalResultDataBasePath( evalResultDBPath );

    /* ǰ�᣺�����̵�������ʵ��(������һ������ʵ��) */

    // �ռ���Ҫ���۵�������
    // Ŀǰ��ʵ���Ǵ����ݿ��ж�ȡ���еı��
    AcDbIntArray allClauses;
    GetClauseNums( m_regulateDB, regulationName, allClauses );

    // ִ�����ۣ��������д�뵽���ݿ�
    pEvalService->doRegEval( regulationName, allClauses );

    EvalService::DestroyInstance();

    MessageBox( _T( "����������δʵ���������ۣ���ȡ�˹�ѯ�ʵķ�ʽ��\n���û������ֳ�ʵ������������۽���" ) );

    /* ִ���˹����� */
    QuestionNaire( m_regulateDB, regulationName, evalResultDBPath, TOTAL_EVAL_RESULT_TABLE_NAME );
}

bool EvalDlg::generateReport( const CString& outputPath )
{
    CString path;
    path.Format( _T( "%s\\%s.mdb" ), outputPath, EVAL_RESULT_DATA_BASE_NAME );
    bool regulationImpl = IsFileExist( path );
    // ���۽�����ݿⲻ���ڣ���ʾ���û��ʵ��
    if( !regulationImpl ) return false;

    return writeEvalDataIniFile( outputPath, regulationImpl );
}

bool EvalDlg::writeEvalDataIniFile( const CString& outputPath, bool regulationImpl )
{
    // ���������ļ�֮ǰɾ���Ѵ��ڵ������ļ�
    CString path;
    path.Format( _T( "%s\\%s.ini" ), outputPath, EVAL_DATA_INFO_FILE_NAME );
    if( !CreateEmptyFile( path ) )
    {
        MessageBox( _T( "���������ļ�ʧ��!" ) );
        return false;
    }

    CIniFile iniFile;
    iniFile.SetPath( path );

    // �����Ϣ
    iniFile.SetKeyValue( REGULATION_INFO_SECTION, REGULATION_DATABASE_PATH_KEY, m_regulateDB );
    iniFile.SetKeyValue( REGULATION_INFO_SECTION, REGULATION_TABLE_KEY, getSelRegulationTableName() );

    // ���۽��
    iniFile.SetKeyValue( EVAL_RESULT_SECTION, EVAL_RESULT_KEY, ( regulationImpl ? "1" : "0" ) );

    // ���۱���·��
    iniFile.SetKeyValue( EVAL_REPORT_FILE_NAME_SECTION, EVAL_REPORT_FILE_NAME_KEY, m_outputReport );

    // ����Ҫд����������Ϣ
    // ����Ϣ��������Ŀ��Ϣ
    bool ret = WriteExtraData( iniFile );

    if( !ret )
    {
        RemoveFile( path ); // д������ʧ�ܣ�ɾ��ini�ļ�
    }
    return ret;
}

bool EvalDlg::checkInput()
{
    if( m_regulateDB.GetLength() == 0 )
    {
        MessageBox( _T( "��ѡ���������ݿ�!" ) );
        return false;
    }

    if( m_list.GetItemCount() == 0 )
    {
        MessageBox( _T( "�������ݿ���û���κι����Ϣ!" ) );
        return false;
    }

    CString regulationTable = getSelRegulationTableName();
    if( regulationTable.GetLength() == 0 )
    {
        MessageBox( _T( "ѡ�еĹ�̲������κ�������Ϣ!" ) );
        return false;
    }

    return true;
}

bool EvalDlg::checkOutput()
{
    if( m_outputReport.GetLength() == 0 )
    {
        MessageBox( _T( "��ѡ�񱨸����·��!" ) );
        return false;
    }
    return true;
}

void EvalDlg::cleanTempFile( const CString& outputPath )
{
    CString path;
    path.Format( _T( "%s\\%s.ini" ), outputPath, EVAL_DATA_INFO_FILE_NAME );
    RemoveFile( path );

    path.Format( _T( "%s\\%s.mdb" ), outputPath, EVAL_RESULT_DATA_BASE_NAME );
    RemoveFile( path );

    path.Format( _T( "%s\\%s.mdb" ), outputPath, _T( "afc" ) );
    RemoveFile( path );
}