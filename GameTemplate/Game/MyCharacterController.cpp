#include "stdafx.h"
#include "MyCharacterController.h"


void CMyCharacterController::Init(const SInitOBBData& initOBBData)
{
	m_obb.Init(initOBBData);
}

const Vector3& CMyCharacterController::Execute(const Vector3& moveSpeed, const float deltaTime)
{
	//�ړ��O�̍��W
	Vector3 oldPosition = m_obb.GetPosition();

	//���̈ړ���̍��W
	Vector3 nextPosition = oldPosition;

	if (moveSpeed.Length() <= 0.001f)
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
	hitOBB = COBBWorld::GetInstance()->HitAllOBB(m_obb, hitOBB);

	if (hitOBB)
	{

		//�Փ˂��Ă���OBB�����݂���

		//�ړ�����x�N�g���̒P�ʃx�N�g��
		Vector3 addNorm = addPos;
		addNorm.Normalize();	//���K������

		Vector3 myOBBNormX = m_obb.GetNormalDirection(COBB::enLocalX);


		Vector3 hitOBBNormX = hitOBB->GetNormalDirection(COBB::enLocalX);

		//�ړ�����x�N�g���ƁA�߂�x�N�g���̌������ׂ�
		float dot = Dot(addNorm, hitOBBNormX);

		//���������Ȃ�
		if (dot > 0.0f)
		{
			//���Ό����ɂ���
			myOBBNormX.Scale(-1.0f);
			hitOBBNormX.Scale(-1.0f);
		}

		//��������OBB�̓����������̕ǂ̍��W
		Vector3 hitPos = hitOBB->GetPosition();
		hitPos += hitOBBNormX * hitOBB->GetDirectionLength(COBB::enLocalX);

		float between = Dot(m_obb.GetPosition() - hitPos, hitOBBNormX);

		float myObbLen =
		CalcProjectionLen(hitOBBNormX,
			m_obb.GetNormalDirection(COBB::enLocalX) * m_obb.GetDirectionLength(COBB::enLocalX),
			m_obb.GetNormalDirection(COBB::enLocalY) * m_obb.GetDirectionLength(COBB::enLocalY),
			m_obb.GetNormalDirection(COBB::enLocalZ) * m_obb.GetDirectionLength(COBB::enLocalZ)
			);


		float backLen = myObbLen - between;

		Vector3 backVec = hitOBBNormX * backLen;

		nextPosition += backVec;
		//OBB���ړ�������
		m_obb.SetPosition(nextPosition);
	}

	//�ŏI�I�Ȉړ���̍��W��߂�
	return nextPosition;

}