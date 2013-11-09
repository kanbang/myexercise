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

/* 全局函数(实现在QuestionNaire.cpp) */
extern void QuestionNaire( const CString& regulateDB, const CString& regulationTable,
                           const CString& evalResultDBPath, const CString& caclResultTable );

// 作为listCtrl的标题头(header)
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
        sql.Format( _T( "select * from %s" ), tblName );  // 读取规程表--tablename
        rs.Open( dbOpenDynaset, sql, 0 );

        rs.MoveFirst () ;
        while( !rs.IsEOF() )
        {
            COleVariant v;
            rs.GetFieldValue( CLAUSE_NUM_FIELD , v ); // 读取条款编号
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

        allClauses.removeAll(); // 出现异常，清空
    }
}

static bool WriteCoalMineInfo( CIniFile& iniFile )
{
    // "矿井信息"数据对象
    AcDbObjectId objId;
    if( !SingleDataObjectHelper::GetObjectId( COAL_MINE_SECTION, objId ) )
    {
        CString msg;
        msg.Format( _T( "【%s】数据对象不存在" ), COAL_MINE_SECTION );
        AfxMessageBox( msg );
        return false;
    }

    // "名称"
    CString v1;
    if( !DataHelper::GetPropertyData( objId, COAL_MINE_NAME_KEY, v1 ) ) return false;

    // "隶属集团公司"
    CString v2;
    if( !DataHelper::GetPropertyData( objId, COMPANY_NAME_KEY, v2 ) ) return false;

    // "通风需风系数"
    // 如果为空(未输入数据)，默认为1.15
    CString v3;
    if( !DataHelper::GetPropertyData( objId, COAL_MINE_AIR_FLOW_COEFF_KEY, v3 ) ) return false;

    // "通风系数"
    // 如果为空(未输入数据)，默认为1.15
    CString v4;
    if( !DataHelper::GetPropertyData( objId, COAL_MINE_AIR_FLOW_COEFF2_KEY, v4 ) ) return false;

    if( v1.GetLength() == 0 ) v1 = _T( "xxx矿" );
    if( v2.GetLength() == 0 ) v2 = _T( "xxx集团公司" );
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
        msg.Format( _T( "【%s】数据对象不存在" ), EVAL_PROJECT_INFO_SECTION );
        AfxMessageBox( msg );
        return false;
    }

    // “评价依据”
    CString v1;
    if( !DataHelper::GetPropertyData( objId, EVAL_REGULATION_KEY, v1 ) ) return false;

    // "制定评价方案开始时间"
    CString v2;
    if( !DataHelper::GetPropertyData( objId, PREPARE_SOLUTION_BEGIN_KEY, v2 ) ) return false;

    // "制定评价方案结束时间"
    CString v3;
    if( !DataHelper::GetPropertyData( objId, PREPARE_SOLUTION_END_KEY, v3 ) ) return false;

    // "收集评价资料开始时间"
    CString v4;
    if( !DataHelper::GetPropertyData( objId, PREPARE_INFO_BEGIN_KEY, v4 ) ) return false;

    // "收集评价资料结束时间"
    CString v5;
    if( !DataHelper::GetPropertyData( objId, PREPARE_INFO_END_KEY, v5 ) ) return false;

    // "签订评价合同开始时间"
    CString v6;
    if( !DataHelper::GetPropertyData( objId, PREPARE_CONTRACT_BEGIN_KEY, v6 ) ) return false;

    // "签订评价合同结束时间"
    CString v7;
    if( !DataHelper::GetPropertyData( objId, PREPARE_CONTRACT_END_KEY, v7 ) ) return false;

    // "评价宣传贯彻开始时间"
    CString v8;
    if( !DataHelper::GetPropertyData( objId, SURVEY_LEARN_BEGIN_KEY, v8 ) ) return false;

    // "评价宣传贯彻结束时间"
    CString v9;
    if( !DataHelper::GetPropertyData( objId, SURVEY_LEARN_END_KEY, v9 ) ) return false;

    // "开展评价工作开始时间"
    CString v10;
    if( !DataHelper::GetPropertyData( objId, WORK_DO_BEGIN_KEY, v10 ) ) return false;

    // "开展评价工作结束时间"
    CString v11;
    if( !DataHelper::GetPropertyData( objId, WORK_DO_END_KEY, v11 ) ) return false;

    // "编制评价报告开始时间"
    CString v12;
    if( !DataHelper::GetPropertyData( objId, WORK_REPORT_BEGIN_KEY, v12 ) ) return false;

    // "编制评价报告结束时间"
    CString v13;
    if( !DataHelper::GetPropertyData( objId, WORK_REPORT_END_KEY, v13 ) ) return false;

    if( v1.GetLength() == 0 ) v1 = _T( "煤矿安全规程" );

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
    // 1、写入矿井信息
    if( !WriteCoalMineInfo( iniFile ) ) return false;

    // 2、写入评价项目信息
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


// EvalDlg 消息处理程序

void EvalDlg::OnBnClickedEvalDbSelBtn()
{
    CFileDialog openDialog(
        TRUE,
        _T( "mdb" ),
        NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
        _T( "Access2003数据库(*.mdb)|*.mdb||" ) );

    if( IDOK == openDialog.DoModal() )
    {
        m_regulateDB = openDialog.GetPathName();
        UpdateData( FALSE );        // 显示在编辑框中

        readFromDB(); // 读取数据库
    }
}

void EvalDlg::OnBnClickedEvalOutputButton()
{
    CFileDialog SaveDialog( FALSE, _T( "*doc" ), NULL, OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T( "Word2003文档(*.doc)|*.doc||" ) );

    if( IDOK == SaveDialog.DoModal() )
    {
        m_outputReport = SaveDialog.GetPathName();
        UpdateData( FALSE ); // 显示输出路径
    }
}

void EvalDlg::OnBnClickedEvalDoBtn()
{
    UpdateData( TRUE );

    if( !checkInput() ) return; // 检查输入失败
    if( !checkOutput() ) return; // 检查输出失败

    CString dir = GetPathDir( m_regulateDB );
    if( dir.CompareNoCase( GetPathDir( m_outputReport ) ) == 0 )
    {
        MessageBox( _T( "【警告】评价报告不允许输出到评价报告数据库所在路径\n\t请选择其它路径!!!" ) );
        m_outputReport = _T( "" );
        UpdateData( FALSE );
        return;
    }

    if( !ConnectivityHelper::DAG() ) // 不是dag--有向无环图
    {
        CString msg;
        msg.Format( _T( "通风系统中存在单向回路\n请使用【通风现状评价】菜单中的【网络检查】工具进行检查" ) );
        MessageBox( msg );
        return;
    }

    // 清理与评价数据库在同一目录下的临时文件
    cleanTempFile( dir );

    // 询问
    if( IDYES != MessageBox( _T( "是否执行评价" ), _T( "提示" ), MB_YESNO ) ) return;

    // 执行评价
    doNormalEvalWithoutUnit( dir );

    // 生成报告用的一些中间数据文件
    if( generateReport( dir ) )
    {
        CString msg;
        msg.Format( _T( "评价完成，下一步生成评价报告!!!" ) );
        msg.AppendFormat( _T( "\n提示:" ) );
        msg.AppendFormat( _T( "\n\t1) 生成报告前请保存并关闭已打开的word文档" ) );
        msg.AppendFormat( _T( "\n\t2) 如果word弹出\"安全警告\"对话框，请选择\"启用宏\"" ) );
        MessageBox( msg );

        // 在规程信息数据库所在目录下打开评价报告模板
        // 评价报告模板包含vba代码，自动生成报告
        CString filePath;
        filePath.Format( _T( "%s\\%s.doc" ), dir, EVAL_REPORT_TEMPLATE_FILE_NAME );
        OpenWordDocument( filePath );

        MessageBox( _T( "生成评价报告成功!" ) );

        // 打开生成的报告
        OpenWordDocument( m_outputReport );
    }
    else
    {
        MessageBox( _T( "生成评价报告失败!" ) );
    }

    // 清除临时文件
    cleanTempFile( dir );

    OnOK();
}

// 判断是否有效的mdb文件
static bool IsValidMDB( const CString& path )
{
    bool ret = false;
    if( IsFileExist( path ) )
    {
        TCHAR ext[_MAX_EXT];
        _tsplitpath( path, NULL, NULL, NULL, ext );
        //acutPrintf(_T("\n后缀:%s"), ext);
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

    // 不插入最后一列(列"对应的数据库表")
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

    // 开始读取数据，并插入到listctrl
    const int len = sizeof( REGULATION_INFO ) / sizeof( REGULATION_INFO[0] );
    UINT nIndex = 0 ;

    CDaoDatabase db;
    try
    {
        db.Open( m_regulateDB );

        CDaoRecordset rs( &db );
        CString sql;
        sql.Format( _T( "select * from %s" ), REGULATION_INFO_MAIN_TABLE_NAME ); // 读取regulationInfo表
        rs.Open( dbOpenDynaset, sql, 0 );

        rs.MoveFirst () ;
        while( !rs.IsEOF() )
        {
            // 在列表控件添加记录
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
                    if( i == len - 1 ) // 不插入最后一项, 记录在m_tables中
                    {
                        m_tables.append( ( LPCTSTR )v.bstrVal );
                    }
                    else
                    {
                        this->m_list.SetItemText( nIndex, i, ( LPCTSTR )v.bstrVal );
                    }
                }
            }
            // 移到下一条记录
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

        // 异常处理
        m_tables.removeAll();
        m_regulateDB = _T( "" );
        UpdateData( FALSE ); // 更新
    }

    // 选中第1个
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
//	return m_list.GetItemText(row, 4); // 获取table name
    return m_tables[row].kACharPtr();
}

void EvalDlg::doNormalEvalWithoutUnit( const CString& outputDirPath )
{
    // 收集所选择的规程中所有条款编号
    CString regulationName = getSelRegulationTableName();
    if( regulationName.GetLength() == 0 ) return ;

    EvalService* pEvalService = EvalService::CreateInstance();

    // 加载插件
    // 目前暂时这么处理
    // 后期再考虑插件加载...
    pEvalService->loadPlugin( BuildPath( GetAppPathDir(), _T( "CMSR2010.arx" ) ) );

    // 设置评价结果数据库路径
    // 将数据库与报告放在同一个目录下
    CString evalResultDBPath;
    evalResultDBPath.Format( _T( "%s\\%s.mdb" ), outputDirPath, EVAL_RESULT_DATA_BASE_NAME );
    pEvalService->setEvalResultDataBasePath( evalResultDBPath );

    /* 前提：假设规程的评价有实现(至少有一个条款实现) */

    // 收集需要评价的条款编号
    // 目前的实现是从数据库中读取所有的编号
    AcDbIntArray allClauses;
    GetClauseNums( m_regulateDB, regulationName, allClauses );

    // 执行评价，并将结果写入到数据库
    pEvalService->doRegEval( regulationName, allClauses );

    EvalService::DestroyInstance();

    MessageBox( _T( "部分条款尚未实现智能评价，采取人工询问的方式，\n由用户根据现场实际情况给出评价结论" ) );

    /* 执行人工调查 */
    QuestionNaire( m_regulateDB, regulationName, evalResultDBPath, TOTAL_EVAL_RESULT_TABLE_NAME );
}

bool EvalDlg::generateReport( const CString& outputPath )
{
    CString path;
    path.Format( _T( "%s\\%s.mdb" ), outputPath, EVAL_RESULT_DATA_BASE_NAME );
    bool regulationImpl = IsFileExist( path );
    // 评价结果数据库不存在，表示规程没有实现
    if( !regulationImpl ) return false;

    return writeEvalDataIniFile( outputPath, regulationImpl );
}

bool EvalDlg::writeEvalDataIniFile( const CString& outputPath, bool regulationImpl )
{
    // 生成数据文件之前删除已存在的数据文件
    CString path;
    path.Format( _T( "%s\\%s.ini" ), outputPath, EVAL_DATA_INFO_FILE_NAME );
    if( !CreateEmptyFile( path ) )
    {
        MessageBox( _T( "生成数据文件失败!" ) );
        return false;
    }

    CIniFile iniFile;
    iniFile.SetPath( path );

    // 规程信息
    iniFile.SetKeyValue( REGULATION_INFO_SECTION, REGULATION_DATABASE_PATH_KEY, m_regulateDB );
    iniFile.SetKeyValue( REGULATION_INFO_SECTION, REGULATION_TABLE_KEY, getSelRegulationTableName() );

    // 评价结果
    iniFile.SetKeyValue( EVAL_RESULT_SECTION, EVAL_RESULT_KEY, ( regulationImpl ? "1" : "0" ) );

    // 评价报告路径
    iniFile.SetKeyValue( EVAL_REPORT_FILE_NAME_SECTION, EVAL_REPORT_FILE_NAME_KEY, m_outputReport );

    // 还需要写入其它的信息
    // 矿井信息及评价项目信息
    bool ret = WriteExtraData( iniFile );

    if( !ret )
    {
        RemoveFile( path ); // 写入数据失败，删除ini文件
    }
    return ret;
}

bool EvalDlg::checkInput()
{
    if( m_regulateDB.GetLength() == 0 )
    {
        MessageBox( _T( "请选择评价数据库!" ) );
        return false;
    }

    if( m_list.GetItemCount() == 0 )
    {
        MessageBox( _T( "评价数据库中没有任何规程信息!" ) );
        return false;
    }

    CString regulationTable = getSelRegulationTableName();
    if( regulationTable.GetLength() == 0 )
    {
        MessageBox( _T( "选中的规程不包含任何条款信息!" ) );
        return false;
    }

    return true;
}

bool EvalDlg::checkOutput()
{
    if( m_outputReport.GetLength() == 0 )
    {
        MessageBox( _T( "请选择报告输出路径!" ) );
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