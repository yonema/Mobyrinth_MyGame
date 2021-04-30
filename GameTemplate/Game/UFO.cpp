#include "stdafx.h"
#include "UFO.h"

//�X�^�[�g�֐�
bool CUFO::PureVirtualStart()
{
	//���f�������_���[�̐����Ə�����
	m_modelRender = NewGO<CModelRender>(0);
	m_modelRender->Init("Assets/modelData/UFO.tkm");
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);

	//�A�b�v�x�N�g�������݂̉�]�ɍ��킹��
	m_rotation.Apply(m_upVec);
	//�E�F�C�|�C���g��̍��W���X�V����
	m_onWayPosition = m_position;

	//OBB�𒲐߂���
	GetOBB().SetPivot({ 0.5f,0.0f,0.5f });
	GetOBB().SetDirectionLength({ 250.0f,200.0f,400.0f });

	//�X�|�b�g���C�g�̐����Ə�����
	m_spotLight = NewGO<CSpotLight>(0);
	Vector3 up = m_upVec;
	up.Scale(100.0f);
	m_spotLight->SetPosition({ 0.0f,2000.0f, 0.0f });
	m_spotLight->SetColor({ 300.0f, 300.0f, 0.0f, 0.0f });
	m_spotLight->SetRange(500.0f);
	up.Normalize();
	up.Scale(-1.0f);
	m_spotLight->SetDirection(g_vec3Down);

	//UFO�̒��n�_�̐����Ə�����
	m_ufoLandingPoint = NewGO<CUFOLandingPoint>(0);
	m_ufoLandingPoint->SetPosition(m_position);


	//�f�o�b�N�p
	//��ŏ���
	Vector3* vertPos = GetOBB().GetBoxVertex();
	for (int i = 0; i < m_vertNum; i++)
	{
		//OBB�̒��_�����邽�߂̃��f��
		m_dbgVertPosMR[i] = NewGO<CModelRender>(0);
		m_dbgVertPosMR[i]->Init("Assets/modelData/dbgBox.tkm");
		m_dbgVertPosMR[i]->SetPosition(vertPos[i]);
		m_dbgVertPosMR[i]->SetRotation(m_rotation);

	}
	for (int i = 0; i < 2; i++)
	{
		//���C�̎n�_�ƏI�_�����邽�߂̃��f��
		m_dbgRay[i] = NewGO<CModelRender>(0);
		m_dbgRay[i]->Init("Assets/modelData/dbgBox.tkm");
	}
	//�f�o�b�N�p�����܂�

	return true;
}

//�f�X�g���N�^
CUFO::~CUFO()
{
	//���f�������_���[�̔j��
	DeleteGO(m_modelRender);

	//�X�|�b�g���C�g�̔j��
	DeleteGO(m_spotLight);

	//UFO�̒��n�_�̔j��
	DeleteGO(m_ufoLandingPoint);

	//�f�o�b�N�p
	//��ŏ���
	for (int i = 0; i < m_vertNum; i++)
	{
		//���_�����邽�߂̃��f���̔j��
		DeleteGO(m_dbgVertPosMR[i]);
	}
	for (int i = 0; i < 2; i++)
	{
		//���C�����邽�߂̃��f����j��
		DeleteGO(m_dbgRay[i]);
	}
	//�f�o�b�N�p�����܂�
}

//�A�b�v�f�[�g�֐�
void CUFO::PureVirtualUpdate()
{
	//�ړ�����
	Move();
	//��]�����݂̍��W�ɍ��킹�Ă���]�ɂ���
	CheckRotation();
	//�A�b�v�x�N�g������]�ɍ��킹��
	m_upVec = g_vec3Up;
	m_rotation.Apply(m_upVec);
	m_upVec.Normalize();
	//�X�e�[�W�̏�ɏ��
	GetOnStage();

	//�A�b�v�f�[�g�̃X�e�[�g�ōs��������U��
	switch (m_updateState)
	{
	case enSearch:
		//�v���C���[��T������
		Search();
		break;
	case enCapture:
		//�v���C���[�����������̏���
		Capture();
		break;
	case enTransport:
		//�v���C���[���^�ԏ���
		Transport();
		break;
	case enLanding:
		//���n�̏���
		Landing();
		break;
	case enLeave:
		//���n�_���痣��鏈��
		Leave();
		break;
	}

	//���C�g�̍X�V
	UpdateLight();

	//���f�������_���[�̍X�V
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);

	//�f�o�b�N�p
	//��ŏ���
	Vector3* vertPos = GetOBB().GetBoxVertex();
	for (int i = 0; i < m_vertNum; i++)
	{
		//���_�����邽�߂̃��f���̍X�V
		m_dbgVertPosMR[i]->SetPosition(vertPos[i]);
		m_dbgVertPosMR[i]->SetRotation(m_rotation);

	}
	//�f�o�b�N�p�����܂�
}

//�v���C���[��T������
void CUFO::Search()
{
	//�X�s�[�h��0.0f�Ȃ�T���Ȃ�
	if (m_moveSpeed == 0.0f)
		return;


	//�^�C�}�[�Ƀf���^�^�C�������Z
	m_timer += GameTime().GetFrameDeltaTime();

	//�{���t���O�̐؂�ւ��Ǝ���
	const float switchingTime = 1.5f;

	//�؂�ւ����ԂɂȂ�����
	if (m_timer >= switchingTime)
	{
		//�t���O��؂荇����
		m_searchFlag = !m_searchFlag;
		//�^�C�}�[��0�ɖ߂�
		m_timer = 0.0f;
	}

	//�T�������H
	if (m_searchFlag)
	{
		//�{�����Ȃ�

		//���F�Ɍ���
		m_modelRender->SetEmissionColor({ 0.5f,0.5f,0.0f,1.0f });
		m_spotLight->SetColor({ 300.0f,300.0f,0.0f,1.0f });

		//�v���C���[���Փ˂��Ă��邩�H
		if (IsHitPlayer())
		{
			//�v���C���[��������

			//�A�b�v�f�[�g�X�e�[�g���v���C���[����������Ԃɂ���
			m_updateState = enCapture;

			//�ԐF�Ɍ���
			m_modelRender->SetEmissionColor({ 1.0f,0.0f,0.0f,1.0f });
			m_spotLight->SetColor({ 600.0f,0.0f,0.0f,1.0f });
			
			//�v���C���[��UFO�ɕ߂܂�����Ԃɂ���
			m_pPlayer->SetCapturedUFOFlag(true);
			m_pPlayer->SetRotation(m_rotation);
			//�v���C���[�����������Ă�����A��������
			if (m_pPlayer->GetHoldObject())
				m_pPlayer->GetReversibleObject()->StateToCancel();
			//�X�s�[�h��0�ɂ���
			m_moveSpeed = 0.0f;
			//�^�C�}�[������������
			m_timer = 0.0f;

		}
	}
	else
	{
		//�{�����ł͂Ȃ�

		//����Ȃ�
		m_modelRender->SetEmissionColor({ 0.0f,0.0f,0.0f,1.0f });
		m_spotLight->SetColor({ 0.0f,0.0f,0.0f,1.0f });

	}
}

//�v���C���[�����������̏���
void CUFO::Capture()
{
	//�A�b�v�x�N�g���𓾂�
	Vector3 upVec = m_upVec;
	//�C�C�����ɐL�΂�
	const float upVecLen = 500.0f;
	upVec.Scale(upVecLen);

	//UFO�ɕ߂܂�ʒu
	Vector3 capturePos = m_position + upVec;

	//�v���C���[�ɉ�����x�N�g��
	Vector3 addVec = capturePos - m_pPlayer->GetPosition();
	//�f���^�^�C�����|���Ă���
	addVec.Scale(GameTime().GetFrameDeltaTime());
	//�v���C���[�̍��W��ݒ�
	m_pPlayer->SetPosition(m_pPlayer->GetPosition() + addVec);
	m_pPlayer->SetRotation(m_rotation);

	//�^�C�}�[�̐؂�ւ�����
	const float switchingTimer = 2.0f;
	if (m_timer >= switchingTimer)
	{
		//�^�C�}�[���؂�ւ����ԂɂȂ�����
		//�^�C�}�[������������
		m_timer = 0.0f;
		//�A�b�v�f�[�g�X�e�[�g���v���C���[���^�ԏ�����Ԃɂ���
		m_updateState = enTransport;
	}
	else
		//�܂��Ȃ�^�C�}�[��i�߂�
		m_timer += GameTime().GetFrameDeltaTime();

}

//�v���C���[���^�ԏ���
void CUFO::Transport()
{
	//�^�C�}�[��0.0f�̎���
	//�܂�ŏ��̈�񂾂��Ă΂��
	if (m_timer == 0.0f)
	{
		//�E�F�C�|�C���g�̍ő�l
		const int maxWayPoint = 31;

		//���݂̍����̃E�F�C�|�C���g�̔��Α��̃E�F�C�|�C���g���o���B
		//0���܂ނ��ߍő�l��1���Z���Ă���A���Α����o�����ߔ����ɂ��āA
		//���݂̍����̃E�F�C�|�C���g�ɂ��̒l�����Z���Ă���B
		int reverseLp = GetLeftWayPointIndex() + (maxWayPoint + 1) / 2;

		//�E���������čs���̂���������s���̂����߂�
		if (GetLeftWayPointIndex() == m_ufoLandingPoint->GetLeftWayPointIndex())
		{
			//���g�̍����̃E�F�C�|�C���g�ƁA���n�_�̍����̃E�F�C�|�C���g�������Ȃ�

			//�E�̒P�ʕ����x�N�g��
			Vector3 rightVec = g_vec3Right;
			//���g�̉�]���|����
			m_rotation.Apply(rightVec);
			//���g�iUFO�j���璅�n�_�ւ̃x�N�g��
			Vector3 UFOToLandPointVec = m_ufoLandingPoint->GetPosition() - m_position;
			//���K������
			UFOToLandPointVec.Normalize();
			//���n�_�ւ̃x�N�g���ƉE�̃x�N�g���̓��ς����
			float landVecDotRightVec = Dot(UFOToLandPointVec, rightVec);
			//���ς̐��������ō��E��U�蕪����
			if (landVecDotRightVec >= 0.0f)
				//���ɐi��
				m_leftOrRight = enLeft;
			else
				//�E�ɐi��
				m_leftOrRight = enRight;
		}
		//���Α��̃E�F�C�|�C���g���ő�l�𒴂��Ă��邩�H
		else if (reverseLp > maxWayPoint)
		{
			//�����Ă��鎞

			//�ő�l+1�i0���܂ނ��߁j�����Z����
			reverseLp -= ( maxWayPoint + 1 );

			//���n�_�̈ʒu�𒲂ׂāA�E����s����������s������U�蕪����
			if (GetLeftWayPointIndex() < m_ufoLandingPoint->GetLeftWayPointIndex() ||
				m_ufoLandingPoint->GetLeftWayPointIndex() <= reverseLp)
				//���n�_���A���݂̃E�F�C�|�C���g�����傫����
				//���Α��̃E�F�C�|�C���g�ȉ���������
				//���ɐi��
				m_leftOrRight = enLeft;
			else
				//����ȊO�Ȃ�
				//�E�ɐi��
				m_leftOrRight = enRight;
		}
		else
		{
			//�����Ă��Ȃ���

			//���n�_�̈ʒu�𒲂ׂāA�E����s����������s������U�蕪����
			if (m_ufoLandingPoint->GetLeftWayPointIndex() <= reverseLp)
			{
				//���n�_�����Α��̃E�F�C�|�C���g�ȉ���������
				//���ɐi��
				m_leftOrRight = enLeft;
			}
			else
			{
				//����ȊO�Ȃ�
				//�E�ɐi��
				m_leftOrRight = enRight;
			}
		}

		//�ړ����x���f�t�H���g�̑��x�ɂ���
		SetMoveSpeed();
	}

	//�A�b�v�x�N�g���𓾂�
	Vector3 upVec = m_upVec;
	//�C�C�����ɐL�΂�
	const float upVecLen = 500.0f;
	upVec.Scale(upVecLen);

	//UFO�ɕ߂܂�ʒu
	Vector3 capturePos = m_position + upVec;

	//���g��OBB�ƒ��n�_��OBB���Փ˂��Ă��邩�H
	if (IsHitObject(*this, *m_ufoLandingPoint))
	{
		//�Փ˂�����

		//�A�b�v�f�[�g�X�e�[�g�𒅒n��Ԃɂ���
		m_updateState = enLanding;
		//�^�C�}�[������������
		m_timer = 0.0f;
	}
	else
		//�Փ˂��Ă��Ȃ�������
		//�^�C�}�[��i�߂�
		m_timer += GameTime().GetFrameDeltaTime();

	//�v���C���[�̏����X�V����
	m_pPlayer->SetPosition(capturePos);
	m_pPlayer->SetRotation(m_rotation);
	m_pPlayer->SetLeftPointIndex(GetLeftWayPointIndex());
	m_pPlayer->SetRightPointIndex(GetRightWayPointIndex());
}

//���n�̏���
void CUFO::Landing()
{
	//���ɉ����鎞��
	const float goDownTimer = 1.0f;
	//��ɏオ�鎞��
	const float goUpTimer = goDownTimer * 2.0f;

	//�A�b�v�x�N�g���𓾂�
	Vector3 upVec = m_upVec;
	//�C�C�����ɐL�΂�
	const float upVecLen = 500.0f;
	upVec.Scale(upVecLen);
	//UFO�ɕ߂܂�ʒu
	Vector3 capturePos = m_position + upVec;
	//UFO�ɕ߂܂�ʒu�ƒ��n�_�̏�����̊Ԃ̃x�N�g��
	Vector3 dist = (m_ufoLandingPoint->GetPosition() + upVec);
	dist -= capturePos;
	//�������o��
	float distLen = dist.Length();
	//�����̍ő�l
	const float maxDistLen = 1000.0f;

	//�^�C�}�[��0.0f���H
	//���g��OBB�ƒ��n�_�̒[��OBB���Փ˂��Ă��邩�H
	if (!CollisionOBBs(GetOBB(), m_ufoLandingPoint->GetSideOBB(m_leftOrRight)) && 
		m_timer == 0.0f)
	{
		//0.0f����
		//�Փ˂��Ă��Ȃ���

		//�ړ����x���f�t�H���g�ɂ��Ă���
		SetMoveSpeed();
		//�������߂��قǒx������
		m_moveSpeed *= distLen / maxDistLen;
	}
	else 
	{
		//0.0f�łȂ��A�܂��́A
		//�Փ˂��Ă��鎞

		//�^�C�}�[��i�߂�
		m_timer += GameTime().GetFrameDeltaTime();

		//�㉺�ɓ����x�N�g���̑傫��
		const float vecScale = 400.0f;

		//�^�C�}�[�̐i��𒲂ׂ�
		if (m_timer <= goDownTimer)
		{
			//���ɉ����鎞�Ԉȉ��Ȃ�

			//�X�e�[�W�̏�ɏ�鏈�����s��Ȃ��悤�ɂ���
			m_getOnStageFlag = false;
			//�ړ����x��0�ɂ���
			m_moveSpeed = 0.0f;
			//���ɉ�����x�N�g��
			Vector3 goDownVec = m_upVec;
			goDownVec.Scale(-vecScale);
			//�f���^�^�C�����|���Ă���
			goDownVec.Scale(GameTime().GetFrameDeltaTime());
			//�f���^�^�C�����|���Ă���
			m_position += goDownVec;
		}
		else if (m_timer <= goUpTimer)
		{
			//���ɉ����鎞�Ԃ��傫���A���A
			//��ɏオ�鎞�Ԉȉ��Ȃ�

			//��ɏオ��x�N�g��
			Vector3 goUpVec = m_upVec;
			goUpVec.Scale(vecScale);
			//�f���^�^�C�����|���Ă���
			goUpVec.Scale(GameTime().GetFrameDeltaTime());
			//���W����ɏグ��
			m_position += goUpVec;
		}
		else
		{
			//�㉺�ɓ������Ԃ��I�������

			//�A�b�v�f�[�g�X�e�[�g�𒅒n�_���痣��鏈����Ԃɂ���
			m_updateState = enLeave;
		}
	}

	if (m_timer <= goDownTimer)
	{
		//�^�C�}�[�����ɉ����鎞�Ԉȉ��̂Ƃ�����
		//�v���C���[�̃p�����[�^�[���X�V����
		m_pPlayer->SetPosition(capturePos);
		m_pPlayer->SetRotation(m_rotation);
		m_pPlayer->SetWayPointState(GetLeftWayPointIndex());
		m_pPlayer->SetLeftPointIndex(GetLeftWayPointIndex());
		m_pPlayer->SetRightPointIndex(GetRightWayPointIndex());
	}
	else if (m_updateState == enLeave)
	{
		//�A�b�v�f�[�g�p�����[�^��enLeave��ԂȂ�

		//�^�C�}�[��0�ɂ���
		m_timer = 0.0f;

	}

}

//���n�_���痣��鏈��
void CUFO::Leave()
{
	//�i�ތ������E�����ɖ߂��B
	m_leftOrRight = enRight;

	//�^�C�}�[��0.0f�̎���
	//�܂�ŏ��̈�񂾂��Ă΂��
	if (m_timer == 0.0f)
	{
		//�E�F�C�|�C���g��̍��W�ɍ���
		m_onWayPosition = m_position;

		//�X�e�[�W�̏�ɏ�鏈�����s���悤�ɂ���
		m_getOnStageFlag = true;

		//�E�F�C�|�C���g�̍ő吔
		const int maxWayPointNum = 31;
		//���ɐi�ރE�F�C�|�C���g
		int nextIndex = m_ufoLandingPoint->GetLeftWayPointIndex();
		if (m_leftOrRight == enLeft)
		{
			//���ɓ�������A���ɉ���������Ȃ�
		}
		else
		{
			//�E�ɓ������̂ŁA���̃E�F�C�|�C���g�����Z����

			nextIndex--;
			//0��菬����������
			if (nextIndex < 0)
				//������čő�l�ɂ���
				nextIndex = maxWayPointNum;
		}
		//���g�̍����̃E�F�C�|�C���g���X�V����
		SetLeftWayPointIndex(nextIndex);
	}

	//�ړ����x���f�t�H���g�̑��x�ɂ���
	SetMoveSpeed();
	//�؂�ւ��^�C��
	const float switchingTime = 2.0f;

	//�^�C�}�[�̒��ׂ�
	if (m_timer >= switchingTime)
	{
		//�؂�ւ����Ԉȏ�ɂȂ�����

		//�A�b�v�f�[�g�X�e�[�g���v���C���[��T��������Ԃɂ���
		m_updateState = enSearch;
		//�^�C�}�[������������
		m_timer = 0.0f;
		//�v���C���[��UFO�ɕ߂܂��Ă��Ȃ���Ԃɂ���
		m_pPlayer->SetCapturedUFOFlag(false);
	}
	else
		//�؂�ւ����Ԃ�菬����������
		//�^�C�}�[��i�߂�
		m_timer += GameTime().GetFrameDeltaTime();

}

//�ړ�����
void CUFO::Move()
{
	//���̃E�F�C�|�C���g
	int nextIndex = GetRightWayPointIndex();
	//�ŏI�I�Ȉړ����x	//�ړ����x�Ƀf���^�^�C�����|����
	float finalMoveSpeed = m_moveSpeed * GameTime().GetFrameDeltaTime();
	//�E�F�C�|�C���g��̎��̍��W���v�Z����
	m_onWayPosition = CLevelObjectManager::GetInstance()->CalcWayPointNextPos
	(GetRightWayPointIndex(), m_onWayPosition, finalMoveSpeed, m_leftOrRight, &nextIndex);

	//�E�F�C�|�C���g�̍ő吔
	const int maxWayPointNum = 31;
	if (m_leftOrRight == enLeft)
	{
		//���ɓ�������A���ɉ���������Ȃ�
	}
	else
	{
		//�E�ɓ������̂ŁA���̃E�F�C�|�C���g�����Z����

		nextIndex++;
		//�E�F�C�|�C���g�̍ő吔���傫��������
		if (nextIndex > maxWayPointNum)
			//�������0�ɂ���
			nextIndex = 0;
	}
	//���g�̍����̃E�F�C�|�C���g���X�V����
	SetLeftWayPointIndex(nextIndex);
}

//���r�E�X�̗ւ̃X�e�[�W�ɃC�C�����ɍ��킹�鏈��
void CUFO::GetOnStage()
{
	if (!m_getOnStageFlag)
		return;
	//Up�x�N�g�����C�C�����ɐL�΂��B
	//�L�΂����悪���C�̎n�_�ƂȂ�
	Vector3 upVec = g_vec3Up;
	m_rotation.Apply(upVec);
	upVec.Scale(500.0f);

	//�f�o�b�N�p
	//��ŏ���
	m_dbgRay[0]->SetPosition(m_onWayPosition + upVec);
	m_dbgRay[1]->SetPosition(m_onWayPosition - upVec);
	//�f�o�b�N�p�����܂�
	if (m_mobius)
	{
		//���r�E�X�̗ւ��������Ă�����
		//���r�E�X�̗ւ̃��f���̃|���S���ƁA���C�̓����蔻������
		if (m_mobius->GetModel()
			->InIntersectLine(m_onWayPosition + upVec, m_onWayPosition - upVec))
		{
			//�|���S���ƃ��C�̌����_��������Ă��ă��f���̍��W�ɓ����
			m_position = m_mobius->GetModel()->GetIntersectPos();

		}
		else
		{
			m_position = m_onWayPosition;
		}
	}
	else
	{
		//���r�E�X�̗ցi�X�e�[�W�j���������Ă��Ȃ�������
		//�T����return����
		m_mobius = FindGO<Mobius>("Mobius");
		return;
	}
}

//���C�g�̍X�V
void CUFO::UpdateLight()
{
	//���C�g�̍X�V
	Vector3 upVec = m_upVec;
	upVec.Scale(300.0f);
	//�Ƃ炷�ʒu
	m_spotLight->SetPosition(m_position + upVec);
	Vector3 downVec = m_upVec;
	downVec.Scale(-1.0f);
	//�Ƃ炷����
	m_spotLight->SetDirection(downVec);
}