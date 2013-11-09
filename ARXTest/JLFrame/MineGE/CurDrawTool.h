#pragma once

class MineGEDraw;

/*
 * ����ÿ��ͼԪ���ܰ���������ӻ�Ч��
 * ���磺
 *		������ܰ������ӻ�Ч����˫��������������������������ȵ�
 *
 * ϵͳʹ��"�ʵ�"��¼ÿ��ͼԪ�ĵ�ǰ���ӻ�Ч��
 * ���磺
 *		Tunnel     --> DoubleTunnelDraw        (���      -->  ˫��������ӻ�Ч��)
 *		Direction  --> PolylineDirectionDraw   (��������  -->  ����߷���������ӻ�Ч��)
 *		Gate       --> SimpleGateDraw          (����      -->  �򵥵ķ��ſ��ӻ�Ч��)
 *		...        --> ...
 */

/*
 * ��ʼ������ͼԪ�ĵ�ǰ���ӻ�Ч��
 * ע�����ͼԪ�Ŀ��ӻ�Ч����ע��(���ڴʵ��и�ͼԪ���͵�key����)���򱣳ֲ���
 */
extern void InitAllCurDraws();

/* ��ȡͼԪ�ĵ�ǰ���ӻ�Ч�� */
extern bool GetCurDraw(const CString& type, CString& draw);

/* ����ͼԪ�ĵ�ǰ���ӻ�Ч�� */
extern bool SetCurDraw(const CString& type, const CString& draw);
