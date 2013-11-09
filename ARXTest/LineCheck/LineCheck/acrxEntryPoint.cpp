#include "StdAfx.h"
#include "resource.h"

#include "TolSetter.h"
TolSetter* pTS = 0;

#define GLOBAL_TOLERANCE 1e-15

class CLineCheckApp : public AcRxArxApp
{

public:
    CLineCheckApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {
        // TODO: Load dependencies here

        // You *must* call On_kInitAppMsg here
        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        // �޸�cadϵͳ����
        pTS = new TolSetter( GLOBAL_TOLERANCE );

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {
        // TODO: Add your code here

        // You *must* call On_kUnloadAppMsg here
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        delete pTS;
        pTS = 0;

        return ( retCode ) ;
    }

    virtual void RegisterServerComponents ()
    {
    }

    static void DLJ_ZeroLineWidth( void )
    {
        acutPrintf( _T( "\n�޸Ķ���ߺ�ֱ���߿�ΪĬ��ֵ..." ) );
        extern void ZeroLineWidth();
        ZeroLineWidth();
    }

    static void DLJ_DrawLinePoints( void )
    {
        acutPrintf( _T( "\n���Ƶ�����..." ) );
        extern void DrawLinePoints();
        DrawLinePoints();
    }

    static void DLJ_FixLinePoints( void )
    {
        acutPrintf( _T( "\n�������������..." ) );
        extern void FixLinePoints();

        FixLinePoints();
    }

    static void DLJ_FindSuperpositionLine( void )
    {
        acutPrintf( _T( "\n�����غ�ֱ��..." ) );
        extern void FindSuperpositionLine();

        FindSuperpositionLine();
    }
} ;

IMPLEMENT_ARX_ENTRYPOINT( CLineCheckApp )

ACED_ARXCOMMAND_ENTRY_AUTO( CLineCheckApp, DLJ, _ZeroLineWidth, ZeroLineWidth, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CLineCheckApp, DLJ, _DrawLinePoints, DrawLinePoints, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CLineCheckApp, DLJ, _FixLinePoints, FixLinePoints, ACRX_CMD_TRANSPARENT, NULL )
ACED_ARXCOMMAND_ENTRY_AUTO( CLineCheckApp, DLJ, _FindSuperpositionLine, FindSuperpositionLine, ACRX_CMD_TRANSPARENT, NULL )
