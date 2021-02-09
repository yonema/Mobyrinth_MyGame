#pragma once
#include "LightData.h"
#include "DirectionLight.h"


class CLightManager
{
private:
	static CLightManager* m_instance;	//シングルトンパターン
	CLightManager();	//コンストラクタと
	~CLightManager();	//デストラクタをprivateに隠す
	//void Init();
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
	//ライトを追加
	void AddLight(CLightBase* light);
	//ライトを消去
	void RemoveLight(const CLightBase* light);

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

private:
	struct SLightParam {
		Vector3 eyePos;				//視線の位置。
		int numDirectionLight;		//ディレクションライトの数。
		Vector3 ambientLight;		//アンビエントライト。
		int numPointLight;			//ポイントライトの数。
		float specPow;				//スペキュラの絞り
	};
	SLightParam m_lightParam;
	std::list<CDirectionLight*> m_directionLights;	//ディレクションライトのリスト


};

