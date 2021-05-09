#include "stdafx.h"
#include "ROleft_right.h"

//スタート関数
bool ROleft_right::StartSub()
{
	//初期化用関数
	Init("Assets/modelData/left.tkm", enLeftType,
		"Assets/modelData/right.tkm", enRightType);

	//OBBのサイズを設定
	Vector3 obbSize;
	obbSize = { 200.0f,200.0f,400.0f };
	//OBBの方向ベクトルの長さを設定
	GetOBB().SetDirectionLength(obbSize);

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

void ROleft_right::ObjectReverse()
{
	std::vector<ILevelObjectBase*> levelObjects =
		CLevelObjectManager::GetInstance()->GetLevelObjects();

	for (auto lo : levelObjects)
	{
		if (lo == this)
			continue;

		if (IsHitObject(*this, *lo))
		{
			if (m_actionFlag)
			{
				float xScale = 1.0f;
				if (GetObjectType() == enRightType)
					xScale *= -1.0f;
				lo->SetScale({ xScale,1.0f,1.0f });
				//自身は破棄する
				DeleteGO(this);
				//行動できなくする
				m_actionFlag = false;
			}
		}
	}
}