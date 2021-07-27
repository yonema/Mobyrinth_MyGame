#include "stdafx.h"
#include "ROleft_right.h"

//ReversibleObjectのモデルのファイルパスの定数データを使用可能にする
using namespace ROsFilepathConstdata;

//スタート関数
bool ROleft_right::StartSub()
{
	//初期化用関数
	Init(MODEL_FILEPATH_LEFT, EN_RO_TYPE_LEFT,
		MODEL_FILEPATH_RIGHT, EN_RO_TYPE_RIGHT);


	return true;
}

/// <summary>
/// クエリしてほしいタイミングで呼ばれる関数
/// </summary>
void ROleft_right::QuerySub()
{

	//障害オブジェクトの「一方通行」をクエリ
	QueryLOs<OOoneway>(EN_OO_TYPE_ONEWAY, [&](OOoneway* oneway) -> bool
		{
			//「一方通行」と衝突した時の処理
			HitOneway(oneway);

			//trueを戻す
			return true;
		}
	);

	//行動できるか？
	if (m_actionFlag)
		//当たったときオブジェクトを左右反転させる
		ObjectReverse();

	return;
}

/**
 * @brief 「一方通行」と衝突した時の処理
 * @param [in] oneway 「一方通行」のポインタ
*/
void ROleft_right::HitOneway(OOoneway* oneway)
{
	//行動できるか？
	if (m_actionFlag)
	{
		//自身と「一方通行」が衝突したら
		if (IsHitLevelObject(*this, *oneway))
		{
			//右か左か？
			//デフォは左
			EN_LEFT_OR_RIGHT leftOrRight = EN_LEFT;

			//自身が「右」の時
			if (GetObjectType() == EN_RO_TYPE_RIGHT)
				//右にする
				leftOrRight = EN_RIGHT;

			//「一方通行」の向きを左向きにする
			oneway->SetLeftOrRight(leftOrRight);
			//行動できなくする
			m_actionFlag = false;
			//自身は破棄する
			DeleteGO(this);
		}
	}

	return;
}

/// <summary>
/// 当たったオブジェクトを左右反転させる
/// </summary>
void ROleft_right::ObjectReverse()
{
	//配置してあるすべてのレベルオブジェクトの参照のベクター
	std::vector<ILevelObjectBase*> levelObjects =
		CLevelObjectManager::GetInstance()->GetLevelObjects();

	//レベルオブジェクトたちを一つずつ取り出す
	for (auto lo : levelObjects)
	{

		//自分自身の時はスキップ
		if (lo == this)
			continue;

		//自身とオブジェクトが衝突しているか？
		if (IsHitLevelObject(*this, *lo))
		{
			//衝突していたら



				//左右反転する方向
			Vector3 scale = lo->GetScale();
			if (GetObjectType() == EN_RO_TYPE_RIGHT)
				//右向きなら、左右反転させる
				scale.x *= -1.0f;
			//衝突したオブジェクトのスケールを設定する
			lo->SetScale(scale);
			//自身は破棄する
			DeleteGO(this);
			//行動できなくする
			m_actionFlag = false;

			//これ以上行動しなくて良いので、returnする
			return;

		}
	}

	return;
}