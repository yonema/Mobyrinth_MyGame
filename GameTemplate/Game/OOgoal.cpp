#include "stdafx.h"
#include "OOgoal.h"

//スタート関数
bool OOgoal::StartSub()
{
	//初期化用関数
	Init("Assets/modelData/goal.tkm", EN_OO_TYPE_GOAL);

	//フォントレンダラーの生成
	//m_spriteRender = NewGO<CSpriteRender>(2);
	//初期化
	//m_spriteRender->Init
	//("Assets/Image/Clear.DDS", 1024.0f, 512.0f, {0.5f,0.5f}, AlphaBlendMode_Trans);

	//無効化する
	//m_spriteRender->Deactivate();

	m_effect = NewGO<Effect>(0);
	m_effect->Init(u"Assets/effect/wing.efk");
	m_effect->SetPosition(m_position);
	m_effect->SetRotation(m_rotation);
	const float scale = 50.0f;
	m_effect->SetScale({ scale ,scale ,scale });
	//m_effect->Play();


	//OBBWorldに自身のOBBの登録を消去させる
	COBBWorld::GetInstance()->RemoveOBB(&GetOBB());

	return true;
}

//デストラクタ
OOgoal::~OOgoal()
{
	//DeleteGO(m_spriteRender);
}

//アップデート関数
void OOgoal::UpdateSub()
{
	//ヒットフラグが折られているか？
	if (!GetIsHitFlag())
	{
		//折られている
		
		m_effect->Deactivate();
		return;
	}

	if (!m_effect->IsActive())
	{
		m_effect->Activate();
	}
	const float m_rePlayTime = 1.5;
	if (m_timer >= m_rePlayTime)
	{
		//m_effect->Play();

		m_timer = 0.0f;
	}
	else
	{
		m_timer += GameTime().GetFrameDeltaTime();
	}

	//落下中か？
	if (m_player->GetFallFalg())
		//落下中はゴール判定を取らない
		return;

	//自身とプレイヤーが衝突したら
	if (IsHitPlayer())
	{
		//ゴール状態にする
		m_isGoal = true;
		//フォントレンダラーを有効化する
		//m_spriteRender->Activate();
		//プレイヤーをゴール状態にする
		m_player->SetGoal(true);
		////モデルを見えなくする
		//GetModelRender()->Deactivate();
		DeleteGO(this);
	}

}

