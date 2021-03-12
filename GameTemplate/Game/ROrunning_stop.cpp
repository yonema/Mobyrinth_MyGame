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
	const float hitDot = 0.005f;

	if (GetObjectType() == enRunning)
	{
		QueryLOs<OOwall>(enWall, [&](OOwall* wall) -> bool
			{
				if (IsHitObject(*this, *wall, hitDot))
				{
					wall->Running();
					m_pWall = wall;
					if (wall->GetRun_stop())
					{
						//���g����������A���̒��g�������Ă���
						//���g������
						DeleteGO(wall->GetRun_stop());
						wall->SetRun_stop(this);
					}
					else
					{
						//���g���Ȃ�������A
						//���̂܂܎��M������
						wall->SetRun_stop(this);
					}
				}
				return true;
			}
		);
	}
	else if (GetObjectType() == enStop)
	{
		QueryLOs<OOwall>(enWall, [&](OOwall* wall) -> bool
			{
				if (IsHitObject(*this, *wall, hitDot))
				{
					wall->Stop();
					wall->Running();
					m_pWall = wall;
					if (wall->GetRun_stop())
					{
						//���g����������A���̒��g�������Ă���
						//���g������
						DeleteGO(wall->GetRun_stop());
						wall->SetRun_stop(this);
					}
					else
					{
						//���g���Ȃ�������A
						//���̂܂܎��M������
						wall->SetRun_stop(this);
					}
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
