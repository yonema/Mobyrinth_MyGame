#include "stdafx.h"
#include "LevelObjectManager.h"
CLevelObjectManager* CLevelObjectManager::m_instance = nullptr;

CLevelObjectManager::CLevelObjectManager()
{
	if (m_instance != nullptr) {
		//インスタンスがすでに作られている。
		std::abort();
	}
}

CLevelObjectManager::~CLevelObjectManager()
{
	//nullptrを入れておく
	m_instance = nullptr;
}


void CLevelObjectManager::InitWayPointPos(const std::size_t vecSize, std::map<int, Vector3>& posMap)
{
	//vectorのサイズの確保
	m_wayPointPos.resize(vecSize);
	//ウェイポイントステートの最大の値を設定
	m_vecSize = vecSize;
	//m_wayPointPosにウェイポイントの「場所」を格納する
	std::map<int, Vector3>::iterator it = posMap.begin();
	for (int index = 0; it != posMap.end(); index++, it++)
	{
		m_wayPointPos[index] = it->second;
	}
}
void CLevelObjectManager::InitWayPointRot(const std::size_t vecSize, std::map<int, Quaternion>& rotMap)
{
	//vectorのサイズの確保
	m_wayPointRot.resize(vecSize);
	//ウェイポイントステートの最大の値を設定
	m_vecSize = vecSize;
	//m_wayPointRotにウェイポイントの「回転」を格納する
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