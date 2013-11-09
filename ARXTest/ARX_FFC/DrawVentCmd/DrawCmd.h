#pragma once

class DrawCmd
{
public:
	/* ʵ����DrawVent.cpp�� */
	//static void DrawJoint(void);
	static void DrawTunnel(void);
	static void DrawArcTunnel(void);
	static void DrawWS(void);

	/* ʵ����DrawTTunnel.cpp�� */
	static void DrawTTunnel();
	static void ResetInTunnel();
	static void ShowInTunnel();

	static void DrawMainFan();
	static void DrawLocalFan();
	static void DrawGate(int flag);
	static void DrawCasement(int flag);
	static void DrawWindBridge();
	static void DrawWindStation();
	static void DrawWall(int flag);
	static void DrawStorage(int flag);
	static void DrawDirection(void);

	static void DrawChimney(void);
	//static void DrawCompass();

	//static void DrawJointByJig(void);
	static void DrawTunnelByJig(void);
	static void DrawArcTunnel_ByJig(void);
	static void DrawWSByJig(void);

	/* ʵ����AutoDraw.cpp�� */
	static void AutoDraw();
	static void AutoDraw2();

	/* �Զ���ע�������� */
	static void AutoDirection();

	/* �Զ����²��վ���� */
	static void UpdateAllWindStationData();

	// ͨ�ñ༭����
	static void JunctionEnclose(void);
	static void ReverseDirection();
	static void ReBindEdge();
	static void ZoomMineGE();
	static void SplitTunnel();
};
