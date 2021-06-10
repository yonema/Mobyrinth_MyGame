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
}

//�f�X�g���N�^
CLevelObjectManager::~CLevelObjectManager()
{
	//nullptr�����Ă���
	m_instance = nullptr;
}

/// <summary>
/// �E�F�C�|�C���g�́u�ꏊ�v��������
/// </summary>
/// <param name="vecSize">�E�F�C�|�C���g�̃T�C�Y</param>
/// <param name="posMap">�ꏊ�̃}�b�v</param>
void CLevelObjectManager::InitWayPointPos(std::map<int, Vector3>& posMap)
{
	//�E�F�C�|�C���g�X�e�[�g�̍ő�̒l��ݒ�
	m_vecSize = posMap.size();
	//vector�̃T�C�Y�̊m��
	m_wayPointPos.resize(m_vecSize);
	//m_wayPointPos�ɃE�F�C�|�C���g�́u�ꏊ�v���i�[����
	std::map<int, Vector3>::iterator it = posMap.begin();
	for (int index = 0; it != posMap.end(); index++, it++)
	{
		m_wayPointPos[index] = it->second;
	}
}


/// <summary>
/// �E�F�C�|�C���g�́u��]�v��������
/// </summary>
/// <param name="vecSize">�E�F�C�|�C���g�̃T�C�Y</param>
/// <param name="rotMap">��]�̃}�b�v</param>
void CLevelObjectManager::InitWayPointRot(std::map<int, Quaternion>& rotMap)
{
	//�E�F�C�|�C���g�X�e�[�g�̍ő�̒l��ݒ�
	m_vecSize = rotMap.size();
	//vector�̃T�C�Y�̊m��
	m_wayPointRot.resize(m_vecSize);
	//m_wayPointRot�ɃE�F�C�|�C���g�́u��]�v���i�[����
	std::map<int, Quaternion>::iterator it = rotMap.begin();
	for (int index = 0; it != rotMap.end(); index++, it++)
	{
		m_wayPointRot[index] = it->second;
	}
}


/// <summary>
/// �E�F�C�|�C���g��ł̈ړ�����v�Z����֐�
/// </summary>
/// <param name="rpIndex">���݂̉E���̃E�F�C�|�C���g�̃C���f�b�N�X</param>
/// <param name="pos">���݂̍��W</param>
/// <param name="dist">�ړ����鋗��</param>
/// <param name="leftOrRight">�E���Ɉړ����邩�������ړ����邩�B0:��,1:�E</param>
/// <returns>�ړ���̍��W</returns>
const Vector3 CLevelObjectManager::CalcWayPointNextPos
(const int rpIndex, const Vector3& pos, const float dist, const bool leftOrRight, int* pNextIndex)
{
	int lpIndex = rpIndex + 1;
	const int maxWayPointIndex = m_vecSize - 1;
	if (lpIndex > maxWayPointIndex)
	{
		lpIndex = 0;
	}

	Vector3 addVec = g_VEC3_ZERO;
	int nextIndex = 0;
	if (leftOrRight)
	{
		//�E�ɔ�΂�
		nextIndex = rpIndex;
	}
	else
	{
		//���ɔ�΂�
		nextIndex = lpIndex;
	}
	Vector3 originPos = pos;
	addVec = m_wayPointPos[nextIndex] - originPos;


	float addLen = addVec.Length();
	float rDist = dist;
	if (addLen <= rDist)
	{
		rDist -= addLen;
		int otherIndex = nextIndex;
		if (leftOrRight)
		{
			//���ɔ�΂�
			nextIndex--;
			if (nextIndex < 0)
				nextIndex = maxWayPointIndex;
		}
		else
		{
			//�E�ɔ�΂�
			nextIndex++;
			if (nextIndex > maxWayPointIndex)
				nextIndex = 0;
		}
		originPos = m_wayPointPos[otherIndex];
		addVec = m_wayPointPos[nextIndex] - originPos;
		addLen = addVec.Length();
	}
	addVec.Normalize();
	addVec.Scale(rDist);
	if (pNextIndex)
		*pNextIndex = nextIndex;
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


bool CLevelObjectManager::QueryLevelAllObjects(ILevelObjectBase& thisObject, const int objectType)
{
	for (auto lo : m_levelObjects) {
		//�������g����Ȃ��Ƃ�
		if (lo != &thisObject) {
			//�d�Ȃ��Ă���I�u�W�F�N�g������B
			if (IsHitObject(thisObject, *lo)) {
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
		return enUFOCapture;
	}


	//�E�F�C�|�C���g�̍ő�l
	const int maxWayPoint = m_vecSize - 1;
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
	int objectType = enEmpty;

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
