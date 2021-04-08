#include "stdafx.h"
#include "LightManager.h"
#include "DirectionLight.h"

//staticなデータメンバの初期化
CLightManager* CLightManager::m_instance = nullptr;
SLightParam CLightManager::m_lightParam;

CLightManager::CLightManager()
{
	if (m_instance != nullptr) {
		//インスタンスがすでに作られている。
		std::abort();
	}
	Init();	//初期化処理
}

CLightManager::~CLightManager()
{
	//nullptrを入れておく
	m_instance = nullptr;
}

void CLightManager::Init()
{
	//ライトの共通のパラメータの初期値を設定
	m_lightParam.eyePos = { 0.0f,0.0f,0.0f };
	m_lightParam.numDirectionLight = 0;
	m_lightParam.ambientLight = { 0.6f,0.6f,0.6f };
	m_lightParam.numPointLight = 0;
	m_lightParam.specPow = 5.0f;
	m_lightParam.numShadow = 0;
}



void CLightManager::ExecuteUpdate()
{
	//視点をカメラの位置にする
	m_lightParam.eyePos = g_camera3D->GetPosition();
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
	//消すライトの管理番号を取得
	const int targetLigNumBuff = light->GetControlNumver();
		
	//iの一つ先がディレクションライトの数より小さいとき継続
	for (int i = targetLigNumBuff; i + 1 < m_lightParam.numDirectionLight; i++)
	{
		//Managerの、ディレクションライトを交換
		//交換相手は、Lightの管理番号とその一つ上の番号
		std::swap<SDirectionLight>(m_directionLightsData[i], m_directionLightsData[i + 1]);
		//Managerの、Lightの参照を交換
		std::swap<CDirectionLight*>(m_directionLights[i], m_directionLights[i + 1]);
	}

	//iの一つ先がディレクションライトの数より小さいとき継続
	for (int i = targetLigNumBuff; i + 1 < m_lightParam.numDirectionLight; i++)
	{
		//Lightのディレクションライトの構造体の
		m_directionLights[i]->SetRawData(&m_directionLightsData[i]);
	}

	m_lightParam.numDirectionLight--;


}