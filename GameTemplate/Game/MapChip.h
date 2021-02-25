#pragma once
#include "physics/PhysicsStaticObject.h"
struct LevelObjectData;



class CMapChip
{
public:
	CMapChip(const LevelObjectData& objData/*, CMapChipRender* mapChipRender*/);
	~CMapChip()
	{
	}
	//prefab::CSkinModelRender* GetSkinModelRender()
	//{
	//	if (m_mapChipRender == nullptr) {
	//		return nullptr;
	//	}
	//	return m_mapChipRender->GetSkinModelRender();
	//}
	void Draw(RenderContext& rc)
	{
		m_model.Draw(rc);
	}
private:
	PhysicsStaticObject m_physicsStaticObject;			//!<静的物理オブジェクト。
	Model m_model;
};

