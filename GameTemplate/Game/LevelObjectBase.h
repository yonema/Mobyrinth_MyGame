#pragma once
#include "ModelRender.h"
#include "LevelObjectManager.h"
#include "Player.h"
#include "OBB.h"
#include "ObjectType.h"

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
	void CheckRotation();

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
	const Vector3& GetPosition()const
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
	const Quaternion& GetRotation()const
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
	const Vector3& GetScale()const 
	{
		return m_scale;
	}

	/// <summary>
	/// タイプを設定する
	/// タイプはObjectType.hのEnObjectTypeを参照
	/// </summary>
	/// <param name="objectType">タイプ</param>
	void SetObjectType(int objectType)
	{
		m_objectType = objectType;
	}

	/// <summary>
	/// タイプを取得する
	/// タイプはObjectType.hのEnObjectTypeを参照
	/// </summary>
	/// <returns>タイプ</returns>
	const int GetObjectType()const
	{
		return m_objectType;
	}


	/// <summary>
	/// ロック中か？設定する。
	/// Tips表示や全反転をロックする
	/// </summary>
	/// <param name="lock">ロックするか？</param>
	void SetLock(const bool lock)
	{
		m_lock = lock;
	}

	/// <summary>
	/// ロック中か？を得る
	/// Tips表示や全反転がロックされているか。
	/// </summary>
	/// <returns>ロック中か？</returns>
	const bool GetLock() const
	{
		return m_lock;
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

	/// <summary>
	/// 自身の左側のウェイポイントのインデックスを戻す
	/// </summary>
	/// <returns>左側のウェイポイントのインデックス</returns>
	const int GetLeftWayPointIndex() const
	{
		return m_lpIndex;
	}

	/// <summary>
	/// 自身の左側のウェイポイントのインデックスを設定する
	/// </summary>
	/// <param name="lpIndex">左側のウェイポイントのインデックス</param>
	void SetLeftWayPointIndex(const int lpIndex)
	{
		m_lpIndex = lpIndex;
		m_rpIndex = m_lpIndex - 1;
		const int maxWayPoint = 31;
		if (m_rpIndex < 0)
		{
			m_rpIndex = maxWayPoint;
		}
	}

	/// <summary>
	/// 自身の右側のウェイポイントのインデックスを戻す
	/// </summary>
	/// <returns></returns>
	const int GetRightWayPointIndex() const
	{

		return m_rpIndex;
	}

	/// <summary>
	/// 表側にあるか裏側にあるかを戻す
	/// </summary>
	/// <returns>表側か裏側か</returns>
	const int GetFrontOrBackSide() const
	{
		return m_frontOrBackSide;
	}

	/// <summary>
	/// 自身が表側にあるか裏側にあるかを調べる関数
	/// </summary>
	void CheckFrontOrBackSide();

private:	//privateなメンバ関数

	/// <summary>
	/// OBBの初期化関数
	/// </summary>
	void InitOBB();

protected:	//protectedなデータメンバ	//あんま良くないけど利便性のために
	Vector3 m_position = g_vec3Zero;		//場所
	Quaternion m_rotation = g_quatIdentity;	//回転
	Vector3 m_scale = g_vec3One;			//拡大
	Player* m_pPlayer = nullptr;			//プレイヤーのポインタ
	

private:	//データメンバ
	int m_objectType = enEmpty;				//タイプ
	bool m_isDead = false;					//死んでいるか？
	COBB m_obb;								//OBBの当たり判定
	int m_lpIndex = 0;						//自身の左側のウェイポイントのインデックス
	int m_rpIndex = 0;
	int m_frontOrBackSide = CLevelObjectManager::enNone;	//自身が表側にあるか裏側にあるか
	bool m_lock = false;					//ロック中か？、Tips表示や全反転をロックする


	////////////////////////////////////////////////////////////
	// 透明オブジェクト用の変数と関数
	////////////////////////////////////////////////////////////
public: //Set関数
	/// <summary>
	/// 重なっているかの判定の処理を行うか確認するフラグの値を変更する。
	/// </summary>
	/// <param name="b">フラグの値</param>
	void SetFlagIsHit(const bool b)
	{
		m_flagIsHit = b;
	}

	const bool GetFlagIsHit()const
	{
		return m_flagIsHit;
	}

	/// <summary>
	/// 透明オブジェクトに使用するデータを初期化する。
	/// </summary>
	void SetTransparentObject()
	{
		//透明オブジェクト判定に使用するフラグをtrueにする。
		m_flagTransparentObject = true;
		//オブジェクトの重なっている判定を行わないようにする。
		m_flagIsHit = false;
		//リセット時に使用する位置、回転情報を初期化
		m_startPosition = m_position;
		m_startRotation = m_rotation;
	}


public: //透明スイッチに使用する関数
	void TransparentSwitchOn();


	void TransparentSwitchOff();


protected: //メンバ変数
	bool m_flagTransparentObject = false; //透明オブジェクトどうかのフラグ
	bool m_flagIsHit = true; //重なっているかの判定の処理を行うか確認するフラグ

	Vector3 m_startPosition = { 0.0f,0.0f,0.0f }; //オブジェクトの初期位置を保存する位置情報変数
	Quaternion m_startRotation = g_quatIdentity; //オブジェクトの初期回転を保存する回転情報変数




	//デバック用
	//後で消す
private:
	//int m_objectNumber = 0;		//何番目に作られたオブジェクトか保持する
	//static int objectNumber;	//何番目に作られたオブジェクトかカウントする

	//static const int m_dbgOBBNum = 8;			//OBBの頂点の数
	//CModelRender* m_dbgOBBVert[m_dbgOBBNum];	//OBBの頂点を見るためのモデルレンダラー
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