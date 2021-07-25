#pragma once
#include "ObstacleObject.h"
#include "PointLight.h"
#include "effect/Effect.h"

//「炎」オブジェクトの定数データ
namespace flameConstData
{
	//最大HP
	constexpr int HP_MAX = 3;

	/*
	* ポイントライト関連の定数
	*/

	//ポイントライトの上へ上げるの座標の長さ
	constexpr float LENGTH_UP_POSITION_POINT_LIGHT = 300.0f;
	//ポイントライトのカラー
	const Vector4 COLOR_POINT_LIGHT = { 500.0f,1.0f,1.0f,1.0f };
	//ポイントライトの影響範囲
	constexpr float RANGE_POINT_LIGHT = 500.0f;


	/*
	* サウンド関連の定数
	*/

	//炎の常時鳴っているサウンドのファイルパス
	constexpr const wchar_t* const SOUND_FILEPATH_FLAME = L"Assets/sound/flame.wav";
	//炎の常時鳴っているサウンドの音量
	constexpr float SOUND_VOLUME_FLAME = 0.8f;

	//炎と水の反応サウンドのファイルパス
	constexpr const wchar_t* const SOUND_FILEPATH_FLAME_WATAER = L"Assets/sound/flame_water.wav";
	//炎と水の反応サウンドの音量
	constexpr float SOUND_VOLUME_FLAME_WATER = 2.0f;


	/*
	* エフェクト関連の定数
	*/

	//炎と水の反応エフェクトのファイルパス
	constexpr const char16_t* const EFFECT_FILEPATH_FLAME = u"Assets/effect/flame_water.efk";
	//炎と水の反応エフェクトの拡大率
	const Vector3 EFFECT_SCALE_FLAME = { 200.0f,200.0f,200.0f };
	//炎と水の反応エフェクトの上へ上げる座標の長さ
	constexpr float LENGTH_UP_POSITION_EFFECT = 100.0f;


	//炎の大きさ補正
	//大きさを1/3にすると、小さすぎてゴールできちゃうから、
	//大きさを5/5→4/5→3/5→0と変化させる
	constexpr int SIZE_COMPLEMENT_FLAME = 3;

	//炎の常時鳴っているサウンドが聞こえる最大距離
	constexpr float DISTANCE_MAX_FLAME_SOUND = 1500.0f;
}



/// <summary>
/// 障害オブジェクト
/// 炎オブジェクト
/// </summary>
class OOflame : public CObstacleObject
{
public:		//自動で呼ばれるメンバ関数
	bool StartSub() override final;		//スタート関数
	~OOflame();						//デストラクタ
	void UpdateSub() override final;	//アップデート関数

public:		//メンバ関数
	void Damage();	//ダメージを受ける
	void Burn(); //燃えるSE

private:	//privateなメンバ関数

	/**
	 * @brief ポイントライトの初期化処理
	*/
	void InitPointLight();

	/**
	 * @brief サウンドの初期化処理
	*/
	void InitSound();

	/**
	 * @brief エフェクトの初期化処理
	*/
	void InitEffect();

private:	//データメンバ
	int m_hp = flameConstData::HP_MAX;		//現在のHP
	CPointLight* m_pointLight = nullptr;	//ポイントライト
	CSoundCue* m_flameSE = nullptr;			//炎の常時鳴っているサウンド
	Effect* m_flame_waterEF = nullptr;		//炎と水の反応エフェクト
	CSoundCue* m_flame_waterSE = nullptr;	//炎と水の反応サウンド

};

