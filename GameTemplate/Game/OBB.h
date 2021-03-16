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



bool CollisionOBBs(COBB& obb1, COBB& obb2)
{
	//obb1�̒P�ʕ����x�N�g��
	Vector3 obb1NDir[COBB::enLocalAxisNum];
	//obb2�̒P�ʕ����x�N�g��
	Vector3 obb2NDir[COBB::enLocalAxisNum];

	//obb1�̕����x�N�g���i�傫���t���j
	Vector3 obb1DirVec[COBB::enLocalAxisNum];
	//obb2�̕����x�N�g���i�傫���t���j
	Vector3 obb2DirVec[COBB::enLocalAxisNum];


	//x,y,z�̎������ꂼ�ꏇ�Ԃɒ��ׂāA
	//�P�ʕ����x�N�g���ƁA
	//�傫���t���̕����x�N�g�����m�ۂ���B
	for (int axis = 0; axis < COBB::enLocalAxisNum; axis++)
	{
		//obb1�̒P�ʕ����x�N�g�����m��
		obb1NDir[axis] = obb1.GetNormalDirection(axis);

		//obb2�̒P�ʕ����x�N�g�����m��
		obb2NDir[axis] = obb2.GetNormalDirection(axis);



		//obb1�̑傫���t�̕����x�N�g�����v�Z
		//�P�ʕ����x�N�g���ɁA�����x�N�g���̒�������Z����
		obb1DirVec[axis] = obb1NDir[axis] * obb1.GetDirectionLength(axis);

		//obb2�̑傫���t�̕����x�N�g�����v�Z
		//�P�ʕ����x�N�g���ɁA�����x�N�g���̒�������Z����
		obb2DirVec[axis] = obb2NDir[axis] * obb2.GetDirectionLength(axis);
	}


	//obb1��obb2�̒��S�_�̊Ԃ̃x�N�g��
	Vector3 intervalVec = obb1.GetCenterPosition() - obb2.GetCenterPosition();




	//��������obb1DirVec[COBB::enLocalX]���Ƃ����

	//obb1�̓��e����
	float obb1ProjectionLen = obb1DirVec[COBB::enLocalX].Length();








}