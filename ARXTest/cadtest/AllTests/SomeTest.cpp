#include "StdAfx.h"
#include "SomeTest.h"

#include "FilePathHelper.h"

#include "../MineGE/MineGE.h"
#include "../MineGE/HelperClass.h"

#include "../ArxHelper/HelperClass.h"

void SomeTest::TestData( void )
{
    // 增加字段
    FieldHelper::AddField( _T( "Tunnel" ), _T( "名称" ) );
    FieldHelper::AddField( _T( "Tunnel" ), _T( "风量" ) );
    FieldHelper::AddField( _T( "Tunnel" ), _T( "风速" ) );
    FieldHelper::AddField( _T( "Tunnel" ), _T( "温度" ) );
    FieldHelper::AddField( _T( "Tunnel" ), _T( "湿度" ) );
    FieldHelper::AddField( _T( "Tunnel" ), _T( "空气密度" ) );
    FieldHelper::AddField( _T( "Tunnel" ), _T( "风阻" ) );
    FieldHelper::AddField( _T( "Tunnel" ), _T( "百米风阻" ) );
    FieldHelper::AddField( _T( "Tunnel" ), _T( "支护方式" ) );

    FieldHelper::AddField( _T( "Tunnel" ), _T( "名称" ) );
    FieldHelper::AddField( _T( "TTunnel" ), _T( "局扇台数" ) );
    FieldHelper::AddField( _T( "TTunnel" ), _T( "局扇功率" ) );
    FieldHelper::AddField( _T( "TTunnel" ), _T( "局扇最大风量" ) );
    FieldHelper::AddField( _T( "TTunnel" ), _T( "瓦斯浓度" ) );
    FieldHelper::AddField( _T( "TTunnel" ), _T( "粉尘浓度" ) );

    // 结果==>添加失败
    FieldHelper::AddField( _T( "Joint" ), _T( "名称" ) );
    FieldHelper::AddField( _T( "Joint" ), _T( "坐标" ) );
    FieldHelper::AddField( _T( "Joint" ), _T( "压力" ) );

    FieldHelper::AddField( _T( "MainFan" ), _T( "名称" ) );
    FieldHelper::AddField( _T( "MainFan" ), _T( "最大风量" ) );
    FieldHelper::AddField( _T( "MainFan" ), _T( "功率" ) );
    FieldHelper::AddField( _T( "MainFan" ), _T( "电压" ) );
    FieldHelper::AddField( _T( "MainFan" ), _T( "电流" ) );

    // 临时增加字段
    FieldHelper::AddField( _T( "AFCTest" ), _T( "名称" ) );

    // 删除字段
    FieldHelper::RemoveField( _T( "AFCTest" ), _T( "名称" ) );

    // 删除一个不存在的字段
    FieldHelper::RemoveField( _T( "AFCTest" ), _T( "我的任意" ) );
    FieldHelper::RemoveField( _T( "Tunnel" ), _T( "湿度" ) );
}

void SomeTest::TestData2( void )
{
    FieldHelper::AddField( _T( "ArcTunnel" ), _T( "弯曲程度" ) );
}

void SomeTest::PrintData( void )
{
    // 打印属性数据
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "请选择一个图元:" ) );
    if( objId.isNull() ) return;

    // 不是MineGE类型图元
    if( !ArxUtilHelper::IsEqualType( _T( "MineGE" ), objId ) ) return;

    // 查询类型名称，获取所有的字段
    AcDbObject* pObj;
    acdbOpenObject( pObj, objId, AcDb::kForRead );
    MineGE* pGE = MineGE::cast( pObj );
    AcStringArray fields;
    FieldHelper::GetAllFields( pGE->getTypeName(), fields );
    pGE->close();

    if( fields.isEmpty() )
    {
        acutPrintf( _T( "\n该图元没有数据..." ) );
        return;
    }

    acutPrintf( _T( "\n打印属性数据...\n" ) );
    int len = fields.length();
    for( int i = 0; i < len; i++ )
    {
        CString field = fields.at( i ).kACharPtr();
        CString value;
        if( !DataHelper::GetPropertyData( objId, field, value ) ) continue;
        acutPrintf( _T( "\t%s -- \t%s\n" ), field, value );
    }
}

void SomeTest::ChangeData( void )
{
    // 修改数据测试
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "请选择一条巷道" ) );
    if( objId.isNull() ) return;

    // 不是巷道类型
    if( !ArxUtilHelper::IsEqualType( _T( "Tunnel" ), objId ) ) return;

    DataHelper::SetPropertyData( objId, _T( "名称" ), _T( "主运输大巷" ) );
    DataHelper::SetPropertyData( objId, _T( "风量" ), _T( "300" ) );
    DataHelper::SetPropertyData( objId, _T( "风阻" ), _T( "0.12345" ) );
    DataHelper::SetPropertyData( objId, _T( "风速" ), _T( "3.6" ) );
    DataHelper::SetPropertyData( objId, _T( "温度" ), _T( "26.7" ) );
}

void SomeTest::ConfigDrawTest( void )
{
    acutPrintf( _T( "\n测试config draw功能..." ) );

    //DrawHelper::ConfigDraw(_T("Joint"), _T("CircularJointDraw"));

    DrawHelper::ConfigDraw( _T( "Tunnel" ), _T( "DoubleTunnelDraw" ) );
}

void SomeTest::DisplayAllGETypes( void )
{
    acutPrintf( _T( "\n显示所有已注册的MineGE类型图元...\n" ) );
    AcStringArray allGETypes;
    DrawHelper::GetAllRegGETypesForDraw( allGETypes );

    if( allGETypes.isEmpty() ) return;

    int len = allGETypes.length();
    for( int i = 0; i < len; i++ )
    {
        acutPrintf( _T( "\t%s\n" ), allGETypes[i].kACharPtr() );
    }
}

void SomeTest::DisplayAllGETypes2( void )
{
    acutPrintf( _T( "\n显示所有已注册的MineGE类型图元...\n" ) );
    AcStringArray allGETypes;
    FieldHelper::GetAllRegTypes( allGETypes );
    if( allGETypes.isEmpty() ) return;

    int len = allGETypes.length();
    for( int i = 0; i < len; i++ )
    {
        acutPrintf( _T( "\t%s\n" ), allGETypes[i].kACharPtr() );
    }
}

void SomeTest::AddLayerTest( void )
{
    acutPrintf( _T( "\n建立图层测试...\n" ) );
    LayerHelper::AddLayer( _T( "巷道" ) ); // "巷道"图层
    LayerHelper::AddLayer( _T( "节点" ) ); // "节点"图层
    LayerHelper::AddLayer( _T( "风门" ) ); // "风门"图层
    LayerHelper::AddLayer( _T( "风机" ) ); // "风机"图层
}

void SomeTest::ChangeLayerTest( void )
{
    acutPrintf( _T( "\n修改图层测试...\n" ) );

    LayerHelper::SetCurrentLayer( _T( "巷道" ) );
    LayerHelper::FrozenLayer( _T( "巷道" ), true );

    LayerHelper::AddLayer( _T( "test" ) );
    LayerHelper::RemoveLayer( _T( "test" ) );

    LayerHelper::RenameLayer( _T( "巷道" ), _T( "巷道xxx" ) );
}


static void WriteHeader( CStdioFile& outFile, const CString& geType, const AcStringArray& fields )
{
    outFile.WriteString( geType );
    outFile.WriteString( _T( "\n" ) );

    CString str;
    int len = fields.length();
    for( int i = 0; i < len; i++ )
    {
        str.Append( fields[i].kACharPtr() );
        if( i < len - 1 ) str.Append( _T( "\t" ) );
    }
    outFile.WriteString( str );
    outFile.WriteString( _T( "\n" ) );
}

static void WriteDataToFile( CStdioFile& outFile, const CString& geType, const AcStringArray& fields )
{
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( geType, objIds );
    if( objIds.isEmpty() ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        CString str;
        // 后续写入图元的数据
        int len2 = fields.length();
        for( int j = 0; j < len2; j++ )
        {
            // 字段
            CString field = fields[j].kACharPtr();
            CString value;
            if( !DataHelper::GetPropertyData( objIds[i], field, value ) ) continue;
            str.Append( value );
            if( j < len2 - 1 ) str.Append( _T( "\t" ) );
        }
        outFile.WriteString( str );
        outFile.WriteString( _T( "\n" ) );
    }
}

// 用strtok实现
//static void SplitCString(const TCHAR* content, const TCHAR* token, AcStringArray& cc)
//{
//	if(content == 0) return;
//	int len = _tcslen(content);
//	if(len<=0) return;
//	TCHAR* pBuf = (TCHAR*)malloc(len+1);
//	_tcscpy(pBuf, content);
//	TCHAR* str = _tcstok(pBuf, token);
//	while(str != 0)
//	{
//		cc.append(str);
//		str = _tcstok(0, token);
//	}
//}

static void SplitCString( const CString& content, const CString& token, AcStringArray& cc )
{
    int curPos = 0;
    CString str = content.Tokenize( token, curPos );
    while ( str != _T( "" ) )
    {
        cc.append( str );
        str = content.Tokenize( token, curPos );
    };
}

static void ReadHeader( CStdioFile& inFile, CString& geType, AcStringArray& fields )
{
    // 读取图元类型
    CString strLine;
    inFile.ReadString( strLine );
    geType = strLine.Trim();

    // 读取字段
    inFile.ReadString( strLine );
    SplitCString( strLine, _T( " \t" ), fields );
}

static void ReadDataFromFile( CStdioFile& inFile, const CString& geType, const AcStringArray& fields )
{
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( geType, objIds );
    if( objIds.isEmpty() ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        // 读取数据
        CString strLine;
        if( !inFile.ReadString( strLine ) ) continue;

        AcStringArray values;
        SplitCString( strLine, _T( " \t" ), values );

        // 后续写入图元的数据
        int len2 = values.length();
        if( len2 > fields.length() )
        {
            len2 = fields.length();
        }
        for( int j = 0; j < len2; j++ )
        {
            if( values[j].compareNoCase( _T( "null" ) ) == 0 ) continue; // 将null处理成占位符(忽略大小写)
            DataHelper::SetPropertyData( objIds[i], fields[j].kACharPtr(), values[j].kACharPtr() );
        }
    }
}

static void ExportData( const CString& path )
{
    acutPrintf( _T( "\n导出数据...\n" ) );
    AcStringArray allGETypes;
    FieldHelper::GetAllRegTypes( allGETypes );
    if( allGETypes.isEmpty() ) return;

    AcStringArray fields;
    int len = allGETypes.length();
    for( int i = 0; i < len; i++ )
    {
        FieldHelper::GetAllFields( allGETypes[i].kACharPtr(), fields );
        if( fields.isEmpty() ) continue;

        AcString fileName;
        fileName.format( _T( "%s\\%s.txt" ), path, allGETypes[i].kACharPtr() );
        CStdioFile outFile;
        if( !outFile.Open( fileName.kACharPtr(), CFile::modeCreate | CFile::modeWrite | CFile::typeText ) ) continue;

        WriteHeader( outFile, allGETypes[i].kACharPtr(), fields );
        WriteDataToFile( outFile, allGETypes[i].kACharPtr(), fields );

        outFile.Close();

        fields.removeAll();
    }
}

static void ImportData( const CString& path )
{
    acutPrintf( _T( "\n导入数据...\n" ) );

    // 通过文件对话框选择txt数据文件
    CFileDialog  openDialog( TRUE, _T( "txt" ), NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, _T( "图元属性数据文件(*.txt)|*.txt||" ) );
    openDialog.GetOFN().lpstrInitialDir = path; // 设定初始路径
    if( IDOK != openDialog.DoModal() ) return;

    CStdioFile inFile;
    if( !inFile.Open( ( LPCTSTR )openDialog.GetPathName(), CFile::modeRead | CFile::typeText ) ) return;

    // 读取图元类型和字段名称
    CString geType;
    AcStringArray fields;
    ReadHeader( inFile, geType, fields );		// 读第2行字段名称

    // 判断图元类型和字段是否合法
    AcStringArray allGETypes;
    FieldHelper::GetAllRegTypes( allGETypes );
    if( !allGETypes.contains( geType ) )
    {
        acutPrintf( _T( "\n非法的图元类型: %s..." ), geType );
        inFile.Close();
        return;
    }
    if( fields.isEmpty() )
    {
        inFile.Close();
        return;
    }

    // 读取测试数据到图元中
    ReadDataFromFile( inFile, geType, fields );

    inFile.Close();
}

/* 全局函数(实现在ShowDirectoryDlg.cpp)中 */
extern bool ShowDirectoryDlg( const CString& initPath, CString& dirPath );

void SomeTest::DoExportData()
{
    CString dirPath;
    if( !ShowDirectoryDlg( _T( "D:\\Test\\TestData" ), dirPath ) ) return;
    ExportData( dirPath );
}

void SomeTest::DoImportData()
{
    // 为了方便
    ImportData( _T( "D:\\Test\\TestData" ) );
}