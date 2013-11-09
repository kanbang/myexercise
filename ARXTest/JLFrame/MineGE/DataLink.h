#pragma once

#include "dlimexp.h"
#include "ConstData.h"

/*
 * �������ӻ���
 * ���Թ������ݶ���(DataObject)��MineGE����
 *		1) �������ݶ��󣬿��Թ������е����ݣ���û�м���ͼ����Ϣ
 *		2) ����ͼԪ(MineGE����)���ڹ������ݵ�ͬʱ��Ҳ���Խ���ͼ�β���
 *			����ɾ������ɫ���л�ͼ�㡢�޸�/��ȡ�����
 */
class MINEGE_DLLIMPEXP DataLink
{
public:
	virtual ~DataLink(void);

	/*
	 * ��������Դ
	 * ����Դ�����Ǵ�MineGE������ͼԪ�����ݶ���DataObject
	 */
	void setDataSource(const AcDbObjectId& objId);

	/*
	 * ��ȡ����Դ
	 */
	AcDbObjectId getDataSource() const;

	/*
	 * ��ȡ���������
	 * ������
	 *    save -- false��ʾ�����ݸ��µ�����
	 *         -- true��ʾ���������µ����ݶ���
	 */
	bool updateData(bool save=false);

protected:
	/* ���캯�� */
	DataLink();

	/*
	 * ע��Ҫ��ȡ���ݵ��ֶ� 
	 * ���������Ҫ����ʵ�ָô��麯��
	 */
	virtual void regDatas() = 0;

	/* ��������
	 * �ṩ����������ã����ڽ���ͬ���͵ı���ӳ�䵽ͼԪ����Ӧ�ֶ�����
	 */
	/* ӳ���ַ������� */
	void linkStringData(const CString& field, CString* pValue);
	/* ӳ���������� */
	void linkIntData(const CString& field, int* pValue);
	/* ӳ�両�������� */
	void linkDoubleData(const CString& field, double* pValue);
	/* ӳ�䲼������ */
	void linkBoolData(const CString& field, bool* pValue);
	/* ӳ��ʱ�����ڱ��� */
	void linkDateTimeData(const CString& field, COleDateTime* pValue);

private:
	/* ����regDatas()����ע���ֶ� */
	void initDatas();
	/* ����ֶ��Ƿ���� */
	bool checkFields();
	// resbuf*ʵ�ʾ���һ����������һ���ĸ�ʽ��������
	// ���ݸ�ʽ���ֶ�1->�ֶ�����->ӳ��ı���ָ��->
	//            �ֶ�2->�ֶ�����->ӳ��ı���ָ��->...
	resbuf* m_pDatasToFileds;
	
	// ����Դ(�����Ǵ�MineGE������ͼԪ�����ݶ���DataObject)
	AcDbObjectId m_objId;
};
