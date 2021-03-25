#include "stdafx.h"
#include "MyCharacterController.h"

/// <summary>
/// �������p�֐�
/// </summary>
/// <param name="initOBBData">OBB�̏������p�f�[�^�\����</param>
void CMyCharacterController::Init(const SInitOBBData& initOBBData)
{
	//OBB������������
	m_obb.Init(initOBBData);
}

/// <summary>
/// ���s�֐��B
/// �����X�s�[�h�ƃf���^�^�C����n���ƁA
/// OBBWorld�ɓo�^���Ă���OBB�Ƃ̏Փˉ���������
/// �ړ���̍��W��߂��B
/// </summary>
/// <param name="moveSpeed">�����X�s�[�h</param>
/// <param name="deltaTime">�f���^�^�C��</param>
/// <returns>�ړ���̍��W</returns>
const Vector3& CMyCharacterController::Execute(const Vector3& moveSpeed, const float deltaTime)
{
	//�ړ��O�̍��W
	Vector3 oldPosition = m_obb.GetPosition();

	//���̈ړ���̍��W
	Vector3 nextPosition = oldPosition;

	//�ړ����Ă��Ȃ����́A
	//�Փ˂��C�ɂ��Ȃ��ňړ���̍��W��߂��B
	//���ꂪoldPosition���ƂȂ������܂������Ȃ������B
	//�i�]�B�B�B
	if (moveSpeed.Length() <= FLT_EPSILON)
		return nextPosition;

	//�ړ�����x�N�g��
	Vector3 addPos = moveSpeed;
	//�f���^�^�C�����|���Ă���
	addPos.Scale(deltaTime);

	//���̈ړ���̍��W���ړ�������B
	nextPosition += addPos;

	//OBB�����̈ړ���Ɉړ�������
	m_obb.SetPosition(nextPosition);

	//���������Ƃ���OBB�����Ă����|�C���^
	COBB* hitOBB = nullptr;

	//COOWorld�ɓo�^���Ă���OBB�����ԋ߂��Փ˂��Ă���OBB��T��
	//�Փ˂���OBB���Ȃ��ꍇ��nullptr���߂��Ă���
	hitOBB = COBBWorld::GetInstance()->HitAllOBB(m_obb);

	//�Փ˂��Ă���OBB�����݂���
	if (hitOBB)
	{
		//�ړ�����x�N�g���̒P�ʃx�N�g��
		Vector3 addNorm = addPos;
		addNorm.Normalize();	//���K������

		//�Փ˂���OBB�̃��[�J����X���̒P�ʕ����x�N�g��
		//�ǂ̖@���x�N�g���Ƃ��Ďg��
		Vector3 hitOBBNormX = hitOBB->GetNormalDirection(COBB::enLocalX);

		//�ړ�����x�N�g���ƁA�ǂ̖@���x�N�g���̌������r����
		float dot = Dot(addNorm, hitOBBNormX);

		//���������Ȃ�
		if (dot > 0.0f)
		{
			//���Ό����ɂ���
			hitOBBNormX.Scale(-1.0f);
		}

		//��������OBB�̓����������̕ǂ̍��W���v�Z����
		//�܂��͓�������OBB�̍��W������
		Vector3 hitPos = hitOBB->GetPosition();
		//�����������̕ǂ̖@���x�N�g���ɁA�������|����
		//�����������̕ǂ̍��W�����߂�
		hitPos += hitOBBNormX * hitOBB->GetDirectionLength(COBB::enLocalX);

		//���g��OBB�ƕǂ̊Ԃ̒������A�ǂ̖@���x�N�g���Ɏˉe����
		float between = Dot(m_obb.GetPosition() - hitPos, hitOBBNormX);

		//���g��OBB��ǂ̖@���x�N�g���Ɏˉe����
		float myObbLen =
		CalcProjectionLen(hitOBBNormX,
			m_obb.GetNormalDirection(COBB::enLocalX) * m_obb.GetDirectionLength(COBB::enLocalX),
			m_obb.GetNormalDirection(COBB::enLocalY) * m_obb.GetDirectionLength(COBB::enLocalY),
			m_obb.GetNormalDirection(COBB::enLocalZ) * m_obb.GetDirectionLength(COBB::enLocalZ)
			);

		//�d�Ȃ������߂�����
		float backLen = myObbLen - between;

		//�d�Ȃ������߂��x�N�g��
		Vector3 backVec = hitOBBNormX * backLen;

		//�ړ���̍��W�������Ȃ������߂��B
		nextPosition += backVec;

		//OBB���ړ�������
		m_obb.SetPosition(nextPosition);
	}

	//�ŏI�I�Ȉړ���̍��W��߂�
	return nextPosition;

}