#include "StdAfx.h"
#include "ConstMinQ_AFC_CheckStrategy.h"

namespace PJ
{
    ConstMinQ_AFC_CheckStrategy::ConstMinQ_AFC_CheckStrategy( const CString& tableName ) : AFC_CheckStrategy( tableName )
    {
    }

    double ConstMinQ_AFC_CheckStrategy::minQ()
    {
        return 100;
    }
}