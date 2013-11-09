#pragma once

/*
 * 当删除一个图元，要同时删除图元上的标签图元TagGE
 * 以及关联的模型图元ModelGE
 */
class MineGEErase_DbReactor : public AcDbDatabaseReactor 
{
protected:
	AcDbDatabase *mpDatabase ;

public:
	MineGEErase_DbReactor (AcDbDatabase *pDb =NULL) ;
	virtual ~MineGEErase_DbReactor () ;

	virtual void Attach (AcDbDatabase *pDb) ;
	virtual void Detach () ;
	virtual AcDbDatabase *Subject () const ;
	virtual bool IsAttached () const ;

	/*
	 * 当图元被删除，同时也应删除图元关联的标签图元(TagGE)
	 */
	virtual void objectErased(const AcDbDatabase* dwg, const AcDbObject* dbObj, Adesk::Boolean pErased);
} ;
