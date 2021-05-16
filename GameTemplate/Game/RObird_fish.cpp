#include "stdafx.h"
#include "RObird_fish.h"
#include "OObigFire.h"
#include "ROmizu_kori.h"

//スタート関数
bool RObird_fish::StartSub()
{
	//初期化用関数
	Init("Assets/modelData/bird.tkm", enBird,
		"Assets/modelData/fish.tkm", enFish);

	//表と裏のモデルレンダラーの生成
	for (int i = 0; i < enFrontAndBackNum; i++)
	{
		m_otherModelRender[i] = NewGO<CModelRender>(0);
	}

	//もう一つの表と裏のモデルレンダラーの初期化
	m_otherModelRender[enFront]->Init("Assets/modelData/grilled_chicken.tkm");
	m_otherModelRender[enBack]->Init("Assets/modelData/grilled_fish.tkm");
	//もう一つの表と裏のオブジェクトのタイプを設定
	m_reversibleType[enFront] = enGrilledChicken;
	m_reversibleType[enBack] = enGrilledFish;

	//モデルレンダラーを無効化して非常時にする
	for (int i = 0; i < enFrontAndBackNum; i++)
	{
		m_otherModelRender[i]->Deactivate();
	}

	return true;
}

//デストラクタ
RObird_fish::~RObird_fish()
{
	//表と裏のモデルレンダラーを破棄
	for (int i = 0; i < enFrontAndBackNum; i++)
	{
		DeleteGO(m_otherModelRender[i]);
	}
}

/// <summary>
/// クエリしてほしいタイミングで呼ばれる関数
/// </summary>
void RObird_fish::QuerySub()
{
	//自身が「鳥」の時
	if (GetObjectType() == enBird)
	{
		//障害オブジェクトの「でかい火」をクエリ
		QueryLOs<OObigFire>(enBigFire, [&](OObigFire* bigFire) -> bool
			{
				//自身と「でかい火」が衝突したら
				if (IsHitObject(*this, *bigFire))
				{
					//元の裏表のモデルを無効化する
					SetBothModelActiveFlag(false);
					//もう一つの裏表のモデルを有効化する
					m_otherModelRender[GetFrontOrBack()]->Activate();
					//オブジェクトが重なっているかを判定する処理を動かさないようにする。
					m_flagOverlap = false;
				}
				//trueを戻す
				return true;
			}
		);
		//反転オブジェクトの「火」をクエリ
		QueryLOs<ROmizu_kori>(enFire, [&](ROmizu_kori* Fire) -> bool
			{
				//自身と「火」が衝突したら
				if (IsHitObject(*this, *Fire))
				{
					//元の裏表のモデルを無効化する
					SetBothModelActiveFlag(false);
					//もう一つの裏表のモデルを有効化する
					m_otherModelRender[GetFrontOrBack()]->Activate();
					//オブジェクトが重なっているかを判定する処理を動かさないようにする。
					m_flagOverlap = false;
				}
				//trueを戻す
				return true;
			}
		);
	}
	//自身が「魚」の時
	else if (GetObjectType() == enFish)
	{
		//障害オブジェクトの「でかい火」をクエリ
		QueryLOs<OObigFire>(enBigFire, [&](OObigFire* bigFire) -> bool
			{
				//自身と「でかい火」が衝突したら
				if (IsHitObject(*this, *bigFire))
				{
					//元の裏表のモデルを無効化する
					SetBothModelActiveFlag(false);
					//もう一つの裏表のモデルを有効化する
					m_otherModelRender[GetFrontOrBack()]->Activate();
					//オブジェクトが重なっているかを判定する処理を動かさないようにする。
					m_flagOverlap = false;
				}
				//trueを戻す
				return true;
			}
		);
		//反転オブジェクトの「火」をクエリ
		QueryLOs<ROmizu_kori>(enFire, [&](ROmizu_kori* Fire) -> bool
			{
				//自身と「火」が衝突したら
				if (IsHitObject(*this, *Fire))
				{
					//元の裏表のモデルを無効化する
					SetBothModelActiveFlag(false);
					//もう一つの裏表のモデルを有効化する
					m_otherModelRender[GetFrontOrBack()]->Activate();
					//オブジェクトが重なっているかを判定する処理を動かさないようにする。
					m_flagOverlap = false;
				}
				//trueを戻す
				return true;
			}
		);
	}
}

//アップデート関数
void RObird_fish::UpdateSub()
{
	//もう一つの裏表モデルの更新
	for (int i = 0; i < enFrontAndBackNum; i++)
	{
		//もう一つの裏表モデルの場所と回転と拡大を設定
		m_otherModelRender[i]->SetPosition(m_position);
		m_otherModelRender[i]->SetRotation(m_rotation);
		m_otherModelRender[i]->SetScale(m_scale);
	}
}

//表と裏をセットするサブ関数
void RObird_fish::SetFrontOrBackSub()
{
	//もう一つの裏表のタイプを設定する
	SetObjectType(m_reversibleType[GetFrontOrBack()]);
	//表か裏か？が表なら
	if (m_otherModelRender[GetFrontOrBack()])
		//表を有効化して表示する
		m_otherModelRender[GetFrontOrBack()]->Activate();

	//表か裏か？が裏なら
	if (m_otherModelRender[!GetFrontOrBack()])
		//裏を有効化して表示する
		m_otherModelRender[!GetFrontOrBack()]->Deactivate();
}