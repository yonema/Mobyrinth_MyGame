#include "stdafx.h"
#include "ROleft_right.h"

//スタート関数
bool ROleft_right::StartSub()
{
	//初期化用関数
	Init("Assets/modelData/left.tkm", EN_RO_TYPE_LEFT,
		"Assets/modelData/right.tkm", EN_RO_TYPE_RIGHT);


	return true;
}

/// <summary>
/// クエリしてほしいタイミングで呼ばれる関数
/// </summary>
void ROleft_right::QuerySub()
{
	//自身が「左」の時
	if (GetObjectType() == EN_RO_TYPE_LEFT)
	{

		//左右反転させるか？
		bool reverseFlag = true;

		//障害オブジェクトの「一方通行」をクエリ
		QueryLOs<OOoneway>(EN_OO_TYPE_ONEWAY, [&](OOoneway* oneway) -> bool
			{
				//自身と「一方通行」が衝突したら
				if (IsHitObject(*this, *oneway))
				{
					//「一方通行」の向きを左向きにする
					oneway->SetLeftOrRight(OOoneway::enLeft);
					reverseFlag = false;
					//自身は破棄する
					DeleteGO(this);
					//行動できなくする
					m_actionFlag = false;
				}

				//trueを戻す
				return true;
			}
		);

		if (reverseFlag)
			ObjectReverse();
	}
	//自身が「右」の時
	else if (GetObjectType() == EN_RO_TYPE_RIGHT)
	{

		//左右反転させるか？
		bool reverseFlag = true;

		//障害オブジェクトの「一方通行」をクエリ
		QueryLOs<OOoneway>(EN_OO_TYPE_ONEWAY, [&](OOoneway* oneway) -> bool
			{
				//自身と「一方通行」が衝突したら
				if (IsHitObject(*this, *oneway))
				{
					//「一方通行」の向きを右向きにする
					oneway->SetLeftOrRight(OOoneway::enRight);
					reverseFlag = false;
					//自身は破棄する
					DeleteGO(this);
					//行動できなくする
					m_actionFlag = false;
				}
				//trueを戻す
				return true;
			}
		);

		if (reverseFlag)
			ObjectReverse();
	}
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
		if (IsHitObject(*this, *lo))
		{
			//衝突していたら

			//行動可能か？
			if (m_actionFlag)
			{
				//可能

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
			}
		}
	}
}