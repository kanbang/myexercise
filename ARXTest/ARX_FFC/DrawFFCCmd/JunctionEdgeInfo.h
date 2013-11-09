#pragma once

struct JunctionEdgeInfo
{
	AcDbObjectId objId;             // ͼԪ��id
	bool startOrEnd;                // ���ӵ���ʼ�㻹��ĩ��
	AcGeVector3d angle;             // �������췽������

	bool operator==(const JunctionEdgeInfo& info)
	{
		return (objId == info.objId) &&
			(startOrEnd == info.startOrEnd) && 
			(angle == info.angle);
	}
};

typedef AcArray<JunctionEdgeInfo> JunctionEdgeInfoArray;
