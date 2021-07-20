#pragma once
#include "GaussianBlur.h"
#include "SpriteRenderConstData.h"

/**
 * @brief �u���[���̒萔�f�[�^
*/
namespace BloomConstData
{
	//�K�E�V�A���u���[�̐�
	constexpr int GAUSSIAN_BLUR_NUM = 4;

	//�K�E�V�A���u���[�̋���
	constexpr int POWER_GAUSSIAN_BLUR = 10;

	//�|�X�g�G�t�F�N�g�̃V�F�[�_�[�t�@�C���p�X
	constexpr const char* const SHADER_FILEPATH_POST_EFFECT = "Assets/shader/PostEffect.fx";

	//�P�x���o�p�X�v���C�g�p�̃s�N�Z���V�F�[�_�[
	constexpr const char* const ENTRY_POINT_FUNC_PS_LUMINANCE_SPRITE = "PSSamplingLuminance";

	//�u���[���p�̃s�N�Z���V�F�[�_�[
	constexpr const char* const ENTRY_POINT_FUNC_PS_BLOOM = "PSBloomFinal";
}

class CBloom
{
public:		//�����ŌĂ΂�郁���o�֐�
	CBloom();		//�R���X�g���N�^
	~CBloom();		//�f�X�g���N�^

public:		//�����o�֐�

	/// <summary>
	/// �������֐�
	/// </summary>
	void Init();

	/// <summary>
	/// �`��֐�
	/// </summary>
	/// <param name="renderContext">�����_�[�R���e�L�X�g</param>
	void Draw(RenderContext& renderContext);

	/// <summary>
	/// �u���[���̌��ʂ����C�������_�[�^�[�Q�b�g�ɕ`�悷��֐�
	/// </summary>
	/// <param name="renderContext">�����_�[�R���e�L�X�g</param>
	void DrawToMainRenderTarget(RenderContext& renderContext);

private:	//private�ȃ����o�֐�

	/// <summary>
	/// �P�x���o�p�̃����_�����O�^�[�Q�b�g�̏�����
	/// </summary>
	void InitluminanceRenderTarget();

	/// <summary>
	/// �P�x���o�p�̃X�v���C�g�̏�����
	/// </summary>
	void InitluminanceSprite();

	/// <summary>
	/// �K�E�V�A���u���[�̏�����
	/// </summary>
	void InitGaussianBlur();

private:	//�f�[�^�����o
	RenderTarget m_luminanceRenderTarget;	//�P�x���o�p�̃����_�����O�^�[�Q�b�g
	Sprite m_luminanceSprite;				//�P�x���o�p�̃X�v���C�g
	CGaussianBlur m_gaussianBlur[BloomConstData::GAUSSIAN_BLUR_NUM];	//�K�E�V�A���u���[�S��
	Sprite m_finalSprite;					//�ŏI�I�ȃX�v���C�g

};

