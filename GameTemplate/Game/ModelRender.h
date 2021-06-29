#pragma once
#include "LightManager.h"
#include "ShadowModel.h"
#include "GameTime.h"

/// <summary>
/// ���f���`��N���X
/// </summary>
class CModelRender : public IGameObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	void Update() override final;					//�A�b�v�f�[�g�֐�
	void Render(RenderContext& rc) override final;	//�`��p�֐�
	void UpdateWhenPaused() override final;

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
		D3D12_CULL_MODE cullMode = D3D12_CULL_MODE_BACK,
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
	void PlayAnimation(int animNo, float interpolateTime = 0.2f)
	{
		m_animationPtr->Play(animNo, interpolateTime);
	}

	/// <summary>
	/// �V���h�E���V�[�o�[�t���O��ݒ�
	/// </summary>
	/// <param name="shadowReceiverFlag">�V���h�E���V�[�o�[�H</param>
	void SetShadowReceiverFlag(const bool shadowReceiverFlag)
	{
		m_model.SetShadowReceiverFlag(shadowReceiverFlag);
	}

	/// <summary>
	/// �V���h�E�L���X�^�[�t���O��ݒ�
	/// </summary>
	/// <param name="shadowCasterFlag">�V���h�E�L���X�^�[�H</param>
	void SetShadowCasterFlag(const bool shadowCasterFlag)
	{
		//�V���h�E�L���X�^�[��true�ŁA����
		//�܂��V���h�E�p���f��������������Ă��Ȃ�������
		if (shadowCasterFlag && !m_shadowModel.IsValid())
			//�V���h�E�p���f����������
			InitShadowModel();
		else if (!shadowCasterFlag && m_shadowModel.IsValid())
			m_shadowModel.RemoveShadowModel();
	}

	/// <summary>
	/// ���Ȕ����F��ݒ肷��
	/// </summary>
	/// <param name="color">���Ȕ����F</param>
	void SetModelEmissionColor(const Vector4& color)
	{
		m_model.SetModelEmissionColor(color);
	}

	/// <summary>
	/// ��Z�J���[��ݒ肷��
	/// </summary>
	/// <param name="color">��Z�J���[</param>
	void SetMulColor(const Vector4& color)
	{
		m_model.SetMulColor(color);
	}

	/// <summary>
	/// �֊s����`�悷�邩�H��ݒ肷��
	/// </summary>
	/// <param name="drawOutLineFlag">�֊s����`�悷�邩�H</param>
	void SetDrawOutLineFlag(const bool drawOutLineFlag)
	{
		m_drawOutLineFlag = drawOutLineFlag;
		m_model.SetOutLineFlag(drawOutLineFlag);
	}



	/// <summary>
	/// �X�e���X����H��ݒ肷��
	/// </summary>
	/// <param name="stealthFlag">�X�e���X����H</param>
	void SetStealthFlag(const bool stealthFlag)
	{
		m_model.SetStealthFlag(stealthFlag);
	}

	//���f���̎Q�Ƃ�Ԃ�
	Model& GetModel()
	{
		return m_model;
	}

	/// <summary>
	/// �A�j���[�V�����̍Đ����H
	/// </summary>
	/// <returns></returns>
	const bool IsPlayingAnimation() const
	{
		return m_animationPtr->IsPlaying();
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
	void InitModel(const char* filePath, D3D12_CULL_MODE cullMode, EnModelUpAxis modelUpAxis);

	/// <summary>
	/// �A�j���[�V�������������B
	/// </summary>
	/// <param name="animationClips">�A�j���[�V�����N���b�v</param>
	/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̑���</param>
	void InitAnimation(AnimationClip* animationClips, int numAnimationClips);

	/// <summary>
	/// �V���h�E�p�̃��f����������
	/// </summary>
	void InitShadowModel();

	/// <summary>
	/// Z�v���p�X�p�̃��f����������
	/// </summary>
	void InitZPrepassModel();


private://�f�[�^�����o
	Model m_model;								//���f���\������
	std::unique_ptr<Skeleton> m_skeletonPtr;	//�X�P���g���B
	std::unique_ptr<Animation> m_animationPtr;	//�A�j���V�����Đ������B
	const char* m_tkmFilePath = nullptr;

	Vector3 m_position = g_VEC3_ZERO;			//�ʒu
	Quaternion m_rotation = g_QUAT_IDENTITY;		//��]
	Vector3 m_scale = g_VEC3_ONE;				//�g��

	CShadowModel m_shadowModel;					//�V���h�E��`�悷��p�̃��f��
	Model m_zprepassModel;						// ZPrepass�ŕ`�悳��郂�f��

	bool m_isInited = false;					//�������ς݁H
	bool m_drawOutLineFlag = false;				//�֊s����`�悷�邩�H
};

