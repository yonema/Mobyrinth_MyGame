#pragma once
#include "LightData.h"
#include "DirectionLight.h"
#include "ModelRender.h"

class Player_kari : public IGameObject
{
public://publicなメンバ関数
	bool Start() override final;
	~Player_kari();
	void Update() override final;
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}
	const Vector3& GetPosition()const
	{
		return m_position;
	}


	void InitWayPointPos(const std::size_t vecSize, std::map<int, Vector3>& posMap);
	void InitWayPointRot(const std::size_t vecSize, std::map<int, Quaternion>& rotMap);

	const Quaternion GetFinalWPRot()const
	{
		return m_finalWPRot;
	}

private://privateなメンバ関数

	/// <summary>
	/// ウェイポイントの更新処理
	/// </summary>
	void CheckWayPoint();
	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();
	/// <summary>
	/// モデルの回転処理
	/// </summary>
	void Rotation();

public://デバック用
	void PostRender(RenderContext& rc)override final;
	Font m_font;

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
	Quaternion m_rotation = g_quatIdentity;	//キャラクターの回転
	enum EnLeftOrRight
	{
		enLeft,		//左
		enRight,	//右
	};
	int m_leftOrRight = enLeft;		//キャラクターの左右の向き

	float m_padLStickXF = 0.0f;		//パッドの左スティックのX軸の入力情報

	CModelRender* m_modelRender = nullptr;	//モデルレンダラー

	/// <summary>
	/// ウェイポイント関連のデータメンバ
	/// </summary>
	std::vector<Vector3> m_wayPointPos;		//ウェイポイントの「場所」のコンテナ
	std::vector<Quaternion> m_wayPointRot;	//ウェイポイントの「回転」のコンテナ
	int m_rpIndex = 0;				//右のウェイポイントのインデックス
	int m_lpIndex = m_rpIndex + 1;	//左のウェイポイントのインデックス
	int m_wayPointState = 0;		//自身がどのウェイポイントにいるか表すステート
	int m_maxWayPointState = 0;		//ウェイポイントステートの最大の値
	Quaternion m_finalWPRot = g_quatIdentity;	//補完済みの最終的なウェイポイントの回転

};

