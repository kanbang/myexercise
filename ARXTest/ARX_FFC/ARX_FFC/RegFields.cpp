#include "stdafx.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

/*
 * �������ԣ���������������һ��Ԫ�ض���һ������Ҳ�ܱ����ȥ
   int a[] = {
		1,
		2,
		3,
		10,
   };
*/
#define BEGIN_DEFINE_FIELDS(type) static CString type##_Fields[] = {
#define DEFINE_FIELD(name) _T(#name),
#define END_DEFINE_FIELDS };

static void AddFields( const CString& type, CString* fields, int n )
{
    //acutPrintf(_T("n=%d\n"), n);
    for( int i = 0; i < n; i++ )
    {
        FieldHelper::AddField( type, fields[i] );
    }
}

#define ADD_FIELDS(type) AddFields(_T(#type), type##_Fields, sizeof(type##_Fields)/sizeof(type##_Fields[0]))

// BEGIN_DEFINE_FIELDS --> DEFINE_FIELD --> END_DEFINE_FIELDS
// չ�����
//static CString Tunnel_Fields[] = {
//	_T("�������"),
//	_T("����")
//};

// ����ֶ�(Tunnel)
BEGIN_DEFINE_FIELDS( Tunnel )
DEFINE_FIELD( ������� )
DEFINE_FIELD( ���� )
DEFINE_FIELD( �¶� )
DEFINE_FIELD( ����Ũ�� )
DEFINE_FIELD( ����Ũ�� )
END_DEFINE_FIELDS

// �������ֶ�(WorkSurface)
BEGIN_DEFINE_FIELDS( WorkSurface )
DEFINE_FIELD( ��������˹ӿ���� )
END_DEFINE_FIELDS

// ���������ֶ�(WindDirection)
BEGIN_DEFINE_FIELDS( WindDirection )
DEFINE_FIELD( ͨ������ )
END_DEFINE_FIELDS

// �ܱ��ֶ�(Obturation)
BEGIN_DEFINE_FIELDS( Obturation )
DEFINE_FIELD( ©���� )
DEFINE_FIELD( ��˹Ũ�� )
DEFINE_FIELD( ����Ũ�� )
END_DEFINE_FIELDS

// ��ǽ�ֶ�(SandWall)
BEGIN_DEFINE_FIELDS( SandWall )
DEFINE_FIELD( ���� )
END_DEFINE_FIELDS

// ע�����ֶ�(NitrogenPipe)
BEGIN_DEFINE_FIELDS( NitrogenPipe )
DEFINE_FIELD( ע���� )
END_DEFINE_FIELDS

// ��˹����ֶ�(GasPipe)
BEGIN_DEFINE_FIELDS( GasPipe )
DEFINE_FIELD( ����� )
DEFINE_FIELD( ����Ũ�� )
DEFINE_FIELD( ����Ũ�� )
END_DEFINE_FIELDS

// ©��Դ�ֶ�(SourceAirLeak)
BEGIN_DEFINE_FIELDS( SourceAirLeak )
DEFINE_FIELD( ©���� )
END_DEFINE_FIELDS

// ©����ֶ�(SinkAirLeak)
BEGIN_DEFINE_FIELDS( SinkAirLeak )
DEFINE_FIELD( ©���� )
END_DEFINE_FIELDS

void RegFields()
{
    ADD_FIELDS( Tunnel );
    ADD_FIELDS( WorkSurface );
    ADD_FIELDS( WindDirection );
    ADD_FIELDS( Obturation );
    ADD_FIELDS( SandWall );
    ADD_FIELDS( NitrogenPipe );
    ADD_FIELDS( GasPipe );
    ADD_FIELDS( SourceAirLeak );
    ADD_FIELDS( SinkAirLeak );
}