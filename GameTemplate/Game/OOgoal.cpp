#include "stdafx.h"
#include "OOgoal.h"

//スタート関数
bool OOgoal::StartSub()
{
	//初期化用関数
	Init("Assets/modelData/goal.tkm", enGoal);

	//フォントレンダラーの生成
	m_spriteRender = NewGO<CSpriteRender>(2);
	//初期化
	m_spriteRender->Init
	("Assets/Image/Clear.DDS", 1024.0f, 512.0f, {0.5f,0.5f}, AlphaBlendMode_Trans);

	//無効化する
	m_spriteRender->Deactivate();

	//OBBWorldに自身のOBBの登録を消去させる
	COBBWorld::GetInstance()->RemoveOBB(&GetOBB());

	return true;
}

//デストラクタ
OOgoal::~OOgoal()
{
	DeleteGO(m_spriteRender);
}

//アップデート関数
void OOgoal::UpdateSub()
{
	//自身とプレイヤーが衝突したら
	if (IsHitPlayer())
	{
		//ゴール状態にする
		m_isGoal = true;
		//フォントレンダラーを有効化する
		m_spriteRender->Activate();
	}

}

