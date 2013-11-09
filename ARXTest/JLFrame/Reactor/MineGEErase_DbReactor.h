#pragma once

/*
 * ��ɾ��һ��ͼԪ��Ҫͬʱɾ��ͼԪ�ϵı�ǩͼԪTagGE
 * �Լ�������ģ��ͼԪModelGE
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
	 * ��ͼԪ��ɾ����ͬʱҲӦɾ��ͼԪ�����ı�ǩͼԪ(TagGE)
	 */
	virtual void objectErased(const AcDbDatabase* dwg, const AcDbObject* dbObj, Adesk::Boolean pErased);
} ;
