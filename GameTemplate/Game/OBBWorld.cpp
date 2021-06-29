#include "stdafx.h"
#include "OBBWorld.h"

//�B��̃C���X�^���X�̏�����
COBBWorld* COBBWorld::m_instance = nullptr;

/// <summary>
/// OBB��OBBWorld�ɓo�^����֐�
/// </summary>
/// <param name="obb">�o�^����OBB</param>
void COBBWorld::AddOBB(const COBB& obb)
{
	//������OBB��o�^����
	m_worldOBBs.push_back(&obb);
}

/// <summary>
/// OBBWorld�ɓo�^���Ă���OBB����������֐�
/// </summary>
/// <param name="obb">�o�^����OBB</param>
void COBBWorld::RemoveOBB(const COBB& obb)
{
	//�o�^���Ă���OBB�B��S�����ׂ�
	std::vector<const COBB*>::iterator itr;
	for (itr = m_worldOBBs.begin(); itr != m_worldOBBs.end(); itr++)
	{
		//������OBB�ƈ�v����OBB�̓o�^����������
		if ((*itr) == &obb)
		{
			m_worldOBBs.erase(itr);
			break;
		}
	}
}

/// <summary>
/// OBBWorld�ɓo�^���Ă��邷�ׂĂ�OBB�𒲂ׂāA
/// ��������OBB�ƏՓ˂��Ă����ԋ߂�OBB����
/// �߂��B
/// �ǂ�OBB�Ƃ��Փ˂��Ă��Ȃ�������Anullptr��
/// �߂��ė���B
/// </summary>
/// <param name="myOBB">���g��OBB</param>
/// <returns>�Փ˂��Ă���OBB���Anullptr</returns>
const COBB* COBBWorld::HitAllOBB(const COBB& myOBB)
{
	//��ԋ߂�OBB��T�����߂̋���������ϐ�
	float dist = FLT_MAX;

	//�Փ˂���OBB��ێ����Ă���OBB�̃|�C���^
	const COBB* pOBB = nullptr;

	//OBBWorld�ɓo�^���Ă���OBB��������o���āA���ׂ�
	for (auto OBB : m_worldOBBs)
	{
		//��O�t���O�𒲂ׂ�
		if (OBB->GetExceptionFlag())
			//��O�Ȃ�p�X
			continue;

		//�����œn���ꂽ���g��OBB�ƁAOBBWorld������o����OBB�Ƃ�
		//�Փ˔���
		if (CollisionOBBs(myOBB, *OBB))
		{
			//�Փ˂��Ă���

			//���g��OBB�ƁA�Փ˂���OBB�Ƃ̊Ԃ̃x�N�g��
			Vector3 lenVec = myOBB.GetPosition() - OBB->GetPosition();
			//���g��OBB�ƁA�Փ˂���OBB�Ƃ̊Ԃ̋���
			float len = lenVec.Length();

			//��������ԋ߂�OBB���Փ˂���OBB�Ƃ��ĕێ����Ă���
			if (len <= dist)
			{
				//OBB��ێ�����
				pOBB = OBB;
				//��ԋ߂��������X�V����
				dist = len;
			}
		}
	}

	//�Փ˂���OBB������ꍇ�́A�Փ˂�����ԋ߂�OBB�̃|�C���^���A
	//�Փ˂���OBB���Ȃ��ꍇ�́Anullptr��߂�
	return pOBB;
}


