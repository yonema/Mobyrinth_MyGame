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

	//各軸の方向ベクトル
	Vector3 m_normalDirection[enLocalAxisNum];
	//各軸方向の長さ
	float m_directionLength[enLocalAxisNum];

};

