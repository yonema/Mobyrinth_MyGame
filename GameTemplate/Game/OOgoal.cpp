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

	//フォント
	Font font;
	wchar_t text[256];
	wsprintf(text, L"ゴール！！！");
	font.Begin(rc);

	if (m_goalFlag)
	{
		font.Draw(text,				//表示したいテキスト
			{ -200.0f,100.0f },		//ポジション
			{ 1.0f,1.0f,0.0f,1.0f },	//カラー
			0.0f,						//回転
			2.0f,						//拡大
			{ 0.0f,0.0f }				//ピボット
		);
	}


	swprintf(text, L"ゴール内積:%01.3f", m_dbg);
	font.Draw(text,				//表示したいテキスト
		{ 0.0f,-100.0f },		//ポジション
		{ 1.0f,1.0f,1.0f,1.0f },	//カラー
		0.0f,						//回転
		1.0f,						//拡大
		{ 0.0f,0.0f }				//ピボット
	);

	font.End(rc);
}