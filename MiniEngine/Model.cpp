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
		initData.m_colorBufferFormat,
		initData.m_expandConstantBuffer2,
		initData.m_expandConstantBufferSize2,
		initData.m_shadowConstantBuffer,
		initData.m_shadowConstantBufferSize
	);

	UpdateWorldMatrix(g_vec3Zero, g_quatIdentity, g_vec3One);
	
}

void Model::UpdateWorldMatrix(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
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
		g_camera3D->GetProjectionMatrix(),
		m_shadowReceiverFlag
	);
}
void Model::Draw(RenderContext& rc, const Matrix& viewMatrix,const Matrix& projectionMatrix)
{
	m_meshParts.Draw(
		rc,
		m_world,
		viewMatrix,
		projectionMatrix,
		m_shadowReceiverFlag
	);
}


bool Model::InIntersectLine(const Vector3& start, const Vector3& end)
{
	const auto& meshParts = m_tkmFile.GetMeshParts();

	bool isHit = false;
	float dist = FLT_MAX;

	for (const auto& mesh : meshParts)
	{

		//�܂���16�r�b�g�ł���B
		for (const auto& indexBuffer : mesh.indexBuffer16Array) {
			//�C���f�b�N�X�̐�����|���S���̐����v�Z����B
			int numPolygon = indexBuffer.indices.size() / 3;
			for (int polygonNo = 0; polygonNo < numPolygon; polygonNo++) {
				Vector3 vertPos[3];
				//�|���S�����\�����钸�_�ԍ����C���f�b�N�X�o�b�t�@����擾����B
				int vertexNo_0 = indexBuffer.indices[polygonNo * 3 + 0];
				int vertexNo_1 = indexBuffer.indices[polygonNo * 3 + 1];
				int vertexNo_2 = indexBuffer.indices[polygonNo * 3 + 2];

				vertPos[0] = mesh.vertexBuffer[vertexNo_0].pos;
				vertPos[1] = mesh.vertexBuffer[vertexNo_1].pos;
				vertPos[2] = mesh.vertexBuffer[vertexNo_2].pos;

				m_world.Apply(vertPos[0]);
				m_world.Apply(vertPos[1]);
				m_world.Apply(vertPos[2]);

				Vector3 normalVec;	//���_�̖@���x�N�g��
				Vector3 rNormalVec;
				{
					//�@���͎O���_�̊O�ςŖ@�����o���B
					Vector3 vert0to1 = vertPos[1] - vertPos[0];	//���_0����1�ւ̃x�N�g��
					Vector3 vert0to2 = vertPos[2] - vertPos[0];	//���_0����2�ւ̃x�N�g��
					normalVec.Cross(vert0to1, vert0to2);	//�O�ςŒ�������x�N�g�����o��
					normalVec.Normalize();	//���K�����Ă���
					rNormalVec.Cross(vert0to2, vert0to1);
					rNormalVec.Normalize();
				}
				m_dbgV1 = normalVec;
				m_dbgV2 = rNormalVec;
				///
				///
				/// �菇
				/// 1�A�O�p�`���܂ޖ������ʂƐ����̌�������
				/// 2�A�������Ă�����W�̌v�Z
				/// 3�A2�ŋ��߂����W���O�p�`�̒��ɂ��邩�ǂ�������//�O�ώg����


				//1�A�O�p�`���܂ޖ������ʂƐ����̌�������
				Vector3 VtoS = start - vertPos[0];	//���_��������̎n�_�ւ̃x�N�g��
				VtoS.Normalize();	//�ꉞ���K�����Ă���
				Vector3 VtoE = end - vertPos[0];	//���_��������̏I�_�ւ̃x�N�g��
				VtoE.Normalize();	//�ꉞ���K�����Ă���

				float VtoSdotN = Dot(normalVec, VtoS);	//vertVtoS�Ɩ@���̓���
				float VtoEdotN = Dot(normalVec, VtoE);	//vertVtoE�Ɩ@���̓���
				
				if (VtoSdotN * VtoEdotN >= 0)
				{
					//�������ʂƌ������Ă��Ȃ��̂ŃX�L�b�v
					continue;
				}

				//2�A�������Ă�����W�̌v�Z

				Vector3 addToStart;	//���x�N�g���̎n�_�ɉ��Z����x�N�g��
				addToStart = end - start;	//�����̃x�N�g��������

				Vector3 VtoS2 = start - vertPos[0];	//���_��������̎n�_�ւ̃x�N�g��
				Vector3 VtoE2 = end - vertPos[0];	//���_��������̏I�_�ւ̃x�N�g��
				float VtoSdotN2 = Dot(normalVec, VtoS2);	//vertVtoS�Ɩ@���̓���
				float VtoEdotN2 = Dot(rNormalVec, VtoE2);	//vertVtoE�Ɩ@���̓���


				float f = VtoSdotN2 / (VtoSdotN2 + VtoEdotN2);
				m_dbg = f;
				//addToStart *= VtoSdotN / (VtoSdotN - VtoEdotN);	//�������v�Z����
				addToStart *= f;
				//��_�̍��W���v�Z����
				const Vector3 intersectPos = start + addToStart;	//��_�̍��W


				/// 3�A2�ŋ��߂����W���O�p�`�̒��ɂ��邩�ǂ�������

				Vector3 V0toV1 = vertPos[1] - vertPos[0];	//���_0���璸�_1�ւ̃x�N�g��
				Vector3 V0toI = intersectPos - vertPos[0];	//���_0�����_�ւ̃x�N�g��
				Vector3 cross1;	//�O��1
				cross1.Cross(V0toV1, V0toI);
				cross1.Normalize();
				
				Vector3 V1toV2 = vertPos[2] - vertPos[1];	//���_1���璸�_2�ւ̃x�N�g��
				Vector3 V1toI = intersectPos - vertPos[1];	//���_1�����_�ւ̃x�N�g��
				Vector3 cross2;	//�O��2
				cross2.Cross(V1toV2, V1toI);
				cross2.Normalize();


				Vector3 V2toV0 = vertPos[0] - vertPos[2];	//���_2���璸�_0�ւ̃x�N�g��
				Vector3 V2toI = intersectPos - vertPos[2];	//���_2�����_�ւ̃x�N�g��
				Vector3 cross3;	//�O��3
				cross3.Cross(V2toV0, V2toI);
				cross3.Normalize();

				float res1 = Dot(cross1, cross2);
				float res2 = Dot(cross1, cross3);

				
				if(( res1 > 0.0f && res2 > 0.0f ))
				{
					//��_�̍��W���O�p�`�̒��ɂ��邩��A�q�b�g�I
					isHit = true;
					float distTmp = addToStart.Length();
					if (distTmp < dist)
					{
						m_intersectPos = intersectPos;
						dist = distTmp;
					}
				}

					//��_�̍��W���O�p�`�̒��ɂȂ�����A�X�L�b�v


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




	return isHit;
}

