#include "stdafx.h"
#include "OBB.h"

COBB::COBB()
{
	m_position = g_vec3Zero;
	m_centerPosition = g_vec3Zero;
	m_normalDirection[enLocalX] = g_vec3Right;
	m_normalDirection[enLocalY] = g_vec3Up;
	m_normalDirection[enLocalZ] = g_vec3Front;

	for (int i = 0; i < enLocalAxisNum; i++)
	{
		m_directionLength[i] = 1.0f;
	}

}

void COBB::Init(SInitOBBData& initData)
{
	m_position = initData.position;

	//後でpivotに合わせたセンターポジションにできるようにしよう。
	m_centerPosition = m_position;

	m_directionLength[enLocalX] = initData.width / 2;
	m_directionLength[enLocalY] = initData.length / 2;
	m_directionLength[enLocalZ] = initData.height / 2;

	Rotating(initData.rotation);

}
void COBB::Positioning(const Vector3& pos)
{

}

void COBB::Rotating(const Quaternion& rot)
{
	m_normalDirection[enLocalX] = g_vec3Right;
	m_normalDirection[enLocalY] = g_vec3Up;
	m_normalDirection[enLocalZ] = g_vec3Front;
	rot.Apply(m_normalDirection[enLocalX]);
	rot.Apply(m_normalDirection[enLocalY]);
	rot.Apply(m_normalDirection[enLocalZ]);

	return;
}




const bool CollisionOBBs(COBB& obb1, COBB& obb2)
{
	///
	///
	/// 「分離軸判定」
	/// 1.OBBの分離軸を探す。
	///		辺に垂直な分離軸を探す。OBBは直方体のため、辺に垂直なベクトルは
	///		その辺に隣接している辺を分離軸として使うことができる。（正規化すること）
	///		つまり、単位方向ベクトルをそのまま分離軸として使う。
	/// 2.分離軸にOBBを射影する。
	///		各軸の方向ベクトル（大きさ付き）と分離軸（正規化済み）の内積の絶対値を
	///		足したものが射影した長さの半分になる。
	/// 3.二つのOBBの射影線分が重なっているかの判定。
	///		二つのOBBの中心点間の長さが、
	///		それぞれの半射影線分の長さの合計より
	///		長かったら分離している。
	/// 4.3次元のOBBの場合「双方の方向ベクトルに垂直な分離軸」でも判定する。
	///		双方のOBBの方向ベクトルの外積で求めることができる。
	///		
	///		立体のOBB同士の衝突判定には3+3+9=15本の分離軸を精査する。
	///		・片方のOBBのそれぞれの単位方向ベクトル	：3本
	///		・他方のOBBのそれぞれの単位方向ベクトル	：3本
	///		・双方の方向ベクトルに垂直な分離軸		：3*3=9本
	/// 



	//obb1の単位方向ベクトル（正規化済み）
	Vector3 obb1NDir[COBB::enLocalAxisNum];

	//obb1の単位方向ベクトル（正規化済み）
	Vector3 obb2NDir[COBB::enLocalAxisNum];

	//obb1の方向ベクトル（大きさ付き）
	Vector3 obb1DirVec[COBB::enLocalAxisNum];

	//obb2の方向ベクトル（大きさ付き）
	Vector3 obb2DirVec[COBB::enLocalAxisNum];


	//x,y,zの軸をそれぞれ順番に調べて、
	//大きさ付きの方向ベクトルを確保する。
	for (int axis = 0; axis < COBB::enLocalAxisNum; axis++)
	{
		//obb1の単位方向ベクトルを確保（正規化済み）
		obb1NDir[axis] = obb1.GetNormalDirection(axis);

		//obb2の単位方向ベクトルを確保（正規化済み）
		obb2NDir[axis] = obb2.GetNormalDirection(axis);


		//obb1の大きさ付の方向ベクトルを計算
		//単位方向ベクトルに、方向ベクトルの長さを乗算する
		obb1DirVec[axis] = obb1NDir[axis] * obb1.GetDirectionLength(axis);

		//obb2の大きさ付の方向ベクトルを計算
		//単位方向ベクトルに、方向ベクトルの長さを乗算する
		obb2DirVec[axis] = obb2NDir[axis] * obb2.GetDirectionLength(axis);
	}



	//obb1とobb2の中心点の間のベクトル
	Vector3 obb2ToObb1Vec = obb1.GetCenterPosition() - obb2.GetCenterPosition();

	//中心点間の距離
	float IntervalLen;

	//分離軸
	Vector3 SeparationAxis;

	//obb1の射影線分
	float obb1ProjectionLen;

	//obb2の射影線分
	float obb2ProjectionLen;

	//〇分離軸：obb1のX,Y,Z

	//分離軸にobb1の方向ベクトルを使って、
	//X,Y,Zを順番に精査していく。
	//全部で3回
	for (int axis = 0; axis < COBB::enLocalAxisNum; axis++)
	{
		//1.OBBの分離軸を探す。
		//分離軸にobb1のX,Y,Zをそれぞれを選ぶ
		SeparationAxis = obb1NDir[axis];

		//2.分離軸にOBBを射影する。
		//obb1の射影線分
		obb1ProjectionLen = obb1.GetDirectionLength(axis);

		//obb2の射影線分
		obb2ProjectionLen =
			CalcProjectionLen(
				SeparationAxis,
				obb2DirVec[COBB::enLocalX],
				obb2DirVec[COBB::enLocalY],
				obb2DirVec[COBB::enLocalZ]);

		//3.二つのOBBの射影線分が重なっているかの判定。

		//中心点間の距離を計算
		IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));

		//衝突判定
		if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
			return false;	//衝突していない

	}

	//〇分離軸：obbのX,Y,Z

	//分離軸にobb2の方向ベクトルを使って、
	//X,Y,Zを順番に精査していく。
	//全部で3回
	for (int axis = 0; axis < COBB::enLocalAxisNum; axis++)
	{
		//1.OBBの分離軸を探す。
		//分離軸にobb1のX,Y,Zをそれぞれを選ぶ
		SeparationAxis = obb2NDir[axis];

		//2.分離軸にOBBを射影する。
		//obb1の射影線分
		float obb1ProjectionLen =
			CalcProjectionLen(
				SeparationAxis,
				obb1DirVec[COBB::enLocalX],
				obb1DirVec[COBB::enLocalY],
				obb1DirVec[COBB::enLocalZ]);

		//obb2の射影線分
		float obb2ProjectionLen = obb1.GetDirectionLength(axis);


		//3.二つのOBBの射影線分が重なっているかの判定。

		//中心点間の距離を計算
		IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));

		//衝突判定
		if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
			return false;	//衝突していない

	}


	//4.3次元のOBBの場合「双方の方向ベクトルに垂直な分離軸」でも判定する。
	//3*3=9 全部で9回

	//1
	//〇分離軸：XX

	//1.OBBの分離軸を探す。
	//双方の方向ベクトルに垂直な分離軸を探す。
	//それぞれの方向ベクトルの外積で求める。
	SeparationAxis = Cross(obb1NDir[COBB::enLocalX], obb2NDir[COBB::enLocalX]);

	//2.分離軸にOBBを射影する。
	//obb1の射影線分
	obb1ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb1NDir[COBB::enLocalY], obb1NDir[COBB::enLocalZ]);

	//obb2の射影線分
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2NDir[COBB::enLocalY], obb2NDir[COBB::enLocalZ]);

	//3.二つのOBBの射影線分が重なっているかの判定。

	//中心点間の距離を計算
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));

	//衝突判定
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//衝突していない

	//2
	//〇分離軸：XY

	//1.OBBの分離軸を探す。
	SeparationAxis = Cross(obb1NDir[COBB::enLocalX], obb2NDir[COBB::enLocalY]);
	//2.分離軸にOBBを射影する。
	//obb1の射影線分
	obb1ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb1NDir[COBB::enLocalY], obb1NDir[COBB::enLocalZ]);
	//obb2の射影線分
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2NDir[COBB::enLocalX], obb2NDir[COBB::enLocalZ]);
	//3.二つのOBBの射影線分が重なっているかの判定。
	//中心点間の距離を計算
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));
	//衝突判定
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//衝突していない

	//3
	//〇分離軸：XY

	//1.OBBの分離軸を探す。
	SeparationAxis = Cross(obb1NDir[COBB::enLocalX], obb2NDir[COBB::enLocalX]);
	//2.分離軸にOBBを射影する。
	//obb1の射影線分
	obb1ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb1NDir[COBB::enLocalY], obb1NDir[COBB::enLocalZ]);
	//obb2の射影線分
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2NDir[COBB::enLocalY], obb2NDir[COBB::enLocalZ]);
	//3.二つのOBBの射影線分が重なっているかの判定。
	//中心点間の距離を計算
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));
	//衝突判定
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//衝突していない


	//3
	//〇分離軸：XY

	//1.OBBの分離軸を探す。
	SeparationAxis = Cross(obb1NDir[COBB::enLocalX], obb2NDir[COBB::enLocalX]);
	//2.分離軸にOBBを射影する。
	//obb1の射影線分
	obb1ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb1NDir[COBB::enLocalY], obb1NDir[COBB::enLocalZ]);
	//obb2の射影線分
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2NDir[COBB::enLocalY], obb2NDir[COBB::enLocalZ]);
	//3.二つのOBBの射影線分が重なっているかの判定。
	//中心点間の距離を計算
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));
	//衝突判定
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//衝突していない


}


const float CalcProjectionLen(
	const Vector3& sepAxis,
	const Vector3& dirVec_X,
	const Vector3& dirVec_Y,
	const Vector3& dirVec_Z
)
{
	float proj1 = std::abs(Dot(sepAxis, dirVec_X));
	float proj2 = std::abs(Dot(sepAxis, dirVec_Y));
	float proj3 = std::abs(Dot(sepAxis, dirVec_Z));

	return proj1 + proj2 + proj3;
}