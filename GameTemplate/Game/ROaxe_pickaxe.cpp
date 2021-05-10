#include "stdafx.h"
#include "ROaxe_pickaxe.h"

//スタート関数
bool ROaxe_pickaxe::StartSub()
{
	//初期化用関数
	Init("Assets/modelData/axe.tkm", enAxe,
		"Assets/modelData/pickaxe.tkm", enPickaxe);

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
void ROaxe_pickaxe::QuerySub()
{
	//自身が「斧」の時
	if (GetObjectType() == enAxe)
	{
		//障害オブジェクトの「箱」をクエリ
		QueryLOs<OObox>(enBox, [&](OObox* box) -> bool
			{
				//自身と「箱」が衝突したら
				if (IsHitObject(*this, *box))
				{
					//「箱」を破棄する
					DeleteGO(box);
					//自身を破棄する
					DeleteGO(this);
				}
				return true;
			}
		);
	}
	//自身が「つるはし」の時
	else if (GetObjectType() == enPickaxe)
	{
		//障害オブジェクトの「箱」をクエリ
		QueryLOs<OObox>(enBox, [&](OObox* box) -> bool
			{
				//自身と「箱」が衝突したら
				if (IsHitObject(*this, *box))
				{
					//「箱」を破棄する
					DeleteGO(box);
					//自身を破棄する
					DeleteGO(this);
				}
				return true;
			}
		);
	}
}