#pragma once
#include "LightData.h"
#include "DirectionLight.h"

class CDirectionLight;

class CLightManager
{
private:
	static CLightManager* m_instance;	//シングルトンパターン
	CLightManager();	//コンストラクタと
	~CLightManager();	//デストラクタをprivateに隠す
	void Init();
	//void Release();

	static const int Max_DirectionLight = 4;	//ディレクションライトの最大数
	static const int Max_PointLight = 4;		//ポイントライトの最大数
	static const int Max_SpotLight = 4;			//スポットライトの最大数

public:
	static void CreateInstance()
	{
		m_instance = new CLightManager;
	}
	static void DeleteInstance()
	{
		delete m_instance;
	}
	static CLightManager* GetInstance()
	{
		return m_instance;
	}
	void ExecuteUpdate();
	//ライトを追加
	bool AddLight(CDirectionLight* light);
	//ライトを消去
	void RemoveLight(CDirectionLight* light);

	/// <summary>
	/// アンビエントライトを取得
	/// </summary>
	/// <returns>カラー</returns>
	const Vector3& GetAmbientLight() const
	{
		return m_lightParam.ambientLight;
	}
	/// <summary>
	/// アンビエントライトを設定
	/// </summary>
	/// <param name="ambient">カラー</param>
	void SetAmbientLight(const Vector3& ambient)
	{
		m_lightParam.ambientLight = ambient;
	}

	//std::list<CDirectionLight*>* GetDirectionLights()
	//{
	//	return &m_directionLights;
	//}
	SLightParam* GetLightParam()
	{
		return &m_lightParam;
	}
	const int GetNumDirectionLight() const
	{
		return m_lightParam.numDirectionLight;
	}
	const int GetNumPointLight() const
	{
		return m_lightParam.numPointLight;
	}
	const float GetSpecPow() const
	{
		return m_lightParam.specPow;
	}
	SDirectionLight* GetDirectionLigData()
	{
		return m_directionLightsData;
	}
private:

	static SLightParam m_lightParam;
	//std::list<SDirectionLight> m_lights;	//ディレクションライトの構造体
	//std::list<CDirectionLight*> m_directionLights;	//ディレクションライトのリスト

	//それぞれのディレクションライトのデータ
	SDirectionLight m_directionLightsData[Max_DirectionLight];


	//それぞれのディレクションライトの参照
	CDirectionLight* m_directionLights[Max_DirectionLight] = { nullptr };

	int m_currentNum = 0;

};

