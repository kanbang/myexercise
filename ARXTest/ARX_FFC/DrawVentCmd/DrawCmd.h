#pragma once

class DrawCmd
{
public:
	/* 实现在DrawVent.cpp中 */
	//static void DrawJoint(void);
	static void DrawTunnel(void);
	static void DrawArcTunnel(void);
	static void DrawWS(void);

	/* 实现在DrawTTunnel.cpp中 */
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

	/* 实现在AutoDraw.cpp中 */
	static void AutoDraw();
	static void AutoDraw2();

	/* 自动标注风流方向 */
	static void AutoDirection();

	/* 自动更新测风站数据 */
	static void UpdateAllWindStationData();

	// 通用编辑命令
	static void JunctionEnclose(void);
	static void ReverseDirection();
	static void ReBindEdge();
	static void ZoomMineGE();
	static void SplitTunnel();
};
