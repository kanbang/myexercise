#pragma once

class DrawCmd
{
public:
	/* 1) 实现在DrawVent.cpp中 */
	/* 绘制节点(测试用) */
	//static void DrawJoint(void);
	/* 绘制巷道 */
	static void DrawTunnel(void);
	/* 绘制弧线巷道 */
	static void DrawArcTunnel(void);
	/* 绘制工作面 */
	static void DrawWS(void);
	/* 绘制主要通风机 */
	static void DrawMainFan();
	/* 绘制局部扇风机 */
	static void DrawLocalFan();
	/* 绘制风门 */
	static void DrawGate(int flag);
	/* 绘制风窗 */
	static void DrawCasement(int flag);
	/* 绘制风桥 */
	static void DrawWindBridge();
	/* 绘制测风站 */
	static void DrawWindStation();
	/* 绘制挡风墙(密闭))*/
	static void DrawWall(int flag);
	/* 绘制硐室 */
	static void DrawStorage(int flag);
	/* 绘制风流方向 */
	static void DrawDirection(void);
	/* 绘制风筒 */
	static void DrawChimney(void);
	/* 绘制指北针(测试) */
	//static void DrawCompass();

	/* 2) 实现在DrawTTunnel.cpp中 */
	/* 绘制掘进巷道 */
	static void DrawTTunnel();
	static void ResetInTunnel();
	static void ShowInTunnel();

	/* 3) 实现在AutoDraw.cpp中 */
	/* 导入单线dwg图，自动绘制巷道并标注风流方向 */
	static void AutoDraw();

	/* 4) 实现在AutoTool.cpp中 */
	/* 自动标注风流方向 */
	static void AutoDirection();
	/* 从巷道读取风量数据并更新到测风站数据中 */
	static void UpdateAllWindStationData();

	/* 5) 实现在EditCmd.cpp中 */	
	/* 巷道连接处闭合 */
	static void JunctionEnclose(void);
	/* 重新设置巷道的风流方向 */
	static void ReverseDirection();
	/* 重新绑定 */
	static void ReBindEdge();
	/* 指定图元进行缩放 */
	static void ZoomMineGE();
	/* 分割打断巷道 */
	static void SplitTunnel();

	/* 临时增加测试(修改属性数据) */
	static void ChangeData();
};