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

	//���pivot�ɍ��킹���Z���^�[�|�W�V�����ɂł���悤�ɂ��悤�B
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
	/// �u����������v
	/// 1.OBB�̕�������T���B
	///		�ӂɐ����ȕ�������T���BOBB�͒����̂̂��߁A�ӂɐ����ȃx�N�g����
	///		���̕ӂɗאڂ��Ă���ӂ𕪗����Ƃ��Ďg�����Ƃ��ł���B�i���K�����邱�Ɓj
	///		�܂�A�P�ʕ����x�N�g�������̂܂ܕ������Ƃ��Ďg���B
	/// 2.��������OBB���ˉe����B
	///		�e���̕����x�N�g���i�傫���t���j�ƕ������i���K���ς݁j�̓��ς̐�Βl��
	///		���������̂��ˉe���������̔����ɂȂ�B
	/// 3.���OBB�̎ˉe�������d�Ȃ��Ă��邩�̔���B
	///		���OBB�̒��S�_�Ԃ̒������A
	///		���ꂼ��̔��ˉe�����̒����̍��v���
	///		���������番�����Ă���B
	/// 4.3������OBB�̏ꍇ�u�o���̕����x�N�g���ɐ����ȕ������v�ł����肷��B
	///		�o����OBB�̕����x�N�g���̊O�ςŋ��߂邱�Ƃ��ł���B
	///		
	///		���̂�OBB���m�̏Փ˔���ɂ�3+3+9=15�{�̕������𐸍�����B
	///		�E�Е���OBB�̂��ꂼ��̒P�ʕ����x�N�g��	�F3�{
	///		�E������OBB�̂��ꂼ��̒P�ʕ����x�N�g��	�F3�{
	///		�E�o���̕����x�N�g���ɐ����ȕ�����		�F3*3=9�{
	/// 



	//obb1�̒P�ʕ����x�N�g���i���K���ς݁j
	Vector3 obb1NDir[COBB::enLocalAxisNum];

	//obb1�̒P�ʕ����x�N�g���i���K���ς݁j
	Vector3 obb2NDir[COBB::enLocalAxisNum];

	//obb1�̕����x�N�g���i�傫���t���j
	Vector3 obb1DirVec[COBB::enLocalAxisNum];

	//obb2�̕����x�N�g���i�傫���t���j
	Vector3 obb2DirVec[COBB::enLocalAxisNum];


	//x,y,z�̎������ꂼ�ꏇ�Ԃɒ��ׂāA
	//�傫���t���̕����x�N�g�����m�ۂ���B
	for (int axis = 0; axis < COBB::enLocalAxisNum; axis++)
	{
		//obb1�̒P�ʕ����x�N�g�����m�ہi���K���ς݁j
		obb1NDir[axis] = obb1.GetNormalDirection(axis);

		//obb2�̒P�ʕ����x�N�g�����m�ہi���K���ς݁j
		obb2NDir[axis] = obb2.GetNormalDirection(axis);


		//obb1�̑傫���t�̕����x�N�g�����v�Z
		//�P�ʕ����x�N�g���ɁA�����x�N�g���̒�������Z����
		obb1DirVec[axis] = obb1NDir[axis] * obb1.GetDirectionLength(axis);

		//obb2�̑傫���t�̕����x�N�g�����v�Z
		//�P�ʕ����x�N�g���ɁA�����x�N�g���̒�������Z����
		obb2DirVec[axis] = obb2NDir[axis] * obb2.GetDirectionLength(axis);
	}



	//obb1��obb2�̒��S�_�̊Ԃ̃x�N�g��
	Vector3 obb2ToObb1Vec = obb1.GetCenterPosition() - obb2.GetCenterPosition();

	//���S�_�Ԃ̋���
	float IntervalLen;

	//������
	Vector3 SeparationAxis;

	//obb1�̎ˉe����
	float obb1ProjectionLen;

	//obb2�̎ˉe����
	float obb2ProjectionLen;

	//�Z�������Fobb1��X,Y,Z

	//��������obb1�̕����x�N�g�����g���āA
	//X,Y,Z�����Ԃɐ������Ă����B
	//�S����3��
	for (int axis = 0; axis < COBB::enLocalAxisNum; axis++)
	{
		//1.OBB�̕�������T���B
		//��������obb1��X,Y,Z�����ꂼ���I��
		SeparationAxis = obb1NDir[axis];

		//2.��������OBB���ˉe����B
		//obb1�̎ˉe����
		obb1ProjectionLen = obb1.GetDirectionLength(axis);

		//obb2�̎ˉe����
		obb2ProjectionLen =
			CalcProjectionLen(
				SeparationAxis,
				obb2DirVec[COBB::enLocalX],
				obb2DirVec[COBB::enLocalY],
				obb2DirVec[COBB::enLocalZ]);

		//3.���OBB�̎ˉe�������d�Ȃ��Ă��邩�̔���B

		//���S�_�Ԃ̋������v�Z
		IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));

		//�Փ˔���
		if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
			return false;	//�Փ˂��Ă��Ȃ�

	}

	//�Z�������Fobb��X,Y,Z

	//��������obb2�̕����x�N�g�����g���āA
	//X,Y,Z�����Ԃɐ������Ă����B
	//�S����3��
	for (int axis = 0; axis < COBB::enLocalAxisNum; axis++)
	{
		//1.OBB�̕�������T���B
		//��������obb1��X,Y,Z�����ꂼ���I��
		SeparationAxis = obb2NDir[axis];

		//2.��������OBB���ˉe����B
		//obb1�̎ˉe����
		float obb1ProjectionLen =
			CalcProjectionLen(
				SeparationAxis,
				obb1DirVec[COBB::enLocalX],
				obb1DirVec[COBB::enLocalY],
				obb1DirVec[COBB::enLocalZ]);

		//obb2�̎ˉe����
		float obb2ProjectionLen = obb1.GetDirectionLength(axis);


		//3.���OBB�̎ˉe�������d�Ȃ��Ă��邩�̔���B

		//���S�_�Ԃ̋������v�Z
		IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));

		//�Փ˔���
		if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
			return false;	//�Փ˂��Ă��Ȃ�

	}


	//4.3������OBB�̏ꍇ�u�o���̕����x�N�g���ɐ����ȕ������v�ł����肷��B
	//3*3=9 �S����9��

	//1
	//�Z�������FXX

	//1.OBB�̕�������T���B
	//�o���̕����x�N�g���ɐ����ȕ�������T���B
	//���ꂼ��̕����x�N�g���̊O�ςŋ��߂�B
	SeparationAxis = Cross(obb1NDir[COBB::enLocalX], obb2NDir[COBB::enLocalX]);

	//2.��������OBB���ˉe����B
	//obb1�̎ˉe����
	obb1ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb1NDir[COBB::enLocalY], obb1NDir[COBB::enLocalZ]);

	//obb2�̎ˉe����
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2NDir[COBB::enLocalY], obb2NDir[COBB::enLocalZ]);

	//3.���OBB�̎ˉe�������d�Ȃ��Ă��邩�̔���B

	//���S�_�Ԃ̋������v�Z
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));

	//�Փ˔���
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//�Փ˂��Ă��Ȃ�

	//2
	//�Z�������FXY

	//1.OBB�̕�������T���B
	SeparationAxis = Cross(obb1NDir[COBB::enLocalX], obb2NDir[COBB::enLocalY]);
	//2.��������OBB���ˉe����B
	//obb1�̎ˉe����
	obb1ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb1NDir[COBB::enLocalY], obb1NDir[COBB::enLocalZ]);
	//obb2�̎ˉe����
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2NDir[COBB::enLocalX], obb2NDir[COBB::enLocalZ]);
	//3.���OBB�̎ˉe�������d�Ȃ��Ă��邩�̔���B
	//���S�_�Ԃ̋������v�Z
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));
	//�Փ˔���
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//�Փ˂��Ă��Ȃ�

	//3
	//�Z�������FXY

	//1.OBB�̕�������T���B
	SeparationAxis = Cross(obb1NDir[COBB::enLocalX], obb2NDir[COBB::enLocalX]);
	//2.��������OBB���ˉe����B
	//obb1�̎ˉe����
	obb1ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb1NDir[COBB::enLocalY], obb1NDir[COBB::enLocalZ]);
	//obb2�̎ˉe����
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2NDir[COBB::enLocalY], obb2NDir[COBB::enLocalZ]);
	//3.���OBB�̎ˉe�������d�Ȃ��Ă��邩�̔���B
	//���S�_�Ԃ̋������v�Z
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));
	//�Փ˔���
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//�Փ˂��Ă��Ȃ�


	//3
	//�Z�������FXY

	//1.OBB�̕�������T���B
	SeparationAxis = Cross(obb1NDir[COBB::enLocalX], obb2NDir[COBB::enLocalX]);
	//2.��������OBB���ˉe����B
	//obb1�̎ˉe����
	obb1ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb1NDir[COBB::enLocalY], obb1NDir[COBB::enLocalZ]);
	//obb2�̎ˉe����
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2NDir[COBB::enLocalY], obb2NDir[COBB::enLocalZ]);
	//3.���OBB�̎ˉe�������d�Ȃ��Ă��邩�̔���B
	//���S�_�Ԃ̋������v�Z
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));
	//�Փ˔���
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//�Փ˂��Ă��Ȃ�


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