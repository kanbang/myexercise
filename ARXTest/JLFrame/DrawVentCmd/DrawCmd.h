#pragma once

class DrawCmd
{
public:
	/* 1) ʵ����DrawVent.cpp�� */
	/* ���ƽڵ�(������) */
	//static void DrawJoint(void);
	/* ������� */
	static void DrawTunnel(void);
	/* ���ƻ������ */
	static void DrawArcTunnel(void);
	/* ���ƹ����� */
	static void DrawWS(void);
	/* ������Ҫͨ��� */
	static void DrawMainFan();
	/* ���ƾֲ��ȷ�� */
	static void DrawLocalFan();
	/* ���Ʒ��� */
	static void DrawGate(int flag);
	/* ���Ʒ細 */
	static void DrawCasement(int flag);
	/* ���Ʒ��� */
	static void DrawWindBridge();
	/* ���Ʋ��վ */
	static void DrawWindStation();
	/* ���Ƶ���ǽ(�ܱ�))*/
	static void DrawWall(int flag);
	/* �������� */
	static void DrawStorage(int flag);
	/* ���Ʒ������� */
	static void DrawDirection(void);
	/* ���Ʒ�Ͳ */
	static void DrawChimney(void);
	/* ����ָ����(����) */
	//static void DrawCompass();

	/* 2) ʵ����DrawTTunnel.cpp�� */
	/* ���ƾ����� */
	static void DrawTTunnel();
	static void ResetInTunnel();
	static void ShowInTunnel();

	/* 3) ʵ����AutoDraw.cpp�� */
	/* ���뵥��dwgͼ���Զ������������ע�������� */
	static void AutoDraw();

	/* 4) ʵ����AutoTool.cpp�� */
	/* �Զ���ע�������� */
	static void AutoDirection();
	/* �������ȡ�������ݲ����µ����վ������ */
	static void UpdateAllWindStationData();

	/* 5) ʵ����EditCmd.cpp�� */	
	/* ������Ӵ��պ� */
	static void JunctionEnclose(void);
	/* ������������ķ������� */
	static void ReverseDirection();
	/* ���°� */
	static void ReBindEdge();
	/* ָ��ͼԪ�������� */
	static void ZoomMineGE();
	/* �ָ������ */
	static void SplitTunnel();

	/* ��ʱ���Ӳ���(�޸���������) */
	static void ChangeData();
};