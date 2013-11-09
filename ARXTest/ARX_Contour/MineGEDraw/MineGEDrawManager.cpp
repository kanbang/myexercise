#include "StdAfx.h"
#include "MineGEDrawManager.h"

#include "MineGEDraw.h"
#include "MineGEDraw_Jig.h"

#include <algorithm>
#include <iterator>

struct MineGEDrawFinder
{
    MineGEDrawFinder( const CString& drawName ) : m_drawName( drawName ) {}

    bool operator()( MineGEDrawInfo drawInfo )
    {
        return m_drawName.CompareNoCase( drawInfo.pDraw->isA()->name() ) == 0;
    }

    CString m_drawName; // draw��������
};

void MineGEDrawManager::regGEDraw( const CString& geType, MineGEDraw* pDraw, MineGEDraw_Jig* pJig )
{
    // ��ʼ��Ĭ�ϲ���
    pDraw->setAllExtraParamsToDefault();

    MineGEDrawInfo drawInfo = {pDraw, pJig};

    GEDrawMap::iterator itr = m_drawMap.find( geType );
    if( itr == m_drawMap.end() ) // ��δע�������
    {
        DrawListInfo info;
        info.m_defDraw = pDraw; // �趨��һ��ע��Ļ���Ч��Ϊ��ǰ��ʾЧ��

        info.m_drawInfoList.push_back( drawInfo );
        m_drawMap.insert( GEDrawMap::value_type( geType, info ) );
    }
    else
    {
        MineGEDrawInfoList& draw_list = itr->second.m_drawInfoList;
        MineGEDrawInfoList::iterator list_itr = std::find( draw_list.begin(), draw_list.end(), drawInfo );
        if( list_itr != draw_list.end() ) return;

        itr->second.m_drawInfoList.push_back( drawInfo );
    }
}

void MineGEDrawManager::unRegGEDraw( const CString& geType, const CString& realGEDrawName )
{
    GEDrawMap::iterator itr = m_drawMap.find( geType );
    if( itr == m_drawMap.end() ) return;

    MineGEDrawInfoList& draw_list = itr->second.m_drawInfoList;
    MineGEDrawInfoList::iterator list_itr = std::find_if( draw_list.begin(), draw_list.end(), MineGEDrawFinder( realGEDrawName ) );
    if( list_itr == draw_list.end() ) return;

    MineGEDrawInfo drawInfo = *list_itr;
    draw_list.erase( list_itr );

    // ���Ҫж�صĻ���Ч���ǵ�ǰ����Ч��
    if( itr->second.m_defDraw == drawInfo.pDraw )
    {
        if( draw_list.empty() )
        {
            itr->second.m_defDraw = 0; // ��ôͼԪ����Ч��Ϊ��
        }
        else
        {
            // ������������Ļ���Ч������ôʹ�õ�һ��ע��Ļ���Ч��
            itr->second.m_defDraw = draw_list.begin()->pDraw;
        }
    }
    delete drawInfo.pDraw;  // ɾ���ڴ�(draw��new������)
}

bool MineGEDrawManager::setCurrentGEDraw( const CString& geType, const CString& realGEDrawName )
{
    // ʵ����Щ���£����Ǵ��������Ǳ����!!!��
    GEDrawMap::iterator itr = m_drawMap.find( geType );
    if( itr == m_drawMap.end() ) return false;

    MineGEDrawInfoList& draw_list = itr->second.m_drawInfoList;
    MineGEDrawInfoList::iterator list_itr = std::find_if( draw_list.begin(), draw_list.end(), MineGEDrawFinder( realGEDrawName ) );
    if( list_itr == draw_list.end() ) return false;

    // ָ����ǰ����Ч��
    itr->second.m_defDraw = list_itr->pDraw;
    return true;
}

MineGEDraw* MineGEDrawManager::getCurrentGEDraw( const CString& geType )
{
    GEDrawMap::iterator itr = m_drawMap.find( geType );
    if( itr == m_drawMap.end() ) return 0;

    return itr->second.m_defDraw;
}

MineGEDraw* MineGEDrawManager::getGEDraw( const CString& geType, const CString& realGEDrawName )
{
    GEDrawMap::iterator itr = m_drawMap.find( geType );
    if( itr == m_drawMap.end() ) return 0;

    MineGEDrawInfoList& draw_list = itr->second.m_drawInfoList;
    MineGEDrawInfoList::iterator list_itr = std::find_if( draw_list.begin(), draw_list.end(), MineGEDrawFinder( realGEDrawName ) );
    if( list_itr == draw_list.end() ) return 0;
    return list_itr->pDraw;
}

MineGEDraw_Jig* MineGEDrawManager::getGEDraw_Jig( const CString& geType, const CString& realGEDrawName )
{
    GEDrawMap::iterator itr = m_drawMap.find( geType );
    if( itr == m_drawMap.end() ) return 0;

    MineGEDrawInfoList& draw_list = itr->second.m_drawInfoList;
    MineGEDrawInfoList::iterator list_itr = std::find_if( draw_list.begin(), draw_list.end(), MineGEDrawFinder( realGEDrawName ) );
    if( list_itr == draw_list.end() ) return 0;
    return list_itr->pJig;
}

void MineGEDrawManager::getAllGEDrawsByGEType( const CString& geType, AcStringArray& drawList )
{
    GEDrawMap::iterator itr = m_drawMap.find( geType );
    if( itr == m_drawMap.end() ) return;

    MineGEDrawInfoList& temp_draw_list = itr->second.m_drawInfoList;
    for( MineGEDrawInfoList::iterator itr = temp_draw_list.begin(); itr != temp_draw_list.end(); itr++ )
    {
        drawList.append( itr->pDraw->isA()->name() );
    }
}