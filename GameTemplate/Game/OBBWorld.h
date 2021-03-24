#pragma once
#include "OBB.h"


class COBBWorld
{
private:	//自動で呼ばれるメンバ関数
	COBBWorld() {};			//コンストラクタををprivateに隠す
	~COBBWorld() {};		//デストラクタをprivateに隠す

private:	//staticなデータメンバ
	static COBBWorld* m_instance;	//自身の唯一のインスタンスを持つ変数

public:		//staticなメンバ関数

	/// <summary>
	/// シングルトンパターン
	/// 唯一のインスタンスを作る関数
	/// 最初に呼んでね！
	/// </summary>
	static void CreateInstance()
	{
		//唯一のインスタンスを生成する
		m_instance = new COBBWorld;
	}

	/// <summary>
	/// インスタンスを消す関数
	/// </summary>
	static void DeleteInstance()
	{
		//唯一のインスタンスを破棄する
		delete m_instance;
	}

	/// <summary>
	/// 唯一インスタンスの参照を戻す関数
	/// </summary>
	/// <returns>唯一インスタンスの参照</returns>
	static COBBWorld* GetInstance()
	{
		return m_instance;
	}

public:		//ここのメンバ関数を主に使う

	void AddOBB(COBB* obb);

	void RemoveOBB(COBB* obb);

	bool InIntersectLine(const Vector3& start, const Vector3& end, Vector3* pos = nullptr);

	COBB* HitAllOBB(const COBB& myOBB, COBB* hitOBB);

private:	//データメンバ
	std::vector<COBB*> m_worldOBBs;		//OBBWorldに登録されているOBB達

};

