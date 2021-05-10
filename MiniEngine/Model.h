#pragma once

#include "tkFile/TkmFile.h"
#include "MeshParts.h"
#include "Skeleton.h"

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
	const char* m_vsEntryPointFunc = "VSMain";						//���_�V�F�[�_�[�̃G���g���[�|�C���g�B
	const char* m_vsSkinEntryPointFunc = "VSSkinMain";				//�X�L������}�e���A���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g�B
	const char* m_psEntryPointFunc = "PSMain";						//�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g�B
	const char* m_fxFilePath = nullptr;								//.fx�t�@�C���̃t�@�C���p�X�B
	void* m_expandConstantBuffer = nullptr;							//���[�U�[�g���̒萔�o�b�t�@�B
	int m_expandConstantBufferSize = 0;								//���[�U�[�g���̒萔�o�b�t�@�̃T�C�Y�B
	IShaderResource* m_expandShaderResoruceView = nullptr;			//���[�U�[�g���̃V�F�[�_�[���\�[�X�B
	DXGI_FORMAT m_colorBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;	//�����_�����O����J���[�o�b�t�@�̃t�H�[�}�b�g�B
	Skeleton* m_skeleton = nullptr;									//�X�P���g���B
	EnModelUpAxis m_modelUpAxis = enModelUpAxisZ;					//���f���̏�����B

	D3D12_CULL_MODE m_cullMode = D3D12_CULL_MODE_BACK;

	void* m_expandConstantBuffer2 = nullptr;						//���[�U�[�g���̒萔�o�b�t�@�B
	int m_expandConstantBufferSize2 = 0;							//���[�U�[�g���̒萔�o�b�t�@�̃T�C�Y�B

	void* m_expandConstantBuffer3 = nullptr;						//���[�U�[�g���̒萔�o�b�t�@�B
	int m_expandConstantBufferSize3 = 0;							//���[�U�[�g���̒萔�o�b�t�@�̃T�C�Y�B

	void* m_expandConstantBuffer4 = nullptr;						//���[�U�[�g���̒萔�o�b�t�@�B
	int m_expandConstantBufferSize4 = 0;							//���[�U�[�g���̒萔�o�b�t�@�̃T�C�Y�B

	void* m_shadowConstantBuffer = nullptr;							//�V���h�E�p�̒萔�o�b�t�@
	int m_shadowConstantBufferSize = 0;								//�V���h�E�p�̒萔�o�b�t�@�̃T�C�Y
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

	/// <summary>
	/// �V���h�E���V�[�o�[�t���O��ݒ肷��
	/// </summary>
	/// <param name="shadowReceiverFlag">�V���h�E���V�[�o�[�H</param>
	void SetShadowReceiverFlag(const bool shadowReceiverFlag)
	{
		m_shadowReceiverFlag = shadowReceiverFlag;
	}



	/// <summary>
	/// ���b�V���ƃ��C�̌�������
	/// </summary>
	/// <param name="start">���C�̎n�_</param>
	/// <param name="end">���C�̏I�_</param>
	/// <returns>�����������H</returns>
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
	void SetEmissionColor(const Vector4& color)
	{
		m_meshParts.SetEmissionColor(color);
	}


	//�f�o�b�N�p
	//��ŏ���
	const float getDbg()const
	{
		return m_dbg;
	}
	const Vector3 getDbgV1()const
	{
		return m_dbgV1;
	}
	const Vector3 getDbgV2()const
	{
		return m_dbgV2;
	}
	//�f�o�b�N�p�����܂�

private:

	Matrix m_world;										//���[���h�s��B
	TkmFile* m_tkmFile = nullptr;						//tkm�t�@�C���B
	Skeleton m_skeleton;								//�X�P���g���B
	MeshParts m_meshParts;								//���b�V���p�[�c�B
	EnModelUpAxis m_modelUpAxis = enModelUpAxisY;		//���f���̏�����B

	bool m_shadowReceiverFlag = false;		//�V���h�E���V�[�o�[�H

	Vector3 m_intersectPos = g_vec3Zero;	//�����_
	Vector3 m_lastStart = g_vec3Zero;

	//�f�o�b�N�p
	//��ŏ���
	float m_dbg = 0.0f;	
	Vector3 m_dbgV1 = g_vec3Zero;
	Vector3 m_dbgV2 = g_vec3Zero;


};
