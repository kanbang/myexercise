#include "StdAfx.h"
#include "LabelHelper.h"
#include "config.h"

// 循环到扩展数据标签组的最后一个节点
// 如果标签已存在，则返回0
static resbuf* GoToLastLabelNode( resbuf*& pAppNode, const CString& label )
{
    resbuf* pTemp;
    bool isFieldExist = false;
    // 跳过第1个节点
    for ( pTemp = pAppNode; pTemp->rbnext != NULL; pTemp = pTemp->rbnext )
    {
        // 标签比较不区分大小写
        if( label.CompareNoCase( pTemp->rbnext->resval.rstring ) == 0 )
        {
            // 存在相同的标签
            isFieldExist = true;
            break;
        }
    }
    return ( isFieldExist ? 0 : pTemp );
}

static bool RemoveLabelNode( resbuf* pAppNode, const CString& label )
{
    resbuf* pTemp, *pPos = 0;
    // 跳过第1个节点
    for ( pTemp = pAppNode->rbnext; pTemp != NULL; pTemp = pTemp->rbnext )
    {
        if( label.CompareNoCase( pTemp->resval.rstring ) == 0 )
        {
            // 存在相同的标签
            pPos = pTemp;
            break;
        }
    }
    if( pPos == 0 ) return false;

    // 2、查找字段的前一个位置
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
    pPos->rbnext = 0;    // 注意，要将rbnext置为0，否则acutRelRb会删除后面的链表数据
    acutRelRb( pPos );   // 该函数删除一个resbuf list，不只是一个node

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
        pTemp->rbnext = acutBuildList( AcDb::kDxfXdAsciiString, label, 0 ); // 增加标签
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
    bool xdataModified = false;   // 扩展数据是否被修改(增加或删除)
    for( int i = 0; i < len; i++ )
    {
        CString label( labels[i].kACharPtr() );
        resbuf* pTemp = GoToLastLabelNode( pAppNode, label );
        if( pTemp != 0 )
        {
            pTemp->rbnext = acutBuildList( AcDb::kDxfXdAsciiString, label, 0 ); // 增加标签
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
    bool xdataModified = false;   // 扩展数据是否被修改(增加或删除)
    for( int i = 0; i < len; i++ )
    {
        CString label( labels[i].kACharPtr() );
        // 只要有一个节点被删除，就标记扩展数据为已修改状态
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
    // 跳过第1个节点
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

    bool findLabel = false; // 是否找到标签
    // 跳过第1个节点
    for ( resbuf* pTemp = pAppNode->rbnext; pTemp != NULL; pTemp = pTemp->rbnext )
    {
        // 标签比较不区分大小写
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