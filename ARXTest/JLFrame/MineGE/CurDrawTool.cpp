#include "stdafx.h"

#include "config.h"
#include "CurDrawTool.h"
#include "DrawHelper.h"

#include "../ArxHelper/HelperClass.h"
#include "../MineGEDraw/MineGEDrawSystem.h"

/*
 * ������ǰ���ӻ�Ч���ʵ�
 * ����types���鱣���˵�ǰϵͳ������ע���ͼԪ����
 */
static void BuildCurDrawsDict( const AcStringArray& types )
{
    // ��ȡ���ӻ�ϵͳȫ�ֶ���
    MineGEDrawSystem* pDrawSystem = MineGEDrawSystem::GetInstance();
    if( pDrawSystem == 0 ) return;

    // �򿪴ʵ�
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( CUR_DRAW_DICT );
    int len = types.length();

    for( int i = 0; i < len; i++ )
    {
        // �ַ��������i��Ԫ��
        CString type = types[i].kACharPtr();

        if( !pDictTool->findKey( type ) )
        {
            // ���ͼԪ��Ӧ��key�����ڣ�������ӻ�ϵͳ�����ͼԪ��Ĭ�Ͽ��ӻ�Ч������
            // �����"��-ֵ"�Ե��ʵ�
            // ����: type = Tunnel ==>  �����(Tunnel --> DoubleTunnel)���ʵ�
            pDictTool->addEntry( type, pDrawSystem->getDefaultGEDrawName( type ) );
        }
    }

    delete pDictTool;
}

// ���õ�ǰ���ӻ�Ч��
void InitAllCurDraws()
{
    // �ӿ��ӻ�ϵͳ�ж�ȡ������ע���ͼԪ����
    AcStringArray types;
    DrawHelper::GetAllRegGETypesForDraw( types );
    if( types.isEmpty() ) return;

    // ������ǰ���ӻ�Ч���ʵ�
    BuildCurDrawsDict( types );
}

bool SetCurDraw( const CString& type, const CString& draw )
{
    // ͨ��ArxHelperģ����ArxDictTool��������ݴʵ�

    // �򿪴ʵ�
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( CUR_DRAW_DICT );
    // �޸ĵ�1��λ�õ�����
    // ע����1��λ�õ����ݾ���"��ǰ���ӻ�Ч��"
    bool ret = pDictTool->modifyEntry( type, 1, draw );
    delete pDictTool;

    return ret;
}

bool GetCurDraw( const CString& type, CString& draw )
{
    // ͨ��ArxHelperģ����ArxDictTool��������ݴʵ�

    // �򿪴ʵ�
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( CUR_DRAW_DICT );

    // ��ȡ��1��λ�õ�����
    // ע����1��λ�õ����ݾ���"��ǰ���ӻ�Ч��"
    bool ret = pDictTool->getEntry( type, 1, draw );
    delete pDictTool;

    return ret;
}