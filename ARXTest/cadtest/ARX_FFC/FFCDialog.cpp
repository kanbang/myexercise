#include "stdafx.h"
#include "FFCDialog.h"

#include "SysTool.h"
#include "FluentTool.h"
#include "FFC_Solve.h"
#include "ThreadData.h"

#include "TecplotShowTypeDlg.h"

// 关于log的代码来源:
// http://www.codeproject.com/KB/edit/RichEditLog_Demo.aspx
//	1) AppendToLog()
//	2) AppendToLogAndScroll()
//	3) GetNumVisibleLines()

// 自定义消息 -- 用于追加日志
#define WM_APPEND_LOG WM_USER+1
// 自定义消息 -- 用于清空日志
#define WM_CLEAR_LOG WM_USER+2
// 自定义消息 -- 用于创建线程
#define WM_CREATE_THREAD WM_USER+3
// 自定义消息 -- 用于关闭线程
#define WM_CLOSE_THREAD WM_USER+4

// 发送消息追加日志
static void AppendLogByMsg( HWND hWnd, const CString& str, COLORREF color = RGB( 0, 0 , 0 ) )
{
    Sleep( 0 );

    // 传递给全局变量(避免一些未知的线程错误)
    global_log_msg.str.Format( _T( "%s\n" ), str ); // 自动添加换行
    global_log_msg.color = color;

    SendMessage( hWnd, WM_APPEND_LOG, ( WPARAM )&global_log_msg, 0 );
}

// 发送消息清空日志
static void ClearLogByMsg( HWND hWnd )
{
    SendMessage( hWnd, WM_CLEAR_LOG, 0, 0 );
}

// 关闭线程
static void CloseThreadByMsg( HWND hWnd )
{
    SendMessage( hWnd, WM_CLOSE_THREAD, 0, 0 );
}

static void ClearLog( CRichEditCtrl* pCtrl )
{
    pCtrl->SetFocus();
    pCtrl->SetReadOnly( FALSE );

    pCtrl->SetSel( 0, -1 );
    pCtrl->Clear();

    pCtrl->SetReadOnly( TRUE );
}

IMPLEMENT_DYNAMIC( FFCDialog, CDialog )

FFCDialog::FFCDialog( CWnd* pParent /*=NULL*/ )
    : CDialog( FFCDialog::IDD, pParent )
{

}

FFCDialog::~FFCDialog()
{
}

void FFCDialog::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_FFC_LOG_CTRL, m_ctrlLog );
}

BEGIN_MESSAGE_MAP( FFCDialog, CDialog )
    ON_BN_CLICKED( IDC_FFC_SOLVE_BTN, &FFCDialog::OnBnClickedFfcSolveBtn )
    ON_BN_CLICKED( IDC_FFC_DISPLAY_BTN, &FFCDialog::OnBnClickedFfcDisplayBtn )
    ON_BN_CLICKED( IDC_FFC_KILL_BTN, &FFCDialog::OnBnClickedFfcKillBtn )

    ON_MESSAGE( WM_APPEND_LOG, OnAppendLog )
    ON_MESSAGE( WM_CLEAR_LOG, OnClearLog )
    ON_MESSAGE( WM_CLOSE_THREAD, OnCloseThread )
END_MESSAGE_MAP()

static void MsgWaitForThread( HANDLE hThread )
{
    // http://www.cnblogs.com/Sunwayking/articles/1976980.html
    while( TRUE )
    {
        //wait for m_hThread to be over，and wait for
        //QS_ALLINPUT（Any message is in the queue)
        DWORD dwRet = MsgWaitForMultipleObjects( 1, &hThread, FALSE, INFINITE, QS_ALLINPUT );
        if( WAIT_OBJECT_0 + 1 == dwRet )
        {
            //get the message from Queue
            //and dispatch it to specific window
            MSG msg;
            PeekMessage( &msg, NULL, 0, 0, PM_REMOVE );
            DispatchMessage( &msg );
            continue;
        }
        else
        {
            break;
        }
    }
}

static bool PrepareWork( HWND hWnd )
{
    CString appDir = GetAppPathDir();

    // kill所有的gambit和fluent进程
    // 同时删除所有的临时文件
    CleanWork( appDir );

    // 发送日志消息
    ClearLogByMsg( hWnd );
    AppendLogByMsg( hWnd, _T( "(一) 准备工作" ), RGB( 0, 0, 0 ) );
    AppendLogByMsg( hWnd, _T( "初始化模拟计算环境" ), RGB( 0, 0, 0 ) );

    {
        CString msg;
        msg.Format( _T( "当前路径:%s" ), appDir );
        AppendLogByMsg( hWnd, msg, RGB( 0, 0, 255 ) );
    }

    // 生成ffc_var.jou文件
    GEO_ERROR_CODE gec = GenGeometryAndData( appDir );
    if( gec != GEC_NO_ERROR )
    {
        CString msg;
        msg.Format( _T( "【错误】%s" ), GetGeoErrorMessage( gec ) );
        AppendLogByMsg( hWnd, msg, RGB( 255, 0, 0 ) );
    }

    return ( gec == GEC_NO_ERROR );
}

static void RunGambitThread( HWND hWnd )
{
    AppendLogByMsg( hWnd, _T( "(二) 几何建模" ), RGB( 0, 0, 0 ) );
    AppendLogByMsg( hWnd, _T( "后台调用Gambit..." ), RGB( 0, 0, 0 ) );

    CString appDir = GetAppPathDir();

    HANDLE hThread;
    RUN_ERROR_CODE rec = RunGambit( appDir, hThread );
    if( REC_NO_EEOR != rec )
    {
        CString msg;
        msg.Format( _T( "【错误】%s" ), GetGambitErrorMessage( rec ) );
        AppendLogByMsg( hWnd, msg, RGB( 255, 0, 0 ) );
    }
    else
    {
        //MsgWaitForThread(hThread);
        WaitForSingleObject( hThread, INFINITE );
        CloseHandle( hThread );

        // 判断运行结果状态
        if( IsRunGambitOk( appDir ) )
        {
            AppendLogByMsg( hWnd, _T( "几何建模成功" ), RGB( 0, 0, 0 ) );

            // 通知主线程该线程运行结束
            // 同时调用下一个计算过程(Fluent)
            CloseThreadByMsg( hWnd );
        }
        else
        {
            AppendLogByMsg( hWnd, _T( "几何建模失败" ), RGB( 0, 0, 0 ) );
        }
    }
}

static void RunFluentThread( HWND hWnd )
{
    AppendLogByMsg( hWnd, _T( "(三) 模拟计算" ), RGB( 0, 0, 0 ) );
    AppendLogByMsg( hWnd, _T( "后台调用Fluent..." ), RGB( 0, 0, 0 ) );

    CString appDir = GetAppPathDir();

    HANDLE hThread;
    RUN_ERROR_CODE rec = RunFluent( appDir, hThread );
    if( REC_NO_EEOR != rec )
    {
        CString msg;
        msg.Format( _T( "【错误】%s" ), GetFluentErrorMessage( rec ) );
        AppendLogByMsg( hWnd, msg, RGB( 255, 0, 0 ) );
    }
    else
    {
        AppendLogByMsg( hWnd, _T( "【提示】计算过程可能需要耗费较长时间，请耐心等待..." ), RGB( 255, 0, 0 ) );

        //MsgWaitForThread(hThread);
        WaitForSingleObject( hThread, INFINITE );
        CloseHandle( hThread );

        //Sleep(1000);
        // 判断运行结果状态
        if( IsRunFluentOk( appDir ) )
        {
            AppendLogByMsg( hWnd, _T( "模拟计算成功" ), RGB( 0, 0, 0 ) );

            // 通知主线程该线程运行结束
            // 同时调用下一个计算过程(Fluent)
            CloseThreadByMsg( hWnd );
        }
        else
        {
            AppendLogByMsg( hWnd, _T( "模拟计算失败" ), RGB( 0, 0, 0 ) );
        }
    }
}

static DWORD FFC_Thread( LPVOID lpParam )
{
    FFCThreadData* pData = ( FFCThreadData* )lpParam;
    HWND hWnd = pData->hWnd;
    int state = pData->state;

    switch( state )
    {
    case 1:
        RunGambitThread( hWnd );
        break;

    case 2:
        RunFluentThread( hWnd );
        break;
    }

    return 0;
}

static void CreateWorkThread( HWND hWnd, int state )
{
    // 创建监视线程
    //	1) 线程数据
    FFCThreadData data = {hWnd, state};

    // 传递给全局变量(避免一些未知的线程错误)
    global_ffc_thread_data = data;

    //	2) 创建线程
    DWORD dwThreadId;
    HANDLE m_hThread = CreateThread(
                           NULL,              // default security attributes
                           0,                 // use default stack size
                           ( LPTHREAD_START_ROUTINE )FFC_Thread,        // thread function
                           &data,             // argument to thread function
                           0,                 // use default creation flags
                           &dwThreadId );  // returns the thread identifier

    // 等待线程，同时处理消息
    MsgWaitForThread( m_hThread );
    // 关闭句柄
    CloseHandle( m_hThread );
}

void FFCDialog::SolveFFC()
{
    // 从第一步开始(gambit)
    ffc_step = 1;
    UserAskForThreadEnd( false );

    HWND hWnd = this->GetSafeHwnd();

    // 初始化模拟环境
    if( !PrepareWork( hWnd ) ) return;

    // 开始执行计算
    CreateWorkThread( hWnd, ffc_step );
}

void FFCDialog::OnBnClickedFfcSolveBtn()
{
    GetDlgItem( IDC_FFC_SOLVE_BTN )->EnableWindow( FALSE );

    SolveFFC();

    GetDlgItem( IDC_FFC_SOLVE_BTN )->EnableWindow( TRUE );
}

static int AskForTecplotShowType()
{
    TecplotShowTypeDlg dlg;
    if( IDOK != dlg.DoModal() )
        return -1;
    else
        return dlg.m_tst;
}

void FFCDialog::OnBnClickedFfcDisplayBtn()
{
    HWND hWnd = this->GetSafeHwnd();

    if( IsRunning() )
    {
        AppendLogByMsg( hWnd, _T( "【提示】正在进行模拟计算，没有数据可以显示，请耐心等待计算结果..." ), RGB( 255, 0, 0 ) );
        return;
    }

    // 完整计算过程:
    // 1->2 (gambit几何建模,成功后发送消息开始fluent模拟)
    // 2->3 (fluent模拟，成功后发送消息)
    if( ffc_step < 3 )
    {
        AppendLogByMsg( hWnd, _T( "【提示】模拟计算失败，无法显示图形" ), RGB( 255, 0, 0 ) );
        return;
    }

    // 获取要显示的图形类型
    int tst = AskForTecplotShowType();
    if( tst == -1 )
    {
        AppendLogByMsg( hWnd, _T( "【提示】用户取消显示图形" ), RGB( 255, 0, 0 ) );
        return;
    }
    else
    {
        CString msg;
        msg.Format( _T( "-->显示%s" ), GetTecplotTypeName( ( TECPLOT_SHOW_TYPE )tst ) );
        AppendLogByMsg( hWnd, msg, RGB( 0, 0, 0 ) );
    }

    CString appDir = GetAppPathDir();
    RUN_ERROR_CODE rst = RunTecplot( appDir, tst );
    if( rst != REC_NO_EEOR )
    {
        CString msg;
        msg.Format( _T( "【错误】%s" ), GetTecplotErrorMessage( rst ) );
        AppendLogByMsg( hWnd, msg, RGB( 255, 0, 0 ) );
    }
}

void FFCDialog::OnBnClickedFfcKillBtn()
{
    if( IsRunning() )
    {
        if( IDYES == MessageBox( _T( "正在进行模拟计算，确定要强制结束当前计算过程?" ), _T( "警告" ), MB_YESNO ) )
        {
            //ffc_step = 1;
            CleanWork( GetAppPathDir() );
            UserAskForThreadEnd( true ); // 用户发出消息要求结束线程
            AppendLogByMsg( this->GetSafeHwnd(), _T( "【提示】用户强制结束计算过程..." ), RGB( 255, 0, 0 ) );
        }
    }
}

bool FFCDialog::IsRunning() const
{
    return ( FALSE == GetDlgItem( IDC_FFC_SOLVE_BTN )->IsWindowEnabled() );
}

LRESULT FFCDialog::OnAppendLog( WPARAM wParam, LPARAM lParam )
{
    LogMsg* pMsg = ( LogMsg* )wParam;
    AppendToLogAndScroll( pMsg->str, pMsg->color );
    return 0;
}

LRESULT FFCDialog::OnClearLog( WPARAM wParam, LPARAM lParam )
{
    ClearLog( &m_ctrlLog );
    return 0;
}

LRESULT FFCDialog::OnCloseThread( WPARAM wParam, LPARAM lParam )
{
    if( ffc_step < 3 )
    {
        ffc_step++;
        CreateWorkThread( this->GetSafeHwnd(), ffc_step );
    }
    return 0;
}

//-----------------------------------------------------------------------------
//  AppendToLog()
///
/// \brief	Add a string to the log window at the current position and scroll
///			by the number of inserted lines (the naive solution for
///			auto-scrolling).
///
/// The string is added to the log starting at the current position,
/// i.e. without starting a new line. Then the control scrolls down by the
/// number of lines inserted.
/// The string is displayed in the specified text color.
/// The string may be a multiline string using carriage return/line feed
/// (i.e. newline) characters to indicate a line breaks.
///
/// The scrolling mechanism used here is kind of naive, because it assumes
/// that the user did not touch the scroll bars and that the scroll position
/// is always the end of the text. However, this is not the general case.
/// In general, we need to assume that the current scrolling position is
/// unkown. A solution for that is shown in the AppendToLogAndScroll()
/// method.
///
/// \param [in]		str		The string to add to the message log.
/// \param [in]		color	The text color of the string. You may use the
///							RGB(r,g,b) macro to specify the color byte-wise.
/// \return					An integer indicating sucess or failure:
///							- 0, if the function succeeded.
///							- (-1), if the function failed.
///							(This function always returns 0, because no
///							parameter or failure checking is done.)
///
/// \remark
/// Support for adding multiline strings requires the ES_MULTILINE style
/// to be set.
/// If you are not using the Visual Studio Wizards but create the control
/// indirectly using the Create() method, you should use the following
/// style: WS_CHILD|WS_VSCROLL|WS_HSCROLL|ES_MULTILINE|ES_READONLY.
///
/// \sa AppendToLogAndScroll()
//-----------------------------------------------------------------------------
int FFCDialog::AppendToLog( CString str, COLORREF color )
{
    int nOldLines = 0, nNewLines = 0, nScroll = 0;
    long nInsertionPoint = 0;
    CHARRANGE cr;
    CHARFORMAT cf;

    // Save number of lines before insertion of new text
    nOldLines		= m_ctrlLog.GetLineCount();

    // Initialize character format structure
    cf.cbSize		= sizeof( CHARFORMAT );
    cf.dwMask		= CFM_COLOR;
    cf.dwEffects	= 0;	// To disable CFE_AUTOCOLOR
    cf.crTextColor	= color;

    // Set insertion point to end of text
    nInsertionPoint = m_ctrlLog.GetWindowTextLength();
    /*if (nInsertionPoint > 800)
    {
    	// Delete first half of text to avoid running into the 64k limit
    	m_ctrlLog.SetSel(0, nInsertionPoint / 2);
    	m_ctrlLog.ReplaceSel("");
    	UpdateData(FALSE);
    }*/
    nInsertionPoint = -1;
    m_ctrlLog.SetSel( nInsertionPoint, -1 );

    //  Set the character format
    m_ctrlLog.SetSelectionCharFormat( cf );

    // Replace selection. Because we have nothing selected, this will simply insert
    // the string at the current caret position.
    m_ctrlLog.ReplaceSel( str );

    // Get new line count
    nNewLines		= m_ctrlLog.GetLineCount();

    // Scroll by the number of lines just inserted
    nScroll			= nNewLines - nOldLines;
    m_ctrlLog.LineScroll( nScroll );

    return 0;
}


//-----------------------------------------------------------------------------
//  AppendToLogAndScroll()
///
/// \brief	Add a string to the serial log window at the current position,
///			then scroll to the end of the text such that the last line of
///			the text is shown at the bottom of the CRichEditCtrl.
///
/// The string is added to the message log starting at the current position,
/// i.e. without starting a new line. Then the control scrolls down to show
/// as much text as possible, including the last line of text at the very
/// bottom.
/// The string is displayed in the specified text color.
/// The string may be a multiline string using carriage return/line feed
/// (i.e. newline) characters to indicate a line breaks.
///
/// \param [in]		str		The string to add to the message log.
/// \param [in]		color	The text color of the string. You may use the
///							RGB(r,g,b) macro to specify the color byte-wise.
/// \return					An integer indicating sucess or failure:
///							- 0, if the function succeeded.
///							- (-1), if the function failed.
///							(This function always returns 0, because no
///							parameter or failure checking is done.)
///
/// \remark
/// The automatic scrolling function would be easy, if the MFC documentation
/// was correct. Unfortunetely, it is not as trivial as one might think.
/// If the CRichEditCtrl has the focus, it scrolls automatically if you
/// insert text programatically. If it does not have the focus, it does not
/// scroll automatically, so in that case you can use the LineScroll()
/// method and you get the results you would expect when reading the MFC docs.
/// This is true even if ES_AUTOxSCROLL style is NOT set.
///
/// So the point is to check in the AppendToLogAndScroll() method if the
/// affected CRichEditCtrl has the focus. If so, we must not call
/// LineScroll(). If not, it is safe to call LineSroll() to first scroll to
/// the very end, which means that the last line of text is shown at the top
/// of the CRichEditCtrl.
/// Then we call LineScroll() a second time, this time scrolling back by
/// the number of visible lines. This leads to having the last line of the
/// text being displayed at the bottom of CRichEditCtrl.
///
/// Please note that in this sample application, the CRichEditCtrl never has
/// the focus, because we always have to click a button in order to insert
/// text. However, if you are using the code in an application not based on
/// a dialog and that fills up the control where the user could have set focus
/// to the control first, this method would fail to scroll correctly without
/// checking the focus.
/// I used this code in an MDI application, and there the control claims
/// to have the focus if I click into the control before clicking a menu
/// command (whatever the reason might be why in that case the focus is
/// not lost to the menu command).
///
/// Please note that the code is written for maximum comprehension / good
/// readability, not for code or execution efficiency.
//-----------------------------------------------------------------------------
int FFCDialog::AppendToLogAndScroll( CString str, COLORREF color )
{
    long nVisible = 0;
    long nInsertionPoint = 0;
    CHARFORMAT cf;

    // Initialize character format structure
    cf.cbSize = sizeof( CHARFORMAT );
    cf.dwMask = CFM_COLOR | CFM_SIZE;
    cf.dwEffects = 0; // To disable CFE_AUTOCOLOR
    cf.yHeight = 210;  // 1twips = 15px, 近似使用五号字体
    cf.crTextColor = color;

    // Set insertion point to end of text
    nInsertionPoint = m_ctrlLog.GetWindowTextLength();
    m_ctrlLog.SetSel( nInsertionPoint, -1 );

    // Set the character format
    m_ctrlLog.SetSelectionCharFormat( cf );

    // Replace selection. Because we have nothing
    // selected, this will simply insert
    // the string at the current caret position.
    m_ctrlLog.ReplaceSel( str );

    // Get number of currently visible lines or maximum number of visible lines
    // (We must call GetNumVisibleLines() before the first call to LineScroll()!)
    nVisible   = GetNumVisibleLines( &m_ctrlLog );

    // Now this is the fix of CRichEditCtrl's abnormal behaviour when used
    // in an application not based on dialogs. Checking the focus prevents
    // us from scrolling when the CRichEditCtrl does so automatically,
    // even though ES_AUTOxSCROLL style is NOT set.
    if ( &m_ctrlLog != m_ctrlLog.GetFocus() )
    {
        m_ctrlLog.LineScroll( INT_MAX );
        m_ctrlLog.LineScroll( 1 - nVisible );
    }

    return 0;
}


//-----------------------------------------------------------------------------
//  GetNumVisibleLines()
///
/// \brief	Returns the number of lines that are currently visible in the
///			client area of the given CRichEditCtrl.
///
///
///
/// \param [in]		pCtrl	Pointer to the CRichEditCtrl object to query.
///
/// \return					The number of currently visible lines.
///
/// \remark
/// The code is written for best comprehension / readability, not for code
/// or execution efficiency.
//-----------------------------------------------------------------------------
int FFCDialog::GetNumVisibleLines( CRichEditCtrl* pCtrl )
{
    CRect rect;
    long nFirstChar, nLastChar;
    long nFirstLine, nLastLine;

    // Get client rect of rich edit control
    pCtrl->GetClientRect( rect );

    // Get character index close to upper left corner
    nFirstChar = pCtrl->CharFromPos( CPoint( 0, 0 ) );

    // Get character index close to lower right corner
    nLastChar = pCtrl->CharFromPos( CPoint( rect.right, rect.bottom ) );
    if ( nLastChar < 0 )
    {
        nLastChar = pCtrl->GetTextLength();
    }

    // Convert to lines
    nFirstLine = pCtrl->LineFromChar( nFirstChar );
    nLastLine  = pCtrl->LineFromChar( nLastChar );

    return ( nLastLine - nFirstLine );
}
