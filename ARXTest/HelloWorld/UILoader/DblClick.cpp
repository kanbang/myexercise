#include "StdAfx.h"
#include "DblClick.h"

ACRX_DEFINE_MEMBERS( AcDbDoubleClickEdit );

DblClick::DblClick( void )
{
}

DblClick::~DblClick( void )
{
}

void DblClick::startEdit( AcDbEntity* pEnt, AcGePoint3d pt )
{
    AfxMessageBox( _T( "begin: test33333" ) );
}

void DblClick::finishEdit( void )
{
    AfxMessageBox( _T( "end: test33333" ) );
}