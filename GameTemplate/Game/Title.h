#pragma once
#include "TitleConstData.h"
#include "FontRender.h"
#include "SpriteRender.h"
#include "Level2D.h"
#include "SoundCue.h"

//背景
#include "stage_title.h"
#include "Mobius.h"
//プレイヤー
#include "Player.h"
//ステージ
#include "Stage.h"

/**
 * @brief メビリンス
*/
namespace nsMobyrinth
{
	/**
	 * @brief タイトル
	*/
	namespace nsTitle
	{

		/**
		 * @brief タイトルクラス
		*/
		class CTitle : public IGameObject
		{
		public:		//自動で呼ばれるメンバ関数
			bool Start()override final;		//スタート関数
			~CTitle();						//デストラクタ
			void Update()override final;	//アップデート関数

		public:		//メンバ関数

			/**
			 * @brief ワイプインを行うか？を設定
			 * @param [in] wipeInFlag ワイプインを行うか？
			*/
			void SetWipeInFlag(const bool wipeInFlag)
			{
				m_wipeInFlag = wipeInFlag;
			}

		private:	//privateなメンバ関数

			/**
			 * @brief タイトルで使うスプライトの初期化
			*/
			void InitTitleSprite();

			/**
			 * @brief ステージセレクトで使うスプライトの初期化
			*/
			void InitStageSelectSprite();

			/**
			 * @brief SEの初期化
			*/
			void InitSE();

			/**
			 * @brief BGMの初期化
			*/
			void InitBGM();

			/**
			 * @brief タイトル画面
			*/
			void TitleScreen();

			/**
			 * @brief 起動時の演出
			*/
			void StartTitle();

			/**
			 * @brief ステージセレクト
			*/
			void StageSelect();

			/**
			 * @brief 次のステージを選択する
			*/
			void SelectNextStage();

			/**
			 * @brief 前のステージを選択する
			*/
			void SelectPrevStage();

			/**
			 * @brief ステージを決定した時の処理
			*/
			void StageDecision();

			/**
			 * @brief 自身のオブジェクトを破棄する関数
			*/
			void Release()
			{
				DeleteGO(this);
			}

		private:	//データメンバ

			/*
			* ステート関連
			*/
			//現在のステージのステート（状態）
			int m_stageState = titleConstData::EN_START_TITLE;
			//現在のステージセレクトのステート
			int m_stageSelectState = titleConstData::EN_STAGE_1;

			/*
			* フラグ関連
			*/
			bool m_buttonFlag = true;	//ボタンを押すことができるか？
										//（連続入力防止用）
			bool m_wipeInFlag = false;	//ワイプインするか？
			bool m_blinkingFlag = true;	//m_pressAButtonの透過処理に使用


			//ステージのアイコンからカーソルへのベクトル
			Vector3 m_stageIconToCursorVec = g_VEC3_ZERO;

			int m_countStartTitle = 0;	//起動時演出の変数


			/*
			* 別の場所で生成されるオブジェクト
			*/
			nsPlayer::Player* m_player = nullptr;				//プレイヤーのポインタ
			nsLevelObject::nsMobius::CMobius* m_mobius = nullptr;			//メビウスの輪のステージ
			nsStage::CStage_title* m_stageTitle = nullptr;	//ステージタイトルクラス


			/*
			* スプライト関連
			*/
			nsLevel2D::CLevel2D m_level2D;								//スプライト用のレベルクラス
			nsGraphic::nsSprite::CSpriteRender* m_title = nullptr;				//タイトルのスプライト
			nsGraphic::nsSprite::CSpriteRender* m_pressAButton = nullptr;		//「Aボタンをおしてね」のスプライト
			nsGraphic::nsSprite::CSpriteRender* m_cursor = nullptr;				//カーソルのスプライト
			//ステージのアイコンのスプライト
			nsGraphic::nsSprite::CSpriteRender* m_stageIcon[titleConstData::EN_STAGE_NUM] = { nullptr };
			//ステージクリアのスプライト
			nsGraphic::nsSprite::CSpriteRender* m_stageClear[titleConstData::EN_STAGE_NUM] = { nullptr };
			nsGraphic::nsSprite::CSpriteRender* m_stageSelection = nullptr;		//ステージセレクトのスプライト
			nsGraphic::nsSprite::CSpriteRender* m_stageSelectionBase = nullptr;	//ステージ選択背景のスプライト



			/*
			* サウンド関連
			*/
			nsSound::CSoundCue* m_bgmTitle = nullptr;	//BGMのサウンドキュー
			bool m_initedBGM = false;			//BGMが初期化済みか？
			nsSound::CSoundCue* m_selectSE = nullptr;	//カーソル移動のサウンドキュー
			nsSound::CSoundCue* m_buttonASE = nullptr;	//Aボタンのサウンドキュー
			nsSound::CSoundCue* m_buttonBSE = nullptr;	//Bボタンのサウンドキュー

		};

	}
}