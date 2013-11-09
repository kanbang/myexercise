#include "StdAfx.h"
#include "DataInitHelper.h"

#include "../MineGE/HelperClass.h"

#include <fstream>
// ��Ҫ����<fstream>����ʹ��
#include "AcFStream.h"

void DataInitHelper::InitStrList( const CString& fileName )
{
    acutPrintf( _T( "\n��·��: %s�����ַ����б�..." ), fileName );
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

// ��ʼ��"�ַ���-����"�б�
void DataInitHelper::InitIntStrList( const CString& fileName )
{
    acutPrintf( _T( "\n��·��: %s�����ַ���-�����б�..." ), fileName );
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
    acutPrintf( _T( "\n��·��: %s�������ݶ����б�..." ), fileName );

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
    acutPrintf( _T( "\n��·��: %s����ȫ����Ϣ..." ), fileName );

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

// ��ʼ��ͼԪ�ֶ�
// ���ļ��ж�ȡ���ݣ���ʽ
// ͼԪ����	�ֶ�1
// ͼԪ����	�ֶ�2
// ...
void DataInitHelper::InitDataField( const CString& fileName )
{
    acutPrintf( _T( "\n��·��: %s�����ֶ�" ), fileName );

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
