#pragma once
#include "ModelRender.h"
class CLevelObjectBase : public IGameObject
{
protected:
	bool Init
	(const char* filePath_front, const int type_front,
		const char* filePath_back, const int type_back);
public:
	bool Start()override final;
	virtual bool StartSub() = 0 {};
	void Update()override final;
	virtual ~CLevelObjectBase();
	void SetFrontOrBack(const bool frontOrBack);
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}
	
public:
	enum EnFrontAndBack
	{
		enFront,
		enBack,
		enFrontAndBackNum,
	};
	enum EnObjectType
	{
		enEnpty,
		enWater,
		enIce,
		enObjectTypeNum,
	};
private:
	bool m_frontOrBack = enFront;
	CModelRender* m_modelRender[enFrontAndBackNum] = { nullptr };
	Vector3 m_position = g_vec3Zero ;
	Quaternion m_rotation = g_quatIdentity;
	int m_objectType[enFrontAndBackNum] = { enEnpty };
};

