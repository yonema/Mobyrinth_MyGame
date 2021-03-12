#include "stdafx.h"
#include "OOgoal.h"


bool OOgoal::StartSub()
{
	Init("Assets/modelData/goal.tkm", enGoal);

	return true;
}

void OOgoal::UpdateSub()
{
	if (IsHitPlayer(0.0001))
	{
		m_goalFlag = true;
	}
	else
	{
		m_goalFlag = false;
	}
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


	font.End(rc);
}