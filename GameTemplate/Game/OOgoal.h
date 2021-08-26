#pragma once
#include "ObstacleObject.h"
#include "SpriteRender.h"
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
			 * @brief ゴールの定数データ
			*/
			namespace goalConstData
			{
				/*
				* エフェクト関連の定数
				*/
				//ゴール時のエフェクトのファイルパス
				constexpr const char16_t* const EFFECT_FILEPAHT_GOAL = u"Assets/effect2/goal.efk";
				//ゴール時のエフェクトの拡大率
				const Vector3 EFFECT_SCALE_GOAL = { 150.0f,150.0f,150.0f };

				/*
				* スプライト関連の定数
				*/

				//ゴール時のスプライトのファイルパス
				constexpr const char* const SPRITE_FILEPATH_GOAL = "Assets/Image/Clear.DDS";
				//ゴール時のスプライトの幅
				constexpr float SPRITE_WIDHT_GOAL = 1024.0f;
				//ゴール時のスプライトの高さ
				constexpr float SPRITE_HEIGHT_GOAL = 512.0f;
				//ゴール時のスプライトの座標
				const Vector3 SPRITE_POSITION_GOAL = { 0.0f,250.0f,0.0f };

				/*
				* サウンド関連の定数
				*/

				//ゴールのSEのファイルパス
				constexpr const wchar_t* const SOUND_FILEPATH_SE_GOAL = L"Assets/sound/goal.wav";
				//ゴールのSEの音量
				constexpr float SOUND_VOLUME_SE_GOAL = 0.5f;
			}

			/// <summary>
			/// 障害オブジェクト
			/// ゴールオブジェクト
			/// </summary>
			class OOgoal : public CObstacleObject
			{
			public:		//自動で呼ばれるメンバ関数
				bool StartSub()override final;	//スタート関数
				~OOgoal();						//デストラクタ
				void UpdateSub()override final;	//アップデート関数

			public:		//メンバ関数

				/// <summary>
				/// ゴールしているかどうかを得る
				/// </summary>
				/// <returns>ゴールしているか？</returns>
				bool GetIsGoal()const
				{
					return m_isGoal;
				}

			private:	//privateなメンバ関数

				/**
				 * @brief エフェクトの初期化処理
				*/
				void InitEffect();

				/**
				 * @brief スプライトの初期化処理
				*/
				void InitSprite();

				/**
				 * @brief サウンドの初期化処理
				*/
				void InitSound();

				/**
				 * @brief ゴール時の処理
				*/
				void Goal();

			private:	//データメンバ
				bool m_isGoal = false;				//ゴール判定
				Effect* m_goalEF = nullptr;			//ゴール時のエフェクト
				nsGraphic::nsSprite::CSpriteRender* m_goalSR = nullptr;	//ゴール時のスプライト
				nsSound::CSoundCue* m_goalSC = nullptr;		//ゴール時のサウンドキュー
			};

		}
	}
}