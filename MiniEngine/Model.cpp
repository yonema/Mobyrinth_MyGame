#include "stdafx.h"
#include "Model.h"
#include "Material.h"

void Model::Init(const ModelInitData& initData)
{
	MY_ASSERT(
		initData.m_fxFilePath, 
		"error : initData.m_fxFilePathが指定されていません。"
	);
	MY_ASSERT(
		initData.m_tkmFilePath,
		"error : initData.m_tkmFilePathが指定されていません。"
	);
	//内部のシェーダーをロードする処理が求めているのが
	//wchar_t型の文字列なので、ここで変換しておく。
	wchar_t wfxFilePath[256] = {L""};
	if (initData.m_fxFilePath != nullptr) {
		//MessageBoxA(nullptr, "fxファイルパスが指定されていません。", "エラー", MB_OK);
		//std::abort();
		mbstowcs(wfxFilePath, initData.m_fxFilePath, 256);
	}
	
	if (initData.m_skeleton != nullptr) {
		//スケルトンが指定されている。
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
		//todo マテリアル名をtkmファイルに出力したなかった・・・。
		//todo 今は全マテリアル差し替えます
		for (Material* material : mesh.m_materials) {
			material->GetAlbedoMap().InitFromD3DResource(albedoMap.Get());
		}
	});
	//ディスクリプタヒープの再作成。
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

		//まずは16ビット版から。
		for (const auto& indexBuffer : mesh.indexBuffer16Array) {
			//mesh.vertexBuffer[0].pos;
			//インデックスの数からポリゴンの数を計算する。
			int numPolygon = indexBuffer.indices.size() / 3;
			for (int polygonNo = 0; polygonNo < numPolygon; polygonNo++) {
				Vector3 vertPos[3];

				vertPos[0] = mesh.vertexBuffer[polygonNo * 3 + 0].pos;
				vertPos[1] = mesh.vertexBuffer[polygonNo * 3 + 1].pos;
				vertPos[2] = mesh.vertexBuffer[polygonNo * 3 + 2].pos;//外積使うよ

				//法線は三頂点の外積で法線を出す。


				///
				///
				/// 手順
				/// 1、三角形を含む無限平面と線分の交差判定
				/// 2、交差している座標の計算
				/// 3、2で求めた座標が三角形の中にあるかどうか判定




			}

		}

		//次は32ビット版
		/*for (const auto& indexBuffer : mesh.indexBuffer32Array) {
			//インデックスの数からポリゴンの数を計算する。
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

