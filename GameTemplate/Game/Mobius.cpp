#include "stdafx.h"
#include "Mobius.h"
#include "LevelObjectManager.h"


//スタート関数
bool Mobius::Start()
{
	//モデルレンダラーの生成
	m_modelRender = NewGO<CModelRender>(0);
	//モデルレンダラーの初期化
	m_modelRender->Init("Assets/modelData/Mobius.tkm");
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
	m_modelRender->SetShadowCasterFlag(false);

	return true;
}

//デストラクタ
Mobius::~Mobius()
{
	//モデルレンダラーの破棄
	DeleteGO(m_modelRender);
	//マネージャーにメビウスの輪が消去されたと伝える
	CLevelObjectManager::GetInstance()->SetMobius(nullptr);
}

