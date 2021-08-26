#pragma once
#include "ObstacleObject.h"

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
			 * @brief 「一方通行」の定数データ
			*/
			namespace onewayConstData
			{
				//サイドのOBBへの長さ
				constexpr float LENGHT_TO_SIDE_OBB = 300.0f;
			}

			/// <summary>
			/// 障害オブジェクト
			/// 一方通行オブジェクト
			/// </summary>
			class OOoneway : public CObstacleObject
			{
			public:		//自動で呼ばれるメンバ関数
				bool StartSub() override final;		//スタート関数
				~OOoneway();						//デストラクタ
				void UpdateSub() override final;	//アップデート関数

			public:		//メンバ関数

				/// <summary>
				/// 左か右か？を設定する。
				/// </summary>
				/// <param name="leftOrRight">左か右か？</param>
				void SetLeftOrRight(const int leftOrRight)
				{
					//左か右か？を新しく設定
					m_leftOrRight = leftOrRight;

					//両端のOBBを一度例外にしておく。
					for (int i = 0; i < nsCommonData::EN_LEFT_AND_RIGHT_NUM; i++)
					{
						m_sideOBB[i].SetExceptionFlag(true);
					}

					if (m_leftOrRight == nsCommonData::EN_LEFT)
					{
						if (m_rightCanPassMR)
							m_rightCanPassMR->Deactivate();
						if (GetModelRender())
							GetModelRender()->Activate();
					}
					else
					{
						if (m_rightCanPassMR)
							m_rightCanPassMR->Activate();
						if (GetModelRender())
							GetModelRender()->Deactivate();
					}
				}

			private:	//privateなメンバ関数

				/**
				 * @brief モデルの初期化処理
				*/
				void InitModel();

				/**
				 * @brief 右向きか左向きかに合わせて初期化処理
				*/
				void InitLeftOrRight();

				/**
				 * @brief OBBの初期化処理
				*/
				void InitOBB();

				/**
				 * @brief 通れるモデルの有効化する
				*/
				void CanPassModelActivate();

				/**
				 * @brief 通れないモデルを有効化する
				*/
				void CannotPassModelActivate();

				/**
				 * @brief モデルのトランスフォームの更新
				*/
				void ModelTransformUpdate();

			private:	//データメンバ

				nsOBB::COBB m_sideOBB[nsCommonData::EN_LEFT_AND_RIGHT_NUM];		//両サイドのOBB
				int m_leftOrRight = nsCommonData::EN_LEFT;				//左か右か？
				nsGraphic::nsModel::CModelRender* m_canPassMR = nullptr;		//左向きの通れる時のモデル
				nsGraphic::nsModel::CModelRender* m_rightMR = nullptr;			//右向きのモデル
				nsGraphic::nsModel::CModelRender* m_rightCanPassMR = nullptr;	//右向きの通れる時のモデル



			private:	//デバック用
#ifdef MY_DEBUG
				static const int m_vertNum = 8;
				CModelRender* m_dbgVertMR[enLeftAndRightNum][m_vertNum];
#endif
			};

		}
	}
}