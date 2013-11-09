#pragma once

/*
 * �����ݶ���(DataObject)���޸�ʱ�����¹���ͼԪ�Ŀ��ӻ�Ч��
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
	 * �����ݶ����ں�̨���ı䣬ǿ�Ƹ�����֮������ͼԪ�Ŀ��ӻ�Ч��
	 * ��ЩͼԪ�Ŀ��ӻ�Ч���������йأ�������վ(WindStation)����������(Direction)
	 */
	virtual void objectModified(const AcDbDatabase* dwg, const AcDbObject* dbObj);
} ;
