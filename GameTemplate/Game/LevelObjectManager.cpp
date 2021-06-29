#include "stdafx.h"
#include "LevelObjectManager.h"
#include "LevelObjectBase.h"
#include "ReversibleObject.h"
#include "Player.h"


//�C���X�^���X�̏�����
CLevelObjectManager* CLevelObjectManager::m_instance = nullptr;

//�R���X�g���N�^
CLevelObjectManager::CLevelObjectManager()
{
	if (m_instance != nullptr) {
		//�C���X�^���X�����łɍ���Ă���B
		std::abort();
	}
	return;
}

//�f�X�g���N�^
CLevelObjectManager::~CLevelObjectManager()
{
	//nullptr�����Ă���
	m_instance = nullptr;

	return;
}

/// <summary>
/// �E�F�C�|�C���g�́u�ꏊ�v��������
/// </summary>
/// <param name="vecSize">�E�F�C�|�C���g�̃T�C�Y</param>
/// <param name="posMap">�ꏊ�̃}�b�v</param>
void CLevelObjectManager::InitWayPointPos(std::map<int, Vector3>& posMap)
{
	//vector�̃T�C�Y�̊m��
	m_wayPointPos.resize(posMap.size());
	//m_wayPointPos�ɃE�F�C�|�C���g�́u�ꏊ�v���i�[����
	std::map<int, Vector3>::iterator it = posMap.begin();
	for (int index = 0; it != posMap.end(); index++, it++)
	{
		m_wayPointPos[index] = it->second;
	}

	//�\���Ɨ����̂������l�̏�����
	InitFrontOrBackSideThreshold();

	return;
}


/// <summary>
/// �E�F�C�|�C���g�́u��]�v��������
/// </summary>
/// <param name="vecSize">�E�F�C�|�C���g�̃T�C�Y</param>
/// <param name="rotMap">��]�̃}�b�v</param>
void CLevelObjectManager::InitWayPointRot(std::map<int, Quaternion>& rotMap)
{
	//vector�̃T�C�Y�̊m��
	m_wayPointRot.resize(rotMap.size());
	//m_wayPointRot�ɃE�F�C�|�C���g�́u��]�v���i�[����
	std::map<int, Quaternion>::iterator it = rotMap.begin();
	for (int index = 0; it != rotMap.end(); index++, it++)
	{
		m_wayPointRot[index] = it->second;
	}

	return;
}


/**
 * @brief �\���Ɨ����̂������l�̏�����
*/
void CLevelObjectManager::InitFrontOrBackSideThreshold()
{
	/*
	* �E�F�C�|�C���g�̔ԍ��̍ŏ�4����1�ƍŌ�4����1���\���A
	* 4����2�`4����3�͗����ɂȂ�B
	* 
	* �E�F�C�|�C���g�̔ԍ��͍�����ő����Ă����A�\������������
	* ���g�̍����̃E�F�C�|�C���g���g���Čv�Z���邩��A�������ق���
	* �������l��1���Z����B
	*/

	m_frontOrBackSideThresholdSmall = m_wayPointPos.size() / 4 + 1;
	m_frontOrBackSideThresholdBig = (m_wayPointPos.size() / 4) * 3;

	return;
}


/**
 * @brief �E�F�C�|�C���g��ł̈ړ�����v�Z����֐�
 * @param [in] rpIndex ���݂̉E���̃E�F�C�|�C���g�̃C���f�b�N�X
 * @param [in] pos ���݂̍��W
 * @param [in] dist �ړ����鋗��
 * @param [in] leftOrRight �E���Ɉړ����邩�������ړ����邩�B0:��,1:�E
 * @param [out] pNextIndex �ړ���̃E�F�C�|�C���g�̃C���f�b�N�X
 * @return �ړ���̍��W
*/
const Vector3 CLevelObjectManager::CalcWayPointNextPos
(const int rpIndex, const Vector3& pos, const float dist, const bool leftOrRight, int* pNextIndex)
{
	//�����̃E�F�C�|�C���g�͉E���̃E�F�C�|�C���g��1��
	int lpIndex = rpIndex + 1;
	//�E�F�C�|�C���g�̍ő吔�̓T�C�Y��1��
	const int maxWayPointIndex = m_wayPointPos.size() - 1;
	//�����̃E�F�C�|�C���g���ő�l���傫��������
	if (lpIndex > maxWayPointIndex)
	{
		//�������0�ɂȂ�
		lpIndex = 0;
	}

	//�ړ����邽�߂ɉ�����x�N�g��
	Vector3 addVec = g_VEC3_ZERO;
	//�ړ������ɂ���E�F�C�|�C���g�̃C���f�b�N�X
	int nextIndex = 0;
	//���E�ǂ���ɔ�΂����H
	if (leftOrRight == EN_RIGHT)
	{
		//�E�ɔ�΂�
		nextIndex = rpIndex;
	}
	else
	{
		//���ɔ�΂�
		nextIndex = lpIndex;
	}
	//�ړ��O�̍��W
	Vector3 originPos = pos;
	//���̏ꏊ����ړ���̕����̃E�F�C�|�C���g�ւ̃x�N�g��
	addVec = m_wayPointPos[nextIndex] - originPos;
	//�ړ���ւ̒���
	float addLen = addVec.Length();
	//�w�肳�ꂽ�ړ����鋗��
	float rDist = dist;
	//�w�肳�ꂽ�ړ��������A�v�Z�����ړ��������傫��������
	if (addLen <= rDist)
	{
		//�w�肳�ꂽ�ړ��������v�Z�����ړ����������Z����
		rDist -= addLen;
		//�ύX�O�̈ړ���̃E�F�C�|�C���g�̃C���f�b�N��ύX����
		int otherIndex = nextIndex;
		if (leftOrRight == EN_RIGHT)
		{
			//�E�ɔ�΂�
			nextIndex--;
			if (nextIndex < 0)
				nextIndex = maxWayPointIndex;
		}
		else
		{
			//���ɔ�΂�
			nextIndex++;
			if (nextIndex > maxWayPointIndex)
				nextIndex = 0;
		}
		//�ړ��O�̍��W
		originPos = m_wayPointPos[otherIndex];
		//�ړ��O����ړ���ւ̃x�N�g��
		addVec = m_wayPointPos[nextIndex] - originPos;
		//addLen = addVec.Length();
	}
	//�ړ���������𐳋K������
	addVec.Normalize();
	//�w�肳�ꂽ�ړ��ʂ���������L�΂�
	addVec.Scale(rDist);
	//���̃E�F�C�|�C���g�������œn����Ă�����
	if (pNextIndex)
		//���̃E�F�C�|�C���g�ɒl��ύX����
		*pNextIndex = nextIndex;

	//�ړ��O�̍��W�Ɉړ�����x�N�g���������Ė߂�
	return originPos + addVec;
}



/// <summary>
/// ���g�iLevelObjectManager�j�ɃI�u�W�F�N�g��ǉ�����
/// </summary>
/// <param name="object">�ǉ�����I�u�W�F�N�g</param>
void CLevelObjectManager::AddObject(ILevelObjectBase* object)
{
	m_levelObjects.push_back(object);
}

/// <summary>
/// ���g�iLevelObjectManager�j����I�u�W�F�N�g��j������
/// </summary>
/// <param name="object">�j������I�u�W�F�N�g</param>
void CLevelObjectManager::RemoveObject(ILevelObjectBase* object)
{
	//��x�Ɉ���������Ȃ���
	std::vector<ILevelObjectBase*>::iterator itr;
	for (itr = m_levelObjects.begin(); itr != m_levelObjects.end(); itr++)
	{
		if ((*itr)->GetIsDead())
		{
			m_levelObjects.erase(itr);
			break;
		}
	}
}

/// <summary>
/// ���x���I�u�W�F�N�g�����ׂď�������
/// </summary>
void CLevelObjectManager::AllDeleteLOs()
{
	for (int i = 0; i < m_levelObjects.size(); i++)
	{
		DeleteGO(m_levelObjects[i]);
	}
}

/**
 * @brief �S�Ẵ��x���I�u�W�F�N�g�𒲂ׂāA�Փ˂��Ă��Ȃ������ׂ�
 * @param [in] thisObject ���g�̃C���X�^���X
 * @return �Փ˂��Ă��邩�H
*/
bool CLevelObjectManager::IsHitAllLevelObjects(const ILevelObjectBase& thisObject)
{
	for (auto lo : m_levelObjects) {
		//�������g����Ȃ��Ƃ�
		if (lo != &thisObject) {
			//�d�Ȃ��Ă���I�u�W�F�N�g������B
			if (IsHitLevelObject(thisObject, *lo)) {
				return true;
			}
		}
	}
	return false;
}

/// <summary>
/// �v���C���[�Ɉ�ԋ߂��I�u�W�F�N�g�̃I�u�W�F�N�g�^�C�v��߂�
/// </summary>
/// <returns>�I�u�W�F�N�g�^�C�v</returns>
const int CLevelObjectManager::GetNearestObjectType(const float nearDist)
{
	//�v���C���[�����������Ă�����A
	if (m_player->GetHoldObject())
	{
		//�v���C���[�������Ă���I�u�W�F�N�g�̃^�C�v��߂�
		if (m_player->GetReversibleObject())
			return m_player->GetReversibleObject()->GetObjectType();
	}
	//�v���C���[��UFO�ɕ߂܂��Ă��邩�H
	else if (m_player->GetCapturedUFOFlag())
	{
		//�߂܂��Ă�����

		//UFO�ɕ߂܂��Ă����Ԃ�߂�
		return EN_OO_TYPE_UFO_CAPTURE;
	}


	//�E�F�C�|�C���g�̍ő�l
	const int maxWayPoint = m_wayPointPos.size() - 1;
	//�v���C���[�̂���E�F�C�|�C���g
	int wayPoint = m_player->GetLeftPointIndex();
	//�E�F�C�|�C���g���̃E�F�C�|�C���g
	int leftWayPoint = wayPoint + 1;
	//�ő�l���傫��������
	if (leftWayPoint > maxWayPoint)
		//���������0�ɂ���
		leftWayPoint = 0;
	//�E�F�C�|�C���g���̃E�F�C�|�C���g
	int rightWayPoint = wayPoint - 1;
	//0��菬����������
	if (rightWayPoint < 0)
		//��������čő�l�ɂ���
		rightWayPoint = maxWayPoint;

	//�I�u�W�F�N�g�Ƃ̋����̂������l
	const float threshold = nearDist;

	//�I�u�W�F�N�g�Ƃ̋���
	//�������l�ȉ��̈�ԋ߂�������T��
	float dist = threshold;

	//�߂��I�u�W�F�N�g�^�C�v
	//�߂��ɃI�u�W�F�N�g���Ȃ�������enEmpty��߂�
	int objectType = EN_OBJECT_TYPE_EMPTY;

	//���x���I�u�W�F�N�g��S�����ׂ�
	for (int i = 0; i < m_levelObjects.size(); i++)
	{
		//���b�N���Ȃ�X�L�b�v
		if (m_levelObjects[i]->GetLock())
			continue;

		//�I�u�W�F�N�g������E�F�C�|�C���g�𒲂ׂ�
		int objectsWayPoint = m_levelObjects[i]->GetLeftWayPointIndex();
		//�I�u�W�F�N�g�̈ʒu�𒲂ׂ�
		if (objectsWayPoint == leftWayPoint || objectsWayPoint == wayPoint ||
			objectsWayPoint == rightWayPoint)
		{
			//�v���C���[�Ɠ����E�F�C�|�C���g�A�܂��́A���̍��E�̂ǂ��炩�ɂ�����

			//�v���C���[����I�u�W�F�N�g�ւ̃x�N�g��
			Vector3 playerToObject = m_levelObjects[i]->GetPosition() - m_player->GetPosition();
			//�x�N�g���̒���
			float length = playerToObject.Length();

			//�������A�������l�ȉ���
			//���܂ł̒������߂�������
			if (length <= dist)
			{
				//�߂��I�u�W�F�N�g�^�C�v���X�V
				objectType = m_levelObjects[i]->GetObjectType();
				//�������X�V
				dist = length;
			}
		}
		else
		{
			//�E�F�C�|�C���g�̏ꏊ���������X�L�b�v
			continue;
		}
	}
	
	//�I�u�W�F�N�g�^�C�v��߂�
	return objectType;

}


/**
 * @brief �\���ɂ���̂������ɂ���̂��𒲂ׂ�
 * @param [in] lpIndex ���g�̍����̃E�F�C�|�C���g�̔ԍ�
 * @return �\����������
*/
const int CLevelObjectManager::CheckFrontOrBackSide(const int lpIndex)
{
	//�\����������
	int frontOrBackSide = EB_NONE_SIDE;

	//�����̃E�F�C�|�C���g���������l�Ɣ�ׂāA�\�������������ׂ�
	if (m_frontOrBackSideThresholdSmall <= lpIndex && lpIndex <= m_frontOrBackSideThresholdBig)
	{
		//����
		frontOrBackSide = EN_BACK_SIDE;
	}
	else
	{
		//�\��
		frontOrBackSide = EN_FRONT_SIDE;
	}

	//���ʂ�߂�
	return frontOrBackSide;

}