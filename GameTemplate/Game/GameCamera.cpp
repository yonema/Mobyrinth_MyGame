#include "stdafx.h"
#include "GameCamera.h"

//�Q�[���J�����̒萔�f�[�^���g����悤�ɂ���
using namespace gameCameraConstData;

//�X�^�[�g�֐�
bool CGameCamera::Start()
{
	//�v���C���[��T��
	m_player = FindGO<Player>(GetGameObjectName(EN_GO_TYPE_PLAYER));

	//�J�n���o�N���X�ɃQ�[���J�����i���g�j��ݒ肷��
	m_startDirecting.SetGameCamera(this);

	//�΂˃J�����̏�����
	m_springCamera.Init(
		*g_camera3D, SPEED_MOVE_MAX_SPRINGCAMERA, false, RADIUS_COLLISION_SPHERE_SPRINGCAMERA
	);

	//�ߕ��ʁA�����ʂ̐ݒ�
	m_springCamera.SetNear(CAMERA_PLANE_NEAR);
	m_springCamera.SetFar(CAMERA_PLANE_PAR);

	//�A�b�v�x�N�g����ݒ�
	m_springCamera.SetUp(g_VEC3_AXIS_Y);

	//�J�����ւ̍��W
	Vector3 toCameraPos = POSITION_TO_CAMERA;

	//�^�C�g����ʂ��H
	if (m_titleMode)
	{
		//�^�C�g�����
		//�J�n���o���s��Ȃ��悤�ɂ���
		m_startDirecting.SetStartDirectingFlag(false);
	}
	else
	{	
		//�^�C�g����ʂł͂Ȃ��B�Q�[����ʁB
		//�J�n���o���s���悤�ɂ���
		m_startDirecting.SetStartDirectingFlag(true);
		//�J�n���o���̃J�����ւ̍��W�ɂ���
		toCameraPos = startDirectingConstData::POSITION_TO_CAMERA;
	}

	//�J�����̎��_�̒����_��ݒ肷��
	m_springCamera.SetPosition(toCameraPos);
	m_springCamera.SetTarget(startDirectingConstData::POSITION_TARGET_CAMERA);

	return true;
}

//�A�b�v�f�[�g�֐�
void CGameCamera::Update()
{
	//�J�n���o�����H
	if (m_startDirecting.GetStartDirectingFlag() == true) 
	{
		m_startDirecting.StartDirectingUpdate();
	}
	//�Q�[����
	else 
	{
		InGameCamera();
	}

	//�΂˃J�����̍X�V
	m_springCamera.Update();

	return;
}

/**
 * @brief �Q�[�����̃J����
*/
void CGameCamera::InGameCamera()
{
	//�v���C���[�����邩�H
	if (m_lookPlayerFlag)
	{
		//����

		//�v���C���[���������Ă��邩�H
		if (m_player)
		{
			//�v���C���[���������Ă�����

			//�����_���王�_�ւ̃x�N�g����ݒ肷��
			Vector3 toCameraPos = POSITION_TO_CAMERA;
			//�v���C���[�̉�]�𓾂�
			const Quaternion qRot = m_player->GetFinalWPRot();
			//�J�����ւ̃x�N�g������
			qRot.Apply(toCameraPos);
			//�A�b�v�x�N�g��
			Vector3 cameraUpVec = g_VEC3_AXIS_Y;
			//�A�b�v�x�N�g������
			qRot.Apply(cameraUpVec);

			//�J�����̃v���C���[����̍���
			Vector3 cameraHeightFromPlayer = cameraUpVec;

			//�^�C�g����ʂ��H
			if (m_titleMode)
			{
				//�^�C�g�����
				//������L�΂�
				cameraHeightFromPlayer.Scale(HEIGHT_FROM_PLAYER_CAMERA);
			}
			else
			{
				//�Q�[�����
				//�������Ȃ���
				cameraHeightFromPlayer = g_VEC3_ZERO;
			}

			//���_��ݒ肷��
			m_springCamera.
				SetPosition(m_player->GetPosition() + toCameraPos + cameraHeightFromPlayer);
			//�����_��ݒ肷��
			m_springCamera.SetTarget(m_player->GetPosition() + cameraHeightFromPlayer);
			//�A�b�v����ݒ肷��
			m_springCamera.SetUp(cameraUpVec);
		}
		else
		{
			//�v���C���[���������Ă��Ȃ�������
			//�v���C���[��T��
			m_player = FindGO<Player>(GetGameObjectName(EN_GO_TYPE_PLAYER));
		}
	}

	return;
}