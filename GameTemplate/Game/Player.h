#pragma once
#include "LightData.h"
#include "DirectionLight.h"
#include "ModelRender.h"
#include "Mobius.h"
class ILevelObjectBase;
#include "OBB.h"


class Player : public IGameObject
{
public://publicなメンバ関数
	bool Start() override final;
	~Player();
	void Update() override final;
	/// <summary>
	/// プレイヤーの座標を設定
	/// </summary>
	/// <param name="pos">場所</param>
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// プレイヤーの回転を設定
	/// </summary>
	/// <param name="rot">回転</param>
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// プレイヤーの座標を取得
	/// </summary>
	/// <returns>場所</returns>
	const Vector3 GetPosition()const
	{
		return m_position;
	}

	/// <summary>
	/// ウェイポイントの「場所」を初期化
	/// </summary>
	/// <param name="vecSize">ウェイポイントのサイズ</param>
	/// <param name="posMap">場所のマップ</param>
	//void InitWayPointPos(const std::size_t vecSize, std::map<int, Vector3>& posMap);
	/// <summary>
	/// ウェイポイントの「回転」を初期化
	/// </summary>
	/// <param name="vecSize">ウェイポイントのサイズ</param>
	/// <param name="rotMap">回転のマップ</param>
	//void InitWayPointRot(const std::size_t vecSize, std::map<int, Quaternion>& rotMap);

	void SetWayPointPos(const std::size_t vecSize, std::vector<Vector3>*const posMap);
	void SetWayPointRot(const std::size_t vecSize, std::vector<Quaternion>* rotMap);

	/// <summary>
	/// 補完済みの最終的なウェイポイントの回転を取得
	/// </summary>
	/// <returns>補完済みの最終的なウェイポイントの回転</returns>
	const Quaternion GetFinalWPRot()const
	{
		return m_finalWPRot;
	}

	void SetHoldObject(const bool holdFlag)
	{
		m_holdObject = holdFlag;
	}
	void SetOperationFlag(const bool b) {
		m_operationFlag = b;
	}
	const bool GetHoldObject()const
	{
		return m_holdObject;
	}
	const Vector3 GetUpVec()const
	{
		return m_upVec;
	}
	const int GetLeftPointIndex()const
	{
		return m_lpIndex;
	}
	const int GetRightPointIndex()const
	{
		return m_rpIndex;
	}
	const Vector3 GerWayPointPos(const int index)
	{
		return (*m_wayPointPos)[index];
	}

private://privateなメンバ関数

	/// <summary>
	/// プレイヤーの初期設定
	/// </summary>
	void Init();
	/// <summary>
	/// ウェイポイントの更新処理
	/// </summary>
	void CheckWayPoint();
	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();
	/// <summary>
	/// ステージに乗る
	/// </summary>
	void GetOnStage();
	/// <summary>
	/// モデルの回転処理
	/// </summary>
	void Rotation();

public://デバック用
	void PostRender(RenderContext& rc)override final;
	Font m_font;
	CModelRender* m_dbgModel = nullptr;
	CModelRender* m_dbgModel2 = nullptr;
	CModelRender* m_dbgModel3 = nullptr;


	bool m_dbgHit = false;
	float m_dbgDot1 = 0.0f;
	float m_dbgDot2 = 0.0f;
	COBB m_obb;
	static const int m_obbNum = 8;
	CModelRender* m_dbgObbModel[m_obbNum] = { nullptr };
	COBB m_obb2;
	CModelRender* m_dbgObbCenter = nullptr;
	CModelRender* m_dbgObbModel2[m_obbNum] = { nullptr };
	bool m_dbgOBBHit = false;

private:	//データメンバ
	/// <summary>
	/// アニメーションクリップ。
	/// </summary>
	enum EnAnimationClip {
		enAnimClip_Idle,	//歩きのアニメーションクリップ
		enAnimClip_Run,		//走りのアニメーションクリップ
		enAnimClip_Num,		//アニメーションクリップの総数
	};

	AnimationClip m_animationClips[enAnimClip_Num];	//アニメーションクリップ。

	Vector3 m_moveSpeed = g_vec3Zero;		//キャラクターの移動スピード
	Vector3 m_position = g_vec3Zero;		//キャラクターの座標
	Vector3 m_onWayPosition = g_vec3Zero;		//道の上の座標
	Quaternion m_rotation = g_quatIdentity;	//キャラクターの回転
	bool m_holdObject = false;
	Vector3 m_upVec = g_vec3Up;


	enum EnLeftOrRight
	{
		enLeft,		//左
		enRight,	//右
	};
	int m_leftOrRight = enRight;	//キャラクターの左右の向き

	float m_padLStickXF = 0.0f;		//パッドの左スティックのX軸の入力情報

	CModelRender* m_modelRender = nullptr;	//モデルレンダラー

	/// <summary>
	/// ウェイポイント関連のデータメンバ
	/// </summary>
	//std::vector<Vector3> m_wayPointPos;		//ウェイポイントの「場所」のコンテナ
	//std::vector<Quaternion> m_wayPointRot;	//ウェイポイントの「回転」のコンテナ
	std::vector<Vector3>* m_wayPointPos;		//ウェイポイントの「場所」のコンテナ
	std::vector<Quaternion>* m_wayPointRot;	//ウェイポイントの「回転」のコンテナ
	int m_rpIndex = 0;				//右のウェイポイントのインデックス
	int m_lpIndex = m_rpIndex + 1;	//左のウェイポイントのインデックス
	int m_wayPointState = 0;		//自身がどのウェイポイントにいるか表すステート
	int m_maxWayPointState = 0;		//ウェイポイントステートの最大の値
	Quaternion m_finalWPRot = g_quatIdentity;	//補完済みの最終的なウェイポイントの回転

	Mobius* m_mobius = nullptr;		//ステージのメビウスの輪のポインタ

	bool m_operationFlag = false; //操作できるかのフラグ
};

