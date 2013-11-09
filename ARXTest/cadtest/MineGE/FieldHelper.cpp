#include "StdAfx.h"
#include "FieldHelper.h"

#include "config.h"
#include "MineGE.h"
#include "DataObject.h"
#include "FieldInfoHelper.h"

#include "../ArxHelper/HelperClass.h"

static bool IsEmptyString( const CString& str )
{
    return ( str.GetLength() == 0 );
}

static bool FindKey_Helper( const CString& dictName, const CString& type )
{
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( dictName );
    bool ret = pDictTool->findKey( type );
    delete pDictTool;
    return ret;
}

static void RemoveKey_Helper( const CString& dictName, const CString& type )
{
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( dictName );
    pDictTool->removeKey( type );
    delete pDictTool;
}

static void GetAllKeys_Helper( const CString& dictName, AcStringArray& keys )
{
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( dictName );
    pDictTool->getAllKeys( keys );
    delete pDictTool;
}

static bool AddField_Helper( const CString& dictName, const CString& type, const CString& field )
{
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( dictName );
    int index = pDictTool->addEntry( type, field );
    delete pDictTool;
    return ( index != INVALID_ENTRY );
}

static int RemoveField_Helper( const CString& dictName, const CString& type, const CString& field )
{
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( dictName );
    int index = pDictTool->removeEntry( type, field );
    delete pDictTool;
    return index;
}

static int CountFields_Helper( const CString& dictName, const CString& type )
{
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( dictName );
    int count = pDictTool->countEntries( type );
    delete pDictTool;
    return count;
}

static int FindField_Helper( const CString& dictName, const CString& type, const CString& field )
{
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( dictName );
    int index = pDictTool->findEntry( type, field );
    delete pDictTool;
    return index;
}

static void GetAllFields_Helper( const CString& dictName, const CString& type, AcStringArray& fields )
{
    ArxDictTool* pDictTool = ArxDictTool::GetDictTool( dictName );
    pDictTool->getAllEntries( type, fields );
    delete pDictTool;
}

// ��ͼԪ����չ�ʵ��в������ݶ���
static void GetDataObjectFromExtDict( AcDbObjectIdArray& dbObjIds )
{
    AcDbObjectIdArray allObjIds;
    ArxDataTool::GetEntsByType( _T( "MineGE" ), allObjIds, true );

    // �ж��Ƿ����ݶ���DataObject
    // ����������==type
    if( allObjIds.isEmpty() ) return;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = allObjIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, allObjIds[i], AcDb::kForRead ) ) continue;

        MineGE* pGE = MineGE::cast( pObj );
        if( pGE == 0 ) continue;

        AcDbObjectId dbObjId = pGE->getDataObject();
        if( !dbObjId.isNull() )
        {
            dbObjIds.append( dbObjId );
        }
    }
    actrTransactionManager->endTransaction();
}

// ��ȡ�ʵ��µ�����
static void GetDataObjectFromDict( const CString& dictName, AcDbObjectIdArray& dbObjIds )
{
    AcDbObjectIdArray allObjIds;
    ArxDictTool2* pDict = ArxDictTool2::GetDictTool( dictName );
    pDict->getAllEntries( allObjIds );
    delete pDict;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = allObjIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, allObjIds[i], AcDb::kForRead ) ) continue;

        if( pObj->isKindOf( DataObject::desc() ) )
        {
            dbObjIds.append( allObjIds[i] );
        }
    }
    actrTransactionManager->endTransaction();
}

// �����������Ʋ���
static void FilterDataObject( const AcDbObjectIdArray& dbObjIds, const CString& type, AcDbObjectIdArray& objIds )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = dbObjIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, dbObjIds[i], AcDb::kForRead ) ) continue;

        DataObject* pDO = DataObject::cast( pObj );
        if( pDO == 0 ) continue;

        if( type == pDO->getType() )
        {
            objIds.append( dbObjIds[i] );
        }
    }
    actrTransactionManager->endTransaction();
}

static void GetDataObjectsByType( const CString& type, AcDbObjectIdArray& objIds )
{
    AcDbObjectIdArray dbObjIds;

    // ��ͼԪ����չ�ʵ��в���
    GetDataObjectFromExtDict( dbObjIds );
    // ��OBJECT_LIST_DATA_DICT�ֵ��в���
    GetDataObjectFromDict( OBJECT_LIST_DATA_DICT, objIds );
    // ��GLOBAL_SINGLE_INFO_DICT�ʵ��в���
    GetDataObjectFromDict( GLOBAL_SINGLE_INFO_DICT, objIds );

    // �������Ͳ�����type��DataObject
    FilterDataObject( dbObjIds, type, objIds );
}

static void AddFieldToDataObject( const AcDbObjectIdArray& objIds )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForWrite ) ) continue;

        DataObject* pDO = DataObject::cast( pObj );
        if( pDO == 0 ) continue;

        pDO->addData(); // ��ĩβ����һ������
    }
    actrTransactionManager->endTransaction();
}

static void RemoveFieldFromDataObject( const AcDbObjectIdArray& objIds, int index )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForWrite ) ) continue;

        DataObject* pDO = DataObject::cast( pObj );
        if( pDO == 0 ) continue;

        pDO->removeData( index ); // ɾ��ָ��λ�õ�����
    }
    actrTransactionManager->endTransaction();
}

static void CleanAllFieldFromDataObject( const AcDbObjectIdArray& objIds )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return;

    int len = objIds.length();
    for( int i = 0; i < len; i++ )
    {
        AcDbObject* pObj;
        if( Acad::eOk != pTrans->getObject( pObj, objIds[i], AcDb::kForWrite ) ) continue;

        DataObject* pDO = DataObject::cast( pObj );
        if( pDO == 0 ) continue;

        pDO->clearAll(); // ɾ����������
    }
    actrTransactionManager->endTransaction();
}

bool FieldHelper::AddField( const CString& type, const CString& field )
{
    if( IsEmptyString( type ) || IsEmptyString( field ) ) return false;

    bool ret = AddField_Helper( PROPERTY_DATA_FIELD_DICT, type, field );
    if( ret )
    {
        AcDbObjectIdArray objIds;
        GetDataObjectsByType( type, objIds );
        if( !objIds.isEmpty() )
        {
            AddFieldToDataObject( objIds );
        }
    }
    return ret;
}

bool FieldHelper::RemoveField( const CString& type, const CString& field )
{
    if( IsEmptyString( type ) || IsEmptyString( field ) ) return false;

    int index = RemoveField_Helper( PROPERTY_DATA_FIELD_DICT, type, field );
    bool ret = ( INVALID_ENTRY != index );
    if( ret )
    {
        // ɾ���ֶ���Ϣ
        FieldInfoHelper::RemoveFieldInfo( type, field );

        // ɾ��ͼԪ�е��ֶ�
        AcDbObjectIdArray objIds;
        GetDataObjectsByType( type, objIds );
        if( !objIds.isEmpty() )
        {
            RemoveFieldFromDataObject( objIds, index );
        }
    }
    return ret;
}

// ʵ��������???
void FieldHelper::RemoveAllFields( const CString& type )
{
    AcStringArray fields;
    GetAllFields( type, fields );
    int len = fields.length();
    for( int i = 0; i < len; i++ )
    {
        RemoveField( type, fields[i].kACharPtr() );
    }
    RemoveKey_Helper( PROPERTY_DATA_FIELD_DICT, type );
}

int FieldHelper::CountFields( const CString& type )
{
    return CountFields_Helper( PROPERTY_DATA_FIELD_DICT, type );
}

int FieldHelper::FindField( const CString& type, const CString& field )
{
    return FindField_Helper( PROPERTY_DATA_FIELD_DICT, type, field );
}

void FieldHelper::GetAllFields( const CString& type, AcStringArray& fields )
{
    GetAllFields_Helper( PROPERTY_DATA_FIELD_DICT, type, fields );
}

void FieldHelper::GetAllRegTypes( AcStringArray& types )
{
    // ���Ͱ���3���֣�
    //   1) arx��������MineGE���͵�ͼԪ
    ArxClassHelper::GetAllTopParentClass( _T( "MineGE" ), types );

    //   2) arx��������ModelGE���͵�ͼԪ
    ArxClassHelper::GetAllTopParentClass( _T( "ModelGE" ), types );

    //   3.1) �ڴʵ�OBJECT_LIST_DICT��ע�������
    GetAllKeys_Helper( OBJECT_LIST_DICT, types );

    //   3.2) �ڴʵ�GLOBAL_SINGLE_INFO_DICT��ע�������
    GetAllKeys_Helper( GLOBAL_SINGLE_INFO_DICT, types );
}
