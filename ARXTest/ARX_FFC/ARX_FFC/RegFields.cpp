#include "stdafx.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

/*
 * 经过测试，定义数组如果最后一个元素多了一个逗号也能编译过去
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
// 展开结果
//static CString Tunnel_Fields[] = {
//	_T("断面面积"),
//	_T("风量")
//};

// 巷道字段(Tunnel)
BEGIN_DEFINE_FIELDS( Tunnel )
DEFINE_FIELD( 断面面积 )
DEFINE_FIELD( 风量 )
DEFINE_FIELD( 温度 )
DEFINE_FIELD( 甲烷浓度 )
DEFINE_FIELD( 氧气浓度 )
END_DEFINE_FIELDS

// 工作面字段(WorkSurface)
BEGIN_DEFINE_FIELDS( WorkSurface )
DEFINE_FIELD( 工作面瓦斯涌出量 )
END_DEFINE_FIELDS

// 风流方向字段(WindDirection)
BEGIN_DEFINE_FIELDS( WindDirection )
DEFINE_FIELD( 通风类型 )
END_DEFINE_FIELDS

// 密闭字段(Obturation)
BEGIN_DEFINE_FIELDS( Obturation )
DEFINE_FIELD( 漏风量 )
DEFINE_FIELD( 瓦斯浓度 )
DEFINE_FIELD( 氧气浓度 )
END_DEFINE_FIELDS

// 闭墙字段(SandWall)
BEGIN_DEFINE_FIELDS( SandWall )
DEFINE_FIELD( 名称 )
END_DEFINE_FIELDS

// 注氮孔字段(NitrogenPipe)
BEGIN_DEFINE_FIELDS( NitrogenPipe )
DEFINE_FIELD( 注氮量 )
END_DEFINE_FIELDS

// 瓦斯钻孔字段(GasPipe)
BEGIN_DEFINE_FIELDS( GasPipe )
DEFINE_FIELD( 抽放量 )
DEFINE_FIELD( 甲烷浓度 )
DEFINE_FIELD( 氧气浓度 )
END_DEFINE_FIELDS

// 漏风源字段(SourceAirLeak)
BEGIN_DEFINE_FIELDS( SourceAirLeak )
DEFINE_FIELD( 漏风量 )
END_DEFINE_FIELDS

// 漏风汇字段(SinkAirLeak)
BEGIN_DEFINE_FIELDS( SinkAirLeak )
DEFINE_FIELD( 漏风量 )
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