#include "stdafx.h"
#include "Player_kari.h"
#include "LightManager.h"



bool Player_kari::Start()
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

	m_mobius = FindGO<Mobius>("Mobius");


	m_dbgModel = NewGO<CModelRender>(0);
	m_dbgModel->Init("Assets/modelData/yuka.tkm");
	m_dbgModel2 = NewGO<CModelRender>(0);
	m_dbgModel2->Init("Assets/modelData/yuka.tkm");

	return true;
}

Player_kari::~Player_kari()
{
	DeleteGO(m_modelRender);
}


void Player_kari::CheckWayPoint()
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
	//���E�̃E�F�C�|�C���g�Ƃ̋����ōX�V����B

	//�E�F�C�|�C���g�Ƃ̓����蔻��̋���
	const float hitLen = 10.0f;

	//���̃E�F�C�|�C���g�ւ̃x�N�g��
	Vector3 toLpVec = m_wayPointPos[m_lpIndex] - m_position;

	//�E�̃E�F�C�|�C���g�ւ̃x�N�g��
	Vector3 toRpVec = m_wayPointPos[m_rpIndex] - m_position;

	//���E�̃E�F�C�|�C���g�Ƃ̋����𒲂ׂ�
	if (toLpVec.Length() <= hitLen && m_padLStickXF < 0.0f)
	{
		//���̃E�F�C�|�C���g�Ƃ̋����������蔻��ȉ��A���A
		//���Ɉړ��̓��͂���������A
		//m_wayPointState�����Z���č��ɐi�߂�B
		m_wayPointState += 1;
		if (m_wayPointState > m_maxWayPointState)
		{
			//m_wayPointState��MAX���傫��������
			//��������Ƃ������Ƃ�����A�X�^�[�g��0�ɂ���
			m_wayPointState = 0;
		}

	}
	else if (toRpVec.Length() <= hitLen && m_padLStickXF > 0.0f)
	{
		//�E�̃E�F�C�|�C���g�Ƃ̋����������蔻��ȉ��A���A
		//�E�Ɉړ��̓��͂���������A
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

void Player_kari::Move()
{
	//�ړ���������͖��t���[���v�Z�������������̂��ȁH
	//����Ƃ��Am_wayPointState�̐؂�ւ̎��ɂ������������̂��ȁH
	//����A���̂������Ɩ��t���[�����Ȃ��Ă͂����Ȃ��C������

	//1.���E�ւ̈ړ�����������v�Z����B

	//���ֈړ��������
	Vector3 moveToLeft = m_wayPointPos[m_lpIndex] - m_position;
	moveToLeft.Normalize();
	//�E�ֈړ��������
	Vector3 moveToRight = m_wayPointPos[m_rpIndex] - m_position;
	moveToRight.Normalize();


	//2.�ړ�����

	//�Ƃ肠�����̏���
	//�d�͂�A�����x�A��R����������Ƃ��͕ʂ̂�����
	m_moveSpeed = g_vec3Zero;

	//�ړ����钷��
	const float moveLen = 500.0f;

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

void Player_kari::GetOnStage()
{
	Vector3 addPos = g_vec3AxisY;
	m_finalWPRot.Apply(addPos);
	addPos.Scale(-200.0f);

	if (m_mobius)
	{
		if (m_mobius->GetModel()->InIntersectLine(m_position, m_position + addPos))
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
	m_dbgModel2->SetPosition(m_mobius->GetModel()->GetIntersectPos());

	m_dbgModel->SetPosition(m_position + addPos);

	return;

}

void Player_kari::Rotation()
{
	//���̃E�F�C�|�C���g����E�̃E�F�C�|�C���g�ւ̃x�N�g��
	Vector3 lpToRpLen = m_wayPointPos[m_rpIndex] - m_wayPointPos[m_lpIndex];

	//���̃E�F�C�|�C���g����v���C���[�ւ̃x�N�g��
	Vector3 lpToPlayerLen = m_position - m_wayPointPos[m_lpIndex];

	//�⊮��
	float ComplementRate = lpToPlayerLen.Length() / lpToRpLen.Length();

	//���ʐ��`�⊮
	m_finalWPRot.Slerp(ComplementRate, m_wayPointRot[m_lpIndex], m_wayPointRot[m_rpIndex]);

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

void Player_kari::Update()
{
	//�Q�[���p�b�h�̍��X�e�B�b�N��X���̓��͏����擾
	m_padLStickXF = g_pad[0]->GetLStickXF();
	//���E�̌�����ݒ�
	if (m_padLStickXF < 0.0f)
		m_leftOrRight = enLeft;		//������
	else if (m_padLStickXF > 0.0f)
		m_leftOrRight = enRight;	//�E����

	//�E�F�C�|�C���g�̍X�V����
	CheckWayPoint();
	//�ړ�����
	Move();
	//���f���̉�]����
	Rotation();

	m_position += m_moveSpeed * 1.0 / 60.0f;
	GetOnStage();

	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
}

void Player_kari::InitWayPointPos(const std::size_t vecSize, std::map<int, Vector3>& posMap)
{
	//vector�̃T�C�Y�̊m��
	m_wayPointPos.resize(vecSize);
	//�E�F�C�|�C���g�X�e�[�g�̍ő�̒l��ݒ�
	m_maxWayPointState = vecSize - 1;
	//m_wayPointPos�ɃE�F�C�|�C���g�́u�ꏊ�v���i�[����
	std::map<int, Vector3>::iterator it = posMap.begin();
	for (int index = 0; it != posMap.end(); index++, it++)
	{
		m_wayPointPos[index] = it->second;
	}
}
void Player_kari::InitWayPointRot(const std::size_t vecSize, std::map<int, Quaternion>& rotMap)
{
	//vector�̃T�C�Y�̊m��
	m_wayPointRot.resize(vecSize);
	//�E�F�C�|�C���g�X�e�[�g�̍ő�̒l��ݒ�
	m_maxWayPointState = vecSize - 1;
	//m_wayPointRot�ɃE�F�C�|�C���g�́u��]�v���i�[����
	std::map<int, Quaternion>::iterator it = rotMap.begin();
	for (int index = 0; it != rotMap.end(); index++, it++)
	{
		m_wayPointRot[index] = it->second;
	}
}

void Player_kari::PostRender(RenderContext& rc)
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
	swprintf(text, L"[%02d]", m_rpIndex);
	m_font.Draw(text,
		{ 10.0f,50.0f },
		{ 1.0f,0.0f,0.0f,1.0f },
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

	swprintf(text, L"Hit%d", m_dbgHit);
	m_font.Draw(text,
		{ 110.0f, 150.0f },
		{ 1.0f,0.0f,0.0f,1.0f },
		0.0f,
		1.0f,
		{ 0.0f,0.0f }
	);




	//�`��I��
	m_font.End(rc);
}
