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

		//まずは16ビット版から。
		for (const auto& indexBuffer : mesh.indexBuffer16Array) {
			//インデックスの数からポリゴンの数を計算する。
			int numPolygon = indexBuffer.indices.size() / 3;
			for (int polygonNo = 0; polygonNo < numPolygon; polygonNo++) {
				Vector3 vertPos[3];
				//ポリゴンを構成する頂点番号をインデックスバッファから取得する。
				int vertexNo_0 = indexBuffer.indices[polygonNo * 3 + 0];
				int vertexNo_1 = indexBuffer.indices[polygonNo * 3 + 1];
				int vertexNo_2 = indexBuffer.indices[polygonNo * 3 + 2];

				vertPos[0] = mesh.vertexBuffer[vertexNo_0].pos;
				vertPos[1] = mesh.vertexBuffer[vertexNo_1].pos;
				vertPos[2] = mesh.vertexBuffer[vertexNo_2].pos;

				m_world.Apply(vertPos[0]);
				m_world.Apply(vertPos[1]);
				m_world.Apply(vertPos[2]);

				Vector3 normalVec;	//頂点の法線ベクトル
				Vector3 rNormalVec;
				{
					//法線は三頂点の外積で法線を出す。
					Vector3 vert0to1 = vertPos[1] - vertPos[0];	//頂点0から1へのベクトル
					Vector3 vert0to2 = vertPos[2] - vertPos[0];	//頂点0から2へのベクトル
					normalVec.Cross(vert0to1, vert0to2);	//外積で直交するベクトルを出す
					normalVec.Normalize();	//正規化しておく
					rNormalVec.Cross(vert0to2, vert0to1);
					rNormalVec.Normalize();
				}
				m_dbgV1 = normalVec;
				m_dbgV2 = rNormalVec;
				///
				///
				/// 手順
				/// 1、三角形を含む無限平面と線分の交差判定
				/// 2、交差している座標の計算
				/// 3、2で求めた座標が三角形の中にあるかどうか判定//外積使うよ


				//1、三角形を含む無限平面と線分の交差判定
				Vector3 VtoS = start - vertPos[0];	//頂点から線分の始点へのベクトル
				VtoS.Normalize();	//一応正規化しておく
				Vector3 VtoE = end - vertPos[0];	//頂点から線分の終点へのベクトル
				VtoE.Normalize();	//一応正規化しておく

				float VtoSdotN = Dot(normalVec, VtoS);	//vertVtoSと法線の内積
				float VtoEdotN = Dot(normalVec, VtoE);	//vertVtoEと法線の内積
				
				if (VtoSdotN * VtoEdotN >= 0)
				{
					//無限平面と交差していないのでスキップ
					continue;
				}

				//2、交差している座標の計算

				Vector3 addToStart;	//線ベクトルの始点に加算するベクトル
				addToStart = end - start;	//線分のベクトルを入れる

				Vector3 VtoS2 = start - vertPos[0];	//頂点から線分の始点へのベクトル
				Vector3 VtoE2 = end - vertPos[0];	//頂点から線分の終点へのベクトル
				float VtoSdotN2 = Dot(normalVec, VtoS2);	//vertVtoSと法線の内積
				float VtoEdotN2 = Dot(rNormalVec, VtoE2);	//vertVtoEと法線の内積


				float f = VtoSdotN2 / (VtoSdotN2 + VtoEdotN2);
				m_dbg = f;
				//addToStart *= VtoSdotN / (VtoSdotN - VtoEdotN);	//割合を計算する
				addToStart *= f;
				//交点の座標を計算する
				const Vector3 intersectPos = start + addToStart;	//交点の座標


				/// 3、2で求めた座標が三角形の中にあるかどうか判定

				Vector3 V0toV1 = vertPos[1] - vertPos[0];	//頂点0から頂点1へのベクトル
				Vector3 V0toI = intersectPos - vertPos[0];	//頂点0から交点へのベクトル
				Vector3 cross1;	//外積1
				cross1.Cross(V0toV1, V0toI);
				cross1.Normalize();
				
				Vector3 V1toV2 = vertPos[2] - vertPos[1];	//頂点1から頂点2へのベクトル
				Vector3 V1toI = intersectPos - vertPos[1];	//頂点1から交点へのベクトル
				Vector3 cross2;	//外積2
				cross2.Cross(V1toV2, V1toI);
				cross2.Normalize();


				Vector3 V2toV0 = vertPos[0] - vertPos[2];	//頂点2から頂点0へのベクトル
				Vector3 V2toI = intersectPos - vertPos[2];	//頂点2から交点へのベクトル
				Vector3 cross3;	//外積3
				cross3.Cross(V2toV0, V2toI);
				cross3.Normalize();

				float res1 = Dot(cross1, cross2);
				float res2 = Dot(cross1, cross3);

				
				if(( res1 > 0.0f && res2 > 0.0f ))
				{
					//交点の座標が三角形の中にあるから、ヒット！
					isHit = true;
					float distTmp = addToStart.Length();
					if (distTmp < dist)
					{
						m_intersectPos = intersectPos;
						dist = distTmp;
					}
				}

					//交点の座標が三角形の中にないから、スキップ


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




	return isHit;
}

