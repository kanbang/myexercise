#include "stdafx.h"

#include "Tool.h"

#include "../MineGE/TagGE.h"
#include "../MineGE/ConstData.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

// �ж��Ƿ�������
bool IsNeedAir( const AcDbObjectId& objId )
{
    CString v1, v2;
    DataHelper::GetPropertyData( objId, _T( "��������" ), v1 );
    DataHelper::GetPropertyData( objId, _T( "�Ƿ񵥶����" ), v2 );

    int i1 = 0;
    ArxUtilHelper::StringToInt( v1, i1 );
    int ft = i1 % 6; // 0-5

    int i2 = 0;
    ArxUtilHelper::StringToInt( v2, i2 );
    bool needAir = ( i2 != 0 );

    return ( needAir || ft == FT_ELECTRIC || ft == FT_DIESEL );
}

// �ж��Ƿ�����
// ǰ�᣺��ȷ���ǻ�������
bool IsPowerSubstation( const AcDbObjectId& objId )
{
    CString v;
    DataHelper::GetPropertyData( objId,  _T( "������������" ), v );
    int i = 0;
    ArxUtilHelper::StringToInt( v, i );
    i = i % 4; // �μ�RT_TYPE
    return ( i == RT_POWER_SUBSTATION );
}

void GetHosts( const AcDbObjectIdArray& objIds, AcDbObjectIdArray& hosts )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) ) continue;

        TagGE* pTag = TagGE::cast( pObj );
        if( pTag == 0 ) continue;

        AcDbObjectId host = pTag->getRelatedGE();
        if( !host.isNull() ) hosts.append( host );
    }
    actrTransactionManager->endTransaction();
}

void FindTTunnels( AcDbObjectIdArray& objIds )
{
    DrawHelper::FindMineGEs( _T( "TTunnel" ), objIds );
}

void FindMainFans( AcDbObjectIdArray& objIds )
{
    DrawHelper::FindMineGEs( _T( "MainFan" ), objIds );
}

// �������еķ��ź͵��ڷ細
void FindGates( AcDbObjectIdArray& objIds )
{
    DrawHelper::FindMineGEs( _T( "Gate" ), objIds );
    DrawHelper::FindMineGEs( _T( "Casement" ), objIds );
}

// ������������û�й������ȣ���idĬ��Ϊnull
void FindLocalFans( const AcDbObjectIdArray& tObjIds, AcDbObjectIdArray& objIds )
{
    // �������еľֲ�ͨ���
    AcDbObjectIdArray fanObjIds;
    DrawHelper::FindMineGEs( _T( "LocalFan" ), fanObjIds );

    // ���Ҿֲ�ͨ�����Ӧ�ľ��������
    AcDbObjectIdArray tObjIds2;
    GetHosts( fanObjIds, tObjIds2 );

    // ����ľ������������Ͳ��ҵõ��ľ���������������Ҫ���
    //if(tObjIds2.length() != tObjIds.length()) return false;

    // ����˳��
    int len = tObjIds.length();
    for( int i = 0; i < len; i++ )
    {
        int pos = tObjIds2.find( tObjIds[i] );
        if( pos < 0 )
        {
            objIds.append( AcDbObjectId::kNull ); // ʹ��null id����
        }
        else
        {
            objIds.append( fanObjIds[pos] );
        }
    }

    //return true;
}

void FindChimneys( const AcDbObjectIdArray& tObjIds, AcDbObjectIdArray& objIds )
{
    // �������еķ�Ͳ
    AcDbObjectIdArray cObjIds;
    DrawHelper::FindMineGEs( _T( "Chimney" ), cObjIds );

    // ���ҷ�Ͳ��Ӧ�ľ��������
    AcDbObjectIdArray tObjIds2;
    GetHosts( cObjIds, tObjIds2 );

    // ����ľ������������Ͳ��ҵõ��ľ���������������Ҫ���
    //if(tObjIds2.length() != tObjIds.length()) return false;

    // ����˳��
    int len = tObjIds.length();
    for( int i = 0; i < len; i++ )
    {
        int pos = tObjIds2.find( tObjIds[i] );
        if( pos < 0 )
        {
            objIds.append( AcDbObjectId::kNull ); // ʹ��null id����
        }
        else
        {
            objIds.append( cObjIds[pos] );
        }
    }

    //return true;
}

void GetInsertPts( const AcDbObjectIdArray& objIds, AcGePoint3dArray& pts )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return ;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForRead ) ) continue;

        DirectionTagGE* pDir = DirectionTagGE::cast( pObj );
        if( pDir == 0 ) continue;

        pts.append( pDir->getInsertPt() );
    }

    actrTransactionManager->endTransaction();
}

bool IsMainPowerSubtation( const AcDbObjectId& objId )
{
    //if(!ArxUtilHelper::IsEqualType(_T("MachineRoom"), objId)) return true;

    CString str;
    //DataHelper::GetPropertyData(objId, _T("������������"), str);

    //// �Ƿ�����
    //int i = 0;
    //ArxUtilHelper::StringToInt(str, i);
    //if(i != 1) return true;

    // ����������������Ƿ����"����"��"����"������
    DataHelper::GetPropertyData( objId, _T( "����" ), str );

    if( str.Find( _T( "����" ) ) != -1 ) return false;
    if( str.Find( _T( "����" ) ) != -1 ) return false;
    if( str.Find( _T( "����" ) ) != -1 ) return false;
    return true;
}