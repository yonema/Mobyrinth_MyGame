#include "stdafx.h"
#include "ROaxe_pickaxe.h"

//スタート関数
bool ROaxe_pickaxe::StartSub()
{
	//初期化用関数
	Init("Assets/modelData/axe.tkm", enAxe,
		"Assets/modelData/pickaxe.tkm", enPickaxe);



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