#pragma once


#include <d3d12.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include <dxgi1_4.h>
#include "DirectXTK/Inc/DDSTextureLoader.h"
#include "DirectXTK/Inc/ResourceUploadBatch.h"
#include "RenderContext.h"
#include "rayTracing/RaytracingEngine.h"
#include "Camera.h"
#include "NullTextureMaps.h"
#include "font/FontEngine.h"
#include "RenderTarget.h"
#include "Sprite.h"
#include "PostEffect.h"
#include "ShadowMap.h"
#include "SceneChange.h"




/// <summary>
/// DirectX12に依存するグラフィックスエンジン
/// </summary>
class GraphicsEngine {
public:
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~GraphicsEngine();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <remarks>
	/// 本関数を呼び出すことでDirectX12の初期化が行われます。
	/// </remarks>
	/// <param name="hwnd">Windowハンドル</param>
	/// <param name="frameBufferwidth">フレームバッファの幅</param>
	/// <param name="frameBufferHeight">フレームバッファの高さ</param>
	/// <returns>falseが返ってきたら作成に失敗。</returns>
	bool Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);
	/// <summary>
	/// レンダリング開始。
	/// </summary>
	/// <remarks>
	/// 1フレームのレンダリングの開始時に呼び出してください。
	/// </remarks>
	void BeginRender();
	/// <summary>
	/// レンダリング終了。
	/// </summary>
	/// <remarks>
	/// 1フレームのレンダリングの終了時に呼び出してください。
	/// </remarks>
	void EndRender();
	/// <summary>
	/// D3Dデバイスを取得。
	/// </summary>
	/// <returns></returns>
	ID3D12Device5* GetD3DDevice()
	{
		return m_d3dDevice;
	}
	/// <summary>
	/// バックバッファの番号を取得。
	/// </summary>
	/// <returns>バックバッファの番号。</returns>
	UINT GetBackBufferIndex() const
	{
		return m_frameIndex;
	}
	/// <summary>
	/// コマンドキューを取得。
	/// </summary>
	/// <returns></returns>
	ID3D12CommandQueue* GetCommandQueue() const
	{
		return m_commandQueue;
	}
	/// <summary>
	/// コマンドリストを取得。
	/// </summary>
	/// <returns></returns>
	ID3D12GraphicsCommandList4* GetCommandList() const
	{
		return m_commandList;
	}
	/// <summary>
	/// CBR_SRVのディスクリプタのサイズを取得。
	/// </summary>
	/// <returns></returns>
	UINT GetCbrSrvDescriptorSize() const
	{
		return m_cbrSrvDescriptorSize;
	}
	/// <summary>
	/// サンプラのディスクリプタヒープサイズを取得。
	/// </summary>
	/// <returns></returns>
	UINT GetSapmerDescriptorSize() const
	{
		return m_samplerDescriptorSize;
	}
	/// <summary>
	/// レンダリングコンテキストを取得。
	/// </summary>
	/// <returns></returns>
	RenderContext& GetRenderContext()
	{
		return m_renderContext;
	}
	/// <summary>
	/// フレームバッファの幅を取得。
	/// </summary>
	/// <returns>フレームバッファの幅。</returns>
	UINT GetFrameBufferWidth() const
	{
		return m_frameBufferWidth;
	}
	/// <summary>
	/// フレームバッファの高さを取得。
	/// </summary>
	/// <returns>フレームバッファの高さ。</returns>
	UINT GetFrameBufferHeight() const
	{
		return m_frameBufferHeight;
	}
	/// <summary>
	/// レンダリングターゲットをフレームバッファに変更する。
	/// </summary>
	/// <param name="rc"></param>
	void ChangeRenderTargetToFrameBuffer(RenderContext& rc);
	/// <summary>
	/// 現在のフレームバッファのレンダリングターゲットビューを取得。
	/// </summary>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentFrameBuffuerRTV() const
	{
		return m_currentFrameBufferRTVHandle;
	}
	/// <summary>
	/// フレームバッファへの描画時に使用されているデプスステンシルビューを取得。
	/// </summary>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentFrameBuffuerDSV() const
	{
		return m_currentFrameBufferDSVHandle;
	}
	/// <summary>
	/// 3DModelをレイトレワールドに登録。
	/// </summary>
	/// <param name="model"></param>
	void RegistModelToRaytracingWorld(Model& model)
	{
		m_raytracingEngine.RegistGeometry(model);
	}
	/// <summary>
	/// ここまで登録されたモデルを使ってレイトレワールドを構築。
	/// </summary>
	void BuildRaytracingWorld(RenderContext& rc)
	{
		m_raytracingEngine.CommitRegistGeometry(rc);
	}
	/// <summary>
	/// レイトレーシングをディスパッチ。
	/// </summary>
	/// <param name="rc"></param>
	void DispatchRaytracing(RenderContext& rc)
	{
		m_raytracingEngine.Dispatch(rc);
	}
	/// <summary>
	/// フレームバッファにコピー。
	/// </summary>
	/// <param name="pDst"></param>
	void CopyToFrameBuffer(RenderContext& rc, ID3D12Resource* pSrc)
	{
		D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			m_renderTargets[m_frameIndex],
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_COPY_DEST);
		rc.ResourceBarrier(barrier);
		rc.CopyResource(m_renderTargets[m_frameIndex], pSrc);

		D3D12_RESOURCE_BARRIER barrier2 = CD3DX12_RESOURCE_BARRIER::Transition(
			m_renderTargets[m_frameIndex],
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_RENDER_TARGET);
		rc.ResourceBarrier(barrier2);
	}
	/// <summary>
	/// ヌルテクスチャマップを取得。
	/// </summary>
	/// <returns></returns>
	const NullTextureMaps& GetNullTextureMaps() const
	{
		return m_nullTextureMaps;
	}
	/// <summary>
	/// フォントエンジンを取得。
	/// </summary>
	/// <returns></returns>
	FontEngine& GetFontEngine()
	{
		return m_fontEngine;
	}

	ID3D12Resource* GetRenderTarget()
	{
		return m_renderTargets[m_frameIndex];
	}
private:
	/// <summary>
	/// D3Dデバイスの作成。
	/// </summary>
	/// <returns>trueが返ってきたら作成に成功。</returns>
	bool CreateD3DDevice( IDXGIFactory4* dxgiFactory );
	/// <summary>
	/// コマンドキューの作成。
	/// </summary>
	/// <returns>trueが返ってきたら作成に成功。</returns>
	bool CreateCommandQueue();
	/// <summary>
	/// スワップチェインの作成
	/// </summary>
	/// <param name="hwnd">Windowハンドル</param>
	/// <param name="frameBufferWidth">フレームバッファの幅</param>
	/// <param name="frameBufferHeight">フレームバッファの高さ</param>
	/// <param name="dxgiFactory">DirectX グラフィックス インフラストラクチャー</param>
	/// <returns>trueが返ってきたら作成に成功。</returns>
	bool CreateSwapChain(
		HWND hwnd,
		UINT frameBufferWidth,
		UINT frameBufferHeight,
		IDXGIFactory4* dxgiFactory
	);
	/// <summary>
	/// DirectX グラフィックス インフラストラクチャーの作成。
	/// </summary>
	/// <remarks>
	/// DirectX グラフィックス インフラストラクチャーは
	/// カーネル モード ドライバーおよびシステム ハードウェアと通信するためのインターフェースです。 
	/// </remarks>
	/// <returns>作成されたDXGI</returns>
	IDXGIFactory4* CreateDXGIFactory();
	/// <summary>
	/// フレームバッファ用のディスクリプタヒープを作成。
	/// </summary>
	/// <returns>trueが返ってきたら作成に成功。</returns>
	bool CreateDescriptorHeapForFrameBuffer();
	/// <summary>
	/// フレームバッファ用のレンダリングターゲットビューを作成。
	/// </summary>
	/// <returns>trueが返ってきたら作成に成功。</returns>
	bool CreateRTVForFameBuffer();
	/// <summary>
	/// フレームバッファ用の深度ステンシルビューを作成。
	/// </summary>
	/// <param name="frameBufferWidth">フレームバッファの幅</param>
	/// <param name="frameBufferHeight">フレームバッファの高さ</param>
	/// <returns>trueが返ってきたら作成に成功。</returns>
	bool CreateDSVForFrameBuffer( UINT frameBufferWidth,UINT frameBufferHeight );
	/// <summary>
	/// コマンドリストの作成。
	/// </summary>
	/// <returns>trueが返ってきたら作成に成功。</returns>
	bool CreateCommandList();
	/// <summary>
	/// GPUとの同期オブジェクト作成
	/// </summary>
	/// <returns>trueが返ってきたら作成に成功。</returns>
	bool CreateSynchronizationWithGPUObject();
	/// <summary>
	/// 描画の完了待ち。
	/// </summary>
	void WaitDraw();

	
public:
	enum { FRAME_BUFFER_COUNT = 2 };						//フレームバッファの数。
private:
	//GPUベンダー定義。
	enum GPU_Vender {
		GPU_VenderNvidia,	//NVIDIA
		GPU_VenderAMD,		//Intel
		GPU_VenderIntel,	//AMD
		Num_GPUVender,
	};
	
	ID3D12Device5* m_d3dDevice = nullptr;					//D3Dデバイス。
	ID3D12CommandQueue* m_commandQueue = nullptr;			//コマンドキュー。
	IDXGISwapChain3* m_swapChain = nullptr;					//スワップチェイン。
	ID3D12DescriptorHeap* m_rtvHeap = nullptr;				//レンダリングターゲットビューのディスクリプタヒープ。
	ID3D12DescriptorHeap* m_dsvHeap = nullptr;				//深度ステンシルビューのディスクリプタヒープ。
	ID3D12CommandAllocator* m_commandAllocator = nullptr;	//コマンドアロケータ。
	ID3D12GraphicsCommandList4* m_commandList = nullptr;		//コマンドリスト。
	ID3D12PipelineState* m_pipelineState = nullptr;			//パイプラインステート。
	int m_currentBackBufferIndex = 0;						//現在のバックバッファの番号。
	UINT m_rtvDescriptorSize = 0;							//フレームバッファのディスクリプタのサイズ。
	UINT m_dsvDescriptorSize = 0;							//深度ステンシルバッファのディスクリプタのサイズ。
	UINT m_cbrSrvDescriptorSize = 0;						//CBR_SRVのディスクリプタのサイズ。
	UINT m_samplerDescriptorSize = 0;					//サンプラのディスクリプタのサイズ。			
	ID3D12Resource* m_renderTargets[FRAME_BUFFER_COUNT] = { nullptr };	//フレームバッファ用のレンダリングターゲット。
	ID3D12Resource* m_depthStencilBuffer = nullptr;	//深度ステンシルバッファ。
	D3D12_VIEWPORT m_viewport;			//ビューポート。
	D3D12_RECT m_scissorRect;			//シザリング矩形。
	RenderContext m_renderContext;		//レンダリングコンテキスト。
	D3D12_CPU_DESCRIPTOR_HANDLE m_currentFrameBufferRTVHandle;		//現在書き込み中のフレームバッファのレンダリングターゲットビューのハンドル。
	D3D12_CPU_DESCRIPTOR_HANDLE m_currentFrameBufferDSVHandle;		//現在書き込み中のフレームバッファの深度ステンシルビューの
	// GPUとの同期で使用する変数。
	UINT m_frameIndex = 0;
	HANDLE m_fenceEvent = nullptr;
	ID3D12Fence* m_fence = nullptr;
	UINT64 m_fenceValue = 0;
	UINT m_frameBufferWidth = 0;				//フレームバッファの幅。
	UINT m_frameBufferHeight = 0;				//フレームバッファの高さ。
	Camera m_camera2D;							//2Dカメラ。
	Camera m_camera3D;							//3Dカメラ。
	raytracing::Engine m_raytracingEngine;		//レイトレエンジン。
	NullTextureMaps m_nullTextureMaps;			//ヌルテクスチャマップ。
	FontEngine m_fontEngine;					//フォントエンジン。
	std::unique_ptr<DirectX::GraphicsMemory> m_directXTKGfxMemroy;	//DirectXTKのグラフィックメモリシステム。



	//追加
private:	//データメンバ
	RenderTarget m_mainRenderTarget;	//メインレンダリングターゲット	
	//メインレンダリングターゲットの絵をフレームバッファにコピーするためのスプライト
	Sprite m_copyToFrameBufferSprite;
	int m_refreshRate = 59;						//垂直同期に使うリフレッシュレート
	std::vector< Model* > m_zprepassModels;		// ZPrepassの描画パスで描画されるモデルのリスト
	RenderTarget m_zprepassRenderTarget;		// ZPrepass描画用のレンダリングターゲット
	Texture m_skyCubeTexture;					//スカイキューブのテクスチャ
	CPostEffect m_postEffect;					//ポストエフェクト
	CShadowMap m_shadowMap;						//シャドウマップ
	CSceneChange m_sceneChange;					//場面転換

public:		//メンバ関数

	/**
	 * @brief メインレンダリングターゲットの取得。
	 * @return メインレンダリングターゲット
	*/
	RenderTarget& GetMainRenderTarget()
	{
		return m_mainRenderTarget;
	}

	/**
	 * @brief フレームバッファに描画するときのビューポートを取得。
	 * @return ビューポート
	*/
	D3D12_VIEWPORT& GetFrameBufferViewport()
	{
		return m_viewport;
	}

	/**
	 * @brief メインレンダーターゲットを使用できるようにする
	*/
	void UseMainRenderTarget();

	/**
	 * @brief メインレンダーターゲットの書き込み終了待ち
	*/
	void WaitDrawingMainRenderTarget();

	/**
	 * @brief 場面転換の描画
	*/
	void SceneChangeRender();


	/**
	 * @brief メインレンダリングターゲットの絵をフレームバッファーにコピーする
	*/
	void CopyToFrameBuffer();

	/**
	 * @brief 垂直同期のリフレッシュレートの設定
	 * @param rate リフレッシュレート
	 * @note スピンロックでFPSを固定したため、今は使っていない。
	*/
	void SetRefreshRate(const int rate)
	{
		m_refreshRate = rate;
	};
	
	/**
	 * @brief ZPrepassの描画パスにモデルを追加
	 * @param [in,out] モデル
	*/
	void Add3DModelToZPrepass(Model& model)
	{
		m_zprepassModels.push_back(&model);
	}

	/**
	 * @brief ZPrepassに登録されている3Dモデルをクリア
	*/
	void ClearZPrepassModels();

	/**
	 * @brief ZPrepassで作られた深度値のテクスチャを取得
	 * @return 深度値のテクスチャ
	*/
	Texture& GetZPrepassDepthTexture()
	{
		return m_zprepassRenderTarget.GetRenderTargetTexture();
	}

	/**
	 * @brief ZPrepass
	 * @details モデルたちの深度値を書き込む
	 * @param [in,out] rc レンダリングコンテキスト
	*/
	void ZPrepass(RenderContext& rc);


	/**
	 * @brief スカイキューブ用のテクスチャを取得
	 * @return スカイキューブ用のテクスチャ
	*/
	Texture& GetSkyCubeTexture()
	{
		return m_skyCubeTexture;
	}

	/**
	 * @brief ポストエフェクトを描画する
	 * @param [in,out] rc レンダリングコンテキスト
	*/
	void DrawPostEffect(RenderContext& rc)
	{
		m_postEffect.Draw(rc);
	}

	/**
	 * @brief シャドウマップの参照を得る
	 * @return シャドウマップの参照
	*/
	CShadowMap& GetShadowMap()
	{
		return m_shadowMap;
	}


	/**
	 * @brief 場面転換の参照を得る
	 * @return 場面転換の参照
	*/
	CSceneChange& GetSceneChange()
	{
		return m_sceneChange;
	}

private:	//privateなメンバ関数

	/**
	 * @brief メインレンダーターゲットの初期化
	 * @return 初期化できたか？
	*/
	const bool InitMainRenderTarget();

	/**
	 * @brief フレームバッファにコピーするスプライトの初期化
	*/
	void InitCopyToFrameBufferSprite();

	/**
	 * @brief ZPrepass用のレンダリングターゲットを初期化
	*/
	void InitZPrepassRenderTarget();

};
extern GraphicsEngine* g_graphicsEngine;	//グラフィックスエンジン
extern Camera* g_camera2D;					//2Dカメラ。
extern Camera* g_camera3D;					//3Dカメラ。


