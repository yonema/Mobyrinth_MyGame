#include "stdafx.h"
#include "Light.h"

Light* Light::m_instance = nullptr;

Light::Light()
{
	if (m_instance != nullptr) {
		//インスタンスがすでに作られている。
		std::abort();
	}
	Init();
	m_instance = this;
}

Light::~Light()
{
	m_instance = nullptr;
}

void Light::Init()
{
	//ディレクションライト
	m_directionLight.dirLigDirection.x = 1.0f;
	m_directionLight.dirLigDirection.y = -1.0f;
	m_directionLight.dirLigDirection.z = -1.0f;
	m_directionLight.dirLigDirection.Normalize();			//方向
	m_directionLight.dirLigColor = { 1.0f,0.4f,0.4f };	//色
	m_directionLight.eyePos = g_camera3D->GetPosition();	//視点
	m_directionLight.specPow = 5.0f;						//スペキュラの絞り
	m_directionLight.ambinetLight = { 0.6f,0.6f,0.6f };	//環境光

	//ポイントライト
	m_pointLight.ptPosition = { 10.0f,10.0f,0.0f };
	m_pointLight.ptRange = 100.0f;
	m_pointLight.ptColor = { 1.0f,1.0f,0.0f };
}
