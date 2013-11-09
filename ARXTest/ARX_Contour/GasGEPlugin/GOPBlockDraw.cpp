#include "StdAfx.h"
#include "GOPBlockDraw.h"
#include "DrawTool.h"

ACRX_CONS_DEFINE_MEMBERS( GOPBlockDraw, BlockDraw, 1 )

GOPBlockDraw::GOPBlockDraw () : BlockDraw ()
{
}

void GOPBlockDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "ͻ��ǿ��" ) );
    names.append( _T( "ͻ��ú��" ) );
    names.append( _T( "ӿ����˹����" ) );
    names.append( _T( "���" ) );
    names.append( _T( "ͻ��������" ) );
}

void GOPBlockDraw::readPropertyDataFromGE( const AcStringArray& values )
{
    BlockDraw::readPropertyDataFromGE( values );

    // ���ӵ�λ
    m_attValues[1].append( _T( "t" ) ); // ��λ:t

    AcString str;
    str.format( _T( "��m%s" ), MakeUpperText( _T( "3" ) ) );
    m_attValues[2].append( str );
    m_blockName = m_attValues[0].kACharPtr();
}

