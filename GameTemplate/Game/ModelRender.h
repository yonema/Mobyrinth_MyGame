#pragma once
#include "ShadowModel.h"

/// <summary>
/// ���f���`��N���X
/// </summary>
class CModelRender : public IGameObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	void Update() override final;					//�A�b�v�f�[�g�֐�
	void Render(RenderContext& rc) override final;	//�`��p�֐�

public:		//�����̃����o�֐�����Ɏg���B
	/// <summary>
	/// �f�t�H���g�̏����������֐�
	/// �ŏ��ɌĂ�ŁI
	/// </summary>
	/// <param name="filePath">tkm�̃t�@�C���p�X</param>
	/// <param name="animationClips">�A�j���[�V�����N���b�v</param>
	/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̑���</param>
	/// <param name="modelUpAxis">���f����UP��</param>
	void Init(const char* filePath,
		AnimationClip* animationClips = nullptr,
		int numAnimationClips = 0,
		EnModelUpAxis modelUpAxis = enModelUpAxisZ
		);
	
	/// <summary>
	/// �J�X�^���p�̏������֐�
	/// �ŏ��ɌĂ�ŁI
	/// �f�t�H���g��ModelInitData�Ƃ͈Ⴄ���e�ŏ���������Ƃ��Ɏg��
	/// </summary>
	/// <param name="initData">�������p�̃f�[�^</param>
	/// <param name="animationClips">�A�j���[�V�����N���b�v</param>
	/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̑���</param>
	void Init(ModelInitData initData,
		AnimationClip* animationClips = nullptr,
		int numAnimationClips = 0
	);

	/// <summary>
	/// �ꏊ��ݒ�
	/// </summary>
	/// <param name="pos">�ꏊ</param>
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	/// <summary>
	/// ��]��ݒ�
	/// </summary>
	/// <param name="rot">��]</param>
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

	/// <summary>
	/// �g���ݒ�
	/// </summary>
	/// <param name="scale">�g��</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

	/// <summary>
	/// �A�j���[�V�������Đ�����
	/// </summary>
	/// <param name="animNo">�o�^���Ă���A�j���[�V�����N���b�v�̔ԍ�</param>
	/// <param name="interpolateTime">�⊮���ԁi�P�ʁF�b�j</param>
	void PlayAnimation(int animNo, float interpolateTime = 0.0f)
	{
		m_animationPtr->Play(animNo, interpolateTime);
	}

	void SetShadowReceiverFlag(const bool shadowReceiverFlag)
	{
		m_model.SetShadowReceiverFlag(shadowReceiverFlag);
	}
	void SetShadowCasterFlag(const bool shadowCasterFlag)
	{
		if (shadowCasterFlag && !m_shadowModel.IsValid())
			InitShadowModel();

		m_model.SetShadowCasterFlag(shadowCasterFlag);
	}

	//���f���̎Q�Ƃ�Ԃ�
	Model& GetModel()
	{
		return m_model;
	}
private:	//private�ȃ����o�֐�

	/// <summary>
	/// �X�P���g�����������B
	/// tkm�t�@�C���p�X��tks�t�@�C���p�X�ɕϊ�����
	/// </summary>
	/// <param name="filePath">tkm�t�@�C���p�X</param>
	/// <returns>�������������������H</returns>
	bool InitSkeleton(const char* filePath);

	/// <summary>
	/// ���f�����������B
	/// </summary>
	/// <param name="filePath">tkm�t�@�C���p�X</param>
	/// <param name="modelUpAxis">���f����UP��</param>
	void InitModel(const char* filePath, EnModelUpAxis modelUpAxis);

	/// <summary>
	/// �A�j���[�V�������������B
	/// </summary>
	/// <param name="animationClips">�A�j���[�V�����N���b�v</param>
	/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̑���</param>
	void InitAnimation(AnimationClip* animationClips, int numAnimationClips);

	void InitShadowModel();


private://�f�[�^�����o
	Model m_model;								//���f���\������
	std::unique_ptr<Skeleton> m_skeletonPtr;	//�X�P���g���B
	std::unique_ptr<Animation> m_animationPtr;	//�A�j���V�����Đ������B
	const char* m_tkmFilePath = nullptr;

	Vector3 m_position = g_vec3Zero;			//�ʒu
	Quaternion m_rotation = g_quatIdentity;		//��]
	Vector3 m_scale = g_vec3One;				//�g��

	CShadowModel m_shadowModel;					//�V���h�E��`�悷��p�̃��f��


	bool m_isInited = false;					//�������ς݁H
};

