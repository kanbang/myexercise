#include "StdAfx.h"
#include "LabelHelper.h"
#include "config.h"

// ѭ������չ���ݱ�ǩ������һ���ڵ�
// �����ǩ�Ѵ��ڣ��򷵻�0
static resbuf* GoToLastLabelNode( resbuf*& pAppNode, const CString& label )
{
    resbuf* pTemp;
    bool isFieldExist = false;
    // ������1���ڵ�
    for ( pTemp = pAppNode; pTemp->rbnext != NULL; pTemp = pTemp->rbnext )
    {
        // ��ǩ�Ƚϲ����ִ�Сд
        if( label.CompareNoCase( pTemp->rbnext->resval.rstring ) == 0 )
        {
            // ������ͬ�ı�ǩ
            isFieldExist = true;
            break;
        }
    }
    return ( isFieldExist ? 0 : pTemp );
}

static bool RemoveLabelNode( resbuf* pAppNode, const CString& label )
{
    resbuf* pTemp, *pPos = 0;
    // ������1���ڵ�
    for ( pTemp = pAppNode->rbnext; pTemp != NULL; pTemp = pTemp->rbnext )
    {
        if( label.CompareNoCase( pTemp->resval.rstring ) == 0 )
        {
            // ������ͬ�ı�ǩ
            pPos = pTemp;
            break;
        }
    }
    if( pPos == 0 ) return false;

    // 2�������ֶε�ǰһ��λ��
    resbuf* pPrev;
    for ( pTemp = pAppNode; pTemp != NULL; pTemp = pTemp->rbnext )
    {
        if( pTemp->rbnext == pPos )
        {
            pPrev = pTemp;
            break;
        }
    }
    pPrev->rbnext = pPos->rbnext;
    pPos->rbnext = 0;    // ע�⣬Ҫ��rbnext��Ϊ0������acutRelRb��ɾ���������������
    acutRelRb( pPos );   // �ú���ɾ��һ��resbuf list����ֻ��һ��node

    return true;
}

void LabelHelper::AddLabel( const AcDbObjectId& objId, const CString& label )
{
    if( label.GetLength() == 0 ) return;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
    {
        actrTransactionManager->abortTransaction();
        return;
    }

    resbuf* pAppNode = pObj->xData( LABEL_XDATA_GROUP );
    if( pAppNode == 0 )
    {
        pAppNode = acutNewRb( AcDb::kDxfRegAppName );
        acutNewString( LABEL_XDATA_GROUP, pAppNode->resval.rstring );
    }
    resbuf* pTemp = GoToLastLabelNode( pAppNode, label );
    if( pTemp != 0 )
    {
        pTemp->rbnext = acutBuildList( AcDb::kDxfXdAsciiString, label, 0 ); // ���ӱ�ǩ
        pObj->setXData( pAppNode );
    }
    acutRelRb( pAppNode );

    actrTransactionManager->endTransaction();
}

void LabelHelper::AddMoreLabels( const AcDbObjectId& objId, const AcStringArray& labels )
{
    if( labels.isEmpty() ) return;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
    {
        actrTransactionManager->abortTransaction();
        return;
    }

    resbuf* pAppNode = pObj->xData( LABEL_XDATA_GROUP );
    if( pAppNode == 0 )
    {
        pAppNode = acutNewRb( AcDb::kDxfRegAppName );
        acutNewString( LABEL_XDATA_GROUP, pAppNode->resval.rstring );
    }
    int len = labels.length();
    bool xdataModified = false;   // ��չ�����Ƿ��޸�(���ӻ�ɾ��)
    for( int i = 0; i < len; i++ )
    {
        CString label( labels[i].kACharPtr() );
        resbuf* pTemp = GoToLastLabelNode( pAppNode, label );
        if( pTemp != 0 )
        {
            pTemp->rbnext = acutBuildList( AcDb::kDxfXdAsciiString, label, 0 ); // ���ӱ�ǩ
            xdataModified = true;
        }
    }
    if( xdataModified )
    {
        pObj->setXData( pAppNode );
    }
    acutRelRb( pAppNode );

    actrTransactionManager->endTransaction();
}

void LabelHelper::RemoveLabel( const AcDbObjectId& objId, const CString& label )
{
    if( label.GetLength() == 0 ) return;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
    {
        actrTransactionManager->abortTransaction();
        return;
    }

    resbuf* pAppNode = pObj->xData( LABEL_XDATA_GROUP );
    if( pAppNode != 0 )
    {
        if( RemoveLabelNode( pAppNode, label ) )
        {
            pObj->setXData( pAppNode );
        }
        acutRelRb( pAppNode );
    }

    actrTransactionManager->endTransaction();
}

void LabelHelper::RemoveMoreLabels( const AcDbObjectId& objId, const AcStringArray& labels )
{
    if( labels.isEmpty() ) return;

    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
    {
        actrTransactionManager->abortTransaction();
        return;
    }

    resbuf* pAppNode = pObj->xData( LABEL_XDATA_GROUP );
    if( pAppNode == 0 )
    {
        actrTransactionManager->abortTransaction();
        return;
    }

    int len = labels.length();
    bool xdataModified = false;   // ��չ�����Ƿ��޸�(���ӻ�ɾ��)
    for( int i = 0; i < len; i++ )
    {
        CString label( labels[i].kACharPtr() );
        // ֻҪ��һ���ڵ㱻ɾ�����ͱ����չ����Ϊ���޸�״̬
        if( RemoveLabelNode( pAppNode, label ) ) xdataModified = true;
    }
    if( xdataModified )
    {
        pObj->setXData( pAppNode );
    }
    acutRelRb( pAppNode );

    actrTransactionManager->endTransaction();
}

void LabelHelper::RemoveAllLabels( const AcDbObjectId& objId )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForWrite ) )
    {
        actrTransactionManager->abortTransaction();
        return;
    }

    resbuf* pAppNode = pObj->xData( LABEL_XDATA_GROUP );
    if( pAppNode != 0 )
    {
        resbuf* pTemp = pAppNode->rbnext;
        acutRelRb( pTemp );
        pAppNode->rbnext = 0;
        pObj->setXData( pAppNode );
        acutRelRb( pAppNode );
    }

    actrTransactionManager->endTransaction();
}

void LabelHelper::GetAllLabels( const AcDbObjectId& objId, AcStringArray& labels )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForRead ) )
    {
        actrTransactionManager->abortTransaction();
        return;
    }

    resbuf* pAppNode = pObj->xData( LABEL_XDATA_GROUP );
    if( pAppNode == 0 )
    {
        actrTransactionManager->abortTransaction();
        return;
    }

    resbuf* pTemp;
    // ������1���ڵ�
    for ( pTemp = pAppNode->rbnext; pTemp != NULL; pTemp = pTemp->rbnext )
    {
        labels.append( pTemp->resval.rstring );
    }
    acutRelRb( pAppNode );

    actrTransactionManager->endTransaction();
}

bool LabelHelper::HasLabel( const AcDbObjectId& objId, const CString& label )
{
    AcTransaction* pTrans = actrTransactionManager->startTransaction();
    AcDbObject* pObj;
    if( Acad::eOk != pTrans->getObject( pObj, objId, AcDb::kForRead ) )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }

    resbuf* pAppNode = pObj->xData( LABEL_XDATA_GROUP );
    if( pAppNode == 0 )
    {
        actrTransactionManager->abortTransaction();
        return false;
    }

    bool findLabel = false; // �Ƿ��ҵ���ǩ
    // ������1���ڵ�
    for ( resbuf* pTemp = pAppNode->rbnext; pTemp != NULL; pTemp = pTemp->rbnext )
    {
        // ��ǩ�Ƚϲ����ִ�Сд
        if( label.CompareNoCase( pTemp->resval.rstring ) == 0 )
        {
            findLabel = true;
            break;
        }
    }
    acutRelRb( pAppNode );

    actrTransactionManager->endTransaction();

    return findLabel;
}