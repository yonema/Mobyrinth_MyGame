#pragma once

/**
 * @brief �K�E�V�A���u���[�̒萔�f�[�^
*/
namespace gaussianBlurConstData
{
	//�d�݂̐�
	constexpr int NUM_WEIGHTS = 8;

	//�K�E�V�A���u���[�̃V�F�[�_�[�t�@�C���p�X
	constexpr const char* const SHADER_FILEPATH_GAUSSIAN_BLUR = "Assets/shader/gaussianBlur.fx";

	//X�u���[�p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g
	constexpr const char* const ENTRY_POINT_FUNC_VS_X_BLUR = "VSXBlur";

	//Y�u���[�p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g
	constexpr const char* const ENTRY_POINT_FUNC_VS_Y_BLUR = "VSYBlur";

	//�u���[�p�̃s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g
	constexpr const char* const ENTRY_POINT_FUNC_PS_BLUR = "PSBlur";
}

/// <summary>
/// �K�E�V�A���u���[�N���X�B
/// </summary>
class CGaussianBlur : private Noncopyable
{
public:		//�����o�֐�
	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="originalTexture">�K�E�V�A���u���[��������I���W�i���e�N�X�`���B</param>
	void Init(Texture* originalTexture);

	/// <summary>
	/// �K�E�V�A���u���[��GPU��Ŏ��s�B
	/// </summary>
	/// <remarks>
	/// �{�֐��̌Ăяo���́ADirectX12�𗘗p�����`��R�}���h�������ɌĂяo���K�v������܂��B
	/// </remarks>
	/// <param name="rc">�����_�����O�^�[�Q�b�g</param>
	/// <param name="blurPower">�u���[�̋����B�l���傫���قǃ{�P�������Ȃ�B</param>
	void ExecuteOnGPU(RenderContext& rc, float blurPower);

	/// <summary>
	/// �{�P�e�N�X�`�����擾�B
	/// </summary>
	/// <returns></returns>
	Texture& GetBokeTexture()
	{
		return m_yBlurRenderTarget.GetRenderTargetTexture();
	}

private:	//private�ȃ����o�֐�
	/// <summary>
	/// �����_�����O�^�[�Q�b�g���������B
	/// </summary>
	void InitRenderTargets();

	/// <summary>
	/// �X�v���C�g���������B
	/// </summary>
	void InitSprites();

	/// <summary>
	/// �d�݃e�[�u�����X�V����B
	/// </summary>
	void UpdateWeightsTable(float blurPower);

private:	//�f�[�^�����o
	float m_weights[gaussianBlurConstData::NUM_WEIGHTS];			//�d�݃e�[�u���B
	Texture* m_originalTexture = nullptr;	//�I���W�i���e�N�X�`���B
	RenderTarget m_xBlurRenderTarget;		//���{�P�摜��`�悷�郌���_�����O�^�[�Q�b�g�B
	RenderTarget m_yBlurRenderTarget;		//�c�{�P�摜��`�悷�郌���_�����O�^�[�Q�b�g�B
	Sprite m_xBlurSprite;					//���{�P�摜��`�悷�邽�߂̃X�v���C�g�B
	Sprite m_yBlurSprite;					//�c�{�P�摜��`�悷�邽�߂̃X�v���C�g�B
};

