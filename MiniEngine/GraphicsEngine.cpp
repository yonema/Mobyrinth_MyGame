#include "stdafx.h"
#include "../GameTemplate/Game/stdafx.h"
#include "GraphicsEngine.h"
#include "../GameTemplate/Game/SpriteRenderConstData.h"

GraphicsEngine* g_graphicsEngine = nullptr;	//グラフィックスエンジン
Camera* g_camera2D = nullptr;				//2Dカメラ。
Camera* g_camera3D = nullptr;				//3Dカメラ。


GraphicsEngine::~GraphicsEngine()
{
	WaitDraw();
	//後始末。
	if (m_commandQueue) {
		m_commandQueue->Release();
	}
	if (m_swapChain) {
		m_swapChain->Release();
	}
	if (m_rtvHeap) {
		m_rtvHeap->Release();
	}
	if (m_dsvHeap) {
		m_dsvHeap->Release();
	}
	if (m_commandAllocator) {
		m_commandAllocator->Release();
	}
	if (m_commandList) {
		m_commandList->Release();
	}
	if (m_pipelineState) {
		m_pipelineState->Release();
	}
	for (auto& rt : m_renderTargets) {
		if (rt) {
			rt->Release();
		}
	}
	if (m_depthStencilBuffer) {
		m_depthStencilBuffer->Release();
	}
	if (m_fence) {
		m_fence->Release();
	}

	if (m_d3dDevice) {
		m_d3dDevice->Release();
	}

	//追加
	//G_BUFFER用のレンダリングターゲットの破棄処理
	ReleaseGBufferRenderTarget();

	CloseHandle(m_fenceEvent);
}
void GraphicsEngine::WaitDraw()
{
	//描画終了待ち
	// Signal and increment the fence value.
	const UINT64 fence = m_fenceValue;
	m_commandQueue->Signal(m_fence, fence);
	m_fenceValue++;

	// Wait until the previous frame is finished.
	if (m_fence->GetCompletedValue() < fence)
	{
		m_fence->SetEventOnCompletion(fence, m_fenceEvent);
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}
}

/**
 * @brief メインレンダーターゲットの初期化
 * @return 初期化できたか？
*/
bool GraphicsEngine::InitMainRenderTarget()
{
	bool ret =
		m_mainRenderTarget.Create(
			m_frameBufferWidth,
			m_frameBufferHeight,
			1,
			1,
			// 【注目】カラーバッファーのフォーマットを32bit浮動小数点にしている
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

	return ret;
}

/**
 * @brief フレームバッファにコピーするスプライトの初期化
*/
void GraphicsEngine::InitCopyToFrameBufferSprite()
{
	// mainRenderTargetのテクスチャをフレームバッファーに貼り付けるためのスプライトを初期化する
	// スプライトの初期化オブジェクトを作成する
	SpriteInitData spriteInitData;

	// テクスチャはmainRenderTargetのカラーバッファー
	spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
	spriteInitData.m_width = 1280;
	spriteInitData.m_height = 720;

	// モノクロ用のシェーダーを指定する
	spriteInitData.m_fxFilePath = 
		nsMobyrinth::nsGraphic::nsSprite::spriteRenderConstData::SHADER_FILEPATH_DEFAULT;

	// 初期化オブジェクトを使って、スプライトを初期化する
	m_copyToFrameBufferSprite.Init(spriteInitData);
	m_copyToFrameBufferSprite.Update(g_VEC3_ZERO, g_QUAT_IDENTITY, { -1.0f,1.0f,1.0f });
}

/**
 * @brief ZPrepass用のレンダリングターゲットを初期化
*/
void GraphicsEngine::InitZPrepassRenderTarget()
{
	//レンダリングターゲットの生成
	m_zprepassRenderTarget.Create(
		g_graphicsEngine->GetFrameBufferWidth(),
		g_graphicsEngine->GetFrameBufferHeight(),
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);

	return;
}

/**
 * @brief G_BUFFER用のレンダリングターゲットの初期化処理
*/
void GraphicsEngine::InitGBufferRenderTarget()
{
	//G_BUFFER用のレンダリングターゲットを動的に生成
	for (int i = 0; i < EN_G_BUFFER_NUM; i++)
	{
		m_gBufferRenderTargets[i] = new RenderTarget;
	}

	//アルベドカラー描き込み用レンダリングターゲット
	m_gBufferRenderTargets[EN_ALBED]->Create(
		g_graphicsEngine->GetFrameBufferWidth(),
		g_graphicsEngine->GetFrameBufferHeight(),
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_D32_FLOAT
	);

	//法線書き込み用のレンダリングターゲット
	m_gBufferRenderTargets[EN_NORMAL]->Create(
		g_graphicsEngine->GetFrameBufferWidth(),
		g_graphicsEngine->GetFrameBufferHeight(),
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_UNKNOWN
	);

	//ビュー座標系の法線描き込み用のレンダリングターゲット
	m_gBufferRenderTargets[EN_VIEW_NORMAL]->Create(
		g_graphicsEngine->GetFrameBufferWidth(),
		g_graphicsEngine->GetFrameBufferHeight(),
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_SNORM,	//負の数情報あり
		DXGI_FORMAT_UNKNOWN
	);

	//ワールド座標系の座標描き込み用のレンダリングターゲット
	m_gBufferRenderTargets[EN_POS_IN_WRD]->Create(
		g_graphicsEngine->GetFrameBufferWidth(),
		g_graphicsEngine->GetFrameBufferHeight(),
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_UNKNOWN
	);

	//ライトビュープロジェクション座標系の座標描き込み用のレンダリングターゲット
	m_gBufferRenderTargets[EN_POS_IN_LVP]->Create(
		g_graphicsEngine->GetFrameBufferWidth(),
		g_graphicsEngine->GetFrameBufferHeight(),
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_UNKNOWN
	);

	//プロジェクション座標系の座標の描き込みようのレンダリングターゲット
	m_gBufferRenderTargets[EN_POS_IN_PROJ]->Create(
		g_graphicsEngine->GetFrameBufferWidth(),
		g_graphicsEngine->GetFrameBufferHeight(),
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_UNKNOWN
	);

	//自己発光色の描き込みようのレンダリングターゲット
	m_gBufferRenderTargets[EN_EMISSION_COLOR]->Create(
		g_graphicsEngine->GetFrameBufferWidth(),
		g_graphicsEngine->GetFrameBufferHeight(),
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_UNKNOWN
	);


	return;
}

/**
 * @brief G_BUFFER用のレンダリングターゲットの破棄処理
*/
void GraphicsEngine::ReleaseGBufferRenderTarget()
{
	//G_BUFFER用のレンダリングターゲットを動的に開放
	for (int i = 0; i < EN_G_BUFFER_NUM; i++)
	{
		delete m_gBufferRenderTargets[i];
	}
	return;
}

/**
 * @brief ディファ―ドレンダリング用のスプライトの初期化
*/
void GraphicsEngine::InitDefferdRenderSprite()
{
	SpriteInitData defferdSpriteInitData;

	defferdSpriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
	defferdSpriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();

	//ディファ―ドレンダリングで使用するG_BUFFERの設定
	for (int i = 0; i < EN_G_BUFFER_NUM; i++)
	{
		defferdSpriteInitData.m_textures[i] = &m_gBufferRenderTargets[i]->GetRenderTargetTexture();
	}

	defferdSpriteInitData.m_textures[EN_G_BUFFER_NUM + 0] = &m_shadowMap.GetShadowBlur();
	defferdSpriteInitData.m_textures[EN_G_BUFFER_NUM + 1] = 
		&m_zprepassRenderTarget.GetRenderTargetTexture();
	defferdSpriteInitData.m_textures[EN_G_BUFFER_NUM + 2] = &m_skyCubeTexture;

	defferdSpriteInitData.m_expandConstantBuffer = &m_skyIBLcb;
	defferdSpriteInitData.m_expandConstantBufferSize = sizeof(m_skyIBLcb);


	defferdSpriteInitData.m_fxFilePath = "Assets/shader/defferdLightingSprite.fx";

	defferdSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

	m_defferdLightingSprite.Init(defferdSpriteInitData);
	m_defferdLightingSprite.Update(
		g_VEC3_ZERO, g_QUAT_IDENTITY,
		nsMobyrinth::nsGraphic::nsSprite::spriteRenderConstData::SPRITE_SCALE_DEFAULT
	);

	return;
}


bool GraphicsEngine::Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight)
{

	g_graphicsEngine = this;

	m_frameBufferWidth = frameBufferWidth;
	m_frameBufferHeight = frameBufferHeight;

	//デバイスにアクセスするためのインターフェースを作成。
	auto dxgiFactory = CreateDXGIFactory();
	//D3Dデバイスの作成。
	if (!CreateD3DDevice(dxgiFactory)) {
		//D3Dデバイスの作成に失敗した。
		MessageBox(hwnd, TEXT("D3Dデバイスの作成に失敗しました。"), TEXT("エラー"), MB_OK);
		return false;
	}
	//コマンドキューの作成。
	if (!CreateCommandQueue()) {
		//コマンドキューの作成に失敗した。
		MessageBox(hwnd, TEXT("コマンドキューの作成に失敗しました。"), TEXT("エラー"), MB_OK);
		return false;
	}
	//スワップチェインを作成。
	if (!CreateSwapChain(hwnd, frameBufferWidth, frameBufferHeight, dxgiFactory)) {
		//スワップチェインの作成に失敗。
		MessageBox(hwnd, TEXT("スワップチェインの作成に失敗しました。"), TEXT("エラー"), MB_OK);
		return false;
	}

	//フレームバッファ用のディスクリプタヒープを作成する。
	if (!CreateDescriptorHeapForFrameBuffer()) {
		MessageBox(hwnd, TEXT("フレームバッファ用のディスクリプタヒープの作成に失敗しました。"), TEXT("エラー"), MB_OK);
		return false;
	}

	//フレームバッファ用のRTVを作成する。
	if (!CreateRTVForFameBuffer()) {
		MessageBox(hwnd, TEXT("フレームバッファ用のRTVの作成に失敗しました。"), TEXT("エラー"), MB_OK);
		return false;

	}

	//フレームバッファ用のDSVを作成する。
	if (!CreateDSVForFrameBuffer(frameBufferWidth, frameBufferHeight)) {
		MessageBox(hwnd, TEXT("フレームバッファ用のDSVの作成に失敗しました。"), TEXT("エラー"), MB_OK);
		return false;
	}

	//メインレンダリングターゲットの初期化。
	if (!InitMainRenderTarget()) {
		MessageBox(hwnd, TEXT("メインレンダリングターゲット用のRTVの作成に失敗しました。"), TEXT("エラー"), MB_OK);
		return false;
	}
	

	//コマンドアロケータの作成。
	m_d3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&m_commandAllocator));

	if (!m_commandAllocator) {
		MessageBox(hwnd, TEXT("コマンドアロケータの作成に失敗しました。"), TEXT("エラー"), MB_OK);
		return false;
	}

	//コマンドリストの作成。
	if (!CreateCommandList()) {
		MessageBox(hwnd, TEXT("コマンドリストの作成に失敗しました。"), TEXT("エラー"), MB_OK);
		return false;
	}

	//GPUと同期をとるためのオブジェクトを作成する。
	if (!CreateSynchronizationWithGPUObject()) {
		MessageBox(hwnd, TEXT("GPUと同期をとるためのオブジェクトの作成に失敗しました。"), TEXT("エラー"), MB_OK);
		return false;
	}

	//レンダリングコンテキストの作成。
	m_renderContext.Init(m_commandList);

	//ビューポートを初期化。
	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Width = static_cast<FLOAT>(frameBufferWidth);
	m_viewport.Height = static_cast<FLOAT>(frameBufferHeight);
	m_viewport.MinDepth = D3D12_MIN_DEPTH;
	m_viewport.MaxDepth = D3D12_MAX_DEPTH;

	//シザリング矩形を初期化。
	m_scissorRect.left = 0;
	m_scissorRect.top = 0;
	m_scissorRect.right = frameBufferWidth;
	m_scissorRect.bottom = frameBufferHeight;

	//CBR_SVRのディスクリプタのサイズを取得。
	m_cbrSrvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//Samplerのディスクリプタのサイズを取得。
	m_samplerDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

	//初期化が終わったのでDXGIを破棄。
	dxgiFactory->Release();

	//ヌルテクスチャを初期化
	m_nullTextureMaps.Init();

	//カメラを初期化する。
	m_camera2D.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	m_camera2D.SetWidth(static_cast<float>(m_frameBufferWidth));
	m_camera2D.SetHeight(static_cast<float>(m_frameBufferHeight));
	m_camera2D.SetPosition({ 0.0f, 0.0f, -1.0f });
	m_camera2D.SetTarget({ 0.0f, 0.0f, 0.0f });

	m_camera3D.SetPosition({ 0.0f, 50.0f, 200.0f });
	m_camera3D.SetTarget({ 0.0f, 50.0f, 0.0f });

	g_camera2D = &m_camera2D;
	g_camera3D = &m_camera3D;

	//DirectXTK用のグラフィックメモリ管理クラスのインスタンスを作成する。
	m_directXTKGfxMemroy = std::make_unique<DirectX::GraphicsMemory>(m_d3dDevice);
	//フォント描画エンジンを初期化。
	m_fontEngine.Init();

	////mainRenderTargetのテクスチャをフレームバッファーに貼り付けるためのスプライトの初期化
	InitCopyToFrameBufferSprite();

	//シャドウマップの初期化
	m_shadowMap.Init();

	// ZPrepass用のレンダリングターゲットを初期化
	InitZPrepassRenderTarget();

	//ポストエフェクトの初期化
	m_postEffect.Init();
	//
	m_hud.Init();

	//m_skyCubeTexture.InitFromDDSFile(L"Assets/modelData/preset/sky.dds" );

	//G_BUFFER用のレンダリングターゲットの初期化処理
	InitGBufferRenderTarget();

	//ディファ―ドレンダリング用のスプライトの初期化
	InitDefferdRenderSprite();

	return true;
}

/**
 * @brief スカイキューブ用のテクスチャを初期化しなおす
 * @param skyIBLData IBL用の空のデータ
*/
void GraphicsEngine::ReInitSkyCubeTexture(const nsMobyrinth::nsSky::SSkyIBLData& skyIBLData)
{
	m_skyCubeTexture.InitFromDDSFile(skyIBLData.m_skyTextureFilepath);
	m_skyIBLcb.m_luminance = skyIBLData.m_luminance;
	m_skyIBLcb.m_IBLRate = skyIBLData.m_IBLRate;

	InitDefferdRenderSprite();

	return;
}

IDXGIFactory4* GraphicsEngine::CreateDXGIFactory()
{
	UINT dxgiFactoryFlags = 0;
#ifdef _DEBUG
	//デバッグコントローラーがあれば、デバッグレイヤーがあるDXGIを作成する。
	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();

		// Enable additional debug layers.
		dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		debugController->Release();
	}
#endif
	IDXGIFactory4* factory;
	CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory));
	return factory;
}

bool GraphicsEngine::CreateD3DDevice(IDXGIFactory4* dxgiFactory)
{
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_12_1,	//Direct3D 12.1の機能を使う。
		D3D_FEATURE_LEVEL_12_0	//Direct3D 12.0の機能を使う。
	};

	IDXGIAdapter* adapterTmp = nullptr;
	IDXGIAdapter* adapterVender[Num_GPUVender] = { nullptr };	//各ベンダーのアダプター。
	IDXGIAdapter* adapterMaxVideoMemory = nullptr;				//最大ビデオメモリのアダプタ。
	IDXGIAdapter* useAdapter = nullptr;							//最終的に使用するアダプタ。
	SIZE_T videoMemorySize = 0;
	for (int i = 0; dxgiFactory->EnumAdapters(i, &adapterTmp) != DXGI_ERROR_NOT_FOUND; i++) {
		DXGI_ADAPTER_DESC desc;
		adapterTmp->GetDesc(&desc);

		if (desc.DedicatedVideoMemory > videoMemorySize) {
			//こちらのビデオメモリの方が多いので、こちらを使う。
			adapterMaxVideoMemory = adapterTmp;
			videoMemorySize = desc.DedicatedVideoMemory;
		}
		if (wcsstr(desc.Description, L"NVIDIA") != nullptr) {
			//NVIDIA製
			adapterVender[GPU_VenderNvidia] = adapterTmp;
		}
		else if (wcsstr(desc.Description, L"AMD") != nullptr) {
			//AMD製
			adapterVender[GPU_VenderAMD] = adapterTmp;
		}
		else if (wcsstr(desc.Description, L"Intel") != nullptr) {
			//Intel製
			adapterVender[GPU_VenderIntel] = adapterTmp;
		}
	}
	//使用するアダプターを決める。
	if (adapterVender[GPU_VenderNvidia] != nullptr) {
		//NVIDIA製が最優先
		useAdapter = adapterVender[GPU_VenderNvidia];
	}
	else if (adapterVender[GPU_VenderAMD] != nullptr) {
		//次はAMDが優先。
		useAdapter = adapterVender[GPU_VenderAMD];
	}
	else {
		//NVIDIAとAMDのGPUがなければビデオメモリが一番多いやつを使う。
		useAdapter = adapterMaxVideoMemory;
	}
	for (auto featureLevel : featureLevels) {

		auto hr = D3D12CreateDevice(
			useAdapter,
			featureLevel,
			IID_PPV_ARGS(&m_d3dDevice)
		);
		if (SUCCEEDED(hr)) {
			//D3Dデバイスの作成に成功した。
			break;
		}
	}
	return m_d3dDevice != nullptr;
}
bool GraphicsEngine::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	auto hr = m_d3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
	if (FAILED(hr)) {
		//コマンドキューの作成に失敗した。
		return false;
	}
	return true;
}
bool GraphicsEngine::CreateSwapChain(
	HWND hwnd,
	UINT frameBufferWidth,
	UINT frameBufferHeight,
	IDXGIFactory4* dxgiFactory
)
{
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = FRAME_BUFFER_COUNT;
	swapChainDesc.Width = frameBufferWidth;
	swapChainDesc.Height = frameBufferHeight;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	IDXGISwapChain1* swapChain;
	dxgiFactory->CreateSwapChainForHwnd(
		m_commandQueue,
		hwnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChain
	);
	//IDXGISwapChain3のインターフェースを取得。
	swapChain->QueryInterface(IID_PPV_ARGS(&m_swapChain));
	swapChain->Release();
	//IDXGISwapChain3のインターフェースを取得。
	m_currentBackBufferIndex = m_swapChain->GetCurrentBackBufferIndex();
	return true;
}
bool GraphicsEngine::CreateDescriptorHeapForFrameBuffer()
{
	//RTV用のディスクリプタヒープを作成する。
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = FRAME_BUFFER_COUNT;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	auto hr = m_d3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_rtvHeap));
	if (FAILED(hr)) {
		//RTV用のディスクリプタヒープの作成に失敗した。
		return false;
	}
	//ディスクリプタのサイズを取得。
	m_rtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//DSV用のディスクリプタヒープを作成する。
	desc.NumDescriptors = 1;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	hr = m_d3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_dsvHeap));
	if (FAILED(hr)) {
		//DSV用のディスクリプタヒープの作成に失敗した。
		return false;
	}
	//ディスクリプタのサイズを取得。
	m_dsvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	return true;
}
bool GraphicsEngine::CreateRTVForFameBuffer()
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();

	//フロントバッファをバックバッファ用のRTVを作成。
	for (UINT n = 0; n < FRAME_BUFFER_COUNT; n++) {
		m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n]));
		m_d3dDevice->CreateRenderTargetView(
			m_renderTargets[n], nullptr, rtvHandle
		);
		rtvHandle.ptr += m_rtvDescriptorSize;
	}
	return true;
}
bool GraphicsEngine::CreateDSVForFrameBuffer(UINT frameBufferWidth, UINT frameBufferHeight)
{
	D3D12_CLEAR_VALUE dsvClearValue;
	dsvClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	dsvClearValue.DepthStencil.Depth = 1.0f;
	dsvClearValue.DepthStencil.Stencil = 0;

	CD3DX12_RESOURCE_DESC desc(
		D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		0,
		frameBufferWidth,
		frameBufferHeight,
		1,
		1,
		DXGI_FORMAT_D32_FLOAT,
		1,
		0,
		D3D12_TEXTURE_LAYOUT_UNKNOWN,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE);

	auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto hr = m_d3dDevice->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&dsvClearValue,
		IID_PPV_ARGS(&m_depthStencilBuffer)
	);
	if (FAILED(hr)) {
		//深度ステンシルバッファの作成に失敗。
		return false;
	}
	//ディスクリプタを作成
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();

	m_d3dDevice->CreateDepthStencilView(
		m_depthStencilBuffer, nullptr, dsvHandle
	);

	return true;
}
bool GraphicsEngine::CreateCommandList()
{
	//コマンドリストの作成。
	m_d3dDevice->CreateCommandList(
		0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator, nullptr, IID_PPV_ARGS(&m_commandList)
	);
	if (!m_commandList) {
		return false;
	}
	//コマンドリストは開かれている状態で作成されるので、いったん閉じる。
	m_commandList->Close();

	return true;
}
bool GraphicsEngine::CreateSynchronizationWithGPUObject()
{
	m_d3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
	if (!m_fence) {
		//フェンスの作成に失敗した。
		return false;
	}
	m_fenceValue = 1;
	//同期を行うときのイベントハンドラを作成する。
	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (m_fenceEvent == nullptr) {
		return false;
	}
	return true;
}
void GraphicsEngine::BeginRender()
{
	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

	//カメラを更新する。
	m_camera2D.Update();
	m_camera3D.Update();

	//コマンドアロケータををリセット。
	m_commandAllocator->Reset();
	//レンダリングコンテキストもリセット。
	m_renderContext.Reset(m_commandAllocator, m_pipelineState);
	//ビューポートを設定。
	//ビューポートを設定。
	m_renderContext.SetViewport(m_viewport);
	//シザリング矩形を設定。
	m_renderContext.SetScissorRect(m_scissorRect);

	m_currentFrameBufferRTVHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
	m_currentFrameBufferRTVHandle.ptr += m_frameIndex * m_rtvDescriptorSize;
	//深度ステンシルバッファのディスクリプタヒープの開始アドレスを取得。
	m_currentFrameBufferDSVHandle = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
	//バックバッファがレンダリングターゲットとして設定可能になるまで待つ。
	m_renderContext.WaitUntilToPossibleSetRenderTarget(m_renderTargets[m_frameIndex]);

	//レンダリングターゲットを設定。
	m_renderContext.SetRenderTarget(m_currentFrameBufferRTVHandle, m_currentFrameBufferDSVHandle);

	const float clearColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_renderContext.ClearRenderTargetView(m_currentFrameBufferRTVHandle, clearColor);
	m_renderContext.ClearDepthStencilView(m_currentFrameBufferDSVHandle, 1.0f);

	//UseMainRenderTarget();

	return;
}


void GraphicsEngine::ChangeRenderTargetToFrameBuffer(RenderContext& rc)
{
	rc.SetRenderTarget(m_currentFrameBufferRTVHandle, m_currentFrameBufferDSVHandle);

	return;
}

///// <summary>
///// シャドウを描画する
///// </summary>
//void GraphicsEngine::ShadowRender()
//{
//	m_shadowMap.Draw(m_renderContext);
//}

/// <summary>
/// メインレンダーターゲットを使用できるようにする
/// </summary>
void GraphicsEngine::UseMainRenderTarget()
{
	// レンダリングターゲットをmainRenderTargetに変更する
	// レンダリングターゲットとして利用できるまで待つ
	m_renderContext.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
	// レンダリングターゲットを設定
	m_renderContext.SetRenderTargetAndViewport(m_mainRenderTarget);
	// レンダリングターゲットをクリア
	//m_renderContext.ClearRenderTargetView(m_mainRenderTarget);
	//m_renderContext.ClearRenderTargetView(m_mainRenderTarget.GetRTVCpuDescriptorHandle(), m_mainRenderTarget.GetRTVClearColor());

	return;
}

/// <summary>
/// メインレンダーターゲットの書き込み終了待ち
/// </summary>
void GraphicsEngine::WaitDrawingMainRenderTarget()
{
	m_renderContext.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

	return;
}

/**
 * @brief HUDの描画
*/
void GraphicsEngine::HUDRender()
{
	//パラメータのアップデート
	m_hud.UpdateParam();
	//描画
	m_hud.Draw(m_renderContext);

	return;
}

/**
 * @brief メインレンダリングターゲットの絵をフレームバッファーにコピーする
*/
void GraphicsEngine::CopyToFrameBuffer()
{
	// メインレンダリングターゲットの絵をフレームバッファーにコピー
	m_renderContext.SetRenderTarget(
		m_currentFrameBufferRTVHandle,
		m_currentFrameBufferDSVHandle
	);
	m_copyToFrameBufferSprite.Draw(m_renderContext);

	return;
}

/**
 * @brief ZPrepass
 * @details モデルたちの深度値を書き込む
 * @param [in,out] rc レンダリングコンテキスト
*/
void GraphicsEngine::ZPrepass(RenderContext& rc)
{
	// まず、レンダリングターゲットとして設定できるようになるまで待つ
	rc.WaitUntilToPossibleSetRenderTarget(m_zprepassRenderTarget);

	// レンダリングターゲットを設定
	rc.SetRenderTargetAndViewport(m_zprepassRenderTarget);

	// レンダリングターゲットをクリア
	rc.ClearRenderTargetView(m_zprepassRenderTarget);

	for (auto& model : m_zprepassModels)
	{
		model->Draw(rc);
	}

	rc.WaitUntilFinishDrawingToRenderTarget(m_zprepassRenderTarget);

	return;
}

/**
 * @brief ZPrepassに登録されている3Dモデルをクリア
*/
void GraphicsEngine::ClearZPrepassModels()
{
	m_zprepassModels.clear();

	return;
}

/**
 * @brief ディファ―ドレンダリングを使用可能にする
*/
void GraphicsEngine::UseDefferdRendering()
{
	// まず、レンダリングターゲットとして設定できるようになるまで待つ
	m_renderContext.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(m_gBufferRenderTargets), m_gBufferRenderTargets);

	// レンダリングターゲットを設定
	m_renderContext.SetRenderTargets(ARRAYSIZE(m_gBufferRenderTargets), m_gBufferRenderTargets);

	// レンダリングターゲットをクリア
	m_renderContext.ClearRenderTargetViews(ARRAYSIZE(m_gBufferRenderTargets), m_gBufferRenderTargets);

	return;
}

/**
 * @brief ディファ―ドレンダリングの書き込み終了待ち
*/
void GraphicsEngine::WaitDefferdRenderring()
{
	m_renderContext.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(m_gBufferRenderTargets), m_gBufferRenderTargets);

	return;
}

/**
 * @brief ディファ―ドレンダリング用のスプライトの描画処理
*/
void GraphicsEngine::DrawDefferdRenderSprite()
{
	m_defferdLightingSprite.Draw(m_renderContext);

	return;
}

void GraphicsEngine::EndRender()
{
	// レンダリングターゲットへの描き込み完了待ち
	m_renderContext.WaitUntilFinishDrawingToRenderTarget(m_renderTargets[m_frameIndex]);


	m_directXTKGfxMemroy->Commit(m_commandQueue);

	//レンダリングコンテキストを閉じる。
	m_renderContext.Close();

	//コマンドを実行。
	ID3D12CommandList* ppCommandLists[] = { m_commandList };
	m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	//FPX_FIX Present the frame.
	/*if (m_refreshRate >= 100)
		m_swapChain->Present(2, 0);
	else
		m_swapChain->Present(1, 0);*/
		// 垂直同期を待たないようにする。
	m_swapChain->Present(0, 0);

	m_directXTKGfxMemroy->GarbageCollect();
	//描画完了待ち。
	WaitDraw();

	return;
}