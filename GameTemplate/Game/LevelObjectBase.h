#pragma once
#include "ModelRender.h"
#include "LevelObjectManager.h"
#include "Player.h"
#include "OBB.h"
#include "ObjectType.h"
#include "FontRender.h"
#include "effect/Effect.h"
#include "ModelRender.h"

/**
 * @brief レベルオブジェクトの抽象クラス
 * @note ReversibleObjectとObstacleObjectはこの抽象クラスを継承している
*/
class ILevelObjectBase : public IGameObject
{
public:		//自動で呼ばれるメンバ関数
	bool Start()override final;				//スタート関数
	virtual ~ILevelObjectBase();			//デストラクタ
	void Update()override final;			//アップデート関数

public:		//オーバーライドしてほしいメンバ関数
	virtual bool PureVirtualStart() = 0 {};		//スタート関数
	virtual void PureVirtualUpdate() = 0 {};	//アップデート関数

	/// <summary>
	/// 表裏がある場合は、表か裏かのモデルの参照を得る。
	/// ない場合は、普通にモデルの参照を得る
	/// </summary>
	/// <param name="frontOrBack">表か裏か？ない場合は関係ない</param>
	/// <returns>モデルの参照</returns>
	virtual CModelRender* GetModelRender(const int frontOrBack = 0)
	{
		return nullptr;
	}


public:		//CReversibleObjectでオーバーライドしてほしいメンバ関数

	/**
	 * @brief 反転オブジェクト用のスイッチをオンにするときの処理
	*/
	virtual void ReversibleSwitchOn() {};

	/**
	 * @brief 反転オブジェクト用のスイッチをオフにするときの処理
	*/
	virtual void ReversibleSwitchOff() {};

	/**
	 * @brief 反転オブジェクト用の全反転するときの処理
	*/
	virtual void AllReverse() {};

public:		//メンバ関数

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
		m_isLock = lock;
	}

	/// <summary>
	/// ロック中か？を得る
	/// Tips表示や全反転がロックされているか。
	/// </summary>
	/// <returns>ロック中か？</returns>
	const bool GetLock() const
	{
		return m_isLock;
	}

	/// <summary>
	/// OBBの参照を戻す
	/// </summary>
	/// <returns>OBBの参照</returns>
	const COBB& GetOBB() const
	{
		return m_obb;
	}

	/**
	 * @brief OBBの方向ベクトルの長さを設定する
	 * @param [in] directionLenght 方向ベクトルの長さ
	*/
	void SetOBBDirectionLength(const Vector3& directionLenght)
	{
		m_obb.SetDirectionLength(directionLenght);
	}

	/**
	 * @brief OBBのピボットを設定する
	 * @param [in] pivot ピボット
	*/
	void SetOBBPivot(const Vector3& pivot)
	{
		m_obb.SetPivot(pivot);
	}

	/**
	 * @brief OBBのタグを設定する
	 * @param [in] tag タグを設定する
	*/
	void SetOBBTag(const int tag)
	{
		m_obb.SetTag(tag);
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

	/**
	 * @brief ウェイポイントからの上への距離を設定
	 * @param [in] yPosLen 上への距離
	*/
	void SetYPosLen(const float yPosLen)
	{
		m_yPosLen = yPosLen;
	}

	/// <summary>
	/// 重なっているかの判定の処理を行うか確認するフラグの値を変更する。
	/// </summary>
	/// <param name="b">フラグの値</param>
	void SetIsHitFlag(const bool b)
	{
		m_isHitFlag = b;
	}

	/// <summary>
	/// 重なっているかの判定の処理を行うか確認するフラグの値を取得
	/// </summary>
	/// <returns>m_flagIsHitの値</returns>
	const bool GetIsHitFlag()const
	{
		return m_isHitFlag;
	}

	/// <summary>
	/// 透明オブジェクトに使用するデータを初期化する。
	/// </summary>
	void SetTransparentObject();

	/// <summary>
	/// 透明スイッチが押されたときに使用される関数
	/// </summary>
	void TransparentSwitchOn();

	/// <summary>
	/// 透明スイッチの効果が消えたときに使用される関数
	/// </summary>
	void TransparentSwitchOff();


	/**
	 * @brief 透明オブジェクトかどうか？を得る
	 * @return 透明オブジェクトかどうか？
	*/
	const bool GetFlagTransparentObject() const
	{
		return m_transparentObjectFlag;
	}


	/**
	 * @brief タイマーのフォントの座標を設定する
	 * @param [in] position 座標
	*/
	void SetTimerFontPosition(const Vector2& position)
	{
		m_timerFR->SetPosition(position);
	}

	/**
	 * @brief タイマーのフォントのカラーを設定する
	 * @param  [in] color カラー
	*/
	void SetTimerFontColor(const Vector4& color)
	{
		m_timerFR->SetColor(color);
	}

	/**
	 * @brief タイマーのフォントのテキストを設定
	 * @param [in] text テキスト
	*/
	void SetTimerFontText(const wchar_t* const text)
	{
		m_timerFR->SetText(text);
	}


	/**
	 * @brief タイマーのフォントのアクティブフラグを設定する
	 * @param [in] activeFlag アクティブフラグ
	*/
	void SetTimerFontAitiveFlag(const bool activeFlag)
	{
		if (activeFlag)
			m_timerFR->Activate();
		else
			m_timerFR->Deactivate();
	}

protected:	//protectedなメンバ関数

	/**
	 * @brief 自身の現在の座標から、ウェイポイントの番号を決定し、回転と座標をイイ感じに合わせる
	 * @param [in] checkRotaton 回転チェックを行うか？
	 * @param [in] checkPosition 座標チェックを行うか？
	*/
	void CheckWayPoint(const bool checkRotaton = true, const bool checkPosition = true);

	/**
	 * @brief 現在の座標に合わせた回転にする
	*/
	void CheckRotation();

	/**
	 * @brief 座標をきれいにメビウスの輪の上に配置する
	*/
	void CheckPosition();





private:	//privateなメンバ関数

	/// <summary>
	/// OBBの初期化関数
	/// </summary>
	void InitOBB();




protected:	//protectedなデータメンバ	//あんま良くないけど利便性のために
	Vector3 m_position = g_VEC3_ZERO;			//場所
	Quaternion m_rotation = g_QUAT_IDENTITY;	//回転
	Vector3 m_scale = g_VEC3_ONE;				//拡大
	Player* m_player = nullptr;					//プレイヤーのポインタ

private:	//データメンバ

	/*
	* フラグ関連
	*/
	bool m_isDead = false;					//死んでいるか？
	bool m_isLock = false;					//ロック中か？、Tips表示や全反転をロックする
	bool m_transparentObjectFlag = false;	//透明オブジェクトどうかのフラグ
	bool m_isHitFlag = true;				//重なっているかの判定の処理を行うか確認するフラグ

	/*
	* ウェイポイント関連
	*/
	int m_lpIndex = 0;						//自身の左側のウェイポイントのインデックス
	int m_rpIndex = 0;						//自身の右側のウェイポイントのインデックス
	float m_yPosLen = 0.0f;					//ウェイポイントからの上への距離
	float m_zPosLen = 0.0f;					//ウェイポイントからの奥行の距離
	int m_frontOrBackSide = EB_NONE_SIDE;	//自身が表側にあるか裏側にあるか

	/*
	* オブジェクトタイプ関連
	*/
	int m_objectType = EN_OBJECT_TYPE_EMPTY;//タイプ

	/*
	* OBB関連
	*/
	COBB m_obb;								//OBBの当たり判定


	/*
	* エフェクト関連
	*/
	Effect* m_swichonEffect = nullptr;				//スイッチを押したときに出るエフェクト
	Effect* m_swichoffEffect = nullptr;				//スイッチが戻るときのエフェクト


	/*
	* フォント関連
	*/
	CFontRender* m_timerFR = nullptr;			//タイマーのフォントレンダラー


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
bool IsHitLevelObject
(const ILevelObjectBase& lhs, const ILevelObjectBase& rhs);