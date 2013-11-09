// (C) Copyright 2002-2007 by Autodesk, Inc.
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted,
// provided that the above copyright notice appears in all copies and
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS.
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC.
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- TestDlg.cpp : Implementation of TestDlg
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "TestDlg.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC ( TestDlg, CAcUiDialog )

BEGIN_MESSAGE_MAP( TestDlg, CAcUiDialog )
    ON_MESSAGE( WM_ACAD_KEEPFOCUS, OnAcadKeepFocus )
    ON_BN_CLICKED( IDC_BUTTON1, &TestDlg::OnBnClickedButton1 )
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
TestDlg::TestDlg ( CWnd* pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/ ) : CAcUiDialog ( TestDlg::IDD, pParent, hInstance )
{
}

//-----------------------------------------------------------------------------
void TestDlg::DoDataExchange ( CDataExchange* pDX )
{
    CAcUiDialog::DoDataExchange ( pDX ) ;
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT TestDlg::OnAcadKeepFocus ( WPARAM, LPARAM )
{
    return ( TRUE ) ;
}

void TestDlg::OnBnClickedButton1()
{
    BeginEditorCommand();
    AcGePoint3d basePoint;
    if( acedGetPoint( NULL, _T( "\n ‰»Î≤Â»Îµ„£∫\n" ), asDblArray( basePoint ) ) != RTNORM )
    {
        return ;
    }
    CompleteEditorCommand();
}
