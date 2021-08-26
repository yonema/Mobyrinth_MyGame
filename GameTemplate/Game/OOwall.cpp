#include "stdafx.h"
#include "OOwall.h"
#include "ROrunning_stop.h"

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
			//「壁」の定数データを使用可能にする
			using namespace wallConstData;

			//コンストラクタ
			OOwall::OOwall()
			{
				//スタート関数では間に合わない。
				//オブジェクトが生成された瞬間に呼ばれてほしい処理

				//ウェイポイントからの上の距離を設定
				SetYPosLen(LENGHT_POSITION_FROM_STAGE);

				return;
			}

			//スタート関数
			bool OOwall::StartSub()
			{
				//初期化用関数
				Init(MODEL_FILEPATH_WALL, EN_OO_TYPE_WALL);

				//OBBのサイズを設定
				SetOBBDirectionLength(MODEL_SCALE_WALL);

				//OBBのタグを設定する
				SetOBBTag(nsOBB::COBB::EN_OO_TYPE_WALL);


				//移動前の初期位置と移動先の終端位置の初期化処理
				InitStartAndEndPos();

				//稼働、停止オブジェクトの初期化処理
				InitRun_stopObject();

				//サウンドの初期化処理
				InitSound();

				return true;
			}

			/**
			 * @brief 移動前の初期位置と移動先の終端位置の初期化処理
			*/
			void OOwall::InitStartAndEndPos()
			{
				//移動前の初期位置の設定
				m_startPosition = m_position;

				//アップベクトル
				Vector3 upVec = g_VEC3_UP;
				//現在の自身の回転で、アップベクトルを回す
				m_rotation.Apply(upVec);
				//アップベクトル
				upVec.Scale(LENGTH_MOVE * TIME_MOVE);
				//移動先の終端位置の設定
				m_endPosition = m_startPosition + upVec;

				return;
			}

			/**
			 * @brief 稼働、停止オブジェクトの初期化処理
			*/
			void OOwall::InitRun_stopObject()
			{
				//最初に入れていおく稼働、停止オブジェクトを
				//停止で生成する
				m_pRun_stop = NewGO<nsReversibleObject::ROrunning_stop>(nsCommonData::PRIORITY_FIRST);
				m_pRun_stop->SetPosition(m_position);
				m_pRun_stop->SetFrontOrBack(nsCommonData::EN_BACK);
				//全反転しないようにと、Tipsを表示しないようにする
				m_pRun_stop->SetLock(true);

				return;
			}

			/**
			 * @brief サウンドの初期化処理
			*/
			void OOwall::InitSound()
			{
				//壁が動くときのサウンドを生成する
				m_wallmoveSE = NewGO<nsSound::CSoundCue>(nsCommonData::PRIORITY_FIRST);
				//壁が動くときのサウンドを、waveファイルを指定して初期化する。
				m_wallmoveSE->Init(SOUND_FILEPATH_WALL_MOVE);
				//音量調節
				m_wallmoveSE->SetVolume(SOUND_VALUME_WALL_MOVE);

				return;
			}

			//デストラクト
			OOwall::~OOwall()
			{
				DeleteGO(m_wallmoveSE);

				//稼働、停止オブジェクトはILevelObjectBaseを継承しているから、ここで消す必要はない

				return;
			}

			//アップデート関数
			void OOwall::UpdateSub()
			{
				//最初のアップデートか？
				if (m_firstUpdateFlag)
					FirstUpdate();

				//稼働中か？
				if (m_moveFlag)
				{
					//座標の更新処理
					PositionUpdate();

					//サウンドの更新処理
					SEUpdate();
				}

				return;
			}

			/**
			 * @brief 座標の更新処理
			*/
			void OOwall::PositionUpdate()
			{
				//初期位置から終端位値へのベクトル
				Vector3 movePos = m_endPosition - m_startPosition;	//終端位置へのベクトル

				//移動速度の倍率
				float moveScale = 0.0f;

				//タイマーが片道分の時間より小さいか？
				if (m_moveTimer < TIME_MOVE)
				{
					//タイマーが片道分の時間より小さいとき

					//現在の時間 / 片道分の時間 で倍率を出す
					moveScale = m_moveTimer / TIME_MOVE;
				}
				//タイマーが往復分の時間より小さいか？
				else if (m_moveTimer < TIME_MOVE * 2)
				{
					//タイマーが片道分の時間より大きい、かつ、
					//往復分の時間より小さいとき

					//終端位置への残りのタイマーを出して、倍率を出す
					moveScale = (TIME_MOVE * 2.0f - m_moveTimer) / TIME_MOVE;
				}
				else
				{
					//タイマーが往復分の時間より大きい時

					//タイマーを初期化する
					m_moveTimer = 0.0f;
				}

				//移動先へのベクトルに、倍率を掛ける
				movePos.Scale(moveScale);

				//現在の場所を、初期位置から移動先へのベクトルを加算した場所にする
				m_position = m_startPosition + movePos;

				//カウンターを進める
				m_moveTimer += nsTimer::GameTime().GetFrameDeltaTime();

				return;
			}

			/**
			 * @brief サウンドの更新処理
			*/
			void OOwall::SEUpdate()
			{
				//プレイヤーから自身へのベクトル
				Vector3 fromPlayerVec = m_position - m_player->GetPosition();
				//プレイヤーからの距離
				const float distFromPlayer = fromPlayerVec.Length();

				//距離が最大距離より小さいか？
				if (distFromPlayer < DISTANCE_MAX_WALL_MOVE_SOUND)
				{
					//音量の倍率
					//distFromPlayerが小さくなるほど、倍率が～1.0fに近づく
					const float volumeRate =
						(DISTANCE_MAX_WALL_MOVE_SOUND - distFromPlayer) / DISTANCE_MAX_WALL_MOVE_SOUND;
					//音量計算
					const float volume = SOUND_VALUME_WALL_MOVE * volumeRate;


					//UFOmoveSEをループ再生をオンで再生する。
					m_wallmoveSE->Play(true);

					//音量調節
					m_wallmoveSE->SetVolume(volume);

				}
				else
				{
					//距離が最大距離より大きい

					//再生中か？
					if (m_wallmoveSE->IsPlaying())
					{
						//再生中なら停止する
						m_wallmoveSE->Stop();
					}
				}

				return;
			}

			/// <summary>
			/// 一回目のアップデートでだけ呼ばれる関数
			/// </summary>
			void OOwall::FirstUpdate()
			{
				//稼働、停止オブジェクトがクエリすると、
				//m_pRun_stopの中身のオブジェクトを消してしまうから、
				//いったんローカル変数のポインタに避難させておいて
				//m_pRun_stopにはnullptrを入れておく
				nsReversibleObject::ROrunning_stop* pRun_stop = m_pRun_stop;
				m_pRun_stop = nullptr;
				//クエリをして壁を探す
				pRun_stop->QuerySub();

				//一回目のアップデートの終了
				m_firstUpdateFlag = false;

				return;
			}
		}
	}
}