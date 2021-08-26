#pragma once
#include "ObstacleObject.h"
#include "ReversibleObject.h"
#include "FontRender.h"
#include "effect/Effect.h"

/**
 * @brief メビリンス
*/
namespace nsMobyrinth
{
	/**
	 * @brief レベルオブジェクト
	*/
	namespace nsLevelObject
	{
		/**
		 * @brief 障害オブジェクト
		*/
		namespace nsObstacleObject
		{

			/**
			 * @brief 「全反転」の定数データ
			*/
			namespace reverseAllConstData
			{
				//アップデートステート
				enum EN_UPDATE_STATE
				{
					EN_BEFOR_HIT_PLAYER,	//プレイヤーと衝突前の状態
					EN_HIT_PLAYER,		//プレイヤーと衝突時の状態
					EN_AFTER_HIT_PLAYER,	//プレイヤーと衝突後の状態
				};

				//モデルの自己発光カラー
				const Vector4 MODEL_COLOR_EMISSION = { 0.5f,0.0f,0.5f,1.0f };

				/*
				* サウンド関連の定数データ
				*/

				//全反転の時のサウンドのファイルパス
				constexpr const wchar_t* const SOUND_FILEPATH_CHANGE_ALL = L"Assets/sound/allchange.wav";
				//全反転の時のサウンドの音量
				constexpr float SOUND_VALUME_CHANGE_ALL = 1.0f;

				/*
				* エフェクト関連の定数データ
				*/

				//全反転自身を表すエフェクトのファイルパス
				constexpr const char16_t* const EFFECT_FILEPATH_REVERSE_ALL = u"Assets/effect/reverseall.efk";
				//全反転自身を表すエフェクトの拡大率
				const Vector3 EFFECT_SCALE_REVERSE_ALL = { 200.0f,200.0f,200.0f };
				//全反転自身を表すエフェクトのループ切り替え時間
				constexpr float EFFECT_TIME_LOOP = 2.0f;

				//全反転する時のエフェクトのファイルパス
				constexpr const char16_t* const EFFECT_FILEPATH_CHANGE_ALL = u"Assets/effect/reverseall2.efk";
				//全反転する時のエフェクトの拡大率
				const Vector3 EFFECT_SCALE_CHANGE_ALL = { 50.0f,50.0f,50.0f };
				//全反転する時のエフェクトの上にあげる長さ
				constexpr float EFFECT_LENGHT_POSITION_CHANGE_ALL = 100.0f;
			}

			/// <summary>
			/// 障害オブジェクト
			/// 全反転オブジェクト
			/// </summary>
			class OOReverseALL : public CObstacleObject
			{
			public:		//自動で呼ばれるメンバ関数
				bool StartSub() override final;		//スタート関数
				~OOReverseALL();					//デストラクタ
				void UpdateSub() override final;	//アップデート関数
				void AlwaysUpdate()override final;	//ポーズ中でもいつでもアップデートする関数

			private:	//privateなメンバ関数

				/**
				 * @brief サウンドの初期化処理
				*/
				void InitSound();

				/**
				 * @brief エフェクトの初期化処理
				*/
				void InitEffect();

				void BeforeHitPlayer();	//プレイヤーと衝突前の処理

				void HitPlayer();		//プレイヤーと衝突時の処理

				void AfterHitPlayer();	//プレイヤーと衝突後の処理

			private:	//データメンバ

				//アップデートステート
				reverseAllConstData::EN_UPDATE_STATE m_updateState = reverseAllConstData::EN_BEFOR_HIT_PLAYER;
				Vector3 m_playerBeforePosition = g_VEC3_ZERO;	//衝突前のプレイヤーの座標
				Vector3 m_playerHitPosition = g_VEC3_ZERO;		//衝突時のプレイヤーの座標
				Vector3 m_playerAfterPosition = g_VEC3_ZERO;	//衝突後のプレイヤーの座標
				float m_effectLoopTimer = 0.0f;		//エフェクトをループさせるためのタイマー
				Effect* m_reverslAllEF = nullptr;	//全反転自身を表すエフェクト
				Effect* m_changeAllEF = nullptr;	//全反転するときのエフェクト
				nsSound::CSoundCue* m_changeAllSC = nullptr; //全反転する時のサウンド

			private:	//デバック用

#ifdef MY_DEBUG
//後で消す
				static const int m_vertNum = 8;		//頂点の数
				CModelRender* m_dbgVertPosMR[m_vertNum] = { nullptr };	//頂点を見るためのモデル
#endif
			};

		}
	}
}