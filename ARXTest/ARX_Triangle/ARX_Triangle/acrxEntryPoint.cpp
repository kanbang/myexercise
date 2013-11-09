#include "StdAfx.h"
#include "resource.h"

class CARX_TriangleApp : public AcRxArxApp
{

public:
    CARX_TriangleApp () : AcRxArxApp () {}

    virtual AcRx::AppRetCode On_kInitAppMsg ( void* pkt )
    {
        // TODO: Load dependencies here

        // You *must* call On_kInitAppMsg here
        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg ( pkt ) ;

        // TODO: Add your initialization code here

        return ( retCode ) ;
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg ( void* pkt )
    {
        // TODO: Add your code here

        // You *must* call On_kUnloadAppMsg here
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg ( pkt ) ;

        // TODO: Unload dependencies here

        return ( retCode ) ;
    }

    virtual void RegisterServerComponents ()
    {
    }

    static void JL_dt()
    {
        extern void DrawTriangle( const CString & path );

        CString path = _T( "D:\\Test\\CppTest\\triangle\\Release\\face.poly" );
        //CString path = _T("D:\\Test\\CppTest\\triangle\\Release\\spiral.node");
        //CString path = _T("D:\\Test\\CppTest\\triangle\\Release\\box.poly");

        DrawTriangle( path );
    }

    static void JL_da()
    {
        extern void DrawData( const CString & path );

        CString path = _T( "D:\\Test\\CppTest\\triangle\\Release\\face.poly" );
        //CString path = _T("D:\\Test\\CppTest\\triangle\\Release\\spiral.node");
        //CString path = _T("D:\\Test\\CppTest\\triangle\\Release\\box.poly");

        DrawData( path );
    }
} ;

IMPLEMENT_ARX_ENTRYPOINT( CARX_TriangleApp )

ACED_ARXCOMMAND_ENTRY_AUTO( CARX_TriangleApp, JL, _dt, dt, ACRX_CMD_TRANSPARENT, NULL )

ACED_ARXCOMMAND_ENTRY_AUTO( CARX_TriangleApp, JL, _da, da, ACRX_CMD_TRANSPARENT, NULL )
