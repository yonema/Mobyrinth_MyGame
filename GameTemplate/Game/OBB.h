#pragma once

struct SInitOBBData
{
	Vector3 position = g_vec3Zero;
	const float width = 1.0f;	//Xの辺の長さ
	const float length = 1.0f;	//Yの辺の長さ
	const float height = 1.0f;	//Zの辺の長さ
	const Quaternion rotation = g_quatIdentity;
	Vector2 pivot = { 0.5f,0.5f };
};

class COBB
{
public:
	COBB();
	void Init(SInitOBBData& initData);
	const Vector3 GetPosition()const
	{
		return m_position;
	}
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}
	void SetRotation(const Quaternion& rot)
	{
		Rotating(rot);
	}

	const Vector3 GetCenterPosition()const
	{
		return m_centerPosition;
	}

	const Vector3 GetNormalDirection(const int num)const
	{
		return m_normalDirection[num];
	}
	const float GetDirectionLength(const int num)const
	{
		return m_directionLength[num];
	}
private:
	void Rotating(const Quaternion& rot);
	void Positioning(const Vector3& pos);
public:
	//オブジェクトのローカルな軸
	enum ENLocalAxis
	{
		enLocalX,
		enLocalY,
		enLocalZ,
		enLocalAxisNum,
	};
private:
	Vector3 m_position;
	Vector3 m_centerPosition;
	//各軸の単位方向ベクトル
	Vector3 m_normalDirection[enLocalAxisNum];
	//各軸の方向ベクトルの長さ
	float m_directionLength[enLocalAxisNum];

};



bool CollisionOBBs(COBB& obb1, COBB& obb2)
{
	//obb1の単位方向ベクトル
	Vector3 obb1NDir[COBB::enLocalAxisNum];
	//obb2の単位方向ベクトル
	Vector3 obb2NDir[COBB::enLocalAxisNum];

	//obb1の方向ベクトル（大きさ付き）
	Vector3 obb1DirVec[COBB::enLocalAxisNum];
	//obb2の方向ベクトル（大きさ付き）
	Vector3 obb2DirVec[COBB::enLocalAxisNum];


	//x,y,zの軸をそれぞれ順番に調べて、
	//単位方向ベクトルと、
	//大きさ付きの方向ベクトルを確保する。
	for (int axis = 0; axis < COBB::enLocalAxisNum; axis++)
	{
		//obb1の単位方向ベクトルを確保
		obb1NDir[axis] = obb1.GetNormalDirection(axis);

		//obb2の単位方向ベクトルを確保
		obb2NDir[axis] = obb2.GetNormalDirection(axis);



		//obb1の大きさ付の方向ベクトルを計算
		//単位方向ベクトルに、方向ベクトルの長さを乗算する
		obb1DirVec[axis] = obb1NDir[axis] * obb1.GetDirectionLength(axis);

		//obb2の大きさ付の方向ベクトルを計算
		//単位方向ベクトルに、方向ベクトルの長さを乗算する
		obb2DirVec[axis] = obb2NDir[axis] * obb2.GetDirectionLength(axis);
	}


	//obb1とobb2の中心点の間のベクトル
	Vector3 intervalVec = obb1.GetCenterPosition() - obb2.GetCenterPosition();




	//分離軸がobb1DirVec[COBB::enLocalX]だとすると

	//obb1の投影線分
	float obb1ProjectionLen = obb1DirVec[COBB::enLocalX].Length();








}