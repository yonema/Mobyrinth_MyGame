#include "stdafx.h"
#include "TipsController.h"

//スタート関数
bool CTipsController::Start()
{
	return true;
}

//アップデート関数
void CTipsController::Update()
{
	//プレイヤーに一番近いオブジェクトのタイプを取得
	int objectType = CLevelObjectManager::GetInstance()->GetNearestObjectType();

	//Tipsにセットする
	m_tips.SetText(objectType);
	
}