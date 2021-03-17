#include "stdafx.h"
#include "Player.h"
#include "LightManager.h"
#include "LevelObjectManager.h"


bool Player::Start()
{
	m_animationClips[enAnimClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimClip_Run].Load("Assets/animData/walk.tka");
	m_animationClips[enAnimClip_Run].SetLoopFlag(true);
	m_modelRender = NewGO<CModelRender>(0);
	m_modelRender->Init
	("Assets/modelData/unityChan.tkm",m_animationClips,enAnimClip_Num, enModelUpAxisY);
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
	m_onWayPosition = m_position;
	m_mobius = FindGO<Mobius>("Mobius");


	//�f�o�b�N�p
	//��ŏ���
	m_dbgModel = NewGO<CModelRender>(0);
	m_dbgModel->Init("Assets/modelData/yuka.tkm");
	m_dbgModel2 = NewGO<CModelRender>(0);
	m_dbgModel2->Init("Assets/modelData/yuka.tkm");
	m_dbgModel3 = NewGO<CModelRender>(0);
	m_dbgModel3->Init("Assets/modelData/yuka.tkm");

	SInitOBBData initData;
	m_obb.Init(initData);

	for (int i = 0; i < m_obbNum; i++)
	{
		m_dbgObbModel[i] = NewGO<CModelRender>(0);
		m_dbgObbModel[i]->Init("Assets/modelData/yuka.tkm");
	}
	return true;
}

Player::~Player()
{
	DeleteGO(m_modelRender);

	//�f�o�b�N�p
	//��ŏ���
	DeleteGO(m_dbgModel);
	DeleteGO(m_dbgModel2);
	DeleteGO(m_dbgModel3);
	for (int i = 0; i < m_obbNum; i++)
	{
		DeleteGO(m_dbgObbModel[i]);
	}

}


void Player::CheckWayPoint()
{
	///
	///
	/// m_wayPointState�͍������0���瑝���Ă����B
	/// m_wayPointState��0�̎��Am_lpIndex��0,m_rpIndex��1�ɂȂ�B
	/// �܂�Am_lpIndex�͏��m_wayPointState�Ɠ����l�ɂȂ�A
	/// m_rpIndex��m_wayPointState��1���Z�����l�ɂȂ�B
	/// ������m_maxWayPointState��m_wayPointState�̍ő吔��\���B
	/// m_maxWayPointState��31��������m_wayPointState��31�܂ő��݂���B
	

	//1.�v���C���[���g�̍��E�̃E�F�C�|�C���g��ݒ肷��
	//m_wayPointState�����ƂɃE�F�C�|�C���g��ݒ肷��B
	m_rpIndex = m_wayPointState;	//�E�̃E�F�C�|�C���g�͂�m_wayPointState�͓����l
	m_lpIndex = m_rpIndex + 1;		//���̃E�F�C�|�C���g�͉E�̃E�F�C�|�C���g��1��̒l

	if (m_lpIndex > m_maxWayPointState)
	{
		//���̃E�F�C�|�C���g��MAX���傫��������
		//��������Ƃ������Ƃ�����A�X�^�[�g��0�ɂ���
		m_lpIndex = 0;
	}


	//2.m_wayPointState�̍X�V�B

	Vector3 LpToRpVec = (*m_wayPointPos)[m_rpIndex] - (*m_wayPointPos)[m_lpIndex];
	LpToRpVec.Normalize();
	Vector3 LpToPlayerVec = m_position - (*m_wayPointPos)[m_lpIndex];
	LpToPlayerVec.Normalize();
	Vector3 RpToPlayerVec = m_position - (*m_wayPointPos)[m_rpIndex];
	RpToPlayerVec.Normalize();

	float LpDotPlayer = Dot(LpToRpVec, LpToPlayerVec);
	float RpDotPlayer = Dot(LpToRpVec, RpToPlayerVec);
	m_dbgDot1 = LpDotPlayer;
	m_dbgDot2 = RpDotPlayer;


	//���E�̃E�F�C�|�C���g�Ƃ̋����𒲂ׂ�
	float f = 0.35f;
	if (LpDotPlayer > f && RpDotPlayer < -f)
	{
		//���̃E�F�C�|�C���g�̊Ԃɂ���
	}
	else if (LpDotPlayer <= f && m_padLStickXF < 0.0f)
	{
		//���̃E�F�C�|�C���g�̊Ԃ���A�����ɏo�Ă�����
		m_wayPointState += 1;
		if (m_wayPointState > m_maxWayPointState)
		{
			//m_wayPointState��MAX���傫��������
			//��������Ƃ������Ƃ�����A�X�^�[�g��0�ɂ���
			m_wayPointState = 0;
		}
	}
	else if (RpDotPlayer >= -f && m_padLStickXF > 0.0f)
	{
		//���̃E�F�C�|�C���g�̊Ԃ���A�E������o�Ă�����
		//m_wayPointState�����Z���ĉE�ɐi�߂�B
		m_wayPointState -= 1;
		if (m_wayPointState < 0)
		{
			//m_wayPointState��0��菬����������
			//��������Ƃ������Ƃ�����AMAX�̒l�ɂ���
			m_wayPointState = m_maxWayPointState;
		}
	}



	return;

}

void Player::Move()
{
	//�ړ���������͖��t���[���v�Z�������������̂��ȁH
	//����Ƃ��Am_wayPointState�̐؂�ւ̎��ɂ������������̂��ȁH
	//����A���̂������Ɩ��t���[�����Ȃ��Ă͂����Ȃ��C������

	//1.���E�ւ̈ړ�����������v�Z����B

	//���ֈړ��������
	Vector3 moveToLeft = (*m_wayPointPos)[m_lpIndex] - m_onWayPosition;
	moveToLeft.Normalize();
	//�E�ֈړ��������
	Vector3 moveToRight = (*m_wayPointPos)[m_rpIndex] - m_onWayPosition;
	moveToRight.Normalize();


	//2.�ړ�����

	//�Ƃ肠�����̏���
	//�d�͂�A�����x�A��R����������Ƃ��͕ʂ̂�����
	m_moveSpeed = g_vec3Zero;

	//�ړ����钷��
	float moveLen = 1000.0f;

	//�Q�[���p�b�h��R1�{�^���̓��͏����擾(�_�b�V�����)
	if (g_pad[0]->IsPress(enButtonRB1) == true) {
		moveLen = 3000.0f;
	}


	if (m_padLStickXF < 0.0f)
	{
		//���ւ̈ړ��̓��͂���������
		//���ւ̈ړ��̌v�Z����
		m_moveSpeed += moveToLeft * m_padLStickXF * -moveLen;
	}
	else if (m_padLStickXF > 0.0f)
	{
		//�E�ւ̈ړ��̓��͂���������
		//�E�ւ̈ړ��̌v�Z������
		m_moveSpeed += moveToRight * m_padLStickXF * moveLen;
	}


	return;
}

void Player::GetOnStage()
{
	m_upVec = g_vec3AxisY;
	m_finalWPRot.Apply(m_upVec);
	m_upVec.Scale(150.0f);
	if (m_mobius)
	{
		if (m_mobius->GetModel()->InIntersectLine(m_onWayPosition + m_upVec, m_onWayPosition - m_upVec))
		{
			m_position = m_mobius->GetModel()->GetIntersectPos();
			m_dbgHit = true;
		}
		else
			m_dbgHit = false;
	}
	else
	{
		m_mobius = FindGO<Mobius>("Mobius");
	}
	auto hitPos = m_mobius->GetModel()->GetIntersectPos();
	m_dbgModel->SetPosition(m_onWayPosition + m_upVec);
	m_dbgModel2->SetPosition(m_onWayPosition - m_upVec);
	m_dbgModel3->SetPosition(hitPos);
	

	//m_modelRender->SetPosition(hitPos);
	return;

}

void Player::Rotation()
{
	//���̃E�F�C�|�C���g����E�̃E�F�C�|�C���g�ւ̃x�N�g��
	Vector3 lpToRpLen = (*m_wayPointPos)[m_rpIndex] - (*m_wayPointPos)[m_lpIndex];

	//���̃E�F�C�|�C���g����v���C���[�ւ̃x�N�g��
	Vector3 lpToPlayerLen = m_onWayPosition - (*m_wayPointPos)[m_lpIndex];

	//�⊮��
	float ComplementRate = lpToPlayerLen.Length() / lpToRpLen.Length();

	//���ʐ��`�⊮
	m_finalWPRot.Slerp(ComplementRate, (*m_wayPointRot)[m_lpIndex], (*m_wayPointRot)[m_rpIndex]);

	//�L�����N�^�[�̍��E�̌����ɍ��킹�ĉ�]
	if (m_leftOrRight == enLeft)
	{
		m_rotation.SetRotationDegY(90.0f);
	}
	else if (m_leftOrRight == enRight)
	{
		m_rotation.SetRotationDegY(-90.0f);
	}
	m_rotation.Multiply(m_finalWPRot);

	return;
}

void Player::Update()
{

	//�Q�[���p�b�h�̍��X�e�B�b�N��X���̓��͏����擾
	m_padLStickXF = g_pad[0]->GetLStickXF();
	//���E�̌�����ݒ�
	if (m_padLStickXF < 0.0f)
		m_leftOrRight = enLeft;		//������
	else if (m_padLStickXF > 0.0f)
		m_leftOrRight = enRight;	//�E����
	m_obb.SetRotation(m_finalWPRot);
	m_obb.SetPosition(m_position);
	Vector3 vert[m_obbNum];
	Vector3 addVec[m_obbNum];
	for (int i = 0; i < m_obbNum; i++)
	{
		vert[i] = m_obb.GetPosition();
		addVec[i] = g_vec3Zero;
	} 
	addVec[0] -= m_obb.GetNormalDirection(COBB::enLocalX);
	addVec[0] += m_obb.GetNormalDirection(COBB::enLocalY);
	addVec[0] -= m_obb.GetNormalDirection(COBB::enLocalZ);
	addVec[1] -= m_obb.GetNormalDirection(COBB::enLocalX);
	addVec[1] += m_obb.GetNormalDirection(COBB::enLocalY);
	addVec[1] += m_obb.GetNormalDirection(COBB::enLocalZ);
	addVec[2] -= m_obb.GetNormalDirection(COBB::enLocalX);
	addVec[2] -= m_obb.GetNormalDirection(COBB::enLocalY);
	addVec[2] -= m_obb.GetNormalDirection(COBB::enLocalZ);
	addVec[3] -= m_obb.GetNormalDirection(COBB::enLocalX);
	addVec[3] -= m_obb.GetNormalDirection(COBB::enLocalY);
	addVec[3] += m_obb.GetNormalDirection(COBB::enLocalZ);
	addVec[4] += m_obb.GetNormalDirection(COBB::enLocalX);
	addVec[4] += m_obb.GetNormalDirection(COBB::enLocalY);
	addVec[4] -= m_obb.GetNormalDirection(COBB::enLocalZ);
	addVec[5] += m_obb.GetNormalDirection(COBB::enLocalX);
	addVec[5] += m_obb.GetNormalDirection(COBB::enLocalY);
	addVec[5] += m_obb.GetNormalDirection(COBB::enLocalZ);
	addVec[6] += m_obb.GetNormalDirection(COBB::enLocalX);
	addVec[6] -= m_obb.GetNormalDirection(COBB::enLocalY);
	addVec[6] -= m_obb.GetNormalDirection(COBB::enLocalZ);
	addVec[7] += m_obb.GetNormalDirection(COBB::enLocalX);
	addVec[7] -= m_obb.GetNormalDirection(COBB::enLocalY);
	addVec[7] += m_obb.GetNormalDirection(COBB::enLocalZ);

	for (int i = 0; i < m_obbNum; i++)
	{
		addVec[i].Scale(200.0f);
		vert[i] += addVec[i];
		m_dbgObbModel[i]->SetPosition(vert[i]);
	}

	//�E�F�C�|�C���g�̍X�V����
	CheckWayPoint();
	//�ړ�����
	Move();
	//���f���̉�]����
	Rotation();

	m_onWayPosition += m_moveSpeed * 1.0 / 60.0f;
	GetOnStage();

	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);

}



void Player::SetWayPointPos
(const std::size_t vecSize, std::vector<Vector3>*const posMap)
{
	//vector�̃T�C�Y�̊m��
	//m_wayPointPos->resize(vecSize);
	//�E�F�C�|�C���g�X�e�[�g�̍ő�̒l��ݒ�
	m_maxWayPointState = vecSize - 1;
	//m_wayPointPos�ɃE�F�C�|�C���g�́u�ꏊ�v���i�[����
	m_wayPointPos = posMap;
	//std::vector<Vector3>::iterator it = posMap->begin();
	//for (int index = 0; it != posMap->end(); index++, it++)
	//{
	//	m_wayPointPos[index] = &it;
	//}
}
void Player::SetWayPointRot
(const std::size_t vecSize, std::vector<Quaternion>* rotMap)
{
	//vector�̃T�C�Y�̊m��
	//m_wayPointRot->resize(vecSize);
	//�E�F�C�|�C���g�X�e�[�g�̍ő�̒l��ݒ�
	m_maxWayPointState = vecSize - 1;
	//m_wayPointRot�ɃE�F�C�|�C���g�́u��]�v���i�[����
	m_wayPointRot = rotMap;
	//std::map<int, Quaternion>::iterator it = rotMap->begin();
	//for (int index = 0; it != rotMap->end(); index++, it++)
	//{
	//	m_wayPointRot[index] = &it->second;
	//}
}


void Player::PostRender(RenderContext& rc)
{
	//�e�L�X�g�p��
	wchar_t text[256];




	//�`��J�n
	m_font.Begin(rc);

	swprintf(text, L"wayPointState:%02d", m_wayPointState);
	//�`��
	m_font.Draw(text,
		{ -600.0f, 300.0f },
		{ 0.0f,0.0f,0.0f,1.0f },
		0.0f,
		1.0f,
		{ 0.0f,0.0f }
	);
	swprintf(text, L"[%02d]", m_lpIndex);
	m_font.Draw(text,
		{ -110.0f, 50.0f },
		{ 1.0f,0.0f,0.0f,1.0f },
		0.0f,
		1.0f,
		{ 0.0f,0.0f }
	);
	swprintf(text, L"[%02d]", m_rpIndex);
	m_font.Draw(text,
		{ 10.0f,50.0f },
		{ 1.0f,0.0f,0.0f,1.0f },
		0.0f,
		1.0f,
		{ 0.0f,0.0f }
	);

	swprintf(text, L"Hit%d", m_dbgHit);
	m_font.Draw(text,
		{ 110.0f, 150.0f },
		{ 1.0f,0.0f,0.0f,1.0f },
		0.0f,
		1.0f,
		{ 0.0f,0.0f }
	);
	swprintf(text, L"rate%05f", m_mobius->GetModel()->getDbg());
	m_font.Draw(text,
		{ 110.0f, 120.0f },
		{ 1.0f,0.0f,0.0f,1.0f },
		0.0f,
		1.0f,
		{ 0.0f,0.0f }
	);

	swprintf(text, L"����%02.2f", m_dbgDot1);
	m_font.Draw(text,
		{ -310.0f, 150.0f },
		{ 1.0f,0.0f,0.0f,1.0f },
		0.0f,
		1.0f,
		{ 0.0f,0.0f }
	);
	swprintf(text, L"�E��%02.2f", m_dbgDot2);
	m_font.Draw(text,
		{ -310.0f, 120.0f },
		{ 1.0f,0.0f,0.0f,1.0f },
		0.0f,
		1.0f,
		{ 0.0f,0.0f }
	);

	//�`��I��
	m_font.End(rc);
}
