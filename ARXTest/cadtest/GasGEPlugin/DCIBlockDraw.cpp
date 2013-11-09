#include "StdAfx.h"
#include "DCIBlockDraw.h"

ACRX_CONS_DEFINE_MEMBERS( CSDCIBlockDraw, BlockDraw, 1 )
ACRX_CONS_DEFINE_MEMBERS( WDCI1BlockDraw, BlockDraw, 1 )
ACRX_CONS_DEFINE_MEMBERS( WDCI2BlockDraw, BlockDraw, 1 )

CSDCIBlockDraw::CSDCIBlockDraw () : BlockDraw ()
{
    m_blockName = _T( "ú������ͻ��Σ����Ԥ��ָ��" );
}

void CSDCIBlockDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "��˹��ɢ���ٶ�" ) );
    names.append( _T( "ú�ļ����ϵ��" ) );
    // ʵ��ֻ��2���ֶΣ����ֶβ�������!!!
    // ע��һ�������ڵ��ֶΣ�����ֵΪ""
    names.append( _T( "��˹ͻ��Σ�����ۺ�ָ��Kֵ" ) );
}

// ɾ��β�������0
// ֻ�����ڸ������ַ�����������С����.
// ����123.45000
//static CString RemoveExtraZero(const CString& str)
//{
//	if(str.Right(1) = _T("0"))
//	{
//		return str.Left(str.ReverseFind(_T('0')));
//	}
//	else
//	{
//		return str;
//	}
//}

void CSDCIBlockDraw::readPropertyDataFromGE( const AcStringArray& values )
{
    BlockDraw::readPropertyDataFromGE( values );

    // ��� һ�����ݲ������Ǵ��ֶ��еõ�����Ҫ����
    double v = _tstof( values[0].kACharPtr() );
    double f = _tstof( values[1].kACharPtr() );
    acutPrintf( _T( "\n�����f=%.3f" ), f );
    if( abs( f ) > 0 )
    {
        double k = v / f;
        CString str;
        str.Format( _T( "%.3f" ), k );
        //m_attValues[2] = RemoveExtraZero(str);
        m_attValues[2] = str;
    }
}

WDCI1BlockDraw::WDCI1BlockDraw () : BlockDraw ()
{
    m_blockName = _T( "������ͻ��Σ����Ԥ��ָ��1" );
}

void WDCI1BlockDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "��м����ָ��" ) );
    names.append( _T( "��������м��" ) );
}

WDCI2BlockDraw::WDCI2BlockDraw () : BlockDraw ()
{
    m_blockName = _T( "������ͻ��Σ����Ԥ��ָ��2" );
}

void WDCI2BlockDraw::regPropertyDataNames( AcStringArray& names ) const
{
    names.append( _T( "��������˹ӿ�����ٶ�" ) );
    names.append( _T( "��������м��" ) );
    names.append( _T( "Rֵָ��" ) );
}