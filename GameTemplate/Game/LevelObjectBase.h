#pragma once
#include "ModelRender.h"
#include "LevelObjectManager.h"
#include "Player.h"
#include "OBB.h"

//デバック用
//後で消す
#include "ModelRender.h"

/// <summary>
/// レベルオブジェクトの抽象クラス
/// ReversibleObjectとObstacleObjectはこの抽象クラスを継承している
/// </summary>
class ILevelObjectBase : public IGameObject
{
public:		//自動で呼ばれるメンバ関数
	bool Start()override final;				//スタート関数
	virtual ~ILevelObjectBase();			//デストラクタ
	void Update()override final;			//アップデート関数

public:		//オーバーライドしてほしいメンバ関数
	virtual bool PureVirtualStart() = 0 {};		//スタート関数
	virtual void PureVirtualUpdate() = 0 {};	//アップデート関数

protected:	//ここのメンバ関数を主に使う
	/// <summary>
	/// 近くのウェイポイントを探して、イイ感じに回転する関数
	/// </summary>
	void CheckWayPoint();

public:		//ここのメンバ関数を主に使う
	
	/// <summary>
	///	場所を設定する
	/// </summary>
	/// <param name="pos">場所</param>
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	/// <summary>
	/// 場所を取得する
	/// </summary>
	/// <returns>場所</returns>
	const Vector3 GetPosition()const
	{
		return m_position;
	}

	/// <summary>
	/// 回転を設定する
	/// </summary>
	/// <param name="rot">回転</param>
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

	/// <summary>
	/// 回転を取得する
	/// </summary>
	/// <returns>回転</returns>
	const Quaternion GetRotation()const
	{
		return m_rotation;
	}

	/// <summary>
	/// 拡大を設定
	/// </summary>
	/// <param name="scale">拡大</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

	/// <summary>
	/// 拡大を取得
	/// </summary>
	/// <returns>拡大</returns>
	const Vector3 GetScale()const 
	{
		return m_scale;
	}

	/// <summary>
	/// タイプを設定する
	/// タイプはLevelObjectBase.hのEnObjectTypeを参照
	/// </summary>
	/// <param name="objectType">タイプ</param>
	void SetObjectType(int objectType)
	{
		m_objectType = objectType;
	}

	/// <summary>
	/// タイプを取得する
	/// タイプはLevelObjectBase.hのEnObjectTypeを参照
	/// </summary>
	/// <returns>タイプ</returns>
	const int GetObjectType()const
	{
		return m_objectType;
	}

	/// <summary>
	/// OBBの参照を戻す
	/// </summary>
	/// <returns>OBBの参照</returns>
	COBB& GetOBB()
	{
		return m_obb;
	}

	/// <summary>
	/// 死んでいるフラグを取得
	/// </summary>
	/// <returns></returns>
	const bool GetIsDead()const
	{
		return m_isDead;
	};

	/// <summary>
	/// LevelObjectManagerに死んでいるよって伝える
	/// </summary>
	void Delete()
	{
		m_isDead = true;
	}



	/// <summary>
	/// 自身とプレイヤーの当たり判定
	/// </summary>
	/// <returns>trueが戻ってきたら当たっている</returns>
	bool IsHitPlayer();

private:	//privateなメンバ関数

	/// <summary>
	/// OBBの初期化関数
	/// </summary>
	void InitOBB();

public:		//publicなデータメンバ
	/// <summary>
	/// オブジェクトの番号
	/// オブジェクトを増やしたらここで番号を増やすこと
	/// </summary>
	enum EnObjectType
	{
		enEnpty,

		//反転オブジェクト
		enWater,
		enIce,
		enBird,
		enFish,
		enGrilledChicken,
		enGrilledFish,
		enRunning,
		enStop,
		enWire,
		enString,
		enNail,
		enBar,
		enAxe,
		enPickaxe,
		enKeymold,
		enKey,
		enPadlock,

		//障害オブジェクト
		enObsatcle,

		enBigFire,
		enWall,
		enGoal,
		enNotHavePadlock,
		enBox,

		enEnd
	};

protected:	//protectedなデータメンバ	//あんま良くないけど利便性のために
	Vector3 m_position = g_vec3Zero;		//場所
	Quaternion m_rotation = g_quatIdentity;	//回転
	Vector3 m_scale = g_vec3One;			//拡大
	Player* m_pPlayer = nullptr;			//プレイヤーのポインタ

private:	//データメンバ
	int m_objectType = enEnpty;				//タイプ
	bool m_isDead = false;					//死んでいるか？
	COBB m_obb;								//OBBの当たり判定

	//デバック用
	//後で消す
private:
	int m_objectNumber = 0;		//何番目に作られたオブジェクトか保持する
	static int objectNumber;	//何番目に作られたオブジェクトかカウントする

	static const int m_dbgOBBNum = 8;			//OBBの頂点の数
	CModelRender* m_dbgOBBVert[m_dbgOBBNum];	//OBBの頂点を見るためのモデルレンダラー
	//デバック用ここまで
};



//ここからクラス外関数


/// <summary>
/// OBB同士の当たり判定
/// </summary>
/// <param name="lhs">レベルオブジェクト1</param>
/// <param name="rhs">レベルオブジェクト2</param>
/// <returns>trueが戻ってきたら当たっている</returns>
bool IsHitObject
(ILevelObjectBase& lhs, ILevelObjectBase& rhs);