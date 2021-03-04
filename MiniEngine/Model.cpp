#include "stdafx.h"
#include "Model.h"
#include "Material.h"

void Model::Init(const ModelInitData& initData)
{
	MY_ASSERT(
		initData.m_fxFilePath, 
		"error : initData.m_fxFilePath���w�肳��Ă��܂���B"
	);
	MY_ASSERT(
		initData.m_tkmFilePath,
		"error : initData.m_tkmFilePath���w�肳��Ă��܂���B"
	);
	//�����̃V�F�[�_�[�����[�h���鏈�������߂Ă���̂�
	//wchar_t�^�̕�����Ȃ̂ŁA�����ŕϊ����Ă����B
	wchar_t wfxFilePath[256] = {L""};
	if (initData.m_fxFilePath != nullptr) {
		//MessageBoxA(nullptr, "fx�t�@�C���p�X���w�肳��Ă��܂���B", "�G���[", MB_OK);
		//std::abort();
		mbstowcs(wfxFilePath, initData.m_fxFilePath, 256);
	}
	
	if (initData.m_skeleton != nullptr) {
		//�X�P���g�����w�肳��Ă���B
		m_meshParts.BindSkeleton(*initData.m_skeleton);
	}
	
	m_modelUpAxis = initData.m_modelUpAxis;

	m_tkmFile.Load(initData.m_tkmFilePath);
	m_meshParts.InitFromTkmFile(
		m_tkmFile, 
		wfxFilePath, 
		initData.m_vsEntryPointFunc,
		initData.m_vsSkinEntryPointFunc,
		initData.m_psEntryPointFunc,
		initData.m_expandConstantBuffer,
		initData.m_expandConstantBufferSize,
		initData.m_expandShaderResoruceView,
		initData.m_expandConstantBuffer2,
		initData.m_expandConstantBufferSize2
	);

	UpdateWorldMatrix(g_vec3Zero, g_quatIdentity, g_vec3One);
	
}

void Model::UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale)
{
	Matrix mBias;
	if (m_modelUpAxis == enModelUpAxisZ) {
		//Z-up
		mBias.MakeRotationX(Math::PI * -0.5f);
	}
	Matrix mTrans, mRot, mScale;
	mTrans.MakeTranslation(pos);
	mRot.MakeRotationFromQuaternion(rot);
	mScale.MakeScaling(scale);
	m_world = mBias * mScale * mRot * mTrans;
}

void Model::ChangeAlbedoMap(const char* materialName, Texture& albedoMap)
{
	m_meshParts.QueryMeshs([&](const SMesh& mesh) {
		//todo �}�e���A������tkm�t�@�C���ɏo�͂����Ȃ������E�E�E�B
		//todo ���͑S�}�e���A�������ւ��܂�
		for (Material* material : mesh.m_materials) {
			material->GetAlbedoMap().InitFromD3DResource(albedoMap.Get());
		}
	});
	//�f�B�X�N���v�^�q�[�v�̍č쐬�B
	m_meshParts.CreateDescriptorHeaps();
	
}
void Model::Draw(RenderContext& rc)
{
	m_meshParts.Draw(
		rc, 
		m_world, 
		g_camera3D->GetViewMatrix(), 
		g_camera3D->GetProjectionMatrix()
	);
}


bool Model::InIntersectLine(const Vector3& start, const Vector3& end)
{
	const auto& meshParts = m_tkmFile.GetMeshParts();

	for (const auto& mesh : meshParts)
	{

		//�܂���16�r�b�g�ł���B
		for (const auto& indexBuffer : mesh.indexBuffer16Array) {
			//mesh.vertexBuffer[0].pos;
			//�C���f�b�N�X�̐�����|���S���̐����v�Z����B
			int numPolygon = indexBuffer.indices.size() / 3;
			for (int polygonNo = 0; polygonNo < numPolygon; polygonNo++) {
				Vector3 vertPos[3];

				vertPos[0] = mesh.vertexBuffer[polygonNo * 3 + 0].pos;
				vertPos[1] = mesh.vertexBuffer[polygonNo * 3 + 1].pos;
				vertPos[2] = mesh.vertexBuffer[polygonNo * 3 + 2].pos;//�O�ώg����

				//�@���͎O���_�̊O�ςŖ@�����o���B


				///
				///
				/// �菇
				/// 1�A�O�p�`���܂ޖ������ʂƐ����̌�������
				/// 2�A�������Ă�����W�̌v�Z
				/// 3�A2�ŋ��߂����W���O�p�`�̒��ɂ��邩�ǂ�������




			}

		}

		//����32�r�b�g��
		/*for (const auto& indexBuffer : mesh.indexBuffer32Array) {
			//�C���f�b�N�X�̐�����|���S���̐����v�Z����B
			int numPolygon = indexBuffer.indices.size() / 3;
			for (int polygonNo = 0; polygonNo < numPolygon; polygonNo++) {
				Vector3 vertPos[3];

				vertPos[0] = mesh.vertexBuffer[polygonNo * 3 + 0].pos;
				vertPos[1] = mesh.vertexBuffer[polygonNo * 3 + 1].pos;
				vertPos[2] = mesh.vertexBuffer[polygonNo * 3 + 2].pos;



			}
		}*/


	}




	return true;
}

