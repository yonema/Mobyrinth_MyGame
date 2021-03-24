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
						//���g����������A���̒��g�������Ă���
						//���g������
						DeleteGO(wall->GetRun_stop());
						wall->SetRun_stop(this);
					}
					else
					{
						//���g���Ȃ�������A
						//���̂܂܎��g������
						wall->SetRun_stop(this);
					}
					//�I�u�W�F�N�g���d�Ȃ��Ă��邩�𔻒肷�鏈���𓮂����Ȃ��悤�ɂ���B
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
						//���g����������A���̒��g�������Ă���
						//���g������
						DeleteGO(wall->GetRun_stop());
						wall->SetRun_stop(this);
					}
					else
					{
						//���g���Ȃ�������A
						//���̂܂܎��g������
						wall->SetRun_stop(this);
					}
					//�I�u�W�F�N�g���d�Ȃ��Ă��邩�𔻒肷�鏈���𓮂����Ȃ��悤�ɂ���B
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
