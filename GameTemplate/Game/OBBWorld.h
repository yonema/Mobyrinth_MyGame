#pragma once
#include "OBB.h"


/// <summary>
/// OBBWorld
/// 衝突解決させるOBBを登録する場所
/// </summary>
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

	/// <summary>
	/// OBBをOBBWorldに登録する関数
	/// </summary>
	/// <param name="obb">登録するOBB</param>
	void AddOBB(const COBB& obb);

	/// <summary>
	/// OBBWorldに登録してあるOBBを解除する関数
	/// </summary>
	/// <param name="obb">登録するOBB</param>
	void RemoveOBB(const COBB& obb);


	
	/// <summary>
	/// OBBWorldに登録してあるすべてのOBBを調べて、
	/// 第一引数のOBBと衝突している一番近いOBBをを
	/// 戻す。
	/// どのOBBとも衝突していなかったら、nullptrが
	/// 戻って来る。
	/// </summary>
	/// <param name="myOBB">自身のOBB</param>
	/// <returns>衝突しているOBBか、nullptr</returns>
	const COBB* HitAllOBB(const COBB& myOBB);

private:	//データメンバ
	std::vector<const COBB*> m_worldOBBs;		//OBBWorldに登録されているOBB達

};

