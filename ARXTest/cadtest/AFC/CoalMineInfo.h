#pragma once

namespace PJ
{
	// �����������
	enum TunnelMaterial
	{
		ROCK   = 0, // ����
		COAL   = 1, // ú��
		MIXED  = 2  // ��ú����
	};

	// ����˹��Ϣ
	enum CoalMineGasInfo
	{
		LOW_GAS  = 0, // ����˹��
		HIGH_GAS = 1, // ����˹��
		OUTBURST = 2  // ú����˹ͻ��
	};
}