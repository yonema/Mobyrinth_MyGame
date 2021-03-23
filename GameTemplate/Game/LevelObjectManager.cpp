#include "stdafx.h"
#include "LevelObjectManager.h"
#include "LevelObjectBase.h"

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
void CLevelObjectManager::InitWayPointPos(const std::size_t vecSize, std::map<int, Vector3>& posMap)
{
	//vector�̃T�C�Y�̊m��
	m_wayPointPos.resize(vecSize);
	//�E�F�C�|�C���g�X�e�[�g�̍ő�̒l��ݒ�
	m_vecSize = vecSize;
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
void CLevelObjectManager::InitWayPointRot(const std::size_t vecSize, std::map<int, Quaternion>& rotMap)
{
	//vector�̃T�C�Y�̊m��
	m_wayPointRot.resize(vecSize);
	//�E�F�C�|�C���g�X�e�[�g�̍ő�̒l��ݒ�
	m_vecSize = vecSize;
	//m_wayPointRot�ɃE�F�C�|�C���g�́u��]�v���i�[����
	std::map<int, Quaternion>::iterator it = rotMap.begin();
	for (int index = 0; it != rotMap.end(); index++, it++)
	{
		m_wayPointRot[index] = it->second;
	}
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
		if (lo->GetObjectType() != objectType) {
			//�d�Ȃ��Ă���I�u�W�F�N�g������B
			if (IsHitObject(thisObject, *lo)) {
				return true;
			}
		}
	}
	return false;
}
