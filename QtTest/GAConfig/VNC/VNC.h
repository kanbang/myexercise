#pragma once

#include "dlimexp.h"

#include "GraphData.h"

/*
* ͨ���������(Ventilation Network Calculation)
*   ͨ���������:
*		dg    -- ͨ������(��ͨ���޵����·)
*		ed    -- ��֧����
*		fd    -- ��֧�ϵķ������
*	 ע: �����֧��û�з������fd[e]=0;
*
*	�����������:
*		totalQ   -- ��ʼ�ܷ���
*		precise  -- �������㾫��
*		maxCount -- ����������
*	������:
*		����ֵ����ʾ�����Ƿ�ɹ�
*
*
*	ʹ��ʾ��:
*		Digraph dg;
*		VNC_EdgeDataMap ed(dg); // ͼ��֧�󶨷�֧����
*		VNC_FanDataMap fd(dg);  // ͼ��֧�󶨷������
*
*		// ��������dg������ʼ������(ed��fd)
*		// ...
*
*		VNC(dg, ed, fd);               // ����VNCģ��
*
*		ClearAllVNCDatas(dg, ed, fd);  // ����ڴ�
*/
extern "C" VNC_EXPORT_API 
bool VNC(Digraph& dg, VNC_EdgeDataMap& ed, VNC_FanDataMap& fd,
		 double totalQ = 50, double precise = 0.0001, int maxCount = 1000);

/*
 * ����������(�ų��������)
 * ǰ�᣺�������ɹ���������ƽ���
 */ 
extern "C" VNC_EXPORT_API 
double VNC_H(Digraph& dg, VNC_EdgeDataMap& ed, VNC_FanDataMap& fd);

extern "C" VNC_EXPORT_API 
void ClearAllVNCDatas(Digraph& dg, VNC_EdgeDataMap& ed, VNC_FanDataMap& fd);