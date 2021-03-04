#pragma once
#include "ModelRender.h"
class Mobius : public IGameObject
{
public:
	bool Start()override final;
	~Mobius();

	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}
	Model* GetModel()
	{
		return &m_modelRender->GetModel();
	}

private:
	CModelRender* m_modelRender = nullptr;
	Vector3 m_position = g_vec3Zero;
	Quaternion m_rotation = g_quatIdentity;
};

