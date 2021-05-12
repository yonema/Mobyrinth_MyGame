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

	m_effect = NewGO<Effect>(0);
	m_effect->Init(u"Assets/effect/ster.efk");
	float scale = 50.0f;
	m_effect->SetScale({ scale ,scale ,scale });
	m_effect->SetPosition(m_position);
	m_effect->Play();


	return true;
}

//デストラクタ
OOgoal::~OOgoal()
{
	DeleteGO(m_spriteRender);
	DeleteGO(m_effect);
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

