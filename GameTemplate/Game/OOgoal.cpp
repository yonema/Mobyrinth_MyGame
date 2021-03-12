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


	font.End(rc);
}