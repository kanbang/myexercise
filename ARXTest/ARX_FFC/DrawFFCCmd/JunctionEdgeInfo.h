#pragma once

struct JunctionEdgeInfo
{
	AcDbObjectId objId;             // 图元的id
	bool startOrEnd;                // 连接点是始点还是末点
	AcGeVector3d angle;             // 向内延伸方向向量

	bool operator==(const JunctionEdgeInfo& info)
	{
		return (objId == info.objId) &&
			(startOrEnd == info.startOrEnd) && 
			(angle == info.angle);
	}
};

typedef AcArray<JunctionEdgeInfo> JunctionEdgeInfoArray;
