#include "stdafx.h"
#include "ROnail_bar.h"

//スタート関数
bool ROnail_bar::StartSub()
{
	//初期化用関数
	Init("Assets/modelData/nail.tkm", EN_RO_TYPE_NAIL,
		"Assets/modelData/hammer.tkm", EN_RO_TYPE_BAR);

	//壊れるときのサウンドの生成と初期化
	m_brokenSE = NewGO<CSoundCue>(0);
	m_brokenSE->Init(L"Assets/sound/boxBreak.wav");
	m_brokenSE->SetVolume(0.3f);

	//壊れるときのエフェクトの生成と初期化
	m_effect = NewGO<Effect>(0);
	m_effect->Init(u"Assets/effect/flame_water.efk");
	float scale = 200.0f;
	m_effect->SetScale({ scale,scale ,scale });


	return true;
}

//デストラクタ
ROnail_bar::~ROnail_bar()
{
	DeleteGO(m_brokenSE);
	DeleteGO(m_effect);
}

/// <summary>
/// クエリしてほしいタイミングで呼ばれる関数
/// </summary>
void ROnail_bar::QuerySub()
{
	//自身が「釘」の時
	if (GetObjectType() == EN_RO_TYPE_NAIL)
	{

	}
	//自身が「バール」の時
	else if (GetObjectType() == EN_RO_TYPE_BAR)
	{
		////「持てない南京錠」と「箱」だと、「持てない南京錠」の方が優先的に破棄するから
		////「持てない南京錠」を先にクエリする
		////障害オブジェクトの「持てない南京錠」をクエリ
		//QueryLOs<OOpadlock>(EN_OO_TYPE_BIG_PADLOCK, [&](OOpadlock* padlock) -> bool
		//	{
		//		//自身と「持てない南京錠」が衝突したら
		//		if (IsHitObject(*this, *padlock))
		//		{
		//			//行動できたら
		//			if (m_actionFlag)
		//			{
		//				//「持てない南京錠」を破棄
		//				DeleteGO(padlock);
		//				//自身のオブジェクトを破棄
		//				DeleteGO(this);
		//				//行動できなくする
		//				m_actionFlag = false;
		//			}
		//		}
		//		return true;
		//	}
		//);
		//障害オブジェクトの「箱」をクエリ
		QueryLOs<OObox>(EN_OO_TYPE_BOX, [&](OObox* box) -> bool
			{
				//自身と「箱」が衝突したら
				if (IsHitObject(*this, *box))
				{
					//行動できたら
					if (m_actionFlag)
					{
						//「箱」を破棄
						DeleteGO(box);
						//自身のオブジェクトを破棄
						DeleteGO(this);
						//行動できないようにする
						m_actionFlag = false;

						//壊れるときのサウンドの再生
						m_brokenSE->Play(false);

						//アップベクトル
						Vector3 upVec = g_VEC3_UP;
						//アップベクトルを自身の回転で回す
						m_rotation.Apply(upVec);
						//この値を変更して高さを調節する
						const float upVecLne = 100.0f;
						upVec.Scale(upVecLne);
						//エフェクトの座標を回転を調節する
						m_effect->SetPosition(m_position + upVec);		//座標を渡す
						m_effect->SetRotation(m_rotation);
						//エフェクトの再生
						m_effect->Play();

					}
				}
				return true;
			}
		);

	}
}