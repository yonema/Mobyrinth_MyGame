#include "stdafx.h"
#include "ROrunning_stop.h"

//スタート関数
bool ROrunning_stop::StartSub()
{
	//初期化用関数
	Init("Assets/modelData/running.tkm", enRunning,
		"Assets/modelData/stop.tkm", enStop);

	//OBBのサイズを設定
	Vector3 obbSize;
	obbSize = { 200.0f,200.0f,100.0f };
	//OBBの方向ベクトルの長さを設定
	GetOBB().SetDirectionLength(obbSize);

	return true;
}

/// <summary>
/// クエリしてほしいタイミングで呼ばれる関数
/// </summary>
void ROrunning_stop::QuerySub()
{
	//自身が「稼働」の時
	if (GetObjectType() == enRunning)
	{
		//障害オブジェクトの「壁」をクエリ
		QueryLOs<OOwall>(enWall, [&](OOwall* wall) -> bool
			{
				//自身と「壁」が衝突したら
				if (IsHitObject(*this, *wall))
				{
					//壁を稼働モードにする
					wall->Running();
					//衝突した壁のアドレスをポインタに保持
					m_pWall = wall;

					//衝突した壁に、すでに中身があるかどうか調べる
					if (wall->GetRun_stop() && wall->GetRun_stop() != this)
					{
						//中身があったら、その中身を消してから
						//自身を入れる
						DeleteGO(wall->GetRun_stop());
						wall->SetRun_stop(this);
					}
					else
					{
						//中身がなかったら、
						//そのまま自身を入れる
						wall->SetRun_stop(this);
					}
					//オブジェクトが重なっているかを判定する処理を動かさないようにする。
					m_flagOverlap = false;
					//SetFlagOverlap(false);
				}
				return true;
			}
		);
	}
	//自身が「停止」の時
	else if (GetObjectType() == enStop)
	{
		//障害オブジェクトの「壁」をクエリ
		QueryLOs<OOwall>(enWall, [&](OOwall* wall) -> bool
			{
				//自身と「壁」が衝突したら
				if (IsHitObject(*this, *wall))
				{
					//壁を停止モードにする
					wall->Stop();
					//衝突した壁のアドレスをポインタに保持
					m_pWall = wall;

					//衝突した壁に、すでに中身があるかどうか調べる
					if (wall->GetRun_stop() && wall->GetRun_stop() != this)
					{
						//中身があったら、その中身を消してから
						//自身を入れる
						DeleteGO(wall->GetRun_stop());
						wall->SetRun_stop(this);
					}
					else
					{
						//中身がなかったら、
						//そのまま自身を入れる
						wall->SetRun_stop(this);
					}
					//オブジェクトが重なっているかを判定する処理を動かさないようにする。
					m_flagOverlap = false;
					//SetFlagOverlap(false);
				}
				return true;
			}
		);
	}
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
		Vector3 upVec = g_vec3Up;
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
}
