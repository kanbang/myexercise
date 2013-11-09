#include "stdafx.h"

/* ʵ����ARX_VNGGen.cpp */
extern bool GetTypeById( const AcDbObjectId& objId, CString& type );

static CString VNG_NODE_FIELDS[] =
{
    _T( "����" ),
    _T( "���ѹ��" ),
    _T( "����ѹ" )
};

static CString VNG_EDGE_FIELDS[] =
{
    _T( "����" ),
    _T( "����" ),
    _T( "������" )
};

static CString VNG_GATE_FIELDS[] =
{
    _T( "����" ),
    _T( "ѹ��" ),
    _T( "©����" ),
    _T( "����ѹ��" )
};

static CString VNG_FAN_FIELDS[] =
{
    _T( "����" ),
    _T( "����" ),
    _T( "��ѹ" ),
    _T( "����" ),
    _T( "Ч��" ),
    _T( "����" ),
    _T( "�Ȼ���" )
};

static void GetVNGFields( const CString& type, AcStringArray& fields )
{
    if( type.CompareNoCase( _T( "VNG_Node" ) ) == 0 )
    {
        int n = sizeof( VNG_NODE_FIELDS ) / sizeof( CString );
        for( int i = 0; i < n; i++ )
        {
            fields.append( VNG_NODE_FIELDS[i] );
        }
    }
    else if( type.CompareNoCase( _T( "VNG_Edge" ) ) == 0 )
    {
        int n = sizeof( VNG_EDGE_FIELDS ) / sizeof( CString );
        for( int i = 0; i < n; i++ )
        {
            fields.append( VNG_EDGE_FIELDS[i] );
        }
    }
    else if( type.CompareNoCase( _T( "VNG_Gate" ) ) == 0 )
    {
        int n = sizeof( VNG_GATE_FIELDS ) / sizeof( CString );
        for( int i = 0; i < n; i++ )
        {
            fields.append( VNG_GATE_FIELDS[i] );
        }
    }
    else if( type.CompareNoCase( _T( "VNG_Fan" ) ) == 0 )
    {
        int n = sizeof( VNG_FAN_FIELDS ) / sizeof( CString );
        for( int i = 0; i < n; i++ )
        {
            fields.append( VNG_FAN_FIELDS[i] );
        }
    }
}

void ReadFieldsById( const AcDbObjectId& objId, AcStringArray& fields )
{
    CString type;
    if( !GetTypeById( objId, type ) ) return;

    GetVNGFields( type, fields );
}