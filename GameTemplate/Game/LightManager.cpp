#include "stdafx.h"
#include "LightManager.h"
#include "LightBase.h"
#include "DirectionLight.h"

CLightManager* CLightManager::m_instance = nullptr;
SLightParam CLightManager::m_lightParam;

CLightManager::CLightManager()
{
	if (m_instance != nullptr) {
		//インスタンスがすでに作られている。
		std::abort();
	}
	Init();
	m_instance = this;
}

CLightManager::~CLightManager()
{
	//Release()
	m_instance = nullptr;
}

void CLightManager::Init()
{
	m_lightParam.eyePos = { 0.0f,0.0f,0.0f };
	m_lightParam.numDirectionLight = 0;
	m_lightParam.ambientLight = { 0.6f,0.6f,0.6f };
	m_lightParam.numPointLight = 0;
	m_lightParam.specPow = 5.0f;
}
//void CLightManager::Release()
//{
//
//}


void CLightManager::ExecuteUpdate()
{
	m_lightParam.eyePos = g_camera3D->GetPosition();
	//m_lightParam.numDirectionLight = m_directionLights.size();
}
bool CLightManager::AddLight(CDirectionLight* light)
{

	//ディレクションライトの数がMAXより多かったら作られない
	if (m_lightParam.numDirectionLight >= Max_DirectionLight) {
		//TK_WARNING("m_directionLights.size() is full!!");
		return false;
	}

		//Managerにディレクションライトの参照を渡す
		m_directionLights[m_lightParam.numDirectionLight]
			= light;
		//Managerのディレクションライトの初期方向を設定
		m_directionLightsData[m_lightParam.numDirectionLight].dirLigDirection
			= { 0.0f,0.0f,0.0f };
		m_directionLightsData[m_lightParam.numDirectionLight].dirLigColor
			= { 0.0f,0.0f,0.0f,1.0f };
		//LightにManagerのディレクションライトの参照を渡す
		light->SetRawData(&m_directionLightsData[m_lightParam.numDirectionLight]);
		//LightにManagerの管理番号を渡す。
		light->SetControlNumber(m_lightParam.numDirectionLight);
		
		//現在のライトの数を増やす
		m_lightParam.numDirectionLight++;
	
		return true;
}


void CLightManager::RemoveLight(CDirectionLight* light)
{


		const int targetLigNumBuff = dynamic_cast<CDirectionLight*>(light)->GetControlNumver();

		for (int i = targetLigNumBuff; i + 1 < m_lightParam.numDirectionLight; i++)
		{
			//Managerのディレクションライトを、Lightの管理番号とその一つ上の番号と交換
			std::swap<SDirectionLight>(m_directionLightsData[i], m_directionLightsData[i + 1]);
			//Lightのポジションの参照を交換
			std::swap<CDirectionLight*>(m_directionLights[i], m_directionLights[i + 1]);
		}

		for (int i = targetLigNumBuff; i + 1 < m_lightParam.numDirectionLight; i++)
		{
			m_directionLights[i]->SetRawData(&m_directionLightsData[i]);
		}

		m_lightParam.numDirectionLight--;

	
}