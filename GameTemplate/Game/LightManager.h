#pragma once
#include "LightData.h"


class CDirectionLight;
class CPointLight;
class CSpotLight;

/// <summary>
/// ライトマネージャークラス
/// </summary>
class CLightManager
{
private://privateなメンバ関数
	CLightManager();	//コンストラクタと
	~CLightManager();	//デストラクタをprivateに隠す
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

private://staticなデータメンバ
	static CLightManager* m_instance;			//唯一のインスタンス。シングルトンパターン
	static const int Max_DirectionLight = 4;	//ディレクションライトの最大数
	static const int Max_PointLight = 32;		//ポイントライトの最大数
	static const int Max_SpotLight = 4;			//スポットライトの最大数

public://staicなメンバ関数
	/// <summary>
	/// シングルトンパターン
	/// 唯一のインスタンスを作る関数
	/// 最初に呼んでね！
	/// </summary>
	static void CreateInstance()
	{
		m_instance = new CLightManager;
	}

	/// <summary>
	/// インスタンスを消す関数
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
	}

	/// <summary>
	/// インスタンスの参照を戻す関数
	/// </summary>
	/// <returns>インスタンスの参照</returns>
	static CLightManager* GetInstance()
	{
		return m_instance;
	}

	/// <summary>
	/// ディレクションライトの最大数を戻す関数
	/// </summary>
	/// <returns>ディレクションライトの最大数</returns>
	static const int GetMax_DirectionLight()
	{
		return Max_DirectionLight;
	}

	/// <summary>
	/// ポイントライトの最大数を戻す関数
	/// </summary>
	/// <returns>ポイントライトの最大数</returns>
	static const int GetMax_PointLight()
	{
		return Max_PointLight;
	}

	/// <summary>
	/// スポットライトの最大数を戻す関数
	/// </summary>
	/// <returns>スポットライトの最大数</returns>
	static const int GetMax_SpotLight()
	{
		return Max_SpotLight;
	}

public://publidなメンバ関数

	/// <summary>
	/// ゲームループで読んでほしい
	/// アップデート関数関数
	/// </summary>
	void Update();

	/// <summary>
	/// ライトを追加する関数（ディレクションライト）
	/// </summary>
	/// <param name="light">追加するライト</param>
	/// <returns>追加できたらtrueを戻す</returns>
	bool AddLight(CDirectionLight* light);

	/// <summary>
	/// ライトを追加する関数（ポイントライト）
	/// </summary>
	/// <param name="light">追加するライト</param>
	/// <returns>追加できたらtrueを戻す</returns>
	bool AddLight(CPointLight* light);

	/// <summary>
	/// ライトを追加する関数（スポットライト）
	/// </summary>
	/// <param name="light">追加するライト</param>
	/// <returns>追加できたらtrueを戻す</returns>
	bool AddLight(CSpotLight* light);

	/// <summary>
	/// ライトを消去する関数（ディレクションライト）
	/// </summary>
	/// <param name="light">消去するライト</param>
	void RemoveLight(CDirectionLight* light);

	/// <summary>
	/// ライトを消去する関数（ポイントライト）
	/// </summary>
	/// <param name="light">消去するライト</param>
	void RemoveLight(CPointLight* light);

	/// <summary>
	/// ライトを消去する関数（スポットライト）
	/// </summary>
	/// <param name="light">消去するライト</param>
	void RemoveLight(CSpotLight* light);

	/// <summary>
	/// ライトの共通パラメータの参照を戻す関数
	/// </summary>
	/// <returns>ライトの共通パラメータの参照</returns>
	SLightParam* const GetLightParam() const
	{
		return &m_lightParam;
	}

	/// <summary>
	/// ディレクションライト達の先頭アドレスを戻す関数
	/// </summary>
	/// <returns>ディレクションライト達の先頭アドレス</returns>
	SDirectionLight* const GetDirectionLigData() //const	//なんかダメみたい
	{
		return m_directionLightsData;
	}

	/// <summary>
	/// ポイントライト達の先頭アドレスを戻す関数
	/// </summary>
	/// <returns>ポイントライト達の先頭アドレス</returns>
	SPointLight* const GetPointLigData()
	{
		return m_pointLightsData;
	}

	/// <summary>
	/// スポットライト達の先頭アドレスを戻す関数
	/// </summary>
	/// <returns>スポットライト達の先頭アドレス</returns>
	SSpotLight* const GetSpotLigData()
	{
		return m_spotLightsData;
	}

	/// <summary>
	/// 現在のシャドウの数を取得
	/// </summary>
	/// <returns></returns>
	int GetShadowNum()const
	{
		return m_lightParam.numShadow;
	}

	/// <summary>
	/// シャドウの数を加算
	/// </summary>
	void AddShadowNum()
	{
		m_lightParam.numShadow++;
	}

	/// <summary>
	/// シャドウの数の減算
	/// </summary>
	void RemoveShadowNum()
	{
		m_lightParam.numShadow--;
	}

private://データメンバ

	static SLightParam m_lightParam;	//ライトの共通パラメータ

	//////////ディレクションライト//////////
	//それぞれのディレクションライトのデータ
	SDirectionLight m_directionLightsData[Max_DirectionLight];

	//それぞれのディレクションライトの参照
	CDirectionLight* m_directionLights[Max_DirectionLight] = { nullptr };


	//////////ポイントライト//////////
	//それぞれのポイントライトのデータ
	SPointLight m_pointLightsData[Max_PointLight];

	//それぞれのポイントライトの参照
	CPointLight* m_pointLights[Max_PointLight] = { nullptr };


	//////////スポットライト//////////
	//それぞれのスポットライトのデータ
	SSpotLight m_spotLightsData[Max_SpotLight];

	//それぞれのスポットライトの参照
	CSpotLight* m_spotLights[Max_SpotLight] = { nullptr };

};

