#include "stdafx.h"
#include "OBB.h"

//OBB�̒萔�f�[�^���g����悤�ɂ���
using namespace OBBConstData;

//�R���X�g���N�^
COBB::COBB()
{
	//�e�f�[�^�����o�ɏ����l������
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
/// �������֐��B
/// �ŏ��ɌĂ�łˁB
/// </summary>
/// <param name="initData">COBB�̏������p�̍\����</param>
void COBB::Init(const SInitOBBData& initData)
{
	//�e���̕����x�N�g���̒�����ݒ�
	m_directionLength[enLocalX] = initData.width / 2;
	m_directionLength[enLocalY] = initData.height / 2;
	m_directionLength[enLocalZ] = initData.length / 2;


	//�|�W�V������ݒ�
	m_position = initData.position;

	//�s�{�b�g��ݒ�
	m_pivot = initData.pivot;

	//��]��ݒ�
	Rotating(initData.rotation);

	//�Z���^�[�|�W�V�������v�Z����
	CalcCenterPosition();
}

/// <summary>
/// �{�b�N�X��8�̒��_�̍��W���擾����B
/// �i���Ӂj�z��̐擪�A�h���X��߂��Ă��邯�ǁA
/// �z��̗v�f����8����B
/// �g���Ƃ��͔O�̂���GetBoxVertexNum()��
/// ���_�������Ă��ĂˁB
/// </summary>
/// <returns>���_�̔z��̐擪�A�h���X</returns>
Vector3* COBB::GetBoxVertex()
{

	//�{�b�N�X�̒��_�̏ꏊ
	Vector3 boxVertex[m_boxVertexNum];
	//�{�b�N�X�̒��_�ɂ܂��Z���^�[�|�W�V����������
	for (int i = 0; i < m_boxVertexNum; i++)
	{
		boxVertex[i] = m_centerPosition;
	}

	//�e���_���Z���^�[�|�W�V������������x�N�g���i�傫���t���j���������āA
	//���_�̏ꏊ�܂ňړ�������B
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


	//���_�̔z��̐擪�A�h���X��߂�
	return boxVertex;
}


/// <summary>
/// �Z���^�[�|�W�V�����̎擾
/// </summary>
/// <returns>�Z���^�[�|�W�V����</returns>
void COBB::CalcCenterPosition()
{
	//�s�{�b�g�����ƂɃZ���^�[�|�W�V�������v�Z����

	//�����x�N�g���i�傫���t���j
	Vector3 localVecX = m_normalDirection[enLocalX] * m_directionLength[enLocalX];
	Vector3 localVecY = m_normalDirection[enLocalY] * m_directionLength[enLocalY];
	Vector3 localVecZ = m_normalDirection[enLocalZ] * m_directionLength[enLocalZ];


	//OBB�̍�����O�̍��W������
	Vector3 centerPos = m_position;	//�Z���^�[�|�W�V�������v�Z����
	centerPos -= localVecX;
	centerPos += localVecY;
	centerPos -= localVecZ;

	//������O�̍��W����Z���^�[�|�W�V�����ւ̃x�N�g��
	Vector3 addVec;

	//X���W�̃s�{�b�g�ɃZ���^�[�|�W�V���������킹��B
	addVec = localVecX * 2.0f * m_pivot.x;
	centerPos += addVec;
	//Y���W�̃s�{�b�g�ɃZ���^�[�|�W�V���������킹��B
	addVec = localVecY * 2.0f * m_pivot.y;
	centerPos -= addVec;
	//Z���W�̃s�{�b�g�ɃZ���^�[�|�W�V���������킹��B
	addVec = localVecZ * 2.0f * m_pivot.z;
	centerPos += addVec;

	//�Z���^�[�|�W�V������ݒ肷��B
	m_centerPosition = centerPos;
}

/// <summary>
/// �P�ʕ����x�N�g�����񂷊֐�
/// </summary>
/// <param name="rot">��]</param>
void COBB::Rotating(const Quaternion& rot)
{
	//�e���̒P�ʕ����x�N�g�������������Ă���
	m_normalDirection[enLocalX] = g_VEC3_RIGHT;
	m_normalDirection[enLocalY] = g_VEC3_UP;
	m_normalDirection[enLocalZ] = g_VEC3_FRONT;
	//�N�H�[�^�j�I���ŉ�
	rot.Apply(m_normalDirection[enLocalX]);
	rot.Apply(m_normalDirection[enLocalY]);
	rot.Apply(m_normalDirection[enLocalZ]);

	return;
}


//�N���X�O
//�N���X�̃����o�֐��ł͂Ȃ��֐�

/// <summary>
/// OBB���m�̓����蔻��
/// ����������true��߂�
/// </summary>
/// <param name="obb1">�Е���OBB</param>
/// <param name="obb2">�����Е���OBB</param>
/// <returns>�����������ǂ���</returns>
const bool CollisionOBBs(const COBB& obb1, const COBB& obb2)
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
		//���OBB�̎ˉe�������A���S�_�Ԃ̋������Z�����ǂ���
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
		//��������obb2��X,Y,Z�����ꂼ���I��
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
		float obb2ProjectionLen = obb2.GetDirectionLength(axis);


		//3.���OBB�̎ˉe�������d�Ȃ��Ă��邩�̔���B

		//���S�_�Ԃ̋������v�Z
		IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));

		//�Փ˔���
		//���OBB�̎ˉe�������A���S�_�Ԃ̋������Z�����ǂ���
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
		CalcProjectionLen(SeparationAxis, obb1DirVec[COBB::enLocalY], obb1DirVec[COBB::enLocalZ]);

	//obb2�̎ˉe����
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2DirVec[COBB::enLocalY], obb2DirVec[COBB::enLocalZ]);

	//3.���OBB�̎ˉe�������d�Ȃ��Ă��邩�̔���B

	//���S�_�Ԃ̋������v�Z
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));

	//�Փ˔���
	//���OBB�̎ˉe�������A���S�_�Ԃ̋������Z�����ǂ���
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//�Փ˂��Ă��Ȃ�

	//2
	//�Z�������FXY

	//1.OBB�̕�������T���B
	SeparationAxis = Cross(obb1NDir[COBB::enLocalX], obb2NDir[COBB::enLocalY]);
	//2.��������OBB���ˉe����B
	//obb1�̎ˉe����
	obb1ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb1DirVec[COBB::enLocalY], obb1DirVec[COBB::enLocalZ]);
	//obb2�̎ˉe����
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2DirVec[COBB::enLocalX], obb2DirVec[COBB::enLocalZ]);
	//3.���OBB�̎ˉe�������d�Ȃ��Ă��邩�̔���B
	//���S�_�Ԃ̋������v�Z
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));
	//�Փ˔���
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//�Փ˂��Ă��Ȃ�

	//3
	//�Z�������FXZ

	//1.OBB�̕�������T���B
	SeparationAxis = Cross(obb1NDir[COBB::enLocalX], obb2NDir[COBB::enLocalZ]);
	//2.��������OBB���ˉe����B
	//obb1�̎ˉe����
	obb1ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb1DirVec[COBB::enLocalY], obb1DirVec[COBB::enLocalZ]);
	//obb2�̎ˉe����
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2DirVec[COBB::enLocalX], obb2DirVec[COBB::enLocalY]);
	//3.���OBB�̎ˉe�������d�Ȃ��Ă��邩�̔���B
	//���S�_�Ԃ̋������v�Z
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));
	//�Փ˔���
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//�Փ˂��Ă��Ȃ�


	//4
	//�Z�������FYX

	//1.OBB�̕�������T���B
	SeparationAxis = Cross(obb1NDir[COBB::enLocalY], obb2NDir[COBB::enLocalX]);
	//2.��������OBB���ˉe����B
	//obb1�̎ˉe����
	obb1ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb1DirVec[COBB::enLocalX], obb1DirVec[COBB::enLocalZ]);
	//obb2�̎ˉe����
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2DirVec[COBB::enLocalY], obb2DirVec[COBB::enLocalZ]);
	//3.���OBB�̎ˉe�������d�Ȃ��Ă��邩�̔���B
	//���S�_�Ԃ̋������v�Z
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));
	//�Փ˔���
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//�Փ˂��Ă��Ȃ�

	//5
	//�Z�������FYY

	//1.OBB�̕�������T���B
	SeparationAxis = Cross(obb1NDir[COBB::enLocalY], obb2NDir[COBB::enLocalY]);
	//2.��������OBB���ˉe����B
	//obb1�̎ˉe����
	obb1ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb1DirVec[COBB::enLocalX], obb1DirVec[COBB::enLocalZ]);
	//obb2�̎ˉe����
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2DirVec[COBB::enLocalX], obb2DirVec[COBB::enLocalZ]);
	//3.���OBB�̎ˉe�������d�Ȃ��Ă��邩�̔���B
	//���S�_�Ԃ̋������v�Z
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));
	//�Փ˔���
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//�Փ˂��Ă��Ȃ�

	//6
	//�Z�������FYZ

	//1.OBB�̕�������T���B
	SeparationAxis = Cross(obb1NDir[COBB::enLocalY], obb2NDir[COBB::enLocalZ]);
	//2.��������OBB���ˉe����B
	//obb1�̎ˉe����
	obb1ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb1DirVec[COBB::enLocalX], obb1DirVec[COBB::enLocalZ]);
	//obb2�̎ˉe����
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2DirVec[COBB::enLocalX], obb2DirVec[COBB::enLocalY]);
	//3.���OBB�̎ˉe�������d�Ȃ��Ă��邩�̔���B
	//���S�_�Ԃ̋������v�Z
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));
	//�Փ˔���
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//�Փ˂��Ă��Ȃ�

	//7
	//�Z�������FZX

	//1.OBB�̕�������T���B
	SeparationAxis = Cross(obb1NDir[COBB::enLocalZ], obb2NDir[COBB::enLocalX]);
	//2.��������OBB���ˉe����B
	//obb1�̎ˉe����
	obb1ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb1DirVec[COBB::enLocalX], obb1DirVec[COBB::enLocalY]);
	//obb2�̎ˉe����
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2DirVec[COBB::enLocalY], obb2DirVec[COBB::enLocalZ]);
	//3.���OBB�̎ˉe�������d�Ȃ��Ă��邩�̔���B
	//���S�_�Ԃ̋������v�Z
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));
	//�Փ˔���
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//�Փ˂��Ă��Ȃ�

	//8
	//�Z�������FZY

	//1.OBB�̕�������T���B
	SeparationAxis = Cross(obb1NDir[COBB::enLocalZ], obb2NDir[COBB::enLocalY]);
	//2.��������OBB���ˉe����B
	//obb1�̎ˉe����
	obb1ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb1DirVec[COBB::enLocalX], obb1DirVec[COBB::enLocalY]);
	//obb2�̎ˉe����
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2DirVec[COBB::enLocalX], obb2DirVec[COBB::enLocalZ]);
	//3.���OBB�̎ˉe�������d�Ȃ��Ă��邩�̔���B
	//���S�_�Ԃ̋������v�Z
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));
	//�Փ˔���
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//�Փ˂��Ă��Ȃ�

	//9
	//�Z�������FZZ

	//1.OBB�̕�������T���B
	SeparationAxis = Cross(obb1NDir[COBB::enLocalZ], obb2NDir[COBB::enLocalZ]);
	//2.��������OBB���ˉe����B
	//obb1�̎ˉe����
	obb1ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb1DirVec[COBB::enLocalX], obb1DirVec[COBB::enLocalY]);
	//obb2�̎ˉe����
	obb2ProjectionLen =
		CalcProjectionLen(SeparationAxis, obb2DirVec[COBB::enLocalX], obb2DirVec[COBB::enLocalY]);
	//3.���OBB�̎ˉe�������d�Ȃ��Ă��邩�̔���B
	//���S�_�Ԃ̋������v�Z
	IntervalLen = std::abs(Dot(obb2ToObb1Vec, SeparationAxis));
	//�Փ˔���
	if (IntervalLen > obb1ProjectionLen + obb2ProjectionLen)
		return false;	//�Փ˂��Ă��Ȃ�



	//�������ʂ����݂��Ȃ��̂�
	
	return true;		//�Փ˂��Ă���

}


/// <summary>
/// �ˉe�������v�Z����֐�
/// </summary>
/// <param name="sepAxis">�ˉe����镪�����i���K���ς݂�n�����Ɓj</param>
/// <param name="dirVec_X">�ˉe��������x�N�g��X�i�傫���t����n�����Ɓj</param>
/// <param name="dirVec_Y">�ˉe��������x�N�g��Y�i�傫���t����n�����Ɓj</param>
/// <param name="dirVec_Z">�ˉe��������x�N�g��Z�i�傫���t����n�����Ɓj(Z�͂Ȃ��Ă��ǂ�)</param>
/// <returns>�ˉe����</returns>
const float CalcProjectionLen(
	const Vector3& sepAxis,
	const Vector3& dirVec_X,
	const Vector3& dirVec_Y,
	const Vector3& dirVec_Z
)
{
	//�����x�N�g��X�̎ˉe����
	float projX = std::abs(Dot(sepAxis, dirVec_X));
	//�����x�N�g��Y�̎ˉe����
	float projY = std::abs(Dot(sepAxis, dirVec_Y));
	//�����x�N�g��Z�̎ˉe����
	float projZ = std::abs(Dot(sepAxis, dirVec_Z));

	//X,Y,Z�̎ˉe�����̍��v��߂��B
	return projX + projY + projZ;
}