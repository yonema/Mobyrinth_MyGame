#include "stdafx.h"
#include "MeshParts.h"
#include "Skeleton.h"
#include "Material.h"

MeshParts::~MeshParts()
{
	for (auto& mesh : m_meshs) {
		//インデックスバッファを削除。
		for (auto& ib : mesh->m_indexBufferArray) {
			delete ib;
		}
		//マテリアルを削除。
		for (auto& mat : mesh->m_materials) {
			delete mat;
		}
		//メッシュを削除。
		delete mesh;
	}
}
void MeshParts::InitFromTkmFile(
	const TkmFile& tkmFile, 
	const wchar_t* fxFilePath,
	const char* vsEntryPointFunc,
	const char* vsSkinEntryPointFunc,
	const char* psEntryPointFunc,
	D3D12_CULL_MODE cullMode,
	void* const* expandData,
	const int* expandDataSize,
	IShaderResource* const* expandShaderResourceView,
	DXGI_FORMAT colorBufferFormat,
	const bool trans
)
{
	m_meshs.resize(tkmFile.GetNumMesh());
	int meshNo = 0;
	tkmFile.QueryMeshParts([&](const TkmFile::SMesh& mesh) {
		//tkmファイルのメッシュ情報からメッシュを作成する。
		CreateMeshFromTkmMesh(
			mesh, 
			meshNo, 
			fxFilePath, 
			vsEntryPointFunc, 
			vsSkinEntryPointFunc, 
			psEntryPointFunc,
			colorBufferFormat,
			cullMode,
			trans
		);
		meshNo++;
	});
	//共通定数バッファの作成。
	m_commonConstantBuffer.Init(sizeof(SConstantBuffer), nullptr);
	//ユーザー拡張用の定数バッファを作成。
	for (int i = 0; i < m_maxExCBNum; i++)
	{
		if (expandData[i]) {
			m_expandConstantBuffer[i].Init(expandDataSize[i], nullptr);
			m_expandData[i] = expandData[i];
		}
	}
	
	for (int i = 0; i < m_maxExSRVNum; i++)
	{
		m_expandShaderResourceView[i] = expandShaderResourceView[i];
	}
	//ディスクリプタヒープを作成。
	CreateDescriptorHeaps();
}

void MeshParts::CreateDescriptorHeaps()
{
	//ディスクリプタヒープはマテリアルの数分だけ作成される。
	int numDescriptorHeap = 0;
	for (auto& mesh : m_meshs) {
		for (int matNo = 0; matNo < mesh->m_materials.size(); matNo++) {
			numDescriptorHeap++;
		}
	}
	//ディスクリプタヒープをドカッと確保。
	m_descriptorHeap.resize(numDescriptorHeap);
	//ディスクリプタヒープを構築していく。
	int descriptorHeapNo = 0;
	for (auto& mesh : m_meshs) {
		for (int matNo = 0; matNo < mesh->m_materials.size(); matNo++) {
			auto& descriptorHeap = m_descriptorHeap[descriptorHeapNo];
			//ディスクリプタヒープにディスクリプタを登録していく。
			descriptorHeap.RegistShaderResource(0, mesh->m_materials[matNo]->GetAlbedoMap());		//アルベドマップ。
			descriptorHeap.RegistShaderResource(1, mesh->m_materials[matNo]->GetNormalMap());		//法線マップ。
			descriptorHeap.RegistShaderResource(2, mesh->m_materials[matNo]->GetSpecularMap());		//スペキュラマップ。
			descriptorHeap.RegistShaderResource(3, m_boneMatricesStructureBuffer);							//ボーンのストラクチャードバッファ。
			
			for (int i = 0; i < m_maxExSRVNum; i++)
			{
				if (m_expandShaderResourceView[i]) {
					descriptorHeap.RegistShaderResource(EXPAND_SRV_REG__START_NO + i, *m_expandShaderResourceView[i]);
				}
			}
			descriptorHeap.RegistConstantBuffer(0, m_commonConstantBuffer);
			for (int i = 0; i < m_maxExCBNum; i++)
			{
				if (m_expandConstantBuffer[i].IsValid()) {
					descriptorHeap.RegistConstantBuffer(i + 1, m_expandConstantBuffer[i]);
				}
			}

			//ディスクリプタヒープへの登録を確定させる。
			descriptorHeap.Commit();
			descriptorHeapNo++;
		}
	}
}
void MeshParts::CreateMeshFromTkmMesh(
	const TkmFile::SMesh& tkmMesh, 
	int meshNo,
	const wchar_t* fxFilePath,
	const char* vsEntryPointFunc,
	const char* vsSkinEntryPointFunc,
	const char* psEntryPointFunc,
	DXGI_FORMAT colorBufferFormat,
	D3D12_CULL_MODE cullMode,
	const bool trans
)
{
	//1. 頂点バッファを作成。
	int numVertex = (int)tkmMesh.vertexBuffer.size();
	int vertexStride = sizeof(TkmFile::SVertex);
	auto mesh = new SMesh;
	mesh->skinFlags.reserve(tkmMesh.materials.size());
	mesh->m_vertexBuffer.Init(vertexStride * numVertex, vertexStride);
	mesh->m_vertexBuffer.Copy((void*)&tkmMesh.vertexBuffer[0]);

	auto SetSkinFlag = [&](int index) {
		if (tkmMesh.vertexBuffer[index].skinWeights.x > 0.0f) {
			//スキンがある。
			mesh->skinFlags.push_back(1);
		}
		else {
			//スキンなし。
			mesh->skinFlags.push_back(0);
		}
	};
	//2. インデックスバッファを作成。
	if (!tkmMesh.indexBuffer16Array.empty()) {
		//インデックスのサイズが2byte
		mesh->m_indexBufferArray.reserve(tkmMesh.indexBuffer16Array.size());
		for (auto& tkIb : tkmMesh.indexBuffer16Array) {
			auto ib = new IndexBuffer;
			ib->Init(static_cast<int>(tkIb.indices.size()) * 2, 2);
			ib->Copy((uint16_t*)&tkIb.indices.at(0));
				
			//スキンがあるかどうかを設定する。
			SetSkinFlag(tkIb.indices[0]);

			mesh->m_indexBufferArray.push_back(ib);
		}
	}
	else {
		//インデックスのサイズが4byte
		mesh->m_indexBufferArray.reserve(tkmMesh.indexBuffer32Array.size());
		for (auto& tkIb : tkmMesh.indexBuffer32Array) {
			auto ib = new IndexBuffer;
			ib->Init(static_cast<int>(tkIb.indices.size()) * 4, 4);
			ib->Copy((uint32_t*)&tkIb.indices.at(0));

			//スキンがあるかどうかを設定する。
			SetSkinFlag(tkIb.indices[0]);

			mesh->m_indexBufferArray.push_back(ib);
		}
	}
	//3. マテリアルを作成。
	mesh->m_materials.reserve(tkmMesh.materials.size());
	for (auto& tkmMat : tkmMesh.materials) {
		auto mat = new Material;
		mat->InitFromTkmMaterila(
			tkmMat, 
			fxFilePath,
			vsEntryPointFunc, 
			vsSkinEntryPointFunc, 
			psEntryPointFunc,
			colorBufferFormat,
			cullMode,
			trans
		);
		mesh->m_materials.push_back(mat);
	}

	m_meshs[meshNo] = mesh;
	
}

void MeshParts::BindSkeleton(Skeleton& skeleton)
{
	m_skeleton = &skeleton;
	//構造化バッファを作成する。
	m_boneMatricesStructureBuffer.Init(
		sizeof(Matrix),
		m_skeleton->GetNumBones(),
		m_skeleton->GetBoneMatricesTopAddress()
	);
}
void MeshParts::Draw(
	RenderContext& rc,
	const Matrix& mWorld,
	const Matrix& mView,
	const Matrix& mProj,
	const bool shadowReceiverFlag
)
{
	//メッシュごとにドロー
	//プリミティブのトポロジーはトライアングルリストのみ。
	rc.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//共通定数バッファを更新する。
	SConstantBuffer cb;
	cb.mWorld = mWorld;
	cb.mView = mView;
	cb.mProj = mProj;
	cb.emissionColor = m_emissionColor;
	cb.mulColor = m_mulColor;
	cb.shadowReceiverFlag = shadowReceiverFlag;
	cb.outLineFlag = m_outLineFlag;
	cb.stealthFlag = m_stealthFlag;

	m_commonConstantBuffer.CopyToVRAM(&cb);

	for (int i = 0; i < m_maxExCBNum; i++)
	{
		if (m_expandData[i]) {
			m_expandConstantBuffer[i].CopyToVRAM(m_expandData[i]);
		}
	}

	if (m_boneMatricesStructureBuffer.IsInited()) {
		//ボーン行列を更新する。
		m_boneMatricesStructureBuffer.Update(m_skeleton->GetBoneMatricesTopAddress());
	}
	int descriptorHeapNo = 0;
	for (auto& mesh : m_meshs) {
		//1. 頂点バッファを設定。
		rc.SetVertexBuffer(mesh->m_vertexBuffer);
		//マテリアルごとにドロー。
		for (int matNo = 0; matNo < mesh->m_materials.size(); matNo++) {
			//このマテリアルが貼られているメッシュの描画開始。
			mesh->m_materials[matNo]->BeginRender(rc, mesh->skinFlags[matNo]);
			//2. ディスクリプタヒープを設定。
			rc.SetDescriptorHeap(m_descriptorHeap.at(descriptorHeapNo));
			//3. インデックスバッファを設定。
			auto& ib = mesh->m_indexBufferArray[matNo];
			rc.SetIndexBuffer(*ib);

			//4. ドローコールを実行。
			rc.DrawIndexed(ib->GetCount());
			descriptorHeapNo++;
		}
	}
}