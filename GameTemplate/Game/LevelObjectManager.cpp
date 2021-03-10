#include "stdafx.h"
#include "LevelObjectManager.h"
CLevelObjectManager* CLevelObjectManager::m_instance = nullptr;

CLevelObjectManager::CLevelObjectManager()
{
	if (m_instance != nullptr) {
		//�C���X�^���X�����łɍ���Ă���B
		std::abort();
	}
}

CLevelObjectManager::~CLevelObjectManager()
{
	//nullptr�����Ă���
	m_instance = nullptr;
}


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

void CLevelObjectManager::ExecuteUpdate()
{
	if (m_player)
	{

	}
	else
	{
		m_player = FindGO<Player_kari>("player");
	}
}