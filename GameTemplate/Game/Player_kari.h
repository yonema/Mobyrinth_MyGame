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

private://privateなメンバ関数

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
	CharacterController m_charaCon;		//キャラクターコントローラー

	Vector3 m_moveSpeed = g_vec3Zero;		//キャラクターの移動スピード
	Vector3 m_position = g_vec3Zero;		//キャラクターの座標
	Quaternion m_rotation = g_quatIdentity;	//キャラクターの回転


	CModelRender* m_modelRender = nullptr;


	std::vector<Vector3> m_wayPointPos;
	std::vector<Quaternion> m_wayPointRot;
	int m_lpIndex = 0;
	int m_rpIndex = m_lpIndex + 1;
	int m_wayPointState = 0;
	int m_maxWayPointState = 0;

};

