#include "StdAfx.h"
#include "MVSSData.h"

#define COMMENT_FIELD _T("备注")

// 表名称
static CString MVSS_TABLE_NAME[4] =
{
    _T( "节点" ),         // MDT_JOINT
    _T( "巷道" ),         // MDT_TUNNEL
    _T( "构筑物" ),       // MDT_GATE
    _T( "通风动力装置" ) // MDT_FAN
};

static CString FIELD_TYPE_NAME[4] =
{
    _T( "VNG_Node" ),
    _T( "VNG_Edge" ),
    _T( "VNG_Gate" ),
    _T( "VNG_Fan" )
};

CString GetTableName( MVSS_DATA_TYPE type )
{
    return MVSS_TABLE_NAME[type];
}

CString GetTypeName( MVSS_DATA_TYPE type )
{
    return FIELD_TYPE_NAME[type];
}

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

MVSSData::MVSSData() : id( INVALID_OBJ_ID )
{

}

bool MVSSData::isValid() const
{
    return ( id != INVALID_OBJ_ID );
}

JointData::JointData() : MVSSData()
{

}

void JointData::readData( const CString& data )
{
    this->id = _ttoi( data );
}

TunnelData::TunnelData() : MVSSData(), spt( INVALID_OBJ_ID ), ept( INVALID_OBJ_ID )
{
}

void TunnelData::readData( const CString& data )
{
    AcStringArray cc;
    SplitCString( data, _T( "_" ), cc );

    this->id = _ttoi( cc[0].kACharPtr() );
    this->spt = _ttoi( cc[1].kACharPtr() );
    this->ept = _ttoi( cc[2].kACharPtr() );
}

bool TunnelData::isValid() const
{
    return MVSSData::isValid() &&
           ( spt != INVALID_OBJ_ID ) &&
           ( ept != INVALID_OBJ_ID );
}

GateData::GateData() : MVSSData(), tObjId( INVALID_OBJ_ID ) {}

void GateData::readData( const CString& data )
{
    AcStringArray cc;
    SplitCString( data, _T( "_" ), cc );

    this->id = _ttoi( cc[0].kACharPtr() );
    this->tObjId = _ttoi( cc[1].kACharPtr() );
}

bool GateData::isValid() const
{
    return MVSSData::isValid() &&
           ( tObjId != INVALID_OBJ_ID );
}

FanData::FanData() : MVSSData(), tObjId( INVALID_OBJ_ID )
{

}

void FanData::readData( const CString& data )
{
    AcStringArray cc;
    SplitCString( data, _T( "_" ), cc );

    this->id = _ttoi( cc[0].kACharPtr() );
    this->tObjId = _ttoi( cc[1].kACharPtr() );
}

bool FanData::isValid() const
{
    return MVSSData::isValid() &&
           ( tObjId != INVALID_OBJ_ID );
}

MVSSData* CreateMVSSData( MVSS_DATA_TYPE type )
{
    MVSSData* pData = 0;
    switch( type )
    {
    case MDT_NODE:
        pData = new JointData();
        break;

    case MDT_EDGE:
        pData = new TunnelData();
        break;

    case MDT_GATE:
        pData = new GateData();
        break;

    case MDT_FAN:
        pData = new FanData();
        break;
    }
    return pData;
}

bool ReadComment( CDaoRecordset& rs, CString& data )
{
    COleVariant v;
    rs.GetFieldValue( COMMENT_FIELD, v );
    if( v.vt == VT_NULL ) return false;

    v.ChangeType( VT_BSTR );
    data = v.bstrVal;

    return ( data.CompareNoCase( _T( "null" ) ) != 0 );
}