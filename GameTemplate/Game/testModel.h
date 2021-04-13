#pragma once
#include "ModelRender.h"
#include "PointLight.h"
#include "Sky.h"

class testModel : public IGameObject
{
public:
	bool Start()override final;
	void Update()override final;

private:
	void PointLight();
	void MoveCamera();
private:
	Vector3 m_pointLightPos = g_vec3Zero;
	bool m_pointLigMoveDir = false;
	CModelRender* m_ptLigPosModel = nullptr;

	CModelRender* m_model[5];
	float m_angleX = 0.0f;
	float m_angleY = 0.0f;

	CSky* m_sky = nullptr;
	float m_skyScale = 1.0f;

	CModelRender* m_skyModel = nullptr;
};

