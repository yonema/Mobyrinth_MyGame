#include "stdafx.h"
#include "OOoneway.h"

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

			//ObstacleObjectのモデルのファイルパスとOBBのサイズの定数データを使用可能にする
			using namespace OOsFilepathAndObbSizeConstData;
			//「一方通行」の定数データを使用可能にする
			using namespace onewayConstData;
			//共通定数データを使用可能にする
			using namespace nsCommonData;

			//スタート関数
			bool OOoneway::StartSub()
			{
				//モデルの初期化とタイプの設定
				Init(MODEL_FILEPATH_ONEWAY_LEFT, EN_OO_TYPE_ONEWAY);
				//モデルを無効化して非表示にする
				GetModelRender()->Deactivate();

				//モデルの初期化処理
				InitModel();

				//右向きか左向きかに合わせて初期化処理
				InitLeftOrRight();

				//OBBの初期化処理
				InitOBB();


#ifdef MY_DEBUG
				Vector3* vertPos;
				for (int i = 0; i < enLeftAndRightNum; i++)
				{
					vertPos = m_sideOBB[i].GetBoxVertex();
					for (int j = 0; j < m_vertNum; j++)
					{
						m_dbgVertMR[i][j] = NewGO<CModelRender>(0);
						m_dbgVertMR[i][j]->Init("Assets/modelData/dbgBox.tkm");
						m_dbgVertMR[i][j]->SetPosition(vertPos[j]);
					}
				}
#endif

				return true;
			}

			/**
			 * @brief モデルの初期化処理
			*/
			void OOoneway::InitModel()
			{
				//左向きの通れる時のモデルの生成と初期化
				m_canPassMR = NewGO<nsGraphic::nsModel::CModelRender>(PRIORITY_FIRST);
				m_canPassMR->Init(MODEL_FILEPATH_ONEWAY_LEFT_CAN_PASS, D3D12_CULL_MODE_NONE);
				//座標、回転、拡大率を設定。
				m_canPassMR->SetPosition(m_position);
				m_canPassMR->SetRotation(m_rotation);
				m_canPassMR->SetScale(m_scale);
				//アウトラインを描画する
				m_canPassMR->SetDrawOutLineFlag(true);
				//無効化して非表示にする
				m_canPassMR->Deactivate();

				//右向きのモデルの生成と初期化
				m_rightMR = NewGO<nsGraphic::nsModel::CModelRender>(PRIORITY_FIRST);
				m_rightMR->Init(MODEL_FILEPATH_ONEWAY_RIGHT, D3D12_CULL_MODE_NONE);
				//座標、回転、拡大率を設定。
				m_rightMR->SetPosition(m_position);
				m_rightMR->SetRotation(m_rotation);
				m_rightMR->SetScale(m_scale);
				//アウトラインを描画する
				m_rightMR->SetDrawOutLineFlag(true);
				//無効化して非表示にする
				m_rightMR->Deactivate();

				//右向きの通れる時のモデルの生成と初期化
				m_rightCanPassMR = NewGO<nsGraphic::nsModel::CModelRender>(PRIORITY_FIRST);
				m_rightCanPassMR->Init(MODEL_FILEPATH_ONEWAY_RIGHT_CAN_PASS, D3D12_CULL_MODE_NONE);
				//座標、回転、拡大率を設定。
				m_rightCanPassMR->SetPosition(m_position);
				m_rightCanPassMR->SetRotation(m_rotation);
				m_rightCanPassMR->SetScale(m_scale);
				//アウトラインを描画する
				m_rightCanPassMR->SetDrawOutLineFlag(true);
				//無効化して非表示にする
				m_rightCanPassMR->Deactivate();

				return;
			}

			/**
			 * @brief 右向きか左向きかに合わせて初期化処理
			*/
			void OOoneway::InitLeftOrRight()
			{
				//左向きか？
				if (m_leftOrRight == EN_LEFT)
				{
					//左向き

					//左向き（通常の向き）の通れないモデルを有効化
					GetModelRender()->Activate();
				}
				else
				{
					//右向き

					//右向き（反対向き）の通れないモデルを有効化
					m_rightMR->Activate();

				}

				return;
			}

			/**
			 * @brief OBBの初期化処理
			*/
			void OOoneway::InitOBB()
			{
				//OBBの方向ベクトルの長さを設定
				SetOBBDirectionLength(SIZE_OBB_ONEWAY);

				//OBBWorldに自身のOBBの登録を消去させて、このOBBがプレイヤーとぶつからないようにする
				nsOBB::COBBWorld::GetInstance()->RemoveOBB(GetOBB());

				//OBBの初期化データ
				nsOBB::SInitOBBData initOBBData;
				initOBBData.width = SIZE_OBB_ONEWAY_SIDE.x;				//横：X
				initOBBData.length = SIZE_OBB_ONEWAY_SIDE.z;			//長さ：Z
				initOBBData.height = SIZE_OBB_ONEWAY_SIDE.y;			//高さ：Y
				initOBBData.rotation = m_rotation;						//回転

				//サイドのOBBへのベクトル
				Vector3 toSideObbVec = g_VEC3_RIGHT;
				//自身の回転で回す
				m_rotation.Apply(toSideObbVec);
				//サイドのOBBへのベクトルを伸ばす
				toSideObbVec.Scale(LENGHT_TO_SIDE_OBB);

				//左サイドのOBBの座標
				initOBBData.position = m_position + toSideObbVec;
				//左サイドのOBBの初期化
				m_sideOBB[EN_LEFT].Init(initOBBData);

				//左へのベクトルを右へのベクトルに変換
				toSideObbVec.Scale(-1.0f);
				//右サイドのOBBの座標
				initOBBData.position = m_position + toSideObbVec;
				//右サイドのOBBの初期化
				m_sideOBB[EN_RIGHT].Init(initOBBData);

				//両サイドのOBBをOBBワールドに登録して、プレイヤーと衝突するようにする
				for (int i = 0; i < EN_LEFT_AND_RIGHT_NUM; i++)
				{
					nsOBB::COBBWorld::GetInstance()->AddOBB(m_sideOBB[i]);
				}

				return;
			}


			//デストラクタ
			OOoneway::~OOoneway()
			{

				DeleteGO(m_canPassMR);
				DeleteGO(m_rightCanPassMR);
				DeleteGO(m_rightMR);
				//両サイドのOBBをOBBワールドから解除する
				for (int i = 0; i < EN_LEFT_AND_RIGHT_NUM; i++)
				{
					nsOBB::COBBWorld::GetInstance()->RemoveOBB(m_sideOBB[i]);
#ifdef MY_DEBUG
					for (int j = 0; j < m_vertNum; j++)
					{
						DeleteGO(m_dbgVertMR[i][j]);
					}
#endif
				}


				return;
			}

			//アップデート関数
			void OOoneway::UpdateSub()
			{
				//プレイヤーが自身と同じ向きを向いているか
				if (m_player->GetEnLeftOrRight() == m_leftOrRight)
				{
					//向いている時は
					//通れるため、サイドのOBBを例外設定にする
					m_sideOBB[m_leftOrRight].SetExceptionFlag(true);

					//通れるモデルを有効化する
					CanPassModelActivate();

				}
				else
				{
					//向いていない時は
					//通れないため、再度のOBBを例外設定ではなくする
					m_sideOBB[m_leftOrRight].SetExceptionFlag(false);

					//通れないモデルを有効化する
					CannotPassModelActivate();
				}

				//モデルのトランスフォームの更新処理
				ModelTransformUpdate();

				return;
			}

			/**
			 * @brief 通れるモデルの有効化する
			*/
			void OOoneway::CanPassModelActivate()
			{
				//右か左どちらかの通れるモデルを有効化し、もう片方は無効化する

				//左向きか？
				if (m_leftOrRight == EN_LEFT)
				{
					//左向き

					//左向きの通れるモデルを有効化
					m_canPassMR->Activate();
					//右向きの通れるモデルを無効化
					m_rightCanPassMR->Deactivate();
				}
				else
				{
					//右向き

					//左向きの通れるモデルを無効化
					m_canPassMR->Deactivate();
					//右向きの通れるモデルを有効化
					m_rightCanPassMR->Activate();
				}

				//通れないモデルは両方無効化して非表示にする
				GetModelRender()->Deactivate();
				m_rightMR->Deactivate();

				return;
			}

			/**
			 * @brief 通れないモデルを有効化する
			*/
			void OOoneway::CannotPassModelActivate()
			{
				//右か左どちらかの通れないモデルを有効化し、もう片方は無効化する

				//左向きか？
				if (m_leftOrRight == EN_LEFT)
				{
					//左向き

					//左向きの通れないモデルを有効化
					GetModelRender()->Activate();
					//右向きの通れないモデルを無効化
					m_rightMR->Deactivate();
				}
				else
				{
					//右向き

					//左向きの通れないモデルを無効化
					GetModelRender()->Deactivate();
					//右向きの通れないモデルを有効化
					m_rightMR->Activate();
				}

				//通れるモデルは両方無効化して、非表示にする
				m_canPassMR->Deactivate();
				m_rightCanPassMR->Deactivate();

				return;
			}

			/**
			 * @brief モデルのトランスフォームの更新
			*/
			void OOoneway::ModelTransformUpdate()
			{
				//有効なら更新する
				if (m_canPassMR->IsActive())
				{
					//座標、回転、拡大率を設定する
					m_canPassMR->SetPosition(m_position);
					m_canPassMR->SetRotation(m_rotation);
					m_canPassMR->SetScale(m_scale);
				}
				//有効なら更新する
				if (m_rightCanPassMR->IsActive())
				{
					//座標、回転、拡大率を設定する
					m_rightCanPassMR->SetPosition(m_position);
					m_rightCanPassMR->SetRotation(m_rotation);
					m_rightCanPassMR->SetScale(m_scale);
				}
				//有効なら更新する
				if (m_rightMR->IsActive())
				{
					//座標、回転、拡大率を設定する
					m_rightMR->SetPosition(m_position);
					m_rightMR->SetRotation(m_rotation);
					m_rightMR->SetScale(m_scale);
				}

				return;
			}

		}
	}
}