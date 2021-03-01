#pragma once
#include "LightData.h"
#include "DirectionLight.h"
#include "ModelRender.h"

class Player_kari : public IGameObject
{
public:
	bool Start() override final;
	void Update() override final;
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}


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
	CharacterController m_charaCon;

	Vector3 m_position = g_vec3Zero;

	int a = 0;

	CModelRender* m_modelRender = nullptr;
};

