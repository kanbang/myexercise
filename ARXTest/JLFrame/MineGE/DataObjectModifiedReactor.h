#pragma once

/*
 * 当数据对象(DataObject)被修改时，更新关联图元的可视化效果
 */
class DataObjectModifiedReactor : public AcDbDatabaseReactor 
{
protected:
	AcDbDatabase *mpDatabase ;

public:
	DataObjectModifiedReactor (AcDbDatabase *pDb =NULL) ;
	virtual ~DataObjectModifiedReactor () ;

	virtual void Attach (AcDbDatabase *pDb) ;
	virtual void Detach () ;
	virtual AcDbDatabase *Subject () const ;
	virtual bool IsAttached () const ;

	/*
	 * 当数据对象在后台被改变，强制更新与之关联的图元的可视化效果
	 * 有些图元的可视化效果与数据有关，例如测风站(WindStation)、风流方向(Direction)
	 */
	virtual void objectModified(const AcDbDatabase* dwg, const AcDbObject* dbObj);
} ;
