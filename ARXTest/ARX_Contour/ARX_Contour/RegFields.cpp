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

// 瓦斯突出点
BEGIN_DEFINE_FIELDS( GOP )
DEFINE_FIELD( 突出强度 )
DEFINE_FIELD( 突出煤量 )
DEFINE_FIELD( 涌出瓦斯总量 )
DEFINE_FIELD( 标高 )
DEFINE_FIELD( 突出年月日 )
END_DEFINE_FIELDS

// 瓦斯含量
BEGIN_DEFINE_FIELDS( GCP )
DEFINE_FIELD( 瓦斯含量 )
DEFINE_FIELD( 测点标高 )
DEFINE_FIELD( 埋深 )
END_DEFINE_FIELDS

// 瓦斯压力
BEGIN_DEFINE_FIELDS( GPP )
DEFINE_FIELD( 瓦斯压力值 )
DEFINE_FIELD( 测点标高 )
DEFINE_FIELD( 埋深 )
END_DEFINE_FIELDS

// 煤与瓦斯突出
BEGIN_DEFINE_FIELDS( PPP )
DEFINE_FIELD( 突出煤岩量 )
DEFINE_FIELD( 涌出瓦斯量 )
DEFINE_FIELD( 标高 )
DEFINE_FIELD( 发生年月 )
END_DEFINE_FIELDS

// 预测指标1
BEGIN_DEFINE_FIELDS( CSDCI )
DEFINE_FIELD( 瓦斯放散初速度 )
DEFINE_FIELD( 煤的坚固性系数 )
END_DEFINE_FIELDS

// 工作面预测指标1
BEGIN_DEFINE_FIELDS( WDCI1 )
DEFINE_FIELD( 钻屑解吸指标 )
DEFINE_FIELD( 钻孔最大钻屑量 )
END_DEFINE_FIELDS

// 工作面预测指标2
BEGIN_DEFINE_FIELDS( WDCI2 )
DEFINE_FIELD( 钻孔最大瓦斯涌出初速度 )
DEFINE_FIELD( 钻孔最大钻屑量 )
DEFINE_FIELD( R值指标 )
END_DEFINE_FIELDS

// 工作面瓦斯涌出量
BEGIN_DEFINE_FIELDS( WGFP )
DEFINE_FIELD( 绝对瓦斯涌出量 )
DEFINE_FIELD( 相对瓦斯涌出量 )
DEFINE_FIELD( 工作面日产量 )
DEFINE_FIELD( 回采年月 )
END_DEFINE_FIELDS

// 掘进面瓦斯涌出量
BEGIN_DEFINE_FIELDS( TGFP )
DEFINE_FIELD( 绝对瓦斯涌出量 )
DEFINE_FIELD( 掘进年月 )
END_DEFINE_FIELDS

// 见煤钻孔
BEGIN_DEFINE_FIELDS( CDH )
DEFINE_FIELD( 孔号 )
DEFINE_FIELD( 地面标高 )
DEFINE_FIELD( 煤层底板标高 )
DEFINE_FIELD( 煤厚 )
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