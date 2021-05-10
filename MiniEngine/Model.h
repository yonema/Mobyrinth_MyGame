#pragma once

#include "tkFile/TkmFile.h"
#include "MeshParts.h"
#include "Skeleton.h"

class IShaderResource;

//モデルの上方向
enum EnModelUpAxis {
	enModelUpAxisY,		//モデルの上方向がY軸。
	enModelUpAxisZ,		//モデルの上方向がZ軸。
};
/// <summary>
/// モデルの初期化データ
/// </summary>
struct ModelInitData {
	const char* m_tkmFilePath = nullptr;							//tkmファイルパス。
	const char* m_vsEntryPointFunc = "VSMain";						//頂点シェーダーのエントリーポイント。
	const char* m_vsSkinEntryPointFunc = "VSSkinMain";				//スキンありマテリアル用の頂点シェーダーのエントリーポイント。
	const char* m_psEntryPointFunc = "PSMain";						//ピクセルシェーダーのエントリーポイント。
	const char* m_fxFilePath = nullptr;								//.fxファイルのファイルパス。
	void* m_expandConstantBuffer = nullptr;							//ユーザー拡張の定数バッファ。
	int m_expandConstantBufferSize = 0;								//ユーザー拡張の定数バッファのサイズ。
	IShaderResource* m_expandShaderResoruceView = nullptr;			//ユーザー拡張のシェーダーリソース。
	DXGI_FORMAT m_colorBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;	//レンダリングするカラーバッファのフォーマット。
	Skeleton* m_skeleton = nullptr;									//スケルトン。
	EnModelUpAxis m_modelUpAxis = enModelUpAxisZ;					//モデルの上方向。

	D3D12_CULL_MODE m_cullMode = D3D12_CULL_MODE_BACK;

	void* m_expandConstantBuffer2 = nullptr;						//ユーザー拡張の定数バッファ。
	int m_expandConstantBufferSize2 = 0;							//ユーザー拡張の定数バッファのサイズ。

	void* m_expandConstantBuffer3 = nullptr;						//ユーザー拡張の定数バッファ。
	int m_expandConstantBufferSize3 = 0;							//ユーザー拡張の定数バッファのサイズ。

	void* m_expandConstantBuffer4 = nullptr;						//ユーザー拡張の定数バッファ。
	int m_expandConstantBufferSize4 = 0;							//ユーザー拡張の定数バッファのサイズ。

	void* m_shadowConstantBuffer = nullptr;							//シャドウ用の定数バッファ
	int m_shadowConstantBufferSize = 0;								//シャドウ用の定数バッファのサイズ
};


/// <summary>
/// モデルクラス。
/// </summary>
class Model {

public:

	/// <summary>
	/// tkmファイルから初期化。
	/// </summary>
	/// <param name="initData">初期化データ</param>
	void Init( const ModelInitData& initData );
	/// <summary>
	/// ワールド行列の更新。
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡大率</param>
	void UpdateWorldMatrix(const Vector3& pos, const Quaternion& rot, const Vector3& scale);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="renderContext">レンダリングコンテキスト</param>
	void Draw(RenderContext& renderContext);

	void Draw(RenderContext& renderContext, const Matrix& viewMatrix, const Matrix& projectionMatrix);

	/// <summary>
	/// ワールド行列を取得。
	/// </summary>
	/// <returns></returns>
	const Matrix& GetWorldMatrix() const
	{
		return m_world;
	}
	/// <summary>
	/// メッシュに対して問い合わせを行う。
	/// </summary>
	/// <param name="queryFunc">問い合わせ関数</param>
	void QueryMeshs(std::function<void(const SMesh& mesh)> queryFunc) 
	{
		m_meshParts.QueryMeshs(queryFunc);
	}
	void QueryMeshAndDescriptorHeap(std::function<void(const SMesh& mesh, const DescriptorHeap& ds)> queryFunc)
	{
		m_meshParts.QueryMeshAndDescriptorHeap(queryFunc);
	}
	/// <summary>
	/// アルベドマップを変更。
	/// </summary>
	/// <remarks>
	/// この関数を呼び出すとディスクリプタヒープの再構築が行われるため、処理負荷がかかります。
	/// 毎フレーム呼び出す必要がない場合は呼び出さないようにしてください。
	/// </remarks>
	/// <param name="materialName">変更しいたマテリアルの名前</param>
	/// <param name="albedoMap">アルベドマップ</param>
	void ChangeAlbedoMap(const char* materialName, Texture& albedoMap);
	/// <summary>
	/// TKMファイルを取得。
	/// </summary>
	/// <returns></returns>
	const TkmFile& GetTkmFile() const
	{
		return *m_tkmFile;
	}

	/// <summary>
	/// シャドウレシーバーフラグを設定する
	/// </summary>
	/// <param name="shadowReceiverFlag">シャドウレシーバー？</param>
	void SetShadowReceiverFlag(const bool shadowReceiverFlag)
	{
		m_shadowReceiverFlag = shadowReceiverFlag;
	}



	/// <summary>
	/// メッシュとレイの交差判定
	/// </summary>
	/// <param name="start">レイの始点</param>
	/// <param name="end">レイの終点</param>
	/// <returns>交差したか？</returns>
	bool InIntersectLine(const Vector3& start, const Vector3& end);
	
	/// <summary>
	/// InIntersectLineで交差した交差点を取得
	/// </summary>
	/// <returns>交差点</returns>
	const Vector3& GetIntersectPos() const
	{
		return m_intersectPos;
	}

	/// <summary>
	/// 自己発光色を設定する
	/// </summary>
	/// <param name="color">自己発光色</param>
	void SetEmissionColor(const Vector4& color)
	{
		m_meshParts.SetEmissionColor(color);
	}


	//デバック用
	//後で消す
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
	//デバック用ここまで

private:

	Matrix m_world;										//ワールド行列。
	TkmFile* m_tkmFile = nullptr;						//tkmファイル。
	Skeleton m_skeleton;								//スケルトン。
	MeshParts m_meshParts;								//メッシュパーツ。
	EnModelUpAxis m_modelUpAxis = enModelUpAxisY;		//モデルの上方向。

	bool m_shadowReceiverFlag = false;		//シャドウレシーバー？

	Vector3 m_intersectPos = g_vec3Zero;	//交差点
	Vector3 m_lastStart = g_vec3Zero;

	//デバック用
	//後で消す
	float m_dbg = 0.0f;	
	Vector3 m_dbgV1 = g_vec3Zero;
	Vector3 m_dbgV2 = g_vec3Zero;


};
