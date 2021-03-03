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

	m_charaCon.Init(50.0f, 10.0f, m_position);

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
	/// m_wayPointState��0�̎��Am_lpIndex��0,m_rpIndex��1�ɂȂ�B
	/// �܂�Am_lpIndex�͏��m_wayPointState�Ɠ����l�ɂȂ�A
	/// m_rpIndex��m_wayPointState��1���Z�����l�ɂȂ�B
	/// ������m_maxWayPointState��m_wayPointState�̍ő吔��\���B
	/// m_maxWayPointState��31��������m_wayPointState��31�܂ő��݂���B
	

	//�v���C���[���g�̍��E�̃E�F�C�|�C���g��ݒ肷��
	m_lpIndex = m_wayPointState;
	m_rpIndex = m_lpIndex + 1;
	if (m_rpIndex > m_maxWayPointState)
	{
		m_rpIndex = 0;
	}

	const float hitLen = 10.0f;

	//���̃E�F�C�|�C���g�ւ̃x�N�g��
	Vector3 toLpVec = m_wayPointPos[m_lpIndex] - m_position;
	//�E�̃E�F�C�|�C���g�ւ̃x�N�g��
	Vector3 toRpVec = m_wayPointPos[m_rpIndex] - m_position;

	//���E�̃E�F�C�|�C���g�Ƃ̋����𒲂ׂ�
	if (toLpVec.Length() <= hitLen && g_pad[0]->GetLStickXF() > 0.0f)
	{
		m_wayPointState -= 1;
		if (m_wayPointState < 0)
		{
			m_wayPointState = m_maxWayPointState;
		}
	}
	else if (toRpVec.Length() <= hitLen && g_pad[0]->GetLStickXF() < 0.0f)
	{
		m_wayPointState += 1;
		if (m_wayPointState > m_maxWayPointState)
		{
			m_wayPointState = 0;
		}
	}

	return;

}

void Player_kari::Move()
{
	//�ړ���������͖��t���[���v�Z�������������̂��ȁH
	//����Ƃ��Am_wayPointState�̐؂�ւ̎��ɂ������������̂��ȁH
	//����A���̂������Ɩ��t���[�����Ȃ��Ă͂����Ȃ��C������


	//���ֈړ�����������v�Z
	Vector3 moveToRight = m_wayPointPos[m_rpIndex] - m_position;
	moveToRight.Normalize();
	//�E�ֈړ�����������v�Z
	Vector3 moveToLeft = m_wayPointPos[m_lpIndex] - m_position;
	moveToLeft.Normalize();


	//�Ƃ肠�����̏���
	//�d�͂�A�����x�A��R����������Ƃ��͕ʂ̂�����
	m_moveSpeed = g_vec3Zero;

	const float moveLen = 500.0f;
	if (g_pad[0]->GetLStickXF() > 0.0f)
	//���ւ̈ړ��̌v�Z
	m_moveSpeed += moveToLeft * g_pad[0]->GetLStickXF() * moveLen;
	else if (g_pad[0]->GetLStickXF() < 0.0f)
	//���ւ̈ړ��̌v�Z
	m_moveSpeed += moveToRight * g_pad[0]->GetLStickXF() * -moveLen;


	return;
}

void Player_kari::Rotation()
{
	return;
}

void Player_kari::Update()
{
	CheckWayPoint();
	Move();
	Rotation();

	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
}

void Player_kari::InitWayPointPos(const std::size_t vecSize, std::map<int, Vector3>& posMap)
{
	//vector�̃T�C�Y�̊m��
	m_wayPointPos.resize(vecSize);
	//�E�F�C�|�C���g�̍ő吔�����i�[���Ă���
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
	//�E�F�C�|�C���g�̍ő吔�����i�[���Ă���
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

	swprintf(text, L"wayPointState:%02d", m_wayPointState);


	//�`��J�n
	m_font.Begin(rc);

	//�`��
	m_font.Draw(text,
		{ -200.0f, 200.0f },
		{ 0.0f,0.0f,0.0f,1.0f },
		0.0f,
		1.0f,
		{ 0.5f,0.5f }
	);

	//�`��I��
	m_font.End(rc);
}
