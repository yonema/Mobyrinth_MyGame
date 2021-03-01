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


private:	//�f�[�^�����o
	/// <summary>
	/// �A�j���[�V�����N���b�v�B
	/// </summary>
	enum EnAnimationClip {
		enAnimClip_Idle,	//�����̃A�j���[�V�����N���b�v
		enAnimClip_Run,		//����̃A�j���[�V�����N���b�v
		enAnimClip_Num,		//�A�j���[�V�����N���b�v�̑���
	};

	AnimationClip m_animationClips[enAnimClip_Num];	//�A�j���[�V�����N���b�v�B
	CharacterController m_charaCon;

	Vector3 m_position = g_vec3Zero;

	int a = 0;

	CModelRender* m_modelRender = nullptr;
};

