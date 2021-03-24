#include "stdafx.h"
#include "ROrunning_stop.h"

bool ROrunning_stop::StartSub()
{
	Init("Assets/modelData/kadou.tkm", enRunning,
		"Assets/modelData/teishi.tkm", enStop);

	return true;
}

void ROrunning_stop::QuerySub()
{

	if (GetObjectType() == enRunning)
	{
		QueryLOs<OOwall>(enWall, [&](OOwall* wall) -> bool
			{
				if (IsHitObject(*this, *wall))
				{
					wall->Running();
					m_pWall = wall;
					if (wall->GetRun_stop())
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
					SetFlagCheckOverlap(false);
				}
				return true;
			}
		);
	}
	else if (GetObjectType() == enStop)
	{
		QueryLOs<OOwall>(enWall, [&](OOwall* wall) -> bool
			{
				if (IsHitObject(*this, *wall))
				{
					wall->Stop();
					m_pWall = wall;
					if (wall->GetRun_stop())
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
					SetFlagCheckOverlap(false);
				}
				return true;
			}
		);
	}
}

void ROrunning_stop::UpdateSub()
{
	if (m_pWall)
	{
		Quaternion qRot = m_pWall->GetRotation();
		Vector3 upVec = g_vec3Up;
		qRot.Apply(upVec);
		upVec.Normalize();
		upVec.Scale(100.0f);
		m_position = m_pWall->GetPosition() + upVec;
	}
}
