#pragma once
#include "ReversibleObject.h"
#include "OObox.h"
#include "OOpadlock.h"

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
		 * @brief 反転オブジェクト
		*/
		namespace nsReversibleObject
		{

			/**
			 * @brief 「釘」「金槌」の定数データ
			*/
			namespace nail_hammerConstData
			{
				//箱が壊れるときのサウンドのファイルパス
				constexpr const wchar_t* const SOUNDE_FILEPATH_BREAK_BOX = L"Assets/sound/boxBreak.wav";
				//箱が壊れるときのサウンドの音量
				constexpr float SOUNDE_VOLUME_BREAK_BOX = 0.3f;

				//箱が壊れるときのエフェクトのファイルパス
				constexpr const char16_t* const EFFECT_FILEPATH_BREAK_BOX = u"Assets/effect/flame_water.efk";
				//箱が壊れるときのエフェクトの拡大率
				const Vector3 EFFECT_SCALE_BREAK_BOX = { 200.0f,200.0f,200.0f };

				//エフェクトを上にあげる長さ
				constexpr float LENGTH_UP_POSITION_EFFECT = 100.0f;
			}

			/// <summary>
			/// 反転オブジェクト
			/// 釘、金槌オブジェクト
			/// </summary>
			class ROnail_hammer : public CReversibleObject
			{
			public:		//自動で呼ばれるメンバ関数
				bool StartSub() override final;	//スタート関数
				~ROnail_hammer();					//デストラクタ

				/// <summary>
				/// クエリしてほしいタイミングで呼ばれる関数
				/// </summary>
				void QuerySub()override final;

			private:	//privateなメンバ関数

				/**
				 * @brief サウンドの初期化処理
				*/
				void InitSound();

				/**
				 * @brief エフェクトの初期化処理
				*/
				void InitEffect();

				/**
				 * @brief 自身が「金槌」で、「箱」と衝突した時の処理
				 * @param [in] box 「箱」のポインタ
				*/
				void HammerHitBox(nsObstacleObject::OObox* box);

			private:	//データメンバ
				//一度に複数のオブジェクトを破棄することを防ぐためのフラグ
				bool m_actionFlag = true;			//行動できるか？
				nsSound::CSoundCue* m_brokenSE = nullptr;	//壊れた時のサウンド
				Effect* m_brokenEF = nullptr;			//壊れた時のエフェクト
			};

		}
	}
}