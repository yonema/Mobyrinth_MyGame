#include "stdafx.h"
#include "OOReverseALL.h"

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
			//「全反転」の定数データを使用可能にする
			using namespace reverseAllConstData;

			//スタート関数
			bool OOReverseALL::StartSub()
			{
				//モデルの初期化とタイプの設定
				Init(MODEL_FILEPATH_REVERSE_ALL, EN_OO_TYPE_REVERSE_ALL);

				//OBBのパラメーターを設定する
				SetOBBDirectionLength(SIZE_OBB_REVERSE_ALL);

				//モデルを非表示にする
				GetModelRender()->Deactivate();

				//OBBWorldに自身のOBBの登録を消去さて、プレイヤーと衝突しないようにする
				nsOBB::COBBWorld::GetInstance()->RemoveOBB(GetOBB());

				//オブジェクトと当たらないようにする
				SetIsHitFlag(false);

				//自己発光色を設定
				SetModelEmissionColor(MODEL_COLOR_EMISSION);

				//サウンドの初期化処理
				InitSound();

				//エフェクトの初期化処理
				InitEffect();


				//デバック用
#ifdef MY_DEBUG
	//後で消す
				Vector3* vertPos = GetOBB().GetBoxVertex();
				for (int i = 0; i < m_vertNum; i++)
				{
					//OBBの頂点を見るためのモデルの生成と初期化
					m_dbgVertPosMR[i] = NewGO<CModelRender>(0);
					m_dbgVertPosMR[i]->Init("Assets/modelData/dbgBox.tkm");
					m_dbgVertPosMR[i]->SetPosition(vertPos[i]);
					m_dbgVertPosMR[i]->SetRotation(m_rotation);
				}
				//デバック用ここまで
#endif
				return true;
			}

			/**
			 * @brief サウンドの初期化処理
			*/
			void OOReverseALL::InitSound()
			{
				//changeSEのサウンドキューを生成する
				m_changeAllSC = NewGO<nsSound::CSoundCue>(nsCommonData::PRIORITY_FIRST);
				//changeSEのサウンドキューを、waveファイルを指定して初期化する。
				m_changeAllSC->Init(SOUND_FILEPATH_CHANGE_ALL);
				//音量調節
				m_changeAllSC->SetVolume(SOUND_VALUME_CHANGE_ALL);

				return;
			}

			/**
			 * @brief エフェクトの初期化処理
			*/
			void OOReverseALL::InitEffect()
			{
				//全反転自身を表すエフェクトの生成と初期化
				m_reverslAllEF = NewGO<Effect>(nsCommonData::PRIORITY_FIRST);
				m_reverslAllEF->Init(EFFECT_FILEPATH_REVERSE_ALL);
				//座標と回転と拡大率を設定す
				m_reverslAllEF->SetPosition(m_position);
				m_reverslAllEF->SetRotation(m_rotation);
				m_reverslAllEF->SetScale(EFFECT_SCALE_REVERSE_ALL);
				//再生する
				m_reverslAllEF->Play();

				//全反転する時のエフェクトの生成と初期化
				m_changeAllEF = NewGO<Effect>(nsCommonData::PRIORITY_FIRST);
				m_changeAllEF->Init(EFFECT_FILEPATH_CHANGE_ALL);
				//座標と回転と拡大率を設定す
				m_changeAllEF->SetPosition(m_position);
				m_changeAllEF->SetRotation(m_rotation);
				m_changeAllEF->SetScale(EFFECT_SCALE_CHANGE_ALL);

				return;
			}

			//デストラクタ
			OOReverseALL::~OOReverseALL()
			{
				//エフェクトが再生中なら
				if (m_reverslAllEF->IsPlay())
					//停止する
					m_reverslAllEF->Stop();

				//全反転自身を表すエフェクトを破棄
				DeleteGO(m_reverslAllEF);

				//全反転する時のエフェクトを破棄
				DeleteGO(m_changeAllEF);

				//サウンドの破棄
				DeleteGO(m_changeAllSC);

#ifdef MY_DEBUG
				//デバック用
				//後で消す
				for (int i = 0; i < m_vertNum; i++)
				{
					//OBBの頂点を見るためのモデルの破棄
					DeleteGO(m_dbgVertPosMR[i]);
				}
				//デバック用ここまで
#endif

				return;
			}

			//ポーズ中でもいつでもアップデートする関数
			void OOReverseALL::AlwaysUpdate()
			{
				//エフェクト再生までのタイマー
				m_effectLoopTimer += nsTimer::GameTime().GetFrameDeltaTime();

				//ループ再生にする
				if (m_effectLoopTimer >= EFFECT_TIME_LOOP) {
					m_reverslAllEF->Play();
					m_effectLoopTimer = 0;
				}

				return;
			}


			//アップデート関数
			void OOReverseALL::UpdateSub()
			{

				//アップデートステートで処理を振り分ける
				//switch (m_updateState)
				//{
				//case enBeforHitPlayer:
				//	//プレイヤーと衝突前の処理
				//	BeforeHitPlayer();
				//	break;
				//case enHitPlayer:
				//	//プレイヤーと衝突時の処理
				//	HitPlayer();
				//	break;
				//case enAfterHitPlayer:
				//	//プレイヤーと衝突後の処理
				//	AfterHitPlayer();
				//	break;
				//}


				///
				///
				/// switchやelse ifにしてしまうと、
				/// 目的の処理が1フレーム遅れてしまうため
				/// 全部ifで振り分けた。

				//アップデートステートで処理を振り分ける
				if (m_updateState == EN_BEFOR_HIT_PLAYER)
					//プレイヤーと衝突前の処理
					BeforeHitPlayer();

				if (m_updateState == EN_HIT_PLAYER)
					//プレイヤーと衝突時の処理
					HitPlayer();

				if (m_updateState == EN_AFTER_HIT_PLAYER)
					//プレイヤーと衝突後の処理
					AfterHitPlayer();

				return;
			}

			//プレイヤーと衝突前の処理
			void OOReverseALL::BeforeHitPlayer()
			{
				//プレイヤーとの衝突を調べる
				if (IsHitPlayer())
				{
					//衝突している

					//衝突時のプレイヤーの座標を保治
					m_playerHitPosition = m_player->GetPosition();
					//アップデートステートをプレイヤーと衝突時の状態へ
					m_updateState = EN_HIT_PLAYER;

				}
				else
				{
					//衝突していない

					//衝突前のプレイヤーの座標を保持
					m_playerBeforePosition = m_player->GetPosition();
				}

				return;
			}

			//プレイヤーと衝突時の処理
			void OOReverseALL::HitPlayer()
			{
				//プレイヤーとの衝突を調べる
				if (IsHitPlayer())
				{
					//衝突している

					//衝突時のプレイヤーの座標を保治
					m_playerHitPosition = m_player->GetPosition();
				}
				else
				{
					//衝突していない

					//衝突後のプレイヤーの座標の確保
					m_playerAfterPosition = m_player->GetPosition();
					//アップデートステートをプレイヤーと衝突後の状態へ
					m_updateState = EN_AFTER_HIT_PLAYER;
				}

				return;
			}

			//プレイヤーと衝突後の処理
			void OOReverseALL::AfterHitPlayer()
			{
				//衝突後のプレイヤーの座標の確保
				m_playerAfterPosition = m_player->GetPosition();

				//衝突前から衝突時の座標へのベクトル
				Vector3 beforeToHit = m_playerHitPosition - m_playerBeforePosition;
				//衝突時から衝突後の座標へのベクトル
				Vector3 hitToAfter = m_playerAfterPosition - m_playerHitPosition;
				//正規化しておく
				beforeToHit.Normalize();
				hitToAfter.Normalize();
				//二つの内積を取る
				float inner = Dot(beforeToHit, hitToAfter);

				//内積で二つのベクトルの向きを調べる
				if (inner >= 0.0f)
				{
					//正、同じ向きなら

					//レベルオブジェクトを取ってくる
					std::vector<ILevelObjectBase*> levelObjects
						= CLevelObjectManager::GetInstance()->GetLevelObjects();
					//全てのレベルオブジェクトに検索
					for (ILevelObjectBase* levelObject : CLevelObjectManager::GetInstance()->GetLevelObjects())
					{
						//透明オブジェクトでは無かったら
						if (levelObject->GetIsHitFlag())
						{
							//反転させる
							levelObject->AllReverse();
						}
					}
					//changeSEをループ再生をオフで再生する。
					m_changeAllSC->Play(false);

					//上にあげるベクトル
					Vector3 upVec = g_VEC3_UP;
					m_rotation.Apply(upVec);
					upVec.Scale(EFFECT_LENGHT_POSITION_CHANGE_ALL);
					m_changeAllEF->SetPosition(m_position + upVec);		//座標を渡す
					m_changeAllEF->SetRotation(m_rotation);
					m_changeAllEF->Play();								//再生
				}


				//衝突前のプレイヤーの座標を保持
				m_playerBeforePosition = m_player->GetPosition();

				//アップデートステートをプレイヤーと衝突前の状態へ
				m_updateState = EN_BEFOR_HIT_PLAYER;

				return;
			}

		}
	}
}