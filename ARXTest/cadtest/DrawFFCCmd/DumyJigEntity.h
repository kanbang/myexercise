#pragma once

class DumyJigEntity : public AcDbEntity
{
public:
	~DumyJigEntity();

	void addEntity(const AcDbObjectId& objId);
	AcDbEntity* getEnity(int pos) const;
	void writeBack();

protected:
	Adesk::Boolean subWorldDraw(AcGiWorldDraw *mode);

	AcDbVoidPtrArray m_pEntArray;
	AcDbObjectIdArray m_objIds;
};