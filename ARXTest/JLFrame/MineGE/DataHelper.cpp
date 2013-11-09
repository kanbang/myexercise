#include "StdAfx.h"
#include "DataHelper.h"
#include "DataHelperImpl.h"

#include "MineGE.h"
#include "DrawHelper.h"
#include "FieldHelper.h"

#include "../ArxHelper/HelperClass.h"

static bool GetDataObject_Helper( const AcDbObjectId& objId, AcDbObjectId& dObjId )
{
    if( objId.isNull() ) return false;

    // ʹ��ARX�ṩ��������ƴ�ͼԪ
    // 1) ��������
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    // 2) �򿪶��󣬻�ȡ����ָ��
    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForRead ) )
    {
        // ��ʧ�ܣ�����false
        actrTransactionManager->abortTransaction();
        return false;
    }

    // 3) �ж�ͼԪ������
    bool ret = true;
    if( pObj->isKindOf( MineGE::desc() ) )
    {
        // 3.1) ͼԪ��MineGE������
        // ���ȡͼԪ���������ݶ���
        MineGE* pGE = MineGE::cast( pObj );
        dObjId = pGE->getDataObject();
    }
    else if( pObj->isKindOf( DataObject::desc() ) )
    {
        // 3.2) �����������ݶ���(DataObject)
        // ��ֱ�Ӹ���ID
        dObjId = objId;
    }
    else
    {
        // 3.3) �������߶������ڣ��򷵻�false
        ret = false;
    }
    // 4) �ر�����
    actrTransactionManager->endTransaction();

    return ret;
}

static bool GetPropertyData_Helper( const AcDbObjectId& objId, const CString& fieldName, CString& value )
{
    // ʹ��ARX�ṩ��������ƴ�ͼԪ
    // 1) ��������
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    // 2) �򿪶��󣬻�ȡ����ָ��
    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForRead ) ) // read״̬
    {
        actrTransactionManager->abortTransaction();
        return false;
    }

    // 3) ת�������ݶ���ָ��
    DataObject* pDO = DataObject::cast( pObj );
    if( pDO == 0 )
    {
        // �������ݶ����򷵻�false
        actrTransactionManager->abortTransaction();
        return false;
    }

    // 4) ʹ�����ݶ������������(DataHelperImpl)��ȡ��������
    DataHelperImpl dh( pDO );
    bool ret = dh.getPropertyData( fieldName, value );

    // 5) �ر�����
    actrTransactionManager->endTransaction();

    return ret;
}

static bool SetPropertyData_Helper( const AcDbObjectId& objId, const CString& fieldName, const CString& value )
{
    // ʹ��ARX�ṩ��������ƴ�ͼԪ
    // 1) ��������
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    // 2) �򿪶��󣬻�ȡ����ָ��
    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }

    // 3) ת�������ݶ���ָ��
    DataObject* pDO = DataObject::cast( pObj );
    if( pDO == 0 )
    {
        // �������ݶ����򷵻�false
        actrTransactionManager->abortTransaction();
        return false;
    }

    // 4) ʹ�����ݶ������������(DataHelperImpl)������������
    DataHelperImpl dh( pDO );
    bool ret = dh.setPropertyData( fieldName, value );

    // 5) �ر�����
    actrTransactionManager->endTransaction();

    return ret;
}

static bool CopyPropertyData_Helper( const AcDbObjectId& sourceObjId, const AcDbObjectId& targetObjId )
{
    // ʹ��ARX�ṩ��������ƴ�ͼԪ
    // 1) ��������
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    // 2) ��Դ���ݶ��󣬻�ȡ����ָ��
    AcDbObject* pObj1;
    if( Acad::eOk != pTrans->getObject( pObj1, sourceObjId, AcDb::kForWrite ) )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }

    // 3) ��Ŀ�����ݶ��󣬻�ȡ����ָ��
    AcDbObject* pObj2;
    if( Acad::eOk != pTrans->getObject( pObj2, targetObjId, AcDb::kForWrite ) )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }

    // 4) ת�������ݶ���ָ��(Դ���ݶ���ָ��)
    DataObject* pDO1 = DataObject::cast( pObj1 );
    if( pDO1 == 0 )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }

    // 5) ת�������ݶ���ָ��(Ŀ�����ݶ���ָ��)
    DataObject* pDO2 = DataObject::cast( pObj2 );
    if( pDO2 == 0 )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }

    // 6) �ж��������������Ƿ�ƥ��
    if( pDO1->getType() != pDO2->getType() )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }
    // ������
    //acutPrintf(_T("source:%s \t target:%s\n"), pObj1->isA()->name(), pObj2->isA()->name());

    // 7) ��������(��Դ���ݶ���-->Ŀ�����ݶ���)
    AcStringArray values;
    pDO1->getAllData( values ); // ��ȡ��������
    pDO2->setAllData( values ); // ��������

    // 8) �ر�����
    actrTransactionManager->endTransaction();

    return true;
}

bool DataHelper::GetDataObject( const AcDbObjectId& objId, AcDbObjectId& dObjId )
{
    // ��ȡ���ݶ���
    return GetDataObject_Helper( objId, dObjId );
}

bool DataHelper::IsDataObject( const AcDbObjectId& objId )
{
    return ( !objId.isNull() &&
             ArxDataTool::IsEqualType( _T( "DataObject" ), objId, false ) );
}

bool DataHelper::GetTypeName( const AcDbObjectId& objId, CString& type )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    if( pTrans == 0 ) return false;

    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForRead ) )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }
    bool ret = true;
    if( pObj->isKindOf( MineGE::desc() ) )
    {
        MineGE* pGE = MineGE::cast( pObj );
        type = pGE->getTypeName();
    }
    else if( pObj->isKindOf( DataObject::desc() ) )
    {
        DataObject* pDO = DataObject::cast( pObj );
        type = pDO->getType();
    }
    else
    {
        ret = false;
    }
    actrTransactionManager->endTransaction();

    return ret;
}

bool DataHelper::GetPropertyData( const AcDbObjectId& objId, const CString& fieldName, CString& value )
{
    if( objId.isNull() ) return false;

    // ��ȡ���ݶ���
    AcDbObjectId dObjId;
    if( !DataHelper::GetDataObject( objId, dObjId ) ) return false;

    // ��ȡ��������
    return GetPropertyData_Helper( dObjId, fieldName, value );
}

bool DataHelper::SetPropertyData( const AcDbObjectId& objId, const CString& fieldName, const CString& value )
{
    if( objId.isNull() ) return false;

    // ��ȡ���ݶ���
    AcDbObjectId dObjId;
    if( !DataHelper::GetDataObject( objId, dObjId ) ) return false;

    // ������������
    return SetPropertyData_Helper( dObjId, fieldName, value );
}

bool DataHelper::CopyPropertyData( const AcDbObjectId& sourceObjId, const AcDbObjectId& targetObjId )
{
    if( sourceObjId.isNull() ) return false;
    if( targetObjId.isNull() ) return false;

    // ��ȡԴ���ݶ���
    AcDbObjectId dSourceObjId;
    if( !DataHelper::GetDataObject( sourceObjId, dSourceObjId ) ) return false;
    if( dSourceObjId.isNull() ) return false;

    // ��ȡĿ�����ݶ���
    AcDbObjectId dTargetObjId;
    if( !DataHelper::GetDataObject( targetObjId, dTargetObjId ) ) return false;
    if( dTargetObjId.isNull() ) return false;

    // �����ݴ�Դ���ݶ����Ƶ�Ŀ�����ݶ���
    return CopyPropertyData_Helper( dSourceObjId, dTargetObjId );
}