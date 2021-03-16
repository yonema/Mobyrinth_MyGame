#pragma once

struct SInitOBBData
{
	Vector3 position = g_vec3Zero;
	const float lenght = 1.0f;
	const float width = 1.0f;
	const float leight = 1.0f;
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
private:
	Vector3 m_position;
	//オブジェクトのローカルな軸
	enum ENLocalAxis
	{
		enLocalX,
		enLocalY,
		enLocalZ,
		enLocalAxisNum,
	};
	//各軸の方向ベクトル
	Vector3 m_normalDirection[enLocalAxisNum];
	//各軸方向の長さ
	float m_directionLength[enLocalAxisNum];
	Vector3 v;

};

