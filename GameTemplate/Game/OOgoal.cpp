#include "stdafx.h"
#include "OOgoal.h"

//スタート関数
bool OOgoal::StartSub()
{
	//初期化用関数
	Init("Assets/modelData/goal.tkm", enGoal);

	//フォントレンダラーの生成
	m_fontRender = NewGO<CFontRender>(0);
	//初期化
	m_fontRender->Init(L"ゴール！",
		{-200.0f,100.0f},
		{1.0f,1.0f,0.0f,1.0f},
		0.0f,
		3.0f
		);
	//ポストレンダラーで描画する
	m_fontRender->SetPostRenderFlag(true);
	//無効化する
	m_fontRender->Deactivate();

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
	DeleteGO(m_fontRender);
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
		m_fontRender->Activate();
	}

}

