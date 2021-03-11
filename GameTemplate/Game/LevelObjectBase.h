#pragma once
#include "ModelRender.h"
#include "LevelObjectManager.h"
#include "Player_kari.h"

class ILevelObjectBase : public IGameObject
{
public:
	bool Start()override final;
	virtual bool PureVirtualStart() = 0 {};
	virtual ~ILevelObjectBase();
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}
	void SetObjectType(int objectType)
	{
		m_objectType = objectType;
	}
	const Vector3 GetPosition()const
	{
		return m_position;
	}
	const int GetObjectType()const
	{
		return m_objectType;
	}
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	const bool GetIsDead()const
	{
		return m_isDead;
	};
	void Delete()
	{
		m_isDead = true;
	}
protected:
	void CheckWayPoint();
public:
	enum EnObjectType
	{
		enEnpty,

		//反転オブジェクト
		enWater,
		enIce,
		enBird,
		enFish,
		enGrilledChicken,
		enGrilledFish,

		//障害オブジェクト
		enBigFire,

	};
private:
	int m_objectType = enEnpty;
	bool m_isDead = false;
protected:
	Vector3 m_position = g_vec3Zero;
	Quaternion m_rotation = g_quatIdentity;
	Vector3 m_scale = g_vec3One;
	Player_kari* m_pPlayer = nullptr;

};

