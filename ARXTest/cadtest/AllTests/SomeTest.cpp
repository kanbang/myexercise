#include "StdAfx.h"
#include "SomeTest.h"

#include "FilePathHelper.h"

#include "../MineGE/MineGE.h"
#include "../MineGE/HelperClass.h"

#include "../ArxHelper/HelperClass.h"

void SomeTest::TestData( void )
{
    // �����ֶ�
    FieldHelper::AddField( _T( "Tunnel" ), _T( "����" ) );
    FieldHelper::AddField( _T( "Tunnel" ), _T( "����" ) );
    FieldHelper::AddField( _T( "Tunnel" ), _T( "����" ) );
    FieldHelper::AddField( _T( "Tunnel" ), _T( "�¶�" ) );
    FieldHelper::AddField( _T( "Tunnel" ), _T( "ʪ��" ) );
    FieldHelper::AddField( _T( "Tunnel" ), _T( "�����ܶ�" ) );
    FieldHelper::AddField( _T( "Tunnel" ), _T( "����" ) );
    FieldHelper::AddField( _T( "Tunnel" ), _T( "���׷���" ) );
    FieldHelper::AddField( _T( "Tunnel" ), _T( "֧����ʽ" ) );

    FieldHelper::AddField( _T( "Tunnel" ), _T( "����" ) );
    FieldHelper::AddField( _T( "TTunnel" ), _T( "����̨��" ) );
    FieldHelper::AddField( _T( "TTunnel" ), _T( "���ȹ���" ) );
    FieldHelper::AddField( _T( "TTunnel" ), _T( "����������" ) );
    FieldHelper::AddField( _T( "TTunnel" ), _T( "��˹Ũ��" ) );
    FieldHelper::AddField( _T( "TTunnel" ), _T( "�۳�Ũ��" ) );

    // ���==>���ʧ��
    FieldHelper::AddField( _T( "Joint" ), _T( "����" ) );
    FieldHelper::AddField( _T( "Joint" ), _T( "����" ) );
    FieldHelper::AddField( _T( "Joint" ), _T( "ѹ��" ) );

    FieldHelper::AddField( _T( "MainFan" ), _T( "����" ) );
    FieldHelper::AddField( _T( "MainFan" ), _T( "������" ) );
    FieldHelper::AddField( _T( "MainFan" ), _T( "����" ) );
    FieldHelper::AddField( _T( "MainFan" ), _T( "��ѹ" ) );
    FieldHelper::AddField( _T( "MainFan" ), _T( "����" ) );

    // ��ʱ�����ֶ�
    FieldHelper::AddField( _T( "AFCTest" ), _T( "����" ) );

    // ɾ���ֶ�
    FieldHelper::RemoveField( _T( "AFCTest" ), _T( "����" ) );

    // ɾ��һ�������ڵ��ֶ�
    FieldHelper::RemoveField( _T( "AFCTest" ), _T( "�ҵ�����" ) );
    FieldHelper::RemoveField( _T( "Tunnel" ), _T( "ʪ��" ) );
}

void SomeTest::TestData2( void )
{
    FieldHelper::AddField( _T( "ArcTunnel" ), _T( "�����̶�" ) );
}

void SomeTest::PrintData( void )
{
    // ��ӡ��������
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ��ͼԪ:" ) );
    if( objId.isNull() ) return;

    // ����MineGE����ͼԪ
    if( !ArxUtilHelper::IsEqualType( _T( "MineGE" ), objId ) ) return;

    // ��ѯ�������ƣ���ȡ���е��ֶ�
    AcDbObject* pObj;
    acdbOpenObject( pObj, objId, AcDb::kForRead );
    MineGE* pGE = MineGE::cast( pObj );
    AcStringArray fields;
    FieldHelper::GetAllFields( pGE->getTypeName(), fields );
    pGE->close();

    if( fields.isEmpty() )
    {
        acutPrintf( _T( "\n��ͼԪû������..." ) );
        return;
    }

    acutPrintf( _T( "\n��ӡ��������...\n" ) );
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
    // �޸����ݲ���
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ�����" ) );
    if( objId.isNull() ) return;

    // �����������
    if( !ArxUtilHelper::IsEqualType( _T( "Tunnel" ), objId ) ) return;

    DataHelper::SetPropertyData( objId, _T( "����" ), _T( "���������" ) );
    DataHelper::SetPropertyData( objId, _T( "����" ), _T( "300" ) );
    DataHelper::SetPropertyData( objId, _T( "����" ), _T( "0.12345" ) );
    DataHelper::SetPropertyData( objId, _T( "����" ), _T( "3.6" ) );
    DataHelper::SetPropertyData( objId, _T( "�¶�" ), _T( "26.7" ) );
}

void SomeTest::ConfigDrawTest( void )
{
    acutPrintf( _T( "\n����config draw����..." ) );

    //DrawHelper::ConfigDraw(_T("Joint"), _T("CircularJointDraw"));

    DrawHelper::ConfigDraw( _T( "Tunnel" ), _T( "DoubleTunnelDraw" ) );
}

void SomeTest::DisplayAllGETypes( void )
{
    acutPrintf( _T( "\n��ʾ������ע���MineGE����ͼԪ...\n" ) );
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
    acutPrintf( _T( "\n��ʾ������ע���MineGE����ͼԪ...\n" ) );
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
    acutPrintf( _T( "\n����ͼ�����...\n" ) );
    LayerHelper::AddLayer( _T( "���" ) ); // "���"ͼ��
    LayerHelper::AddLayer( _T( "�ڵ�" ) ); // "�ڵ�"ͼ��
    LayerHelper::AddLayer( _T( "����" ) ); // "����"ͼ��
    LayerHelper::AddLayer( _T( "���" ) ); // "���"ͼ��
}

void SomeTest::ChangeLayerTest( void )
{
    acutPrintf( _T( "\n�޸�ͼ�����...\n" ) );

    LayerHelper::SetCurrentLayer( _T( "���" ) );
    LayerHelper::FrozenLayer( _T( "���" ), true );

    LayerHelper::AddLayer( _T( "test" ) );
    LayerHelper::RemoveLayer( _T( "test" ) );

    LayerHelper::RenameLayer( _T( "���" ), _T( "���xxx" ) );
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
        // ����д��ͼԪ������
        int len2 = fields.length();
        for( int j = 0; j < len2; j++ )
        {
            // �ֶ�
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

// ��strtokʵ��
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
    // ��ȡͼԪ����
    CString strLine;
    inFile.ReadString( strLine );
    geType = strLine.Trim();

    // ��ȡ�ֶ�
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
        // ��ȡ����
        CString strLine;
        if( !inFile.ReadString( strLine ) ) continue;

        AcStringArray values;
        SplitCString( strLine, _T( " \t" ), values );

        // ����д��ͼԪ������
        int len2 = values.length();
        if( len2 > fields.length() )
        {
            len2 = fields.length();
        }
        for( int j = 0; j < len2; j++ )
        {
            if( values[j].compareNoCase( _T( "null" ) ) == 0 ) continue; // ��null�����ռλ��(���Դ�Сд)
            DataHelper::SetPropertyData( objIds[i], fields[j].kACharPtr(), values[j].kACharPtr() );
        }
    }
}

static void ExportData( const CString& path )
{
    acutPrintf( _T( "\n��������...\n" ) );
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
    acutPrintf( _T( "\n��������...\n" ) );

    // ͨ���ļ��Ի���ѡ��txt�����ļ�
    CFileDialog  openDialog( TRUE, _T( "txt" ), NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, _T( "ͼԪ���������ļ�(*.txt)|*.txt||" ) );
    openDialog.GetOFN().lpstrInitialDir = path; // �趨��ʼ·��
    if( IDOK != openDialog.DoModal() ) return;

    CStdioFile inFile;
    if( !inFile.Open( ( LPCTSTR )openDialog.GetPathName(), CFile::modeRead | CFile::typeText ) ) return;

    // ��ȡͼԪ���ͺ��ֶ�����
    CString geType;
    AcStringArray fields;
    ReadHeader( inFile, geType, fields );		// ����2���ֶ�����

    // �ж�ͼԪ���ͺ��ֶ��Ƿ�Ϸ�
    AcStringArray allGETypes;
    FieldHelper::GetAllRegTypes( allGETypes );
    if( !allGETypes.contains( geType ) )
    {
        acutPrintf( _T( "\n�Ƿ���ͼԪ����: %s..." ), geType );
        inFile.Close();
        return;
    }
    if( fields.isEmpty() )
    {
        inFile.Close();
        return;
    }

    // ��ȡ�������ݵ�ͼԪ��
    ReadDataFromFile( inFile, geType, fields );

    inFile.Close();
}

/* ȫ�ֺ���(ʵ����ShowDirectoryDlg.cpp)�� */
extern bool ShowDirectoryDlg( const CString& initPath, CString& dirPath );

void SomeTest::DoExportData()
{
    CString dirPath;
    if( !ShowDirectoryDlg( _T( "D:\\Test\\TestData" ), dirPath ) ) return;
    ExportData( dirPath );
}

void SomeTest::DoImportData()
{
    // Ϊ�˷���
    ImportData( _T( "D:\\Test\\TestData" ) );
}