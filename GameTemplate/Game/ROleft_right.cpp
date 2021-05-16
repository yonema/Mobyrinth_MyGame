#include "stdafx.h"
#include "ROleft_right.h"

//スタート関数
bool ROleft_right::StartSub()
{
	//初期化用関数
	Init("Assets/modelData/left.tkm", enLeftType,
		"Assets/modelData/right.tkm", enRightType);


	return true;
}

/// <summary>
/// クエリしてほしいタイミングで呼ばれる関数
/// </summary>
void ROleft_right::QuerySub()
{
	//自身が「左」の時
	if (GetObjectType() == enLeftType)
	{

		ObjectReverse();

		//障害オブジェクトの「一方通行」をクエリ
		QueryLOs<OOoneway>(enOneway, [&](OOoneway* oneway) -> bool
			{
				//自身と「一方通行」が衝突したら
				if (IsHitObject(*this, *oneway))
				{
					//「一方通行」の向きを左向きにする
					oneway->SetLeftOrRight(OOoneway::enLeft);
				}
				//trueを戻す
				return true;
			}
		);
	}
	//自身が「右」の時
	else if (GetObjectType() == enRightType)
	{
		ObjectReverse();

		//障害オブジェクトの「一方通行」をクエリ
		QueryLOs<OOoneway>(enOneway, [&](OOoneway* oneway) -> bool
			{
				//自身と「一方通行」が衝突したら
				if (IsHitObject(*this, *oneway))
				{
					//「一方通行」の向きを右向きにする
					oneway->SetLeftOrRight(OOoneway::enRight);
				}
				//trueを戻す
				return true;
			}
		);
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
				float xScale = 1.0f;
				if (GetObjectType() == enRightType)
					//右向きなら、左右反転させる
					xScale *= -1.0f;
				//衝突したオブジェクトのスケールを設定する
				lo->SetScale({ xScale,1.0f,1.0f });
				//自身は破棄する
				DeleteGO(this);
				//行動できなくする
				m_actionFlag = false;
			}
		}
	}
}