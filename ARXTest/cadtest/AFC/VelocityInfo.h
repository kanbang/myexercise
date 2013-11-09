#pragma once

namespace PJ
{
	// ��С����ȡֵ
	enum MIN_VELOCITY
	{
		SMALL_MIN_VELOCITY,  // 0.15m/s
		NORMAL_MIN_VELOCITY, // 0.25m/s
		LARGE_MIN_VELOCITY,  // 0.5m/s
		MORE_MIN_VELOCITY    // 1.0m/s
	};

	// ������ȡֵ
	enum MAX_VELOCITY
	{
		NORMAL_MAX_VELOCITY, // 4m/s
		LARGE_MAX_VELOCITY,  // 5m/s
		NO_LIMIT_MAX_VELOCITY // �����Ƶ������٣������趨Ϊ100m/s
	};

	// ������������
	enum VelocityCheckType
	{
		VCT_WS_NORMAL=0,  // �زɹ������������6(1)
		VCT_WS_GASTUNNEL, // �زɹ������������6(3)���䱸ר����
		VCT_ROCK,     // �����������
		VCT_COAL,     // ú���������
		VCT_EL        // ���ߵ糵�����������
	};
}