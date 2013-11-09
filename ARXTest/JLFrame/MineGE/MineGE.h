#pragma once

#include "dlimexp.h"

// ���ӻ�Ч������
#include "../MineGEDraw/MineGEDraw.h"
// ��չ���ݶ�д�ӿڣ�����MineGE��MineGEDraw֮�佻������
#include "../ArxDbgXdata/XDataStream.h"

/*
 * ��ϵͳͼԪ����(������)
 *
 * ע1����������׼������������ģ�������Ӧ�ð���һ�����麯��
 * ע2��MINEGE_DLLIMPEXP���ʾDLL����/����ָ��
 */
class MINEGE_DLLIMPEXP MineGE : public AcDbEntity 
{
	/* arx�����ɵĴ��� */
	/*
	 * ���д�AcDbObject/AcDbEntity�����������ʹ��ACRX_DECLARE_MEMBERS��
	 * ���������RTTI��Ϣ
	 * ������MFC�е�DECLARE_DYNAMIC / IMPLEMENT_DYNAMIC
	 *
	 * ע������ARX����Ҫ�����е��Զ����඼�����ṩһ��Ĭ�Ϲ��캯��(�޲ι��캯��)
	 *      Ĭ�Ϲ��캯���ķ���Ȩ�޿�����public��Ҳ������protected
	 */
public:
	ACRX_DECLARE_MEMBERS(MineGE) ;

	/* arx�����ɵĴ��� */
protected:
	/* �������л�����ʾ��İ汾�� */
	static Adesk::UInt32 kCurrentVersionNumber;

	/* �ֶ���ӵĴ��� */
	/* �����ӿ� */
public:
	/* 
	 * ������������Ϊ�˽��������һ�����⣺
	 *		�����ָ��ָ����������󣬲��û����ָ��ɾ�����������
	 */
	virtual ~MineGE ();	

	/* 
	 * ��ȡͼԪ����������
	 */
	CString getTypeName() const;

	/*
	 * ���Ӳ���(Extra Param)���޸�֮�󣬸���ͼԪ���ӻ�Ч��
	 * ���磺�޸�˫������Ŀ��֮��Ӧ�õ��ø÷������¿��ӻ�Ч��
	 */
	void configDraw(const CString& drawName);

	/*
	 * �ؼ�����(Key Param)���޸�֮�󣬸���ͼԪ���ӻ�Ч��
	 * ���磺�����ʼĩ�����걻�޸ģ�Ӧ�õ��ø÷������¿��ӻ�Ч��
	 */
	void updateDraw();

	/*
	 * ��ȡͼԪ���������ݶ���ID
	 * AcDbObjectId��ARX�ṩ��һ�ֱ�ʾͼԪID�ĸ�����
	 */
	AcDbObjectId getDataObject() const;

	/* �ֶ���ӵĴ��� */
	/* ����ͼԪ�����������Ҫ����ʵ�ֵ��麯�� */
protected:
	/* 
	 * ���ؼ�����д�뵽writer��
	 * DrawParamWriterʵ�ʾ���һ���������ݽṹ
	 * �μ�:ArxDbgXData/XDataStream.h/DrawParamWriter��
	 */
	virtual void writeKeyParam(DrawParamWriter& writer) const = 0;

	/*
	 * ��reader�ж�ȡ�ؼ�����
	 * DrawParamReaderʵ�ʾ���һ���������ݽṹ
	 * �μ�:ArxDbgXData/XDataStream.h/DrawParamReader��
	 */
	virtual void readKeyParam(DrawParamReader& reader) = 0;

	/* arx�����ɵĴ��� */
	/* ����Ҫ����ʵ�ֵ�AcDbObject���麯�� */
public:
	/* 
	 * ��MFC�����л����ƣ�CAD����dwgOutFields()������ͼԪ�����ݱ��浽DWG�ļ���
	 *
	 * ע1������CAD����(�ƶ�����ת������)�Լ��е�༭ʱ�������һЩ��̬Ч��
	 *       ����Щ��̬Ч�������Ĺ����л�Ƶ���ĵ���dwgOutFields()����
	 * ע2���÷�����CAD�ڲ�����!!!
	 */
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;

	/*
	 * ��MFC�����л����ƣ�CAD����dwgInFields()������DWG�ļ��ж�ȡ���ݵ�ͼԪ��
	 *
	 * ע1������CAD����(�ƶ�����ת������)�Լ��е�༭ʱ�������һЩ��̬Ч��
	 *       ����Щ��̬Ч�������Ĺ����л�Ƶ���ĵ���dwgInFields()����
	 * ע2���÷�����CAD�ڲ�����!!!
	 */
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

	/* ͨ��arx��������ӵĴ��� */
	/* 
	 * ����Ҫ����ʵ�ֵ�AcDbEntity���麯�� 
	 * ע������sub��ͷ�ķ���������CAD�ڲ����ã��ұ���ʵ�ֵ��麯��
	 *      AcDbEntity������AcDbObject
	 */
protected:
	/* 
	 * �÷������Զ���ʵ�弼��������Ҫ�ķ���
	 * CAD�ڲ�����subWorldDraw()����������ʵ��ͼ��
	 * ����AcGiWorldDraw������Կ�����MFC�е�CDC��
	 * ���߶�����������Ļ�ϻ���ͼ��
	 *
	 * ע���÷�����CAD�ڲ�����!!!
	 */
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);

	/*
	 * �任����(�ƶ�����ת������)
	 * ����CAD��ѡ��һ��ʵ�壬ִ�����µ����"�ƶ�����ת������"
	 * CAD�ڲ�����subTransformBy()��������ִ��ʵ��ı任����
	 *
	 * �ڼ����ͼ��ѧ�У�ͼ�ε��ƶ�����ת������ͳ��Ϊ"�任(transform)"����,
	 * ���任�������Ƕ�Ӧһ������(AcGeMatrix3d)
	 * 
	 * ע���÷�����CAD�ڲ�����!!!
	 */
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);

	/* 
	 * ��ִ��CAD�����ʱ���������ƶ���ͼԪ�ĸ�����
	 * CAD���Զ�������Ӧ�Ĳ�׽�㣬����һ����������ʾ�Ļ�ɫС������ʾ��׽��
	 *
	 * ע���÷�������CAD�ڲ������õ�!!!
	 */
	virtual Acad::ErrorStatus subGetOsnapPoints(
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;

	/* 
	 * ���û�ѡ��һ��ͼԪ��CADʹ��"��ɫʵ��С����"������ʾ"�е�"
	 *
	 * ע���÷�����CAD�ڲ�����!!!
	 */
	virtual Acad::ErrorStatus subGetGripPoints(
		AcGePoint3dArray &gripPoints, 
		AcDbIntArray &osnapModes, 
		AcDbIntArray &geomIds) const ;

	/*
	 * �����ק��ͬ�ļе���ܻ������ͬ����Ϊ��
	 *      �ƶ�(MOVE)������(STRETCH)����ת(ROTATE)
	 * Ҳ����˵�е�༭������"�ƶ������졢��ת"3�����������һ�ֲ�����ʽ��
	 * ʹ�üе�༭���Լ��������û����˻������������飬����ҪƵ������������
	 *
	 * ע���÷�����CAD�ڲ�����!!!
	 */
	virtual Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray &indices, const AcGeVector3d &offset) ;

	/*
	 * ȷ��ͼԪ��һ��"����"��3d��Χ��(������)
	 * ����Ƕ�άͼ�Σ��Ǿ���һ��������
	 * 
	 * �÷�����Ҫ���ڼ������ŵķ�Χ
	 * �μ�:ArxHelper/ArxEntityHelper.h/ZoomToEntity��ZoomToEntities����
	 * 
	 * ע���÷�����CAD�ڲ�����!!!
	 */
	virtual Acad::ErrorStatus subGetGeomExtents(AcDbExtents& extents) const;

	/* 
	 * ==>ǰ��֪ʶ׼����
	 * CAD�е�ͼ��ʵ����Ȼ��һ��C++�����볣��C++����ͬ����
	 * ͼ��ʵ������������������CADͼ�����ݿ�ϵͳ�������
	 * ������Ա������ֱ�Ӳ���ͼԪ�Ķ���ָ��(������ֱ��ɾ������ָ��)
	 * CAD��ͼԪ������������һ��"��дȨ��"��װ
	 * ���еĶ�д��������Ҫ��ѭ"��(Open)/�ر�(Close)"����
	 *
	 * ��CAD�У�ͨ��������ǲ���ֱ�Ӳ���ͼԪ�Ķ���ָ��ģ�����ͨ��Open/Close����
	 * (1) ��ͼԪ���󣬵õ�����ָ��
	 *     AcDbObjectId objId;      // ͨ��ĳ��;���õ�ͼԪ����id(����ѡ��ͼ��)
	 *     AcDbLine* pLine;         // ����Ҫ�򿪵���һ��ֱ�߶���
	 *     acdbOpenObject(pLine, objId, AcDb::kForRead); // ��read״̬�򿪶���
	 * (2) ͨ��ָ�����ִ������
	 *     ʹ��read״̬��ֻ�ܶ�ȡ���ݣ�����޸�ͼԪ��������ݣ�
	 *     ��ôCAD�ᷢ������Ȼ����ֹ�˳�
	 *     pLine->getXXX();         // ok
	 *     pLine->setStartPoint();  // error
	 * (3) �رն���
	 *     pLine->close();          // Ҫ��ʱ�ر�ͼԪ����ָ�룬��������һЩĪ������Ĵ���
	 *
	 * ���û�����close()����ʱ��CAD�ڲ������subClose()����
	 * ����C++�Ĺ��캯���в��ܵ����麯��������޷�ִ��һЩ����������صĳ�ʼ������
	 * ���û�newһ���µ�ͼԪ��Ȼ����ӵ�CAD��ͼ�����ݿ⣬��ӳɹ�֮�󣬱���Ҫ�ر�ͼԪ����
	 * ���磺 
	 *     Tunnel* pTunnel = new Tunnel(p1, p2);     // ����һ�����,����ʼĩ���������
	 *	   ArxUtilHelper::PostToModelSpace(pTunnel); // �������ӵ�CADͼ�����ݿ⣬�����ӳɹ���
	 *                                               // �����close()�����ر�ͼԪ����ͬʱ��ʾͼ��
	 * ���ǿ��԰�һЩ��ʼ����������subClose()������
	 *
	 * ע1��close()�����ᱻCAD�ں�̨Ƶ�����ã�����꼴ʹֻ����CAD��Ļ���ƶ���������������CADҲ�����close()����
	 * ע2���÷�����CAD�ڲ�����!!!
	 */
	virtual Acad::ErrorStatus subClose(void);

protected:
	/* Ĭ�Ϲ��캯�� */
	MineGE();

	/*
	 * ���»��Ʋ���(�ؼ������͸��Ӳ���)
	 * true  -- ������д�뵽��ǰ���ӻ�Ч��������
	 * false -- ��������ȡ������ge��
	 * ��writeParamToGEDraw��readParamFromGEDraw�����ڹ������ظ�(***)
	 */
	void updateDrawParam(bool readOrWrite = true) const;

	// ��ȡ��ǰdraw����(��������ʹ�ã�����ֱ�Ӳ���˽������)
	MineGEDraw* getCurrentDraw() const;

	/* ˽�и������� */
private:
	/*
	 * �ӿ��ӻ�Ч����������ȡһ������Σ���CAD���ڵı�����ɫ���Ʋ��������
	 * �൱�ڻ�����һ��"����������"��λ���·���ͼԪ��ᱻ"����"һ����ͼ��
	 */
	void drawBackground(MineGEDraw* pGEDraw, AcGiWorldDraw *mode);

	/*
	 * �ӿ��ӻ�Ч������pGEDraw�ж�ȡ�ؼ�����
	 * Ȼ�����readKeyParam()�������ؼ��������µ�������
	 */
	void readKeyParamFromGEDraw(MineGEDraw* pGEDraw);

	/*
	 * ����writeKeyParam()�������������ȡ�ؼ�����
	 * Ȼ�󽫹ؼ�����д�뵽���ӻ�Ч������pGEDraw
	 */
	void writeKeyParamToGEDraw(MineGEDraw* pGEDraw) const;

	/*
	 * �ӿ��ӻ�Ч������pGEDraw�ж�ȡ���Ӳ���
	 * Ȼ�����readKeyParam()���������Ӳ������µ�������
	 */
	void readExtraParamFromGEDraw(MineGEDraw* pGEDraw);

	/*
	 * ����writeKeyParam()�������������ȡ���Ӳ���
	 * Ȼ�󽫸��Ӳ���д�뵽���ӻ�Ч������pGEDraw
	 */
	void writeExtraParamToGEDraw(MineGEDraw* pGEDraw) const;	

	/*
	 * �ӿ��ӻ�Ч������pGEDraw�ж�ȡ�ؼ������͸��Ӳ���
	 * Ȼ�����readKeyParam()�������������µ�������
	 * �ȼ���ͬʱ����
	 *		readKeyParamFromGEDraw(pGEDraw);
	 * ��
	 *		readExtraParamFromGEDraw(pGEDraw);
	 */
	void readParamFromGEDraw(MineGEDraw* pGEDraw);

	/*
	 * ����writeKeyParam()�������������ȡ�ؼ������͸��Ӳ���
	 * Ȼ�󽫲���д�뵽���ӻ�Ч������pGEDraw
	 * �ȼ���ͬʱ����
	 *		writeKeyParamToGEDraw(pGEDraw);
	 * ��
	 *		writeExtraParamToGEDraw(pGEDraw);
	 */
	void writeParamToGEDraw(MineGEDraw* pGEDraw) const;

	/*
	 * �����һ��ͼԪ�����ݿ⣬MineGE��ȡϵͳ���Ѿ�ע������п��ӻ�Ч��
	 * ���ν�ͼԪ�Ĺؼ��������ݸ����ӻ�Ч�����󣬿��ӻ�Ч��������㸽�Ӳ���
	 * Ȼ��ӿ��ӻ�Ч�������ж�ȡ���Ӳ��������浽ͼԪ����չ������
	 * ��ʽ��
	 *		(���ӻ�Ч��1 ���Ӳ���1 ���Ӳ���2 ...) (���ӻ�Ч��2 ���Ӳ���1 ���Ӳ���2 ...) ...
	 * ���磬����Ŀ��ӻ�Ч��������˫��������ӻ�Ч��������������ӻ�Ч����������������ӻ�Ч��
	 *		(DoubleTunnelDraw 20 14.5 12.6 20.1 29.1) --> (SingleTunnelDraw 2) ... 
	 */
	void initAllExtraParamsToXData();

	/*
	 * ��ͼԪ����չ�����ж�ȡ���п��ӻ�Ч������
	 * ע��AcStringArray��ARX�ṩ���ַ���������
	 */
	void extractExistedDraw(AcStringArray& existedDraw);

	/* 
	 * ��ʼ�����ӻ�Ч�� 
	 * ��ȡע��Ŀ��ӻ�Ч�����Ӳ��������浽��չ����(XData)
	 * ��initAllExtraParamsToXData()�����ǵȼ۵�
	 */
	void initDraw();

	/*
	 * ��ʼ���������� 
	 * ����һ�����ݶ���(DataObject)�����浽ͼԪ����չ�ʵ���
	 */
	void initPropertyData();

	// д���������ݵ�MineGEDraw��
	/*
	 * ��ЩͼԪ�Ŀ��ӻ�Ч�����������������й���
	 * ������������·�ͷ�����Ƴ�����Ч���ǲ�һ���ģ������ǵ�ͼԪ��ֻ��һ��
	 * �����ٶ�������"�·�ͼԪ��"��"����ͼԪ��"��ֻ��һ��"��������ͼԪ��(Direction)"
	 * �ڻ���ͼ�ε�ʱ�򣬿��ӻ�Ч������(pGEdraw)��ͼԪ�����ֶ����ƣ�����Ҫ��ȡ����������
	 * ͼԪ������ݶ����ж�ȡ���ݣ�Ȼ���ٴ��ݸ����ӻ�Ч������
	 */
	void writePropertyDataToGEDraw(MineGEDraw* pGEDraw) const;

	/*
	 * ��ͼԪ�Ĺؼ����������仯ʱ��ͼԪ�ĸ��Ӳ���Ҳ����Ҫ��֮�仯
	 * ���������ʼĩ������仯����˫���������������������Ҳ��Ҫ���¼���
	 * MineGE���������ж�ȡ�ؼ����������ݸ����ӻ�Ч�������������㸽�Ӳ���
	 * Ȼ���ٴӿ��ӻ�Ч�������н����º�ĸ��Ӳ�����ȡ��MineGEͼԪ����չ������
	 */
	void updateDrawParams(MineGEDraw* pGEDraw);

	/*
	 * �ƶ�����ת�����š��е�༭����ִ�й�����
	 * ͼ����������ƶ���ͣ�ı仯
	 * �ڶ�̬�仯�Ĺ��̻᲻�ϵĲ����µ�ͼԪ����(new object)
	 * ��Щ��ʱͼԪû���ύ�����ݿ�(��Ϊnew object)���ҽ�������ʾͼ��
	 * �����ЩͼԪ���󲻻����subClose()��������ʱͼԪû�и�����չ����
	 * �Ӷ����¶�̬�仯�Ĺ����У�û��ͼ����ʾ(������ʾ����ȷ)
	 * �÷�����֤��ʱͼԪ(new object)����չ�����ܹ���ȷ�ر���ʼ��
	 */
	void initNewObjectExtraParams();

private:
	// ��ǰ���ӻ�Ч������ָ��
	// ͼԪ�Ŀ��ӻ�Ч������ͨ����ָ��ί��ʵ��
	// ���磺���ơ��任(�ƶ�����ת������)����׽�㡢�е�༭
	MineGEDraw* m_pCurrentGEDraw;
	// ͼԪ���������ݶ���
	AcDbObjectId m_dataObjectId;
};

/* 
 * ARX�����ɵĴ��� 
 * ���ϰ汾��ARX�У�������Ա��AcDbObject/AcDbEntity�����Զ�����
 * ����Ҫ�ֶ��ĵ���ARX�ṩ�ķ������Զ�����ע�ᵽCAD���νṹ����(Class Hierarchy)
 * �μ���ArxHelper/ArxClassHelper.h
 *
 * ���������Ա���������������������������һЩĪ������Ĵ���
 * ���磺���ͼԪ�����ݿ����CAD��Ļ�Ͽ�����ͼ��
 *
 * �°汾��ARX�ṩ�˸�����(ACDB_REGISTER_OBJECT_ENTRY_AUTO)��
 * �Զ�ע���Զ����ൽCAD���νṹ���У������˿�����Ա����Ļ���
 *
 * MINEGE_MODULE����MineGE��Ŀ�ж���
 * ��Ŀ���� --> �������� --> C/C++ --> Ԥ������ --> Ԥ����������
 */
#ifdef MINEGE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(MineGE)
#endif