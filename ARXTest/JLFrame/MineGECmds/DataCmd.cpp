#include "StdAfx.h"
#include "DataCmd.h"

#include "../MineGE/DataHelper.h"

#include "../ArxHelper/HelperClass.h"

void DataCmd::CopyData()
{
    AcDbObjectIdArray objIds;
    ArxUtilHelper::GetPickSetEntity( objIds );
    if( objIds.length() != 1 ) return; // û��ͼԪ���߳���1��ͼԪ��ѡ��

    AcDbObjectId targetObjId = ArxUtilHelper::SelectObject( _T( "ѡ��Ŀ��ͼԪ(target):" ) );
    if( targetObjId.isNull() ) return;

    if( objIds[0] == targetObjId )
    {
        acutPrintf( _T( "\nѡ���2��ͼԪ�Ƕ���ָ��һ��ͼԪ..." ) );
        return;
    }

    DataHelper::CopyPropertyData( objIds[0], targetObjId );
}

void DataCmd::CopyData2( void )
{
    acutPrintf( _T( "\n�������ݸ���...\n" ) );

    AcDbObjectId objId1 = ArxUtilHelper::SelectObject( _T( "ѡ��ԴͼԪ(source):" ) );
    if( objId1.isNull() ) return;

    AcDbObjectId objId2 = ArxUtilHelper::SelectObject( _T( "ѡ��Ŀ��ͼԪ(target):" ) );
    if( objId2.isNull() ) return;

    DataHelper::CopyPropertyData( objId1, objId2 );
}
