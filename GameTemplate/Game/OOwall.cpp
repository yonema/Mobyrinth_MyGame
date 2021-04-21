#include "stdafx.h"
#include "OOwall.h"
#include "ROrunning_stop.h"

//�X�^�[�g�֐�
bool OOwall::StartSub()
{
	//�������p�֐�
	Init("Assets/modelData/wall.tkm", enWall);

	//OBB�̃T�C�Y��ݒ�
	Vector3 obbSize;
	obbSize = { 400.0f,1000.0f,400.0f };
	GetOBB().SetDirectionLength(obbSize);

	//�ړ��O�̏����ʒu�̐ݒ�
	m_startPosition = m_position;

	//�ғ�����Г����̎���
	const int moveTime = 3.0f;
	//�ړ����鋗���̕␳
	const float moveLen = 200.0f;
	//�A�b�v�x�N�g��
	Vector3 upVec = g_vec3Up;
	//���݂̎��g�̉�]�ŁA�A�b�v�x�N�g������
	m_rotation.Apply(upVec);
	//�A�b�v�x�N�g��
	upVec.Scale(moveLen * moveTime);
	//�ړ���̏I�[�ʒu�̐ݒ�
	m_endPosition = m_startPosition + upVec;

	GetOBB().SetTag(COBB::enWall);

	return true;
}

//�A�b�v�f�[�g�֐�
void OOwall::UpdateSub()
{
	//�ғ������H
	if (m_moveFlag)
	{
		//�ғ�����Г����̎���
		const float moveTime = 3.0f;

		//�����ʒu����I�[�ʒl�ւ̃x�N�g��
		Vector3 movePos = m_endPosition - m_startPosition;	//�I�[�ʒu�ւ̃x�N�g��
		
		//���݂̎��� / �Г����̎��� �Ŕ{�����o��
		float moveScale = static_cast<float>(m_moveTimer) / moveTime;	//�ړ����x�̔{��

		//�J�E���^�[���Г����̎��Ԃ��z���Ă�����
		if (m_moveTimer >= moveTime)
		{
			//�I�[�ʒu�ւ̎c��̃J�E���^�[���o���āA�{�����o��
			moveScale = static_cast<float>(moveTime * 2 - m_moveTimer) / moveTime;

			//�J�E���^�[���������̎��Ԃ��z���Ă�����
			if (m_moveTimer >= moveTime * 2)
			{
				//�J�E���^�[��0�ɂ���
				m_moveTimer = 0;
			}
		}

		//�ړ���ւ̃x�N�g���ɁA�{�����|����
		movePos.Scale(moveScale);

		//���݂̏ꏊ���A�����ʒu����ړ���ւ̃x�N�g�������Z�����ꏊ�ɂ���
		m_position = m_startPosition + movePos;

		//�J�E���^�[��i�߂�
		m_moveTimer += GameTime().GetFrameDeltaTime();

	}

	return;
}