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

// ��˹ͻ����
BEGIN_DEFINE_FIELDS( GOP )
DEFINE_FIELD( ͻ��ǿ�� )
DEFINE_FIELD( ͻ��ú�� )
DEFINE_FIELD( ӿ����˹���� )
DEFINE_FIELD( ��� )
DEFINE_FIELD( ͻ�������� )
END_DEFINE_FIELDS

// ��˹����
BEGIN_DEFINE_FIELDS( GCP )
DEFINE_FIELD( ��˹���� )
DEFINE_FIELD( ����� )
DEFINE_FIELD( ���� )
END_DEFINE_FIELDS

// ��˹ѹ��
BEGIN_DEFINE_FIELDS( GPP )
DEFINE_FIELD( ��˹ѹ��ֵ )
DEFINE_FIELD( ����� )
DEFINE_FIELD( ���� )
END_DEFINE_FIELDS

// ú����˹ͻ��
BEGIN_DEFINE_FIELDS( PPP )
DEFINE_FIELD( ͻ��ú���� )
DEFINE_FIELD( ӿ����˹�� )
DEFINE_FIELD( ��� )
DEFINE_FIELD( �������� )
END_DEFINE_FIELDS

// Ԥ��ָ��1
BEGIN_DEFINE_FIELDS( CSDCI )
DEFINE_FIELD( ��˹��ɢ���ٶ� )
DEFINE_FIELD( ú�ļ����ϵ�� )
END_DEFINE_FIELDS

// ������Ԥ��ָ��1
BEGIN_DEFINE_FIELDS( WDCI1 )
DEFINE_FIELD( ��м����ָ�� )
DEFINE_FIELD( ��������м�� )
END_DEFINE_FIELDS

// ������Ԥ��ָ��2
BEGIN_DEFINE_FIELDS( WDCI2 )
DEFINE_FIELD( ��������˹ӿ�����ٶ� )
DEFINE_FIELD( ��������м�� )
DEFINE_FIELD( Rֵָ�� )
END_DEFINE_FIELDS

// ��������˹ӿ����
BEGIN_DEFINE_FIELDS( WGFP )
DEFINE_FIELD( ������˹ӿ���� )
DEFINE_FIELD( �����˹ӿ���� )
DEFINE_FIELD( �������ղ��� )
DEFINE_FIELD( �ز����� )
END_DEFINE_FIELDS

// �������˹ӿ����
BEGIN_DEFINE_FIELDS( TGFP )
DEFINE_FIELD( ������˹ӿ���� )
DEFINE_FIELD( ������� )
END_DEFINE_FIELDS

// ��ú���
BEGIN_DEFINE_FIELDS( CDH )
DEFINE_FIELD( �׺� )
DEFINE_FIELD( ������ )
DEFINE_FIELD( ú��װ��� )
DEFINE_FIELD( ú�� )
END_DEFINE_FIELDS

void RegFields()
{
    ADD_FIELDS( GOP );
    ADD_FIELDS( GCP );
    ADD_FIELDS( GPP );
    ADD_FIELDS( PPP );
    ADD_FIELDS( CSDCI );
    ADD_FIELDS( WDCI1 );
    ADD_FIELDS( WDCI2 );
    ADD_FIELDS( WGFP );
    ADD_FIELDS( TGFP );
    ADD_FIELDS( CDH );
}