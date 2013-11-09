#include "StdAfx.h"
#include "GOPBlockDraw.h"
#include "DrawTool.h"

ACRX_NO_CONS_DEFINE_MEMBERS( GOPBlockDraw, BlockDraw )
ACRX_CONS_DEFINE_MEMBERS( SmallGOPBlockDraw, GOPBlockDraw, 1 )
ACRX_CONS_DEFINE_MEMBERS( MidGOPBlockDraw, GOPBlockDraw, 1 )
ACRX_CONS_DEFINE_MEMBERS( LargeGOPBlockDraw, GOPBlockDraw, 1 )
ACRX_CONS_DEFINE_MEMBERS( LargerGOPBlockDraw, GOPBlockDraw, 1 )

GOPBlockDraw::GOPBlockDraw () : BlockDraw ()
{
}

void GOPBlockDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "ͻ��ú��" ) );
    names.append( _T( "ӿ����˹����" ) );
    names.append( _T( "���" ) );
    names.append( _T( "ͻ��������" ) );
}

//void GOPBlockDraw::readPropertyDataFromGE( const AcStringArray& values )
//{
//	BlockDraw::readPropertyDataFromGE(values);
//
//	// ���ӵ�λ
//	m_attValues[1].append(_T("t")); // ��λ:t
//
//	AcString str;
//	str.format(_T("��m%s"), MakeUpperText(_T("3")));
//	m_attValues[2].append(str);
//}

SmallGOPBlockDraw::SmallGOPBlockDraw () : GOPBlockDraw()
{
    m_blockName = _T( "С��ͻ����" );
}

MidGOPBlockDraw::MidGOPBlockDraw () : GOPBlockDraw()
{
    m_blockName = _T( "����ͻ����" );
}

LargeGOPBlockDraw::LargeGOPBlockDraw () : GOPBlockDraw ()
{
    m_blockName = _T( "����ͻ����" );
}

LargerGOPBlockDraw::LargerGOPBlockDraw () : GOPBlockDraw ()
{
    m_blockName = _T( "�ش���ͻ����" );
}
