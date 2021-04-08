#pragma once
#include "LightData.h"


class CDirectionLight;

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
	static const int Max_PointLight = 4;		//ポイントライトの最大数
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

public://publidなメンバ関数

	/// <summary>
	/// ゲームループで読んでほしい
	/// アップデート関数関数
	/// </summary>
	void ExecuteUpdate();

	/// <summary>
	/// ライトを追加する関数
	/// </summary>
	/// <param name="light">追加するライト</param>
	/// <returns>追加できたらtrueを戻す</returns>
	bool AddLight(CDirectionLight* light);

	/// <summary>
	/// ライトを消去する関数
	/// </summary>
	/// <param name="light">消去するライト</param>
	void RemoveLight(CDirectionLight* light);

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

	int GetShadowNum()const
	{
		return m_lightParam.numShadow;
	}

	void AddShadowNum()
	{
		m_lightParam.numShadow++;
	}

	void RemoveShadowNum()
	{
		m_lightParam.numShadow--;
	}

private://データメンバ

	static SLightParam m_lightParam;	//ライトの共通パラメータ

	//それぞれのディレクションライトのデータ
	SDirectionLight m_directionLightsData[Max_DirectionLight];

	//それぞれのディレクションライトの参照
	CDirectionLight* m_directionLights[Max_DirectionLight] = { nullptr };

	//現在のライトの数
	int m_currentNum = 0;

};

