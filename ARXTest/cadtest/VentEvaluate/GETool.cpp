#include "stdafx.h"

#include "../MineGE/DataLink.h"
#include "../MineGE/HelperClass.h"

#include "../ArxHelper/HelperClass.h"

#include "../VentNetworkAlgo/VNAlgoHelper.h"

class OTDataLink : public DataLink
{
public:
    OTDataLink(): ft( 0 ), needAir( false ) {}

protected:
    virtual void regDatas()
    {
        linkIntData( _T( "��������" ), &ft );
        linkBoolData( _T( "�Ƿ񵥶����" ), &needAir );
        ft = ft % 6; // 0-5
    }

public:
    bool isNeedAir()
    {
        return ( needAir || ft == FT_ELECTRIC || ft == FT_DIESEL );
    }
    bool isMain()
    {
        return ( ft == FT_MAIN );
    }
    bool isSubMain()
    {
        return ( ft == FT_SUBMAIN );
    }

public:
    int ft;              // �����������
    bool needAir;        // �Ƿ���Ҫ�������
};

static void FilterGE( const AcDbObjectIdArray& objIds, const CString& type, AcDbObjectIdArray& filterObjIds )
{
    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        if( ArxUtilHelper::IsEqualType( type, objIds[i] ) )
        {
            filterObjIds.append( objIds[i] );
        }
    }
}

// ���ҽ��ط羮��ȫ������������shafts
// ����ֵ��ʾ���羮��ط羮֮��ķֽ��
int FindShafts( AcDbObjectIdArray& shafts )
{
    // �������е�Դ��ĳ��߷�֧
    // �Լ����л�����߷�֧
    AcDbObjectIdArray objIds1, objIds2;
    ConnectivityHelper::FIOE( objIds1, objIds2 );

    AcDbObjectIdArray inShafts, outShafts;
    // �ų�������ķ�֧�����繤���桢���ҡ�����������
    FilterGE( objIds1, _T( "Tunnel" ), inShafts );
    FilterGE( objIds2, _T( "Tunnel" ), outShafts );

    shafts.append( inShafts );
    shafts.append( outShafts );
    return inShafts.length();
}

void FindTunnelsWithoutShaft( AcDbObjectIdArray& objIds )
{
    AcDbObjectIdArray shafts;
    int pos = FindShafts( shafts );

    AcDbObjectIdArray allObjIds;
    DrawHelper::FindMineGEs( _T( "Tunnel" ), allObjIds );

    int len = allObjIds.length();
    for( int i = 0; i < len; i++ )
    {
        if( !shafts.contains( allObjIds[i] ) )
        {
            objIds.append( allObjIds[i] );
        }
    }
}

// ����������
static void FindUseAirTunnels( AcDbObjectIdArray& objIds )
{
    // �����������(��������Ͳ)
    AcDbObjectIdArray allObjIds;
    FindTunnelsWithoutShaft( allObjIds );

    bool ret = true;
    AcDbObjectIdArray tempObjIds;
    int len = allObjIds.length();
    for( int i = 0; i < len; i++ )
    {
        // ��ȡ��������
        OTDataLink ot;
        ot.setDataSource( allObjIds[i] );
        if( !ot.updateData( false ) ) // ���û����Ӧ���ֶΣ���ȡ����ʧ��
        {
            ret = false;
            break;
        }
        if( ot.isNeedAir() )
        {
            tempObjIds.append( allObjIds[i] );
        }
        else if( ot.isMain() )
        {
            tempObjIds.append( allObjIds[i] );
        }
        else if( ot.isSubMain() )
        {
            tempObjIds.append( allObjIds[i] );
        }
    }
    if( ret )	objIds.append( tempObjIds );
}

// ������Ҫ���ĵص�
void FindUseAirPlaces( AcDbObjectIdArray& objIds, int& p1, int& p2, int& p3 )
{
    // Ӧ���ص������
    //    �÷�ص㡢��Ҫ������������ط����
    AcDbObjectIdArray tempObjIds;

    DrawHelper::FindMineGEs( _T( "WorkSurface" ), tempObjIds ); // ������
    objIds.append( tempObjIds );
    p1 = objIds.length();

    tempObjIds.removeAll();

    DrawHelper::FindMineGEs( _T( "TTunnel" ), tempObjIds );  // ���������
    objIds.append( tempObjIds );
    p2 = objIds.length();

    tempObjIds.removeAll();

    DrawHelper::FindMineGEs( _T( "StorageGE" ), tempObjIds ); // ����
    objIds.append( tempObjIds );
    p3 = objIds.length();

    tempObjIds.removeAll();

    // ������Ҫ�������(��������Ͳ)��
    //    1) �÷����
    //    2) ��Ҫ����Ͳ������ط����
    FindUseAirTunnels( tempObjIds );
    objIds.append( tempObjIds );

    //tempObjIds.removeAll();
}