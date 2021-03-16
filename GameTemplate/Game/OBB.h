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
	//�I�u�W�F�N�g�̃��[�J���Ȏ�
	enum ENLocalAxis
	{
		enLocalX,
		enLocalY,
		enLocalZ,
		enLocalAxisNum,
	};
	//�e���̕����x�N�g��
	Vector3 m_normalDirection[enLocalAxisNum];
	//�e�������̒���
	float m_directionLength[enLocalAxisNum];
	Vector3 v;

};

