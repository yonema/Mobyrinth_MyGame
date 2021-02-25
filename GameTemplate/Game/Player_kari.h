#pragma once
#include "LightData.h"
#include "DirectionLight.h"

class Player_kari : public IGameObject
{
public:
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& rc) override final;

private:	//�����o�֐�
	/// <summary>
	/// �X�P���g�����������B
	/// </summary>
	void InitSkeleton();
	/// <summary>
	/// ���f�����������B
	/// </summary>
	void InitModel();
	/// <summary>
	/// �A�j���[�V�������������B
	/// </summary>
	void InitAnimation();

private:	//�f�[�^�����o
	/// <summary>
	/// �A�j���[�V�����N���b�v�B
	/// </summary>
	enum EnAnimationClip {
		enAnimClip_Idle,	//�����̃A�j���[�V�����N���b�v
		enAnimClip_Run,		//����̃A�j���[�V�����N���b�v
		enAnimClip_Num,		//�A�j���[�V�����N���b�v�̑���
	};

	Model m_model;			//���f���\�������B
	Animation m_animation;	//�A�j���V�����Đ������B
	AnimationClip m_animationClips[enAnimClip_Num];	//�A�j���[�V�����N���b�v�B
	Skeleton m_skeleton;	//�X�P���g���B
	CharacterController m_charaCon;

	CDirectionLight* lig[3] = { nullptr };


	int a = 0;

};

