#pragma once

#include "dlimexp.h"

// ͼ���������
class ARXHELPER_DLLIMPEXP LayerHelper
{
public:
	static void SetCurrentLayer(const CString& layerName);  // ���õ�ǰͼ��
	static CString GetCurrentLayerName();                   // ��ȡ��ǰͼ�������
	static void AddLayer(const CString& layerName);         // ����ͼ��
	static void RemoveLayer(const CString& layerName);      // ɾ��ͼ��
	static bool IsLayerExist(const CString& layerName);     // �ж�ͼ���Ƿ����
	static bool IsCurrentLayer(const CString& layerName);   // �ж�ͼ���Ƿ�ǰͼ��
	static void RenameLayer(const CString& layerName, const CString& newLayerName); // �޸�ͼ������

	// ����!!!
	static bool FrozenLayer(const CString& layerName, bool isFrozen);      // �Ƿ񶳽�ͼ��
	static bool HideLayer(const CString& layerName, bool isOn);            // �Ƿ�����ͼ��(����)
	static bool LockLayer(const CString& layerName, bool locked);          // �Ƿ�����ͼ��
	static bool OffLayer(const CString& layerName, bool off);              // �Ƿ�ر�ͼ��

	// ��ȡͼ���б�
	static void GetAllLayers(AcStringArray& layers);

	// ��ȡͼ���ϵ�����ʵ��
	static bool GetAllEntitiesOnLayer(const CString& layerName, AcDbObjectIdArray &ents);

	// ɾ��ͼ���ϵ�����ʵ��
	static void DeleteAllEntitiesOnLayer(const CString& layerName);
};

// һ���򵥵�ͼ���л���
// ����������֮���Զ���ͼ���л�����
// ���createNewLayer����Ϊ�棬ͼ�㲻�������Զ�����ͼ��
class ARXHELPER_DLLIMPEXP LayerSwitch
{
public:
	LayerSwitch(const CString& layerName, bool createNewLayer=true);
	~LayerSwitch();

private:
	CString m_curLayer; // ��¼��ǰͼ��
};