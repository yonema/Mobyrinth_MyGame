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
	}

	//���C�g�̍X�V
	m_spotLight->SetPosition(m_position);
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
	const int switchingTime = 3.0f;

	//�؂�ւ����ԂɂȂ�����
	if (m_timer >= switchingTime)
	{
		//�t���O��؂荇����
		m_searchFlag = !m_searchFlag;
		//�^�C�}�[��0�ɖ߂�
		m_timer = 0.0f;
	}

	//�{�����Ȃ�
	if (m_searchFlag)
	{
		m_modelRender->SetEmissionColor({ 0.5f,0.5f,0.0f,1.0f });
		m_spotLight->SetColor({ 300.0f,300.0f,0.0f,1.0f });
		//�v���C���[���Փ˂��Ă��邩�H
		if (IsHitPlayer())
		{
			//�v���C���[��������
			m_modelRender->SetEmissionColor({ 1.0f,0.0f,0.0f,1.0f });
			m_spotLight->SetColor({ 600.0f,0.0f,0.0f,1.0f });

		}
	}
	else
	{
		m_modelRender->SetEmissionColor({ 0.0f,0.0f,0.0f,1.0f });
		m_spotLight->SetColor({ 0.0f,0.0f,0.0f,1.0f });

	}
}

//�v���C���[�����������̏���
void CUFO::Capture()
{

}

//�v���C���[���^�ԏ���
void CUFO::Transport()

{

}

//�ړ�����
void CUFO::Move()
{
	//�E�ɓ���
	bool right = true;
	//���̃E�F�C�|�C���g
	int nextIndex = GetRightWayPointIndex();
	//�ŏI�I�Ȉړ����x	//�ړ����x�Ƀf���^�^�C�����|����
	float finalMoveSpeed = m_moveSpeed * GameTime().GetFrameDeltaTime();
	//�E�F�C�|�C���g��̎��̍��W���v�Z����
	m_onWayPosition = CLevelObjectManager::GetInstance()->CalcWayPointNextPos
	(GetRightWayPointIndex(), m_onWayPosition, finalMoveSpeed, right, &nextIndex);

	//�E�ɓ������̂ŁA���̃E�F�C�|�C���g�����Z����
	nextIndex++;
	//�E�F�C�|�C���g�̍ő吔
	const int maxWayPointNum = 31;
	//�E�F�C�|�C���g�̍ő吔���傫��������
	if (nextIndex > maxWayPointNum)
		//�������0�ɂ���
		nextIndex = 0;
	//���g�̍����̃E�F�C�|�C���g���X�V����
	SetLeftWayPointIndex(nextIndex);
}

//���r�E�X�̗ւ̃X�e�[�W�ɃC�C�����ɍ��킹�鏈��
void CUFO::GetOnStage()
{
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