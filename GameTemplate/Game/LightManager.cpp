#include "stdafx.h"
#include "LightManager.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"

//staticなデータメンバの初期化
CLightManager* CLightManager::m_instance = nullptr;
SLightParam CLightManager::m_lightParam;

//コンストラクタ
CLightManager::CLightManager()
{
	if (m_instance != nullptr) {
		//インスタンスがすでに作られている。
		std::abort();
	}
	Init();	//初期化処理
}

//デストラクタ
CLightManager::~CLightManager()
{
	//nullptrを入れておく
	m_instance = nullptr;
}

//初期化関数
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


//アップデート
void CLightManager::Update()
{
	//視点をカメラの位置にする
	m_lightParam.eyePos = g_camera3D->GetPosition();
}

/// <summary>
/// ライトを追加する関数（ディレクションライト）
/// </summary>
/// <param name="light">追加するライト</param>
/// <returns>追加できたらtrueを戻す</returns>
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

/// <summary>
/// ライトを追加する関数（ポイントライト）
/// </summary>
/// <param name="light">追加するライト</param>
/// <returns>追加できたらtrueを戻す</returns>
bool CLightManager::AddLight(CPointLight* light)
{

	//ポイントライトの数がMAXより多かったら作られない
	if (m_lightParam.numPointLight >= Max_PointLight) {
		return false;
	}

	//Managerにポイントライトの参照を渡す
	m_pointLights[m_lightParam.numPointLight]
		= light;
	//Managerのポイントライトの初期方向を設定
	m_pointLightsData[m_lightParam.numPointLight].ptPosition
		= { 0.0f,0.0f,0.0f };
	m_pointLightsData[m_lightParam.numPointLight].ptColor
		= { 0.0f,0.0f,0.0f,1.0f };
	m_pointLightsData[m_lightParam.numPointLight].ptRange = 300.0f;
	//LightにManagerのポイントライトの参照を渡す
	light->SetRawData(&m_pointLightsData[m_lightParam.numPointLight]);
	//LightにManagerの管理番号を渡す。
	light->SetControlNumber(m_lightParam.numPointLight);

	//現在のライトの数を増やす
	m_lightParam.numPointLight++;

	return true;
}

/// <summary>
/// ライトを追加する関数（スポットライト）
/// </summary>
/// <param name="light">追加するライト</param>
/// <returns>追加できたらtrueを戻す</returns>
bool CLightManager::AddLight(CSpotLight* light)
{

	//スポットライトの数がMAXより多かったら作られない
	if (m_lightParam.numSpotLight >= Max_SpotLight) {
		return false;
	}

	//Managerにスポットライトの参照を渡す
	m_spotLights[m_lightParam.numSpotLight]
		= light;
	//Managerのスポットライトの初期値を設定
	m_spotLightsData[m_lightParam.numSpotLight].position
		= { 0.0f,0.0f,0.0f };
	m_spotLightsData[m_lightParam.numSpotLight].color
		= { 0.0f,0.0f,0.0f,1.0f };
	m_spotLightsData[m_lightParam.numSpotLight].range = 300.0f;
	m_spotLightsData[m_lightParam.numSpotLight].direction = g_VEC3_DOWN;
	m_spotLightsData[m_lightParam.numSpotLight].angle = 45.0f;
	//LightにManagerのスポットライトの参照を渡す
	light->SetRawData(&m_spotLightsData[m_lightParam.numSpotLight]);
	//LightにManagerの管理番号を渡す。
	light->SetControlNumber(m_lightParam.numSpotLight);

	//現在のライトの数を増やす
	m_lightParam.numSpotLight++;

	return true;
}

/// <summary>
/// ライトを消去する関数（ディレクションライト）
/// </summary>
/// <param name="light">消去するライト</param>
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

/// <summary>
/// ライトを消去する関数（ポイントライト）
/// </summary>
/// <param name="light">消去するライト</param>
void CLightManager::RemoveLight(CPointLight* light)
{
	//消すライトの管理番号を取得
	const int targetLigNumBuff = light->GetControlNumver();

	//iの一つ先がポイントライトの数より小さいとき継続
	for (int i = targetLigNumBuff; i + 1 < m_lightParam.numPointLight; i++)
	{
		//Managerの、ポイントライトを交換
		//交換相手は、Lightの管理番号とその一つ上の番号
		std::swap<SPointLight>(m_pointLightsData[i], m_pointLightsData[i + 1]);
		//Managerの、Lightの参照を交換
		std::swap<CPointLight*>(m_pointLights[i], m_pointLights[i + 1]);
	}

	//iの一つ先がポイントライトの数より小さいとき継続
	for (int i = targetLigNumBuff; i + 1 < m_lightParam.numPointLight; i++)
	{
		//Lightのポイントライトの構造体の
		m_pointLights[i]->SetRawData(&m_pointLightsData[i]);
	}

	m_lightParam.numPointLight--;

}


/// <summary>
/// ライトを消去する関数（スポットライト）
/// </summary>
/// <param name="light">消去するライト</param>
void CLightManager::RemoveLight(CSpotLight* light)
{
	//消すライトの管理番号を取得
	const int targetLigNumBuff = light->GetControlNumver();

	//iの一つ先がスポットライトの数より小さいとき継続
	for (int i = targetLigNumBuff; i + 1 < m_lightParam.numSpotLight; i++)
	{
		//Managerの、スポットライトを交換
		//交換相手は、Lightの管理番号とその一つ上の番号
		std::swap<SSpotLight>(m_spotLightsData[i], m_spotLightsData[i + 1]);
		//Managerの、Lightの参照を交換
		std::swap<CSpotLight*>(m_spotLights[i], m_spotLights[i + 1]);
	}

	//iの一つ先がスポットライトの数より小さいとき継続
	for (int i = targetLigNumBuff; i + 1 < m_lightParam.numSpotLight; i++)
	{
		//Lightのスポットライトの構造体の
		m_spotLights[i]->SetRawData(&m_spotLightsData[i]);
	}

	m_lightParam.numSpotLight--;

}