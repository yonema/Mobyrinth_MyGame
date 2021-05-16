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

	/// <summary>5
	/// 表裏がある場合は、表か裏かのモデルの参照を得る。
	/// ない場合は、普通にモデルの参照を得る
	/// </summary>
	/// <param name="frontOrBack">表か裏か？ない場合は関係ない</param>
	/// <returns>モデルの参照</returns>
	virtual CModelRender* GetModelRender(const int frontOrBack = 0)
	{
		return nullptr;
	}

	virtual void SwitchReverse(const bool frontOrBack = 0) {};

protected:	//ここのメンバ関数を主に使う

	/// <summary>
	/// 近くのウェイポイントを探して、イイ感じに回転する関数
	/// </summary>
	/// <param name="checkRotaton">回転チェックを行うか？</param>
	/// <param name="checkPosition">座標チェックを行うか？</param>
	void CheckWayPoint(const bool checkRotaton = true, const bool checkPosition = true);

	/// <summary>
	/// 現在の座標に合わせた回転にする
	/// </summary>
	void CheckRotation();

	/// <summary>
	/// ウェイポイントにそろえた座標にする
	/// </summary>
	void CheckPosition();

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
	/// <param name="reversibleObject">反転オブジェクトか？</param>
	void CheckFrontOrBackSide(const bool reversibleObject = true);
	
	/// <summary>
	/// ウェイポイントからの奥行の距離を設定
	/// </summary>
	/// <param name="zPosLen">奥行の距離</param>
	void SetZPosLen(const float zPosLen)
	{
		m_zPosLen = zPosLen;
	}

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
	float m_zPosLen = 0.0f;					//ウェイポイントからの奥行の距離
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

	/// <summary>
	/// 重なっているかの判定の処理を行うか確認するフラグの値を取得
	/// </summary>
	/// <returns>m_flagIsHitの値</returns>
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
		//m_startRotation = m_rotation;
		m_startPosition = m_position;

		//リセット時に使用する表裏情報を初期化
		m_startfrontOrBack = m_frontOrBack;
		

		//オブジェクトを半透明にする。
		//GetModelRender(CReversibleObject::enFront)->SetMulColor({ 1.0f,1.0f,1.0f,0.5f });
		//GetModelRender(CReversibleObject::enBack)->SetMulColor({ 1.0f,1.0f,1.0f,0.5f });
		//上記の半透明にする処理がうまく動かなかった場合、
		//ChangeTransparent()関数を使ってください。
	}


public: //透明スイッチに使用する関数
	/// <summary>
	/// 透明スイッチが押されたときに使用される関数
	/// </summary>
	void TransparentSwitchOn()
	{
		//オブジェクトを持ち上げられるようにする。
		m_flagHeld = true;
		//オブジェクトの衝突判定を行うようにする。
		m_flagIsHit = true;

		//オブジェクトの当たり判定を有効にする。
		m_obb.SetExceptionFlag(false);
	}

	/// <summary>
	/// 透明スイッチの効果が消えたときに使用される関数
	/// </summary>
	void TransparentSwitchOff()
	{
		//オブジェクトを持ち上げられないようにする。
		m_flagHeld = false;
		//オブジェクトの衝突判定を行わないようにする。
		m_flagIsHit = false;
		//位置、回転情報を初期状態に戻す。
		m_rotation = m_startRotation;
		m_position = m_startPosition;
		//表裏情報を初期状態に戻す。
		m_frontOrBack = m_startfrontOrBack;
		CheckWayPoint();
		CheckFrontOrBackSide();
		SwitchReverse(m_frontOrBack);

		//オブジェクトの当たり判定を無効にする。
		m_obb.SetExceptionFlag(true);

	}

	/// <summary>
	/// オブジェクトが現在持ち上げられるかのフラグの値を代入する。
	/// </summary>
	/// <param name="b">オブジェクトが現在持てるか</param>
	void SetFlagHeld(bool b)
	{
		m_flagHeld = b;
	}

	bool GetFlagHeld()
	{
		return m_flagHeld;
	}




	//void SetFlagHeldPlayer(bool b)
	//{
	//	m_flagHeldPlayer = b;
	//}

	//bool GetFlagHeldPlayer()
	//{
	//	return m_flagHeldPlayer;
	//}

	bool GetFlagTransparentObject()
	{
		return m_flagTransparentObject;
	}


private: //メンバ変数
	bool m_flagTransparentObject = false; //透明オブジェクトどうかのフラグ
	bool m_flagIsHit = true; //重なっているかの判定の処理を行うか確認するフラグ
	bool m_flagHeld = true; //オブジェクトが現在持ち上げられるかのフラグ
	//bool m_flagHeldPlayer = false; //現在このオブジェクトが持たれているかのフラグ

	Vector3 m_startPosition = { 0.0f,0.0f,0.0f }; //オブジェクトの初期位置を保存する位置情報変数
	Quaternion m_startRotation = g_quatIdentity; //オブジェクトの初期回転を保存する回転情報変数


	////////////////////////////////////////////////////////////
	// 反転オブジェクト用の変数と関数（透明オブジェクトのために移動）
	////////////////////////////////////////////////////////////

public:		//publicなデータメンバ

	/// <summary>
	/// 表か裏かを表す列挙体
	/// </summary>
	enum EnFrontAndBack
	{
		enFront,			//表状態
		enBack,				//裏状態
		enFrontAndBackNum,	//表裏の数
	};

protected:
	bool m_frontOrBack = enFront;				//表か裏か？
	bool m_startfrontOrBack = enFront;



	//デバック用
	//後で消す
private:
#ifdef MY_DEBUG
	int m_objectNumber = 0;		//何番目に作られたオブジェクトか保持する
	static int objectNumber;	//何番目に作られたオブジェクトかカウントする

	static const int m_dbgOBBNum = 8;			//OBBの頂点の数
	CModelRender* m_dbgOBBVert[m_dbgOBBNum];	//OBBの頂点を見るためのモデルレンダラー
#endif
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