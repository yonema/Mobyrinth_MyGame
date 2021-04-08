#pragma once
#include "ModelRender.h"
#include "DirectionLight.h"
class testModel : public IGameObject
{
public:
	bool Start()override final;
	void Update()override final;
private:
	CModelRender* m_model[5];
	float m_angleX = 0.0f;
	float m_angleY = 0.0f;
};

