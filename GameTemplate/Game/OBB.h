#pragma once

struct SInitOBBData
{
	Vector3 position = g_vec3Zero;
	const float width = 1.0f;	//X�̕ӂ̒���
	const float length = 1.0f;	//Y�̕ӂ̒���
	const float height = 1.0f;	//Z�̕ӂ̒���
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
	//�I�u�W�F�N�g�̃��[�J���Ȏ�
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
	//�e���̒P�ʕ����x�N�g��
	Vector3 m_normalDirection[enLocalAxisNum];
	//�e���̕����x�N�g���̒���
	float m_directionLength[enLocalAxisNum];

};


const bool CollisionOBBs(COBB& obb1, COBB& obb2);

const float CalcProjectionLen(
	const Vector3& sepAxis,
	const Vector3& dirVec_X,
	const Vector3& dirVec_Y,
	const Vector3& dirVec_Z = { 0.0f,0.0f,0.0f }
);