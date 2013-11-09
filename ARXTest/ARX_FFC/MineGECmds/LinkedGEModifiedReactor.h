#pragma once

// ��������仯ʱ�����������С�ı仯
class LinkedGEModifiedReactor : public AcDbDatabaseReactor 
{
protected:
	AcDbDatabase *mpDatabase ;

public:
	LinkedGEModifiedReactor (AcDbDatabase *pDb =NULL) ;
	virtual ~LinkedGEModifiedReactor () ;

	virtual void Attach (AcDbDatabase *pDb) ;
	virtual void Detach () ;
	virtual AcDbDatabase *Subject () const ;
	virtual bool IsAttached () const ;

	virtual void objectOpenedForModify(const AcDbDatabase* dwg, const AcDbObject* dbObj);
	virtual void objectModified(const AcDbDatabase* dwg, const AcDbObject* dbObj);

	AcDbObjectId m_objId;
	AcGePoint3d m_spt, m_ept;
} ;
