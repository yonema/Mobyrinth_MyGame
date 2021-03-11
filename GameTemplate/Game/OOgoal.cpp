#include "stdafx.h"
#include "OOgoal.h"


bool OOgoal::StartSub()
{
	Init("Assets/modelData/goal.tkm", enGoal);

	return true;
}

void OOgoal::UpdateSub()
{
	if (!m_pPlayer)
		return;
	Vector3 up = g_vec3Up;
	m_rotation.Apply(up);
	up.Normalize();
	Vector3 playerUp = m_pPlayer->GetUpVec();
	playerUp.Normalize();
	const float dotLen = 0.0001;
	if (std::abs(Dot(up, playerUp) - 1.0f) <= dotLen)
	{
		m_goalFlag = true;
	}
	m_dbg = std::abs(Dot(up, playerUp) - 1.0f);
}

void OOgoal::PostRender(RenderContext& rc)
{

	//�t�H���g
	Font font;
	wchar_t text[256];
	wsprintf(text, L"�S�[���I�I�I");
	font.Begin(rc);

	if (m_goalFlag)
	{
		font.Draw(text,				//�\���������e�L�X�g
			{ -200.0f,100.0f },		//�|�W�V����
			{ 1.0f,1.0f,0.0f,1.0f },	//�J���[
			0.0f,						//��]
			2.0f,						//�g��
			{ 0.0f,0.0f }				//�s�{�b�g
		);
	}


	swprintf(text, L"�S�[������:%01.3f", m_dbg);
	font.Draw(text,				//�\���������e�L�X�g
		{ 0.0f,-100.0f },		//�|�W�V����
		{ 1.0f,1.0f,1.0f,1.0f },	//�J���[
		0.0f,						//��]
		1.0f,						//�g��
		{ 0.0f,0.0f }				//�s�{�b�g
	);

	font.End(rc);
}