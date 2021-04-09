#pragma once
#include "GaussianBlur.h"

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
	CGaussianBlur m_gaussianBlur[4];		//�K�E�V�A���u���[�S��
	Sprite m_finalSprite;					//�ŏI�I�ȃX�v���C�g

};

