#include "stdafx.h"
#include "MyCharacterController.h"


void CMyCharacterController::Init(const SInitOBBData& initOBBData)
{
	m_obb.Init(initOBBData);
}

const Vector3& CMyCharacterController::Execute(const Vector3& moveSpeed, const float deltaTime)
{
	//移動前の座標
	Vector3 oldPosition = m_obb.GetPosition();

	//仮の移動後の座標
	Vector3 nextPosition = oldPosition;

	if (moveSpeed.Length() <= 0.001f)
		return nextPosition;

	//移動するベクトル
	Vector3 addPos = moveSpeed;
	//デルタタイムを掛けておく
	addPos.Scale(deltaTime);

	//仮の移動後の座標を移動させる。
	nextPosition += addPos;



	//OBBを仮の移動先に移動させる
	m_obb.SetPosition(nextPosition);




	//当たったときのOBBを入れておくポインタ
	COBB* hitOBB = nullptr;

	//COOWorldに登録してあるOBBから一番近い衝突しているOBBを探す
	hitOBB = COBBWorld::GetInstance()->HitAllOBB(m_obb, hitOBB);

	if (hitOBB)
	{

		//衝突しているOBBが存在した

		//移動するベクトルの単位ベクトル
		Vector3 addNorm = addPos;
		addNorm.Normalize();	//正規化する

		Vector3 myOBBNormX = m_obb.GetNormalDirection(COBB::enLocalX);


		Vector3 hitOBBNormX = hitOBB->GetNormalDirection(COBB::enLocalX);

		//移動するベクトルと、戻りベクトルの向きを比べる
		float dot = Dot(addNorm, hitOBBNormX);

		//同じ向きなら
		if (dot > 0.0f)
		{
			//反対向きにする
			myOBBNormX.Scale(-1.0f);
			hitOBBNormX.Scale(-1.0f);
		}

		//当たったOBBの当たった側の壁の座標
		Vector3 hitPos = hitOBB->GetPosition();
		hitPos += hitOBBNormX * hitOBB->GetDirectionLength(COBB::enLocalX);

		float between = Dot(m_obb.GetPosition() - hitPos, hitOBBNormX);

		float myObbLen =
		CalcProjectionLen(hitOBBNormX,
			m_obb.GetNormalDirection(COBB::enLocalX) * m_obb.GetDirectionLength(COBB::enLocalX),
			m_obb.GetNormalDirection(COBB::enLocalY) * m_obb.GetDirectionLength(COBB::enLocalY),
			m_obb.GetNormalDirection(COBB::enLocalZ) * m_obb.GetDirectionLength(COBB::enLocalZ)
			);


		float backLen = myObbLen - between;

		Vector3 backVec = hitOBBNormX * backLen;

		nextPosition += backVec;
		//OBBを移動させる
		m_obb.SetPosition(nextPosition);
	}

	//最終的な移動先の座標を戻す
	return nextPosition;

}