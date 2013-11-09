#pragma once

// ���ݶ���
class DataObject : public AcDbObject {

public:
	ACRX_DECLARE_MEMBERS(DataObject) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	DataObject () ;
	virtual ~DataObject () ;

	// ��ȡͼԪid
	AcDbObjectId getGE() const;

	// ����ͼԪid
	void setGE(const AcDbObjectId& objId);

	// ������������
	bool setType(const CString& type);

	// ��ȡ��������
	CString getType() const;

	// �����������Ƴ�ʼ������
	// ��������ֶΣ�����ӿ��ַ�����������
	void initData();

	// ��ȡ����, posΪ0��ʼ������ֵ
	bool getData(int pos, CString& value) const;

	// ��������, posΪ0��ʼ������ֵ
	bool setData(int pos, const CString& value);

	// ����һ������
	void addData();

	// ɾ��ָ��λ��
	void removeData(int pos);

	// ���
	void clearAll();

	// �ڲ�ʹ��
	void getAllData(AcStringArray& values);
	void setAllData(const AcStringArray& values);

	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

private:
	bool isValidIndex(int pos) const;

private:
	CString m_type;          // ��������
	AcStringArray m_values;  // ����ֵ
	AcDbObjectId m_objId;    // ������ͼԪ(���Ϊnull, ��ʾ��һ�����ַ���ӳ������ݶ���)
	                         // ��ָ������(Soft Pointer),����deepClone��wbloclClone�����ᴦ�����id
} ;

#ifdef MINEGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(DataObject)
#endif
