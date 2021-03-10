#pragma once
#include "ModelRender.h"
#include "LevelObjectManager.h"
#include "Player_kari.h"

class CLevelObjectBase : public IGameObject
{
protected:
	bool Init
	(const char* filePath_front, const int type_front,
		const char* filePath_back, const int type_back);
	void CheckPlayer();
	void HeldPlayer();
	void Thrown();
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
	void CheckWayPoint();
	const bool GetIsDead()const
	{
		return m_isDead;
	};
	

	
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
	Player_kari* m_pPlayer = nullptr;
	enum EnObjectState
	{
		enCheckPlayer,
		enHeldPlayer,
		enThrown,
	};
	int m_objectState = enCheckPlayer;
	int m_throwCounter = 0;

	bool m_isDead = false;
};

