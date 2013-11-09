#pragma once

class DrawCmd
{
public:
	//  µœ÷‘⁄DrawFFC.cpp
	static void DrawSandWall();
	static void DrawObturation();
	static void DrawGoaf();
	static void DrawNitrogenPipe();
	static void DrawGasPipe();
	static void DrawAirLeak(bool st);
	static void SplitGoaf();
	static void RemoveGoafPoint();
	static void AdvanceWS();
};
