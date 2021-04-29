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
	GetOBB().SetDirectionLength({ 400.0f,200.0f,400.0f });

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


	//CModelRender* model = NewGO<CModelRender>(0);
	//model->Init("Assets/modelData/fire.tkm");
	//model->SetPosition({ 0.0f,1600.0f,0.0f });
	//Quaternion qRot;
	//qRot.SetRotationDegX(-20.0f);
	//model->SetScale({ 3.0f,3.0f,3.0f });
	//model->SetRotation(qRot);

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
		DeleteGO(m_dbgVertPosMR[i]);
	}
	for (int i = 0; i < 2; i++)
	{
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
		Landing();
		break;
	case enLeave:
		Leave();
		break;
	}

	//���C�g�̍X�V
	Vector3 upVec = m_upVec;
	upVec.Scale(300.0f);
	m_spotLight->SetPosition(m_position + upVec);
	Vector3 downVec = m_upVec;
	downVec.Scale(-1.0f);
	m_spotLight->SetDirection(downVec);

	//���f�������_���[�̍X�V
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);

	//�f�o�b�N�p
	//��ŏ���
	Vector3* vertPos = GetOBB().GetBoxVertex();
	for (int i = 0; i < m_vertNum; i++)
	{
		m_dbgVertPosMR[i]->SetPosition(vertPos[i]);
		m_dbgVertPosMR[i]->SetRotation(m_rotation);

	}
	//�f�o�b�N�p�����܂�
}

//�v���C���[��T������
void CUFO::Search()
{
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

	
	if (m_searchFlag)
	{
		//�{�����Ȃ�
		m_modelRender->SetEmissionColor({ 0.5f,0.5f,0.0f,1.0f });
		m_spotLight->SetColor({ 300.0f,300.0f,0.0f,1.0f });
		//�v���C���[���Փ˂��Ă��邩�H
		if (IsHitPlayer())
		{
			//�v���C���[��������
			m_modelRender->SetEmissionColor({ 1.0f,0.0f,0.0f,1.0f });
			m_spotLight->SetColor({ 600.0f,0.0f,0.0f,1.0f });
			m_updateState = enCapture;
			m_pPlayer->SetCapturedUFOFlag(true);
			m_pPlayer->SetRotation(m_rotation);
			if (m_pPlayer->GetHoldObject())
				m_pPlayer->GetReversibleObject()->StateToCancel();
			m_moveSpeed = 0.0f;
			m_timer = 0.0f;

		}
	}
	else
	{
		//�{�����ł͂Ȃ�
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
	if (m_timer >= 2.0f)
	{
		//�^�C�}�[���؂�ւ����ԂɂȂ�����
		//�^�C�}�[������������
		m_timer = 0.0f;
		//�v���C���[���^�ԏ�����
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
			//���n�_�ւ�
			float landVecDotRightVec = Dot(UFOToLandPointVec, rightVec);
			if (landVecDotRightVec >= 0.0f)
				m_leftOrRight = enLeft;
			else
				m_leftOrRight = enRight;
		}
		else if (reverseLp > 31)
		{
			reverseLp -= 32;
			if (GetLeftWayPointIndex() < m_ufoLandingPoint->GetLeftWayPointIndex() ||
				m_ufoLandingPoint->GetLeftWayPointIndex() <= reverseLp)
				m_leftOrRight = enLeft;
			else
				m_leftOrRight = enRight;
		}
		else
		{
			if (m_ufoLandingPoint->GetLeftWayPointIndex() <= reverseLp)
			{
				m_leftOrRight = enLeft;
			}
			else
			{
				m_leftOrRight = enRight;
			}
		}


		m_moveSpeed = m_defaultSpeed;
	}

	//�A�b�v�x�N�g���𓾂�
	Vector3 upVec = m_upVec;
	//�C�C�����ɐL�΂�
	const float upVecLen = 500.0f;
	upVec.Scale(upVecLen);

	//UFO�ɕ߂܂�ʒu
	Vector3 capturePos = m_position + upVec;

	if (IsHitObject(*this, *m_ufoLandingPoint))
	{
		m_updateState = enLanding;
		m_timer = 0.0f;
	}
	else
		m_timer += GameTime().GetFrameDeltaTime();

	m_pPlayer->SetPosition(capturePos);
	m_pPlayer->SetRotation(m_rotation);
	m_pPlayer->SetLeftPointIndex(GetLeftWayPointIndex());
	m_pPlayer->SetRightPointIndex(GetRightWayPointIndex());
}

void CUFO::Landing()
{
	//m_ufoLandingPoint->UpdateSideOBB();
	//m_timer += GameTime().GetFrameDeltaTime();
	const float goDownTimer = 1.0f;
	const float goUpTimer = goDownTimer * 2.0f;

	//�A�b�v�x�N�g���𓾂�
	Vector3 upVec = m_upVec;
	//�C�C�����ɐL�΂�
	const float upVecLen = 500.0f;
	upVec.Scale(upVecLen);
	//UFO�ɕ߂܂�ʒu
	Vector3 capturePos = m_position + upVec;

	Vector3 dist = (m_ufoLandingPoint->GetPosition() + upVec);
	dist -= capturePos;
	float distLen = dist.Length();
	const float maxDistLen = 1000.0f;
	const float minDistlen = 100.0f;
	Vector3 landUpVec = g_vec3Up;
	m_ufoLandingPoint->GetRotation().Apply(landUpVec);
	float upDotLandUp = Dot(m_upVec, landUpVec);
	float threshold = 0.0001f;

	if (!CollisionOBBs(GetOBB(), m_ufoLandingPoint->GetSideOBB(m_leftOrRight)) && 
		m_timer == 0.0f)
	{
		m_moveSpeed = m_defaultSpeed;
		m_moveSpeed *= distLen / maxDistLen;
	}
	else 
	{
		m_timer += GameTime().GetFrameDeltaTime();
		const float vecScale = 400.0f;
		if (m_timer <= goDownTimer)
		{
			m_getOnStageFlag = false;
			m_moveSpeed = 0.0f;
			Vector3 goDownVec = m_upVec;
			goDownVec.Scale(-vecScale);
			goDownVec.Scale(GameTime().GetFrameDeltaTime());
			m_position += goDownVec;
		}
		else if (m_timer <= goUpTimer)
		{
			Vector3 goUpVec = m_upVec;
			goUpVec.Scale(vecScale);
			goUpVec.Scale(GameTime().GetFrameDeltaTime());
			m_position += goUpVec;
		}
		else
		{
			m_updateState = enLeave;
		}
	}

	if (m_timer <= goDownTimer)
	{
		m_pPlayer->SetPosition(capturePos);
		m_pPlayer->SetRotation(m_rotation);
		m_pPlayer->SetWayPointState(GetLeftWayPointIndex());
		m_pPlayer->SetLeftPointIndex(GetLeftWayPointIndex());
		m_pPlayer->SetRightPointIndex(GetRightWayPointIndex());
	}
	else if (m_updateState == enLeave)
	{
		m_timer = 0.0f;

	}

}

void CUFO::Leave()
{
	m_leftOrRight = enRight;
	if (m_timer == 0.0f)
	{
		m_onWayPosition = m_position;

		m_getOnStageFlag = true;

		//�E�F�C�|�C���g�̍ő吔
		const int maxWayPointNum = 31;
		int nextIndex = m_ufoLandingPoint->GetLeftWayPointIndex();
		if (m_leftOrRight == enLeft)
		{
			//���ɓ�������A���ɉ���������Ȃ�
		}
		else
		{
			//�E�ɓ������̂ŁA���̃E�F�C�|�C���g�����Z����

			nextIndex--;
			//�E�F�C�|�C���g�̍ő吔���傫��������
			if (nextIndex < 0)
				//�������0�ɂ���
				nextIndex = maxWayPointNum;
		}
		//���g�̍����̃E�F�C�|�C���g���X�V����
		SetLeftWayPointIndex(nextIndex);
	}
	m_moveSpeed = m_defaultSpeed;
	const float switchingTime = 2.0f;
	if (m_timer >= switchingTime)
	{
		m_updateState = enSearch;
		m_timer = 0.0f;
		m_pPlayer->SetCapturedUFOFlag(false);
	}
	else
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