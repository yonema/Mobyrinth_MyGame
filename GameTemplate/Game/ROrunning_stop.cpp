#include "stdafx.h"
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
		 * @brief 反転オブジェクト
		*/
		namespace nsReversibleObject
		{

			//ReversibleObjectのモデルのファイルパスの定数データを使用可能にする
			using namespace ROsFilepathConstdata;

			//スタート関数
			bool ROrunning_stop::StartSub()
			{
				//初期化用関数
				Init(MODEL_FILEPATH_RUNNING, EN_RO_TYPE_RUNNING,
					MODEL_FILEPATH_STOP, EN_RO_TYPE_STOP);


				return true;
			}

			/// <summary>
			/// クエリしてほしいタイミングで呼ばれる関数
			/// </summary>
			void ROrunning_stop::QuerySub()
			{
				//障害オブジェクトの「壁」をクエリ
				QueryLOs<nsObstacleObject::OOwall>(EN_OO_TYPE_WALL, [&](nsObstacleObject::OOwall* wall) -> bool
					{
						//「壁」と衝突した時の処理
						HitWall(wall);

						return true;
					}
				);

				return;
			}

			/**
			 * @brief 「壁」と衝突した時の処理
			 * @param [in] wall 「壁」のポインタ
			*/
			void ROrunning_stop::HitWall(nsObstacleObject::OOwall* wall)
			{
				if (m_actionFlag)
				{
					//自身と「壁」が衝突したら
					if (IsHitLevelObject(*this, *wall))
					{
						//自身が「稼働」か？
						if (GetObjectType() == EN_RO_TYPE_RUNNING)
						{
							//「稼働」
							//壁を稼働モードにする
							wall->Running();
						}
						else
						{
							//「停止」
							//壁を停止モードにする
							wall->Stop();
						}

						//衝突した壁のアドレスをポインタに保持
						m_pWall = wall;
						//ロック中にする
						SetLock(true);
						//オブジェクトが重なっているかを判定する処理を動かさないようにする。
						SetOverlapFlag(false);

						//衝突した壁に、すでに中身があるかどうか調べる
						if (wall->GetRun_stop() && wall->GetRun_stop() != this)
						{
							//中身があったら、その中身を破棄する
							DeleteGO(wall->GetRun_stop());
						}
						//壁に自身のポインタを入れる
						wall->SetRun_stop(this);
					}
				}

				return;
			}

			//アップデート関数
			void ROrunning_stop::UpdateSub()
			{
				//壁のアドレスを保持していたら、場所と回転を更新する
				if (m_pWall)
				{
					//壁の場所からどのくらいの上の場所にいるか
					const float upVecScale = 100.0f;

					//壁の回転を得る
					Quaternion qRot = m_pWall->GetRotation();
					//アップベクトル
					Vector3 upVec = g_VEC3_UP;
					//アップベクトルに壁の回転を乗算する
					qRot.Apply(upVec);
					//正規化する
					upVec.Normalize();
					//イイ感じの大きさにスケールする
					upVec.Scale(upVecScale);

					//場所を更新
					SetPosition(m_pWall->GetPosition() + upVec);
					//回転を更新
					SetRotation(qRot);
				}

				return;
			}
		}
	}
}