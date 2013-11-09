#pragma once

#include "dlimexp.h"
#include "ConstData.h"

/*
 * �������ӻ���
 * ���Թ������ݶ���MineGE��������ModelGE������
 *		1) �������ݶ��󣬿��Թ������е����ݣ���û�м���ͼ����Ϣ
 *		2) ����ͼԪ(MineGE/ModelGE)���ڹ������ݵ�ͬʱ��Ҳ���Խ���ͼ�β���
 *			����ɾ������ɫ���л�ͼ�㡢�޸�/��ȡ�����
 */
class MINEGE_DLLIMPEXP DataLink
{
public:
	virtual ~DataLink(void);

	/*
	 * ��������Դ
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
	 *            true��ʾ���������µ����ݶ���
	 */
	bool updateData(bool save=false);

protected:
	DataLink();
	virtual void regDatas() = 0;
	void linkStringData(const CString& field, CString* pValue);
	void linkIntData(const CString& field, int* pValue);
	void linkDoubleData(const CString& field, double* pValue);
	void linkBoolData(const CString& field, bool* pValue);
	void linkDateTimeData(const CString& field, COleDateTime* pValue);

private:
	void initDatas();
	bool checkFields();
	// ���ݸ�ʽ���ֶ�����->�ֶ�����->ӳ��ı���ָ��->...
	resbuf* m_pDatasToFileds;
	AcDbObjectId m_objId;
};
