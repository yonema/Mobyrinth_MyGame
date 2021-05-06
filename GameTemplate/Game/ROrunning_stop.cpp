#include "stdafx.h"
#include "ROrunning_stop.h"

//�X�^�[�g�֐�
bool ROrunning_stop::StartSub()
{
	//�������p�֐�
	Init("Assets/modelData/running.tkm", enRunning,
		"Assets/modelData/stop.tkm", enStop);

	//OBB�̃T�C�Y��ݒ�
	Vector3 obbSize;
	obbSize = { 200.0f,200.0f,100.0f };
	//OBB�̕����x�N�g���̒�����ݒ�
	GetOBB().SetDirectionLength(obbSize);

	return true;
}

/// <summary>
/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
/// </summary>
void ROrunning_stop::QuerySub()
{
	//���g���u�ғ��v�̎�
	if (GetObjectType() == enRunning)
	{
		//��Q�I�u�W�F�N�g�́u�ǁv���N�G��
		QueryLOs<OOwall>(enWall, [&](OOwall* wall) -> bool
			{
				//���g�Ɓu�ǁv���Փ˂�����
				if (IsHitObject(*this, *wall))
				{
					//�ǂ��ғ����[�h�ɂ���
					wall->Running();
					//�Փ˂����ǂ̃A�h���X���|�C���^�ɕێ�
					m_pWall = wall;

					//�Փ˂����ǂɁA���łɒ��g�����邩�ǂ������ׂ�
					if (wall->GetRun_stop() && wall->GetRun_stop() != this)
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
					m_flagOverlap = false;
					//SetFlagOverlap(false);
				}
				return true;
			}
		);
	}
	//���g���u��~�v�̎�
	else if (GetObjectType() == enStop)
	{
		//��Q�I�u�W�F�N�g�́u�ǁv���N�G��
		QueryLOs<OOwall>(enWall, [&](OOwall* wall) -> bool
			{
				//���g�Ɓu�ǁv���Փ˂�����
				if (IsHitObject(*this, *wall))
				{
					//�ǂ��~���[�h�ɂ���
					wall->Stop();
					//�Փ˂����ǂ̃A�h���X���|�C���^�ɕێ�
					m_pWall = wall;

					//�Փ˂����ǂɁA���łɒ��g�����邩�ǂ������ׂ�
					if (wall->GetRun_stop() && wall->GetRun_stop() != this)
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
					m_flagOverlap = false;
					//SetFlagOverlap(false);
				}
				return true;
			}
		);
	}
}

//�A�b�v�f�[�g�֐�
void ROrunning_stop::UpdateSub()
{
	//�ǂ̃A�h���X��ێ����Ă�����A�ꏊ�Ɖ�]���X�V����
	if (m_pWall)
	{
		//�ǂ̏ꏊ����ǂ̂��炢�̏�̏ꏊ�ɂ��邩
		const float upVecScale = 100.0f;

		//�ǂ̉�]�𓾂�
		Quaternion qRot = m_pWall->GetRotation();
		//�A�b�v�x�N�g��
		Vector3 upVec = g_vec3Up;
		//�A�b�v�x�N�g���ɕǂ̉�]����Z����
		qRot.Apply(upVec);
		//���K������
		upVec.Normalize();
		//�C�C�����̑傫���ɃX�P�[������
		upVec.Scale(upVecScale);

		//�ꏊ���X�V
		SetPosition(m_pWall->GetPosition() + upVec);
		//��]���X�V
		SetRotation(qRot);
	}
}
