#include "stdafx.h"
#include "OBBWorld.h"

//唯一のインスタンスの初期化
COBBWorld* COBBWorld::m_instance = nullptr;

/// <summary>
/// OBBをOBBWorldに登録する関数
/// </summary>
/// <param name="obb">登録するOBB</param>
void COBBWorld::AddOBB(const COBB& obb)
{
	//引数のOBBを登録する
	m_worldOBBs.push_back(&obb);
}

/// <summary>
/// OBBWorldに登録してあるOBBを解除する関数
/// </summary>
/// <param name="obb">登録するOBB</param>
void COBBWorld::RemoveOBB(const COBB& obb)
{
	//登録してあるOBB達を全部調べる
	std::vector<const COBB*>::iterator itr;
	for (itr = m_worldOBBs.begin(); itr != m_worldOBBs.end(); itr++)
	{
		//引数のOBBと一致するOBBの登録を解除する
		if ((*itr) == &obb)
		{
			m_worldOBBs.erase(itr);
			break;
		}
	}
}

/// <summary>
/// OBBWorldに登録してあるすべてのOBBを調べて、
/// 第一引数のOBBと衝突している一番近いOBBをを
/// 戻す。
/// どのOBBとも衝突していなかったら、nullptrが
/// 戻って来る。
/// </summary>
/// <param name="myOBB">自身のOBB</param>
/// <returns>衝突しているOBBか、nullptr</returns>
const COBB* COBBWorld::HitAllOBB(const COBB& myOBB)
{
	//一番近いOBBを探すための距離を入れる変数
	float dist = FLT_MAX;

	//衝突したOBBを保持しておくOBBのポインタ
	const COBB* pOBB = nullptr;

	//OBBWorldに登録してあるOBBを一つずつ取り出して、調べる
	for (auto OBB : m_worldOBBs)
	{
		//例外フラグを調べる
		if (OBB->GetExceptionFlag())
			//例外ならパス
			continue;

		//引数で渡された自身のOBBと、OBBWorldから取り出したOBBとの
		//衝突判定
		if (CollisionOBBs(myOBB, *OBB))
		{
			//衝突している

			//自身のOBBと、衝突したOBBとの間のベクトル
			Vector3 lenVec = myOBB.GetPosition() - OBB->GetPosition();
			//自身のOBBと、衝突したOBBとの間の距離
			float len = lenVec.Length();

			//距離が一番近いOBBを衝突したOBBとして保持しておく
			if (len <= dist)
			{
				//OBBを保持して
				pOBB = OBB;
				//一番近い距離も更新する
				dist = len;
			}
		}
	}

	//衝突したOBBがある場合は、衝突した一番近いOBBのポインタを、
	//衝突したOBBがない場合は、nullptrを戻す
	return pOBB;
}


