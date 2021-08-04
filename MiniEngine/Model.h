#pragma once

#include "tkFile/TkmFile.h"
#include "MeshParts.h"
#include "Skeleton.h"
#include "../GameTemplate/Game/ModelRenderConstData.h"

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
	//頂点シェーダーのエントリーポイント。
	const char* m_vsEntryPointFunc = modelRenderConstData::ENTRY_POINT_FUNC_VS_DEFAULT;
	//スキンありマテリアル用の頂点シェーダーのエントリーポイント。
	const char* m_vsSkinEntryPointFunc = modelRenderConstData::ENTRY_POINT_FUNC_VS_SKIN_DEFAULT;
	//ピクセルシェーダーのエントリーポイント。
	const char* m_psEntryPointFunc = modelRenderConstData::ENTRY_POINT_FUNC_PS_DEFAULT;
	const char* m_fxFilePath = nullptr;								//.fxファイルのファイルパス。
	void* m_expandConstantBuffer[MeshParts::m_maxExCBNum] = { nullptr };	//ユーザー拡張の定数バッファ。
	int m_expandConstantBufferSize[MeshParts::m_maxExCBNum] = { 0 };	//ユーザー拡張の定数バッファのサイズ。
	IShaderResource* m_expandShaderResoruceView[MeshParts::m_maxExSRVNum] = { nullptr };	//ユーザー拡張のシェーダーリソース。
	DXGI_FORMAT m_colorBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;	//レンダリングするカラーバッファのフォーマット。
	Skeleton* m_skeleton = nullptr;									//スケルトン。
	EnModelUpAxis m_modelUpAxis = enModelUpAxisZ;					//モデルの上方向。

	D3D12_CULL_MODE m_cullMode = D3D12_CULL_MODE_BACK;			//カリングモード
	bool m_trans = true;

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

	/**
	 * @brief ビュー行列とプロジェクション行列を指定して描画
	 * @param [in] renderContext レンダリングコンテキスト
	 * @param [in] viewMatrix ビュー行列
	 * @param [in] projectionMatrix プロジェクション行列
	*/
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


private:

	Matrix m_world;										//ワールド行列。
	TkmFile* m_tkmFile = nullptr;						//tkmファイル。
	Skeleton m_skeleton;								//スケルトン。
	MeshParts m_meshParts;								//メッシュパーツ。
	EnModelUpAxis m_modelUpAxis = enModelUpAxisY;		//モデルの上方向。

	//追加
public:
	/// <summary>
/// シャドウレシーバーフラグを設定する
/// </summary>
/// <param name="shadowReceiverFlag">シャドウレシーバー？</param>
	void SetShadowReceiverFlag(const bool shadowReceiverFlag)
	{
		m_shadowReceiverFlag = shadowReceiverFlag;
	}



	/**
	 * @brief メッシュとレイの交差判定
	 * @param [in] start レイの始点
	 * @param [in] end レイの終点
	 * @return 交差したか？
	*/
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
	void SetModelEmissionColor(const Vector4& color)
	{
		m_meshParts.SetModelEmissionColor(color);
	}

	/// <summary>
	/// 乗算カラーを設定する
	/// </summary>
	/// <param name="color">乗算カラー</param>
	void SetMulColor(const Vector4& color)
	{
		m_meshParts.SetMulColor(color);
	}

	/**
	 * @brief アルファ値を設定する
	 * @param [in] alphaValue アルファ値
	*/
	void SetAlphaValue(const float alphaValue)
	{
		m_meshParts.SetAlphaValue(alphaValue);
	}
	/// <summary>
	/// 輪郭線を描画するか？を設定
	/// </summary>
	/// <param name="outLineFlag">輪郭線を描画するか？</param>
	void SetOutLineFlag(const float outLineFlag)
	{
		m_meshParts.SetOutLineFlag(outLineFlag);
	}

	/// <summary>
	/// ステルスする？を設定する
	/// </summary>
	/// <param name="stealthFlag">ステルスする？</param>
	void SetStealthFlag(const bool stealthFlag)
	{
		m_meshParts.SetStealthFlag(stealthFlag);
	}
private:
	bool m_shadowReceiverFlag = false;		//シャドウレシーバー？

	Vector3 m_intersectPos = g_VEC3_ZERO;	//交差点
	Vector3 m_lastStart = g_VEC3_ZERO;

	//デバック用
	//後で消す
	float m_dbg = 0.0f;	
	Vector3 m_dbgV1 = g_VEC3_ZERO;
	Vector3 m_dbgV2 = g_VEC3_ZERO;


};
