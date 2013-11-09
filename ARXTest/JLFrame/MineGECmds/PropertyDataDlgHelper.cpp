#include "StdAfx.h"
#include "PropertyDataDlgHelper.h"

#include "PropertyDataDlg.h"
#include "PropertyDataDlgHelper.h"

#include "../MineGE/FieldHelper.h"
#include "../MineGE/DataHelper.h"

void PropertyDataDlgHelper::DisplayAllPropertyDataDlg( const AcDbObjectId& objId )
{
    if( objId.isNull() ) return;

    // �л���Դ
    CAcModuleResourceOverride myResources;

    PropertyDataDlg pdd;
    pdd.showAllData( true );
    pdd.setMineGE( objId );
    pdd.DoModal();
}

void PropertyDataDlgHelper::DisplayPartialPropertyDataDlg( const AcDbObjectId& objId, const AcStringArray& fields )
{
    if( objId.isNull() ) return;
    if( fields.isEmpty() ) return;

    // �л���Դ
    CAcModuleResourceOverride myResources;

    PropertyDataDlg pdd;
    pdd.showAllData( false );
    pdd.setMineGE( objId );
    int len = fields.length();
    for( int i = 0; i < len; i++ )
    {
        pdd.addField( fields[i].kACharPtr() );
    }
    pdd.DoModal();
}

bool PropertyDataDlgHelper::DisplayPropertyDataDlg( const AcDbObjectId& objId )
{
    if( objId.isNull() ) return false;

    AcDbObjectId dObjId;
    if( !DataHelper::GetDataObject( objId, dObjId ) ) return false;

    CString type;
    if( !DataHelper::GetTypeName( objId, type ) ) return false;

    AcStringArray fields;
    FieldHelper::GetAllFields( type, fields );
    if( fields.isEmpty() )
    {
        CString msg;
        //msg.Format(_T("����:%s���ֶθ���Ϊ0��û�����ݿ�����ʾ..."), type);
        msg.Format( _T( "û�����ݿ�����ʾ..." ) );
        AfxMessageBox( msg );
        return false;
    }
    else
    {
        DisplayPartialPropertyDataDlg( dObjId, fields );
        return true;
    }
}