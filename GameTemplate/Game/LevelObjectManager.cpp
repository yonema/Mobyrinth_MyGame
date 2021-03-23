#include "stdafx.h"
#include "LevelObjectManager.h"
#include "LevelObjectBase.h"

//インスタンスの初期化
CLevelObjectManager* CLevelObjectManager::m_instance = nullptr;

//コンストラクタ
CLevelObjectManager::CLevelObjectManager()
{
	if (m_instance != nullptr) {
		//インスタンスがすでに作られている。
		std::abort();
	}
}

//デストラクタ
CLevelObjectManager::~CLevelObjectManager()
{
	//nullptrを入れておく
	m_instance = nullptr;
}

/// <summary>
/// ウェイポイントの「場所」を初期化
/// </summary>
/// <param name="vecSize">ウェイポイントのサイズ</param>
/// <param name="posMap">場所のマップ</param>
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


/// <summary>
/// ウェイポイントの「回転」を初期化
/// </summary>
/// <param name="vecSize">ウェイポイントのサイズ</param>
/// <param name="rotMap">回転のマップ</param>
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

/// <summary>
/// 自身（LevelObjectManager）にオブジェクトを追加する
/// </summary>
/// <param name="object">追加するオブジェクト</param>
void CLevelObjectManager::AddObject(ILevelObjectBase* object)
{
	m_levelObjects.push_back(object);
}

/// <summary>
/// 自身（LevelObjectManager）からオブジェクトを破棄する
/// </summary>
/// <param name="object">破棄するオブジェクト</param>
void CLevelObjectManager::RemoveObject(ILevelObjectBase* object)
{
	//一度に一個しか消せないよ
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
/// レベルオブジェクトをすべて消去する
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
		//自分自身じゃないとき
		if (lo->GetObjectType() != objectType) {
			//重なっているオブジェクトがある。
			if (IsHitObject(thisObject, *lo)) {
				return true;
			}
		}
	}
	return false;
}
