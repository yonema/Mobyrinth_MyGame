#pragma once
#include "Player_kari.h"

class CLevelObjectBase;

class CLevelObjectManager
{
private:
	CLevelObjectManager();
	~CLevelObjectManager();

private:
	static CLevelObjectManager* m_instance;
public:
	/// <summary>
	/// シングルトンパターン
	/// 唯一のインスタンスを作る関数
	/// 最初に呼んでね！
	/// </summary>
	static void CreateInstance()
	{
		m_instance = new CLevelObjectManager;
	}

	/// <summary>
	/// インスタンスを消す関数
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
	}

	/// <summary>
	/// インスタンスの参照を戻す関数
	/// </summary>
	/// <returns>インスタンスの参照</returns>
	static CLevelObjectManager* GetInstance()
	{
		return m_instance;
	}

public:
	/// <summary>
	/// ゲームループで読んでほしい
	/// アップデート関数関数
	/// </summary>
	void ExecuteUpdate();

	Player_kari* GetPlayer()const
	{
		int a = 1;

		int b = 1;

		return m_player;
	}

	/// <summary>
	/// ウェイポイントの「場所」を初期化
	/// </summary>
	/// <param name="vecSize">ウェイポイントのサイズ</param>
	/// <param name="posMap">場所のマップ</param>
	void InitWayPointPos(const std::size_t vecSize, std::map<int, Vector3>& posMap);
	/// <summary>
	/// ウェイポイントの「回転」を初期化
	/// </summary>
	/// <param name="vecSize">ウェイポイントのサイズ</param>
	/// <param name="rotMap">回転のマップ</param>
	void InitWayPointRot(const std::size_t vecSize, std::map<int, Quaternion>& rotMap);

	std::vector<Vector3>* GetWayPointPos()
	{
		return &m_wayPointPos;
	}
	std::vector<Quaternion>* GetWayPointRot()
	{
		return &m_wayPointRot;
	}

	void AddObject(CLevelObjectBase* object);
	void RemoveObject(CLevelObjectBase* object);
private:
	Player_kari* m_player = nullptr;
	std::vector<Vector3> m_wayPointPos;		//ウェイポイントの「場所」のコンテナ
	std::vector<Quaternion> m_wayPointRot;	//ウェイポイントの「回転」のコンテナ
	int m_vecSize = 0;		//ウェイポイントステートの最大の値

	std::vector<CLevelObjectBase*> m_levelObjects;

};

