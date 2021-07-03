#include "stdafx.h"
#include "OBB.h"

//OBBの定数データを使えるようにする
using namespace OBBConstData;

//コンストラクタ
COBB::COBB()
{
	//各データメンバに初期値を入れる
	m_position = g_VEC3_ZERO;
	m_centerPosition = g_VEC3_ZERO;
	m_normalDirection[enLocalX] = g_VEC3_RIGHT;
	m_normalDirection[enLocalY] = g_VEC3_UP;
	m_normalDirection[enLocalZ] = g_VEC3_FRONT;

	for (int i = 0; i < enLocalAxisNum; i++)
	{
		m_directionLength[i] = 1.0f;
	}

}


/// <summary>
/// 初期化関数。
/// 最初に呼んでね。
/// </summary>
/// <param name="initData">COBBの初期化用の構造体</param>
void COBB::Init(const SInitOBBData& initData)
{
	//各軸の方向ベクトルの長さを設定
	m_directionLength[enLocalX] = initData.width / 2;
	m_directionLength[enLocalY] = initData.height / 2;
	m_directionLength[enLocalZ] = initData.length / 2;


	//ポジションを設定
	m_position = initData.position;

	//ピボットを設定
	m_pivot = initData.pivot;

	//回転を設定
	Rotating(initData.rotation);

	//センターポジションを計算する
	CalcCenterPosition();
}

/// <summary>
/// ボックスの8つの頂点の座標を取得する。
/// （注意）配列の先頭アドレスを戻しているけど、
/// 配列の要素数は8だよ。
/// 使うときは念のためGetBoxVertexNum()で
/// 頂点数持ってきてね。
/// </summary>
/// <returns>頂点の配列の先頭アドレス</returns>
Vector3* COBB::GetBoxVertex()
{

	//ボックスの頂点の場所
	Vector3 boxVertex[m_boxVertexNum];
	//ボックスの頂点にまずセンターポジションを入れる
	for (int i = 0; i < m_boxVertexNum; i++)
	{
		boxVertex[i] = m_centerPosition;
	}

	//各頂点をセンターポジションから方向ベクトル（大きさ付き）分動かして、
	//頂点の場所まで移動させる。
	boxVertex[0] -= m_normalDirection[enLocalX] * m_directionLength[enLocalX];
	boxVertex[0] += m_normalDirection[enLocalY] * m_directionLength[enLocalY];
	boxVertex[0] -= m_normalDirection[enLocalZ] * m_directionLength[enLocalZ];
	boxVertex[1] -= m_normalDirection[enLocalX] * m_directionLength[enLocalX];
	boxVertex[1] += m_normalDirection[enLocalY] * m_directionLength[enLocalY];
	boxVertex[1] += m_normalDirection[enLocalZ] * m_directionLength[enLocalZ];
	boxVertex[2] -= m_normalDirection[enLocalX] * m_directionLength[enLocalX];
	boxVertex[2] -= m_normalDirection[enLocalY] * m_directionLength[enLocalY];
	boxVertex[2] -= m_normalDirection[enLocalZ] * m_directionLength[enLocalZ];
	boxVertex[3] -= m_normalDirection[enLocalX] * m_directionLength[enLocalX];
	boxVertex[3] -= m_normalDirection[enLocalY] * m_directionLength[enLocalY];
	boxVertex[3] += m_normalDirection[enLocalZ] * m_directionLength[enLocalZ];
	boxVertex[4] += m_normalDirection[enLocalX] * m_directionLength[enLocalX];
	boxVertex[4] += m_normalDirection[enLocalY] * m_directionLength[enLocalY];
	boxVertex[4] -= m_normalDirection[enLocalZ] * m_directionLength[enLocalZ];
	boxVertex[5] += m_normalDirection[enLocalX] * m_directionLength[enLocalX];
	boxVertex[5] += m_normalDirection[enLocalY] * m_directionLength[enLocalY];
	boxVertex[5] += m_normalDirection[enLocalZ] * m_directionLength[enLocalZ];
	boxVertex[6] += m_normalDirection[enLocalX] * m_directionLength[enLocalX];
	boxVertex[6] -= m_normalDirection[enLocalY] * m_directionLength[enLocalY];
	boxVertex[6] -= m_normalDirection[enLocalZ] * m_directionLength[enLocalZ];
	boxVertex[7] += m_normalDirection[enLocalX] * m_directionLength[enLocalX];
	boxVertex[7] -= m_normalDirection[enLocalY] * m_directionLength[enLocalY];
	boxVertex[7] += m_normalDirection[enLocalZ] * m_directionLength[enLocalZ];


	//頂点の配列の先頭アドレスを戻す
	return boxVertex;
}


/// <summary>
/// センターポジションの取得
/// </summary>
/// <returns>センターポジション</returns>
void COBB::CalcCenterPosition()
{
	//ピボットをもとにセンターポジションを計算する

	//方向ベクトル（大きさ付き）
	Vector3 localVecX = m_normalDirection[enLocalX] * m_directionLength[enLocalX];
	Vector3 localVecY = m_normalDirection[enLocalY] * m_directionLength[enLocalY];
	Vector3 localVecZ = m_normalDirection[enLocalZ] * m_directionLength[enLocalZ];


	//OBBの左下手前の座標を入れる
	Vector3 centerPos = m_position;	//センターポジションを計算する
	centerPos -= localVecX;
	centerPos += localVecY;
	centerPos -= localVecZ;

	//左下手前の座標からセンターポジションへのベクトル
	Vector3 addVec;

	//X座標のピボットにセンターポジションを合わせる。
	addVec = localVecX * 2.0f * m_pivot.x;
	centerPos += addVec;
	//Y座標のピボットにセンターポジションを合わせる。
	addVec = localVecY * 2.0f * m_pivot.y;
	centerPos -= addVec;
	//Z座標のピボットにセンターポジションを合わせる。
	addVec = localVecZ * 2.0f * m_pivot.z;
	centerPos += addVec;

	//センターポジションを設定する。
	m_centerPosition = centerPos;
}

/// <summary>
/// 単位方向ベクトルを回す関数
/// </summary>
/// <param name="rot">回転</param>
void COBB::Rotating(const Quaternion& rot)
{
	//各軸の単位方向ベクトルを初期化してから
	m_normalDirection[enLocalX] = g_VEC3_RIGHT;
	m_normalDirection[enLocalY] = g_VEC3_UP;
	m_normalDirection[enLocalZ] = g_VEC3_FRONT;
	//クォータニオンで回す
	rot.Apply(m_normalDirection[enLocalX]);
	rot.Apply(m_normalDirection[enLocalY]);
	rot.Apply(m_normalDirection[enLocalZ]);

	return;
}


//クラス外
//クラスのメンバ関数ではない関数

/// <summary>
/// OBB同士の当たり判定
/// 当たったらtrueを戻す
/// </summary>
/// <param name="obb1">片方のOBB</param>
/// <param name="obb2">もう片方のOBB</param>
/// <returns>当たったかどうか</returns>
const bool CollisionOBBs(const COBB& obb1, const COBB& obb2)
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
		//二つのOBBの射影線分が、中心点間の距離より短いかどうか
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
		//分離軸にobb2のX,Y,Zをそれぞれを選ぶ
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
		float obb2ProjectionLen = obb2.GetDirectionLength(axis);


		//3.二つのOBBの射影線分が重なっているかの判定。

		//中心点間の距離を計算
		IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));

		//衝突判定
		//二つのOBBの射影線分が、中心点間の距離より短いかどうか
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
		CalcProjectionLen(SeparationAxis, obb1DirVec[COBB::enLocalY], obb1DirVec[COBB::enLocalZ]);

	//obb2の射影線分
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2DirVec[COBB::enLocalY], obb2DirVec[COBB::enLocalZ]);

	//3.二つのOBBの射影線分が重なっているかの判定。

	//中心点間の距離を計算
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));

	//衝突判定
	//二つのOBBの射影線分が、中心点間の距離より短いかどうか
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//衝突していない

	//2
	//〇分離軸：XY

	//1.OBBの分離軸を探す。
	SeparationAxis = Cross(obb1NDir[COBB::enLocalX], obb2NDir[COBB::enLocalY]);
	//2.分離軸にOBBを射影する。
	//obb1の射影線分
	obb1ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb1DirVec[COBB::enLocalY], obb1DirVec[COBB::enLocalZ]);
	//obb2の射影線分
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2DirVec[COBB::enLocalX], obb2DirVec[COBB::enLocalZ]);
	//3.二つのOBBの射影線分が重なっているかの判定。
	//中心点間の距離を計算
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));
	//衝突判定
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//衝突していない

	//3
	//〇分離軸：XZ

	//1.OBBの分離軸を探す。
	SeparationAxis = Cross(obb1NDir[COBB::enLocalX], obb2NDir[COBB::enLocalZ]);
	//2.分離軸にOBBを射影する。
	//obb1の射影線分
	obb1ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb1DirVec[COBB::enLocalY], obb1DirVec[COBB::enLocalZ]);
	//obb2の射影線分
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2DirVec[COBB::enLocalX], obb2DirVec[COBB::enLocalY]);
	//3.二つのOBBの射影線分が重なっているかの判定。
	//中心点間の距離を計算
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));
	//衝突判定
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//衝突していない


	//4
	//〇分離軸：YX

	//1.OBBの分離軸を探す。
	SeparationAxis = Cross(obb1NDir[COBB::enLocalY], obb2NDir[COBB::enLocalX]);
	//2.分離軸にOBBを射影する。
	//obb1の射影線分
	obb1ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb1DirVec[COBB::enLocalX], obb1DirVec[COBB::enLocalZ]);
	//obb2の射影線分
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2DirVec[COBB::enLocalY], obb2DirVec[COBB::enLocalZ]);
	//3.二つのOBBの射影線分が重なっているかの判定。
	//中心点間の距離を計算
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));
	//衝突判定
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//衝突していない

	//5
	//〇分離軸：YY

	//1.OBBの分離軸を探す。
	SeparationAxis = Cross(obb1NDir[COBB::enLocalY], obb2NDir[COBB::enLocalY]);
	//2.分離軸にOBBを射影する。
	//obb1の射影線分
	obb1ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb1DirVec[COBB::enLocalX], obb1DirVec[COBB::enLocalZ]);
	//obb2の射影線分
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2DirVec[COBB::enLocalX], obb2DirVec[COBB::enLocalZ]);
	//3.二つのOBBの射影線分が重なっているかの判定。
	//中心点間の距離を計算
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));
	//衝突判定
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//衝突していない

	//6
	//〇分離軸：YZ

	//1.OBBの分離軸を探す。
	SeparationAxis = Cross(obb1NDir[COBB::enLocalY], obb2NDir[COBB::enLocalZ]);
	//2.分離軸にOBBを射影する。
	//obb1の射影線分
	obb1ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb1DirVec[COBB::enLocalX], obb1DirVec[COBB::enLocalZ]);
	//obb2の射影線分
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2DirVec[COBB::enLocalX], obb2DirVec[COBB::enLocalY]);
	//3.二つのOBBの射影線分が重なっているかの判定。
	//中心点間の距離を計算
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));
	//衝突判定
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//衝突していない

	//7
	//〇分離軸：ZX

	//1.OBBの分離軸を探す。
	SeparationAxis = Cross(obb1NDir[COBB::enLocalZ], obb2NDir[COBB::enLocalX]);
	//2.分離軸にOBBを射影する。
	//obb1の射影線分
	obb1ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb1DirVec[COBB::enLocalX], obb1DirVec[COBB::enLocalY]);
	//obb2の射影線分
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2DirVec[COBB::enLocalY], obb2DirVec[COBB::enLocalZ]);
	//3.二つのOBBの射影線分が重なっているかの判定。
	//中心点間の距離を計算
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));
	//衝突判定
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//衝突していない

	//8
	//〇分離軸：ZY

	//1.OBBの分離軸を探す。
	SeparationAxis = Cross(obb1NDir[COBB::enLocalZ], obb2NDir[COBB::enLocalY]);
	//2.分離軸にOBBを射影する。
	//obb1の射影線分
	obb1ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb1DirVec[COBB::enLocalX], obb1DirVec[COBB::enLocalY]);
	//obb2の射影線分
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2DirVec[COBB::enLocalX], obb2DirVec[COBB::enLocalZ]);
	//3.二つのOBBの射影線分が重なっているかの判定。
	//中心点間の距離を計算
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));
	//衝突判定
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//衝突していない

	//9
	//〇分離軸：ZZ

	//1.OBBの分離軸を探す。
	SeparationAxis = Cross(obb1NDir[COBB::enLocalZ], obb2NDir[COBB::enLocalZ]);
	//2.分離軸にOBBを射影する。
	//obb1の射影線分
	obb1ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb1DirVec[COBB::enLocalX], obb1DirVec[COBB::enLocalY]);
	//obb2の射影線分
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2DirVec[COBB::enLocalX], obb2DirVec[COBB::enLocalY]);
	//3.二つのOBBの射影線分が重なっているかの判定。
	//中心点間の距離を計算
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));
	//衝突判定
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//衝突していない



	//分離平面が存在しないので
	
	return true;		//衝突している

}


/// <summary>
/// 射影線分を計算する関数
/// </summary>
/// <param name="sepAxis">射影される分離軸（正規化済みを渡すこと）</param>
/// <param name="dirVec_X">射影する方向ベクトルX（大きさ付きを渡すこと）</param>
/// <param name="dirVec_Y">射影する方向ベクトルY（大きさ付きを渡すこと）</param>
/// <param name="dirVec_Z">射影する方向ベクトルZ（大きさ付きを渡すこと）(Zはなくても良い)</param>
/// <returns>射影線分</returns>
const float CalcProjectionLen(
	const Vector3& sepAxis,
	const Vector3& dirVec_X,
	const Vector3& dirVec_Y,
	const Vector3& dirVec_Z
)
{
	//方向ベクトルXの射影線分
	float projX = std::abs(Dot(sepAxis, dirVec_X));
	//方向ベクトルYの射影線分
	float projY = std::abs(Dot(sepAxis, dirVec_Y));
	//方向ベクトルZの射影線分
	float projZ = std::abs(Dot(sepAxis, dirVec_Z));

	//X,Y,Zの射影線分の合計を戻す。
	return projX + projY + projZ;
}