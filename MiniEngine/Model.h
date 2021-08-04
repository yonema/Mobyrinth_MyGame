#pragma once

#include "tkFile/TkmFile.h"
#include "MeshParts.h"
#include "Skeleton.h"
#include "../GameTemplate/Game/ModelRenderConstData.h"

class IShaderResource;

//���f���̏����
enum EnModelUpAxis {
	enModelUpAxisY,		//���f���̏������Y���B
	enModelUpAxisZ,		//���f���̏������Z���B
};
/// <summary>
/// ���f���̏������f�[�^
/// </summary>
struct ModelInitData {
	const char* m_tkmFilePath = nullptr;							//tkm�t�@�C���p�X�B
	//���_�V�F�[�_�[�̃G���g���[�|�C���g�B
	const char* m_vsEntryPointFunc = modelRenderConstData::ENTRY_POINT_FUNC_VS_DEFAULT;
	//�X�L������}�e���A���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g�B
	const char* m_vsSkinEntryPointFunc = modelRenderConstData::ENTRY_POINT_FUNC_VS_SKIN_DEFAULT;
	//�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g�B
	const char* m_psEntryPointFunc = modelRenderConstData::ENTRY_POINT_FUNC_PS_DEFAULT;
	const char* m_fxFilePath = nullptr;								//.fx�t�@�C���̃t�@�C���p�X�B
	void* m_expandConstantBuffer[MeshParts::m_maxExCBNum] = { nullptr };	//���[�U�[�g���̒萔�o�b�t�@�B
	int m_expandConstantBufferSize[MeshParts::m_maxExCBNum] = { 0 };	//���[�U�[�g���̒萔�o�b�t�@�̃T�C�Y�B
	IShaderResource* m_expandShaderResoruceView[MeshParts::m_maxExSRVNum] = { nullptr };	//���[�U�[�g���̃V�F�[�_�[���\�[�X�B
	DXGI_FORMAT m_colorBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;	//�����_�����O����J���[�o�b�t�@�̃t�H�[�}�b�g�B
	Skeleton* m_skeleton = nullptr;									//�X�P���g���B
	EnModelUpAxis m_modelUpAxis = enModelUpAxisZ;					//���f���̏�����B

	D3D12_CULL_MODE m_cullMode = D3D12_CULL_MODE_BACK;			//�J�����O���[�h
	bool m_trans = true;

};


/// <summary>
/// ���f���N���X�B
/// </summary>
class Model {

public:

	/// <summary>
	/// tkm�t�@�C�����珉�����B
	/// </summary>
	/// <param name="initData">�������f�[�^</param>
	void Init( const ModelInitData& initData );
	/// <summary>
	/// ���[���h�s��̍X�V�B
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="rot">��]</param>
	/// <param name="scale">�g�嗦</param>
	void UpdateWorldMatrix(const Vector3& pos, const Quaternion& rot, const Vector3& scale);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="renderContext">�����_�����O�R���e�L�X�g</param>
	void Draw(RenderContext& renderContext);

	/**
	 * @brief �r���[�s��ƃv���W�F�N�V�����s����w�肵�ĕ`��
	 * @param [in] renderContext �����_�����O�R���e�L�X�g
	 * @param [in] viewMatrix �r���[�s��
	 * @param [in] projectionMatrix �v���W�F�N�V�����s��
	*/
	void Draw(RenderContext& renderContext, const Matrix& viewMatrix, const Matrix& projectionMatrix);

	/// <summary>
	/// ���[���h�s����擾�B
	/// </summary>
	/// <returns></returns>
	const Matrix& GetWorldMatrix() const
	{
		return m_world;
	}
	/// <summary>
	/// ���b�V���ɑ΂��Ė₢���킹���s���B
	/// </summary>
	/// <param name="queryFunc">�₢���킹�֐�</param>
	void QueryMeshs(std::function<void(const SMesh& mesh)> queryFunc) 
	{
		m_meshParts.QueryMeshs(queryFunc);
	}
	void QueryMeshAndDescriptorHeap(std::function<void(const SMesh& mesh, const DescriptorHeap& ds)> queryFunc)
	{
		m_meshParts.QueryMeshAndDescriptorHeap(queryFunc);
	}
	/// <summary>
	/// �A���x�h�}�b�v��ύX�B
	/// </summary>
	/// <remarks>
	/// ���̊֐����Ăяo���ƃf�B�X�N���v�^�q�[�v�̍č\�z���s���邽�߁A�������ׂ�������܂��B
	/// ���t���[���Ăяo���K�v���Ȃ��ꍇ�͌Ăяo���Ȃ��悤�ɂ��Ă��������B
	/// </remarks>
	/// <param name="materialName">�ύX�������}�e���A���̖��O</param>
	/// <param name="albedoMap">�A���x�h�}�b�v</param>
	void ChangeAlbedoMap(const char* materialName, Texture& albedoMap);
	/// <summary>
	/// TKM�t�@�C�����擾�B
	/// </summary>
	/// <returns></returns>
	const TkmFile& GetTkmFile() const
	{
		return *m_tkmFile;
	}


private:

	Matrix m_world;										//���[���h�s��B
	TkmFile* m_tkmFile = nullptr;						//tkm�t�@�C���B
	Skeleton m_skeleton;								//�X�P���g���B
	MeshParts m_meshParts;								//���b�V���p�[�c�B
	EnModelUpAxis m_modelUpAxis = enModelUpAxisY;		//���f���̏�����B

	//�ǉ�
public:
	/// <summary>
/// �V���h�E���V�[�o�[�t���O��ݒ肷��
/// </summary>
/// <param name="shadowReceiverFlag">�V���h�E���V�[�o�[�H</param>
	void SetShadowReceiverFlag(const bool shadowReceiverFlag)
	{
		m_shadowReceiverFlag = shadowReceiverFlag;
	}



	/**
	 * @brief ���b�V���ƃ��C�̌�������
	 * @param [in] start ���C�̎n�_
	 * @param [in] end ���C�̏I�_
	 * @return �����������H
	*/
	bool InIntersectLine(const Vector3& start, const Vector3& end);

	/// <summary>
	/// InIntersectLine�Ō������������_���擾
	/// </summary>
	/// <returns>�����_</returns>
	const Vector3& GetIntersectPos() const
	{
		return m_intersectPos;
	}

	/// <summary>
	/// ���Ȕ����F��ݒ肷��
	/// </summary>
	/// <param name="color">���Ȕ����F</param>
	void SetModelEmissionColor(const Vector4& color)
	{
		m_meshParts.SetModelEmissionColor(color);
	}

	/// <summary>
	/// ��Z�J���[��ݒ肷��
	/// </summary>
	/// <param name="color">��Z�J���[</param>
	void SetMulColor(const Vector4& color)
	{
		m_meshParts.SetMulColor(color);
	}

	/**
	 * @brief �A���t�@�l��ݒ肷��
	 * @param [in] alphaValue �A���t�@�l
	*/
	void SetAlphaValue(const float alphaValue)
	{
		m_meshParts.SetAlphaValue(alphaValue);
	}
	/// <summary>
	/// �֊s����`�悷�邩�H��ݒ�
	/// </summary>
	/// <param name="outLineFlag">�֊s����`�悷�邩�H</param>
	void SetOutLineFlag(const float outLineFlag)
	{
		m_meshParts.SetOutLineFlag(outLineFlag);
	}

	/// <summary>
	/// �X�e���X����H��ݒ肷��
	/// </summary>
	/// <param name="stealthFlag">�X�e���X����H</param>
	void SetStealthFlag(const bool stealthFlag)
	{
		m_meshParts.SetStealthFlag(stealthFlag);
	}
private:
	bool m_shadowReceiverFlag = false;		//�V���h�E���V�[�o�[�H

	Vector3 m_intersectPos = g_VEC3_ZERO;	//�����_
	Vector3 m_lastStart = g_VEC3_ZERO;

	//�f�o�b�N�p
	//��ŏ���
	float m_dbg = 0.0f;	
	Vector3 m_dbgV1 = g_VEC3_ZERO;
	Vector3 m_dbgV2 = g_VEC3_ZERO;


};
