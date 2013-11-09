#include "StdAfx.h"
#include "DataInitHelper.h"

#include "../MineGE/HelperClass.h"

#include <fstream>
// 需要包含<fstream>才能使用
#include "AcFStream.h"

void DataInitHelper::InitStrList( const CString& fileName )
{
    acutPrintf( _T( "\n从路径: %s加载字符串列表..." ), fileName );
    AcIfstream inFile( fileName );
    if( !inFile ) return;

    while( !inFile.eof() )
    {
        ACHAR name[_MAX_PATH], strValue[_MAX_PATH];
        inFile >> name >> strValue;
        StringListHelper::AddString( name, strValue );
    }
    inFile.close();
}

// 初始化"字符串-整数"列表
void DataInitHelper::InitIntStrList( const CString& fileName )
{
    acutPrintf( _T( "\n从路径: %s加载字符串-整数列表..." ), fileName );
    AcIfstream inFile( fileName );
    if( !inFile ) return;

    while( !inFile.eof() )
    {
        ACHAR name[_MAX_PATH], strValue[_MAX_PATH];
        int intValue;
        inFile >> name >> intValue >> strValue;
        IntStrListHelper::AddIntStrPair( name, intValue, strValue );
    }
    inFile.close();
}

void DataInitHelper::InitDataObjectList( const CString& fileName )
{
    acutPrintf( _T( "\n从路径: %s加载数据对象列表..." ), fileName );

    AcIfstream inFile( fileName );
    if( !inFile ) return;

    while( !inFile.eof() )
    {
        ACHAR name[_MAX_PATH], value[_MAX_PATH];
        inFile >> name >> value;
        DataObjectListHelper::AddObject( name, value );
    }
    inFile.close();
}


void DataInitHelper::InitSingleDataObject( const CString& fileName )
{
    acutPrintf( _T( "\n从路径: %s加载全局信息..." ), fileName );

    AcIfstream inFile( fileName );
    if( !inFile ) return;

    while( !inFile.eof() )
    {
        ACHAR name[_MAX_PATH];
        inFile >> name;
        SingleDataObjectHelper::AddObject( name );
    }
    inFile.close();
}

// 初始化图元字段
// 从文件中读取数据，格式
// 图元类型	字段1
// 图元类型	字段2
// ...
void DataInitHelper::InitDataField( const CString& fileName )
{
    acutPrintf( _T( "\n从路径: %s加载字段" ), fileName );

    AcIfstream inFile( fileName );
    if( !inFile ) return;

    while( !inFile.eof() )
    {
        ACHAR getype[_MAX_PATH], field[_MAX_PATH];
        inFile >> getype >> field;
        //acutPrintf(_T("\n%s\t%s"), getype, field);
        FieldHelper::AddField( getype, field );
    }
    inFile.close();
}
