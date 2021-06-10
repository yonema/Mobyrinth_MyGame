#pragma once
#include "LightManager.h"

/**
 * @brief CShadowMap������������Ƃ��̒萔�l
*/
namespace shadowConstData
{
	//�e����郉�C�g�̕����̏����l
	const Vector3 SHADOW_INIT_DIRECTION = { 1.0f,-1.0f,-1.0f };
	//�e����郉�C�g�̋����̏����l
	const float SHADOW_INIT_LENGTH = 1000.0f;
}

/// <summary>
/// �V���h�E�}�b�v�N���X
/// </summary>
class CShadowMap
{
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
	/// �e�𐶐����郉�C�g�𐶐�����
	/// </summary>
	/// <param name="direction">�e����郉�C�g�̕���</param>
	/// <param name="length">���C�g���ǂꂭ�炢����Ă��邩</param>
	/// <param name="target">���C�g���Ƃ炷�ڕW</param>
	void CreateShadowMap
	(const Vector3& direction, const float length = 500.0f, const Vector3& target = g_VEC3_ZERO);

	/// <summary>
	/// �e�𐶐����郉�C�g�̃p�����[�^�[�ݒ肷��
	/// </summary>
	/// <param name="direction">�e����郉�C�g�̕���</param>
	/// <param name="length">���C�g���ǂꂭ�炢����Ă��邩</param>
	/// <param name="target">���C�g���Ƃ炷�ڕW</param>
	void SetShadowParam
	(const Vector3& direction, const float length, const Vector3& target);

	/// <summary>
	/// �V���h�E�̃p�����[�^�̃|�C���^�̏�����
	/// </summary>
	/// <returns>�V���h�E�̃p�����[�^�̃|�C���^</returns>
	ShadowParam* GetShadowParam()
	{
		return m_shadowParam;
	}

	/// <summary>
	/// �K�E�V�A���u���[�̃e�N�X�`���̎擾
	/// </summary>
	/// <returns>�K�E�V�A���u���[�̃e�N�X�`��</returns>
	Texture& GetShadowBlur()
	{
		return m_gaussianBlur.GetBokeTexture();
	}

	/// <summary>
	/// �V���h�E�}�b�v�ɕ`�悷��V���h�E�p���f���̓o�^
	/// </summary>
	/// <param name="shadowModel">�o�^����V���h�E�p���f��</param>
	void AddShadowModel(Model& shadowModel);

	/// <summary>
	/// �V���h�E�}�b�v����V���h�E�p���f����j������
	/// </summary>
	/// <param name="shadowModel">�j������V���h�E�p���f��</param>
	void RemoveShadowModel(Model& shadowModel);

private:	//private�ȃ����o�֐�

	/// <summary>
	/// �V���h�E�}�b�v�̃����_�[�^�[�Q�b�g�̏�����
	/// </summary>
	void InitShadowMapRenderTarget();

	/// <summary>
	/// �K�E�V�A���u���[�̏�����
	/// </summary>
	void InitGaussianBlur();

private:	//�f�[�^�����o

	ShadowParam m_shadowParam[g_max_shadowMap];	//�V���h�E�̃p�����[�^
	RenderTarget m_shadowMapRenderTarget;		//�V���h�E�}�b�v�̃����_�[�^�[�Q�b�g
	CGaussianBlur m_gaussianBlur;				//�K�E�V�A���u���[
	std::list<Model*> m_shadowModels;			//�V���h�E�}�b�v�ɕ`�悷��V���h�E�p���f���̃��X�g
	Vector3 m_targetPos = g_VEC3_ZERO;			//�V���h�E���C�g�̒����_
};



