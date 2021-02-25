#pragma once
#include "LightData.h"
#include "DirectionLight.h"

class Player_kari : public IGameObject
{
public:
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& rc) override final;

private:	//メンバ関数
	/// <summary>
	/// スケルトンを初期化。
	/// </summary>
	void InitSkeleton();
	/// <summary>
	/// モデルを初期化。
	/// </summary>
	void InitModel();
	/// <summary>
	/// アニメーションを初期化。
	/// </summary>
	void InitAnimation();

private:	//データメンバ
	/// <summary>
	/// アニメーションクリップ。
	/// </summary>
	enum EnAnimationClip {
		enAnimClip_Idle,	//歩きのアニメーションクリップ
		enAnimClip_Run,		//走りのアニメーションクリップ
		enAnimClip_Num,		//アニメーションクリップの総数
	};

	Model m_model;			//モデル表示処理。
	Animation m_animation;	//アニメション再生処理。
	AnimationClip m_animationClips[enAnimClip_Num];	//アニメーションクリップ。
	Skeleton m_skeleton;	//スケルトン。
	CharacterController m_charaCon;

	CDirectionLight* lig[3] = { nullptr };


	int a = 0;

};

