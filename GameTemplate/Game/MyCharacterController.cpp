#include "stdafx.h"
#include "MyCharacterController.h"

/// <summary>
/// 初期化用関数
/// </summary>
/// <param name="initOBBData">OBBの初期化用データ構造体</param>
void CMyCharacterController::Init(const SInitOBBData& initOBBData)
{
	//OBBを初期化する
	m_obb.Init(initOBBData);
}

/// <summary>
/// 実行関数。
/// 動くスピードとデルタタイムを渡すと、
/// OBBWorldに登録してあるOBBとの衝突解決をした
/// 移動後の座標を戻す。
/// </summary>
/// <param name="moveSpeed">動くスピード</param>
/// <param name="deltaTime">デルタタイム</param>
/// <returns>移動後の座標</returns>
const Vector3& CMyCharacterController::Execute(const Vector3& moveSpeed, const float deltaTime)
{
	m_tag = COBB::EB_NONE_SIDE;

	//移動前の座標
	Vector3 oldPosition = m_obb.GetPosition();

	//仮の移動後の座標
	Vector3 nextPosition = oldPosition;

	////移動していない時は、
	////衝突を気にしないで移動後の座標を戻す。
	////これがoldPositionだとなぜかうまくいかなかった。
	////ナゾ。。。
	//if (moveSpeed.Length() <= FLT_EPSILON)
	//{
	//	//OBBを仮の移動先に移動させる
	//	m_obb.SetPosition(nextPosition);

	//	//当たったときのOBBを入れておくポインタ
	//	m_hitOBB = nullptr;

	//	//COOWorldに登録してあるOBBから一番近い衝突しているOBBを探す
	//	//衝突したOBBがない場合はnullptrが戻ってくる
	//	m_hitOBB = COBBWorld::GetInstance()->HitAllOBB(m_obb);
	//	//衝突しているOBBが存在した
	//	if (m_hitOBB)
	//	{
	//		m_tag = m_hitOBB->GetTag();
	//	}
	//	return nextPosition;
	//}
		

	//移動するベクトル
	Vector3 addPos = moveSpeed;
	//デルタタイムを掛けておく
	addPos.Scale(deltaTime);

	//仮の移動後の座標を移動させる。
	nextPosition += addPos;

	//OBBを仮の移動先に移動させる
	m_obb.SetPosition(nextPosition);

	//当たったときのOBBを入れておくポインタ
	m_hitOBB = nullptr;

	//COOWorldに登録してあるOBBから一番近い衝突しているOBBを探す
	//衝突したOBBがない場合はnullptrが戻ってくる
	m_hitOBB = COBBWorld::GetInstance()->HitAllOBB(m_obb);



	//衝突しているOBBが存在した
	if (m_hitOBB)
	{
		m_tag = m_hitOBB->GetTag();


		//移動するベクトルの単位ベクトル
		Vector3 addNorm = addPos;
		addNorm.Normalize();	//正規化する

		//衝突したOBBのローカルなX軸の単位方向ベクトル
		//壁の法線ベクトルとして使う
		Vector3 hitOBBNormX = m_hitOBB->GetNormalDirection(COBB::enLocalX);

		//衝突したOBBの中央の座標から移動前の座標へのベクトル
		Vector3 hitOBBToOldPos = oldPosition - m_hitOBB->GetCenterPosition();
		//衝突したOBBの中央の座標から移動後の座標へのベクトル
		Vector3 hitOBBToNextPos = nextPosition - m_hitOBB->GetCenterPosition();
		//移動前へのベクトルと、壁の法線ベクトルの内積
		float oldDot = Dot(hitOBBToOldPos, hitOBBNormX);
		//移動後へのベクトルと、壁の法線ベクトルの内積
		float nextDot = Dot(hitOBBToNextPos, hitOBBNormX);


		//移動するベクトルと、壁の法線ベクトルの向きを比較する
		float addDot = Dot(addNorm, hitOBBNormX);

		bool backVecFlag = false;
		//両方同じ向き
		if (oldDot > 0.0f && nextDot > 0.0f)
		{
			//そのまま
			if (addDot <= 0.0f)
				backVecFlag = true;
		}
		//両方反対向き
		else if (oldDot <= 0.0f && nextDot <= 0.0f)
		{
			//反対向きにする
			hitOBBNormX.Scale(-1.0f);

			if (addDot >= 0.0f)
				backVecFlag = true;
		}
		//移動前が反対向きで、移動後が同じ向き
		else if (oldDot <= 0.0f && nextDot > 0.0f)
		{
			//反対向きにする
			hitOBBNormX.Scale(-1.0f);

			backVecFlag = true;
		}
		//移動前が同じ向きで、移動後が反対向き
		else if (oldDot > 0.0f && nextDot <= 0.0f)
		{
			//そのまま
			backVecFlag = true;
		}


		////同じ向きなら
		//if (dot > 0.0f)
		//{
		//	//反対向きにする
		//	hitOBBNormX.Scale(-1.0f);
		//}

		if (backVecFlag)
		{
			//当たったOBBの当たった側の壁の座標を計算する
			//まずは当たったOBBの座標を入れる
			Vector3 hitPos = m_hitOBB->GetPosition();
			//当たった側の壁の法線ベクトルに、長さを掛けて
			//当たった側の壁の座標を求める
			hitPos += hitOBBNormX * m_hitOBB->GetDirectionLength(COBB::enLocalX);

			//自身のOBBと壁の間の長さを、壁の法線ベクトルに射影する
			float between = Dot(m_obb.GetPosition() - hitPos, hitOBBNormX);

			//自身のOBBを壁の法線ベクトルに射影する
			float myObbLen =
				CalcProjectionLen(hitOBBNormX,
					m_obb.GetNormalDirection(COBB::enLocalX) * m_obb.GetDirectionLength(COBB::enLocalX),
					m_obb.GetNormalDirection(COBB::enLocalY) * m_obb.GetDirectionLength(COBB::enLocalY),
					m_obb.GetNormalDirection(COBB::enLocalZ) * m_obb.GetDirectionLength(COBB::enLocalZ)
				);

			//重なった分戻す長さ
			float backLen = myObbLen - between;

			//重なった分戻すベクトル
			Vector3 backVec = hitOBBNormX * backLen;

			//移動先の座標をかさなった分戻す。
			nextPosition += backVec;
		}

		//OBBを移動させる
		m_obb.SetPosition(nextPosition);
	}

	//最終的な移動先の座標を戻す
	return nextPosition;

}