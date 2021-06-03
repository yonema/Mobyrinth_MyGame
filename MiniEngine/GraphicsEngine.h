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
/// DirectX12�Ɉˑ�����O���t�B�b�N�X�G���W��
/// </summary>
class GraphicsEngine {
public:
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~GraphicsEngine();
	/// <summary>
	/// ������
	/// </summary>
	/// <remarks>
	/// �{�֐����Ăяo�����Ƃ�DirectX12�̏��������s���܂��B
	/// </remarks>
	/// <param name="hwnd">Window�n���h��</param>
	/// <param name="frameBufferwidth">�t���[���o�b�t�@�̕�</param>
	/// <param name="frameBufferHeight">�t���[���o�b�t�@�̍���</param>
	/// <returns>false���Ԃ��Ă�����쐬�Ɏ��s�B</returns>
	bool Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);
	/// <summary>
	/// �����_�����O�J�n�B
	/// </summary>
	/// <remarks>
	/// 1�t���[���̃����_�����O�̊J�n���ɌĂяo���Ă��������B
	/// </remarks>
	void BeginRender();
	/// <summary>
	/// �����_�����O�I���B
	/// </summary>
	/// <remarks>
	/// 1�t���[���̃����_�����O�̏I�����ɌĂяo���Ă��������B
	/// </remarks>
	void EndRender();
	/// <summary>
	/// D3D�f�o�C�X���擾�B
	/// </summary>
	/// <returns></returns>
	ID3D12Device5* GetD3DDevice()
	{
		return m_d3dDevice;
	}
	/// <summary>
	/// �o�b�N�o�b�t�@�̔ԍ����擾�B
	/// </summary>
	/// <returns>�o�b�N�o�b�t�@�̔ԍ��B</returns>
	UINT GetBackBufferIndex() const
	{
		return m_frameIndex;
	}
	/// <summary>
	/// �R�}���h�L���[���擾�B
	/// </summary>
	/// <returns></returns>
	ID3D12CommandQueue* GetCommandQueue() const
	{
		return m_commandQueue;
	}
	/// <summary>
	/// �R�}���h���X�g���擾�B
	/// </summary>
	/// <returns></returns>
	ID3D12GraphicsCommandList4* GetCommandList() const
	{
		return m_commandList;
	}
	/// <summary>
	/// CBR_SRV�̃f�B�X�N���v�^�̃T�C�Y���擾�B
	/// </summary>
	/// <returns></returns>
	UINT GetCbrSrvDescriptorSize() const
	{
		return m_cbrSrvDescriptorSize;
	}
	/// <summary>
	/// �T���v���̃f�B�X�N���v�^�q�[�v�T�C�Y���擾�B
	/// </summary>
	/// <returns></returns>
	UINT GetSapmerDescriptorSize() const
	{
		return m_samplerDescriptorSize;
	}
	/// <summary>
	/// �����_�����O�R���e�L�X�g���擾�B
	/// </summary>
	/// <returns></returns>
	RenderContext& GetRenderContext()
	{
		return m_renderContext;
	}
	/// <summary>
	/// �t���[���o�b�t�@�̕����擾�B
	/// </summary>
	/// <returns>�t���[���o�b�t�@�̕��B</returns>
	UINT GetFrameBufferWidth() const
	{
		return m_frameBufferWidth;
	}
	/// <summary>
	/// �t���[���o�b�t�@�̍������擾�B
	/// </summary>
	/// <returns>�t���[���o�b�t�@�̍����B</returns>
	UINT GetFrameBufferHeight() const
	{
		return m_frameBufferHeight;
	}
	/// <summary>
	/// �����_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɕύX����B
	/// </summary>
	/// <param name="rc"></param>
	void ChangeRenderTargetToFrameBuffer(RenderContext& rc);
	/// <summary>
	/// ���݂̃t���[���o�b�t�@�̃����_�����O�^�[�Q�b�g�r���[���擾�B
	/// </summary>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentFrameBuffuerRTV() const
	{
		return m_currentFrameBufferRTVHandle;
	}
	/// <summary>
	/// �t���[���o�b�t�@�ւ̕`�掞�Ɏg�p����Ă���f�v�X�X�e���V���r���[���擾�B
	/// </summary>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentFrameBuffuerDSV() const
	{
		return m_currentFrameBufferDSVHandle;
	}
	/// <summary>
	/// 3DModel�����C�g�����[���h�ɓo�^�B
	/// </summary>
	/// <param name="model"></param>
	void RegistModelToRaytracingWorld(Model& model)
	{
		m_raytracingEngine.RegistGeometry(model);
	}
	/// <summary>
	/// �����܂œo�^���ꂽ���f�����g���ă��C�g�����[���h���\�z�B
	/// </summary>
	void BuildRaytracingWorld(RenderContext& rc)
	{
		m_raytracingEngine.CommitRegistGeometry(rc);
	}
	/// <summary>
	/// ���C�g���[�V���O���f�B�X�p�b�`�B
	/// </summary>
	/// <param name="rc"></param>
	void DispatchRaytracing(RenderContext& rc)
	{
		m_raytracingEngine.Dispatch(rc);
	}
	/// <summary>
	/// �t���[���o�b�t�@�ɃR�s�[�B
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
	/// �k���e�N�X�`���}�b�v���擾�B
	/// </summary>
	/// <returns></returns>
	const NullTextureMaps& GetNullTextureMaps() const
	{
		return m_nullTextureMaps;
	}
	/// <summary>
	/// �t�H���g�G���W�����擾�B
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
	/// D3D�f�o�C�X�̍쐬�B
	/// </summary>
	/// <returns>true���Ԃ��Ă�����쐬�ɐ����B</returns>
	bool CreateD3DDevice( IDXGIFactory4* dxgiFactory );
	/// <summary>
	/// �R�}���h�L���[�̍쐬�B
	/// </summary>
	/// <returns>true���Ԃ��Ă�����쐬�ɐ����B</returns>
	bool CreateCommandQueue();
	/// <summary>
	/// �X���b�v�`�F�C���̍쐬
	/// </summary>
	/// <param name="hwnd">Window�n���h��</param>
	/// <param name="frameBufferWidth">�t���[���o�b�t�@�̕�</param>
	/// <param name="frameBufferHeight">�t���[���o�b�t�@�̍���</param>
	/// <param name="dxgiFactory">DirectX �O���t�B�b�N�X �C���t���X�g���N�`���[</param>
	/// <returns>true���Ԃ��Ă�����쐬�ɐ����B</returns>
	bool CreateSwapChain(
		HWND hwnd,
		UINT frameBufferWidth,
		UINT frameBufferHeight,
		IDXGIFactory4* dxgiFactory
	);
	/// <summary>
	/// DirectX �O���t�B�b�N�X �C���t���X�g���N�`���[�̍쐬�B
	/// </summary>
	/// <remarks>
	/// DirectX �O���t�B�b�N�X �C���t���X�g���N�`���[��
	/// �J�[�l�� ���[�h �h���C�o�[����уV�X�e�� �n�[�h�E�F�A�ƒʐM���邽�߂̃C���^�[�t�F�[�X�ł��B 
	/// </remarks>
	/// <returns>�쐬���ꂽDXGI</returns>
	IDXGIFactory4* CreateDXGIFactory();
	/// <summary>
	/// �t���[���o�b�t�@�p�̃f�B�X�N���v�^�q�[�v���쐬�B
	/// </summary>
	/// <returns>true���Ԃ��Ă�����쐬�ɐ����B</returns>
	bool CreateDescriptorHeapForFrameBuffer();
	/// <summary>
	/// �t���[���o�b�t�@�p�̃����_�����O�^�[�Q�b�g�r���[���쐬�B
	/// </summary>
	/// <returns>true���Ԃ��Ă�����쐬�ɐ����B</returns>
	bool CreateRTVForFameBuffer();
	/// <summary>
	/// �t���[���o�b�t�@�p�̐[�x�X�e���V���r���[���쐬�B
	/// </summary>
	/// <param name="frameBufferWidth">�t���[���o�b�t�@�̕�</param>
	/// <param name="frameBufferHeight">�t���[���o�b�t�@�̍���</param>
	/// <returns>true���Ԃ��Ă�����쐬�ɐ����B</returns>
	bool CreateDSVForFrameBuffer( UINT frameBufferWidth,UINT frameBufferHeight );
	/// <summary>
	/// �R�}���h���X�g�̍쐬�B
	/// </summary>
	/// <returns>true���Ԃ��Ă�����쐬�ɐ����B</returns>
	bool CreateCommandList();
	/// <summary>
	/// GPU�Ƃ̓����I�u�W�F�N�g�쐬
	/// </summary>
	/// <returns>true���Ԃ��Ă�����쐬�ɐ����B</returns>
	bool CreateSynchronizationWithGPUObject();
	/// <summary>
	/// �`��̊����҂��B
	/// </summary>
	void WaitDraw();

	
public:
	enum { FRAME_BUFFER_COUNT = 2 };						//�t���[���o�b�t�@�̐��B
private:
	//GPU�x���_�[��`�B
	enum GPU_Vender {
		GPU_VenderNvidia,	//NVIDIA
		GPU_VenderAMD,		//Intel
		GPU_VenderIntel,	//AMD
		Num_GPUVender,
	};
	
	ID3D12Device5* m_d3dDevice = nullptr;					//D3D�f�o�C�X�B
	ID3D12CommandQueue* m_commandQueue = nullptr;			//�R�}���h�L���[�B
	IDXGISwapChain3* m_swapChain = nullptr;					//�X���b�v�`�F�C���B
	ID3D12DescriptorHeap* m_rtvHeap = nullptr;				//�����_�����O�^�[�Q�b�g�r���[�̃f�B�X�N���v�^�q�[�v�B
	ID3D12DescriptorHeap* m_dsvHeap = nullptr;				//�[�x�X�e���V���r���[�̃f�B�X�N���v�^�q�[�v�B
	ID3D12CommandAllocator* m_commandAllocator = nullptr;	//�R�}���h�A���P�[�^�B
	ID3D12GraphicsCommandList4* m_commandList = nullptr;		//�R�}���h���X�g�B
	ID3D12PipelineState* m_pipelineState = nullptr;			//�p�C�v���C���X�e�[�g�B
	int m_currentBackBufferIndex = 0;						//���݂̃o�b�N�o�b�t�@�̔ԍ��B
	UINT m_rtvDescriptorSize = 0;							//�t���[���o�b�t�@�̃f�B�X�N���v�^�̃T�C�Y�B
	UINT m_dsvDescriptorSize = 0;							//�[�x�X�e���V���o�b�t�@�̃f�B�X�N���v�^�̃T�C�Y�B
	UINT m_cbrSrvDescriptorSize = 0;						//CBR_SRV�̃f�B�X�N���v�^�̃T�C�Y�B
	UINT m_samplerDescriptorSize = 0;					//�T���v���̃f�B�X�N���v�^�̃T�C�Y�B			
	ID3D12Resource* m_renderTargets[FRAME_BUFFER_COUNT] = { nullptr };	//�t���[���o�b�t�@�p�̃����_�����O�^�[�Q�b�g�B
	ID3D12Resource* m_depthStencilBuffer = nullptr;	//�[�x�X�e���V���o�b�t�@�B
	D3D12_VIEWPORT m_viewport;			//�r���[�|�[�g�B
	D3D12_RECT m_scissorRect;			//�V�U�����O��`�B
	RenderContext m_renderContext;		//�����_�����O�R���e�L�X�g�B
	D3D12_CPU_DESCRIPTOR_HANDLE m_currentFrameBufferRTVHandle;		//���ݏ������ݒ��̃t���[���o�b�t�@�̃����_�����O�^�[�Q�b�g�r���[�̃n���h���B
	D3D12_CPU_DESCRIPTOR_HANDLE m_currentFrameBufferDSVHandle;		//���ݏ������ݒ��̃t���[���o�b�t�@�̐[�x�X�e���V���r���[��
	// GPU�Ƃ̓����Ŏg�p����ϐ��B
	UINT m_frameIndex = 0;
	HANDLE m_fenceEvent = nullptr;
	ID3D12Fence* m_fence = nullptr;
	UINT64 m_fenceValue = 0;
	UINT m_frameBufferWidth = 0;				//�t���[���o�b�t�@�̕��B
	UINT m_frameBufferHeight = 0;				//�t���[���o�b�t�@�̍����B
	Camera m_camera2D;							//2D�J�����B
	Camera m_camera3D;							//3D�J�����B
	raytracing::Engine m_raytracingEngine;		//���C�g���G���W���B
	NullTextureMaps m_nullTextureMaps;			//�k���e�N�X�`���}�b�v�B
	FontEngine m_fontEngine;					//�t�H���g�G���W���B
	std::unique_ptr<DirectX::GraphicsMemory> m_directXTKGfxMemroy;	//DirectXTK�̃O���t�B�b�N�������V�X�e���B



	//�ǉ�
private:	//�f�[�^�����o
	RenderTarget m_mainRenderTarget;	//���C�������_�����O�^�[�Q�b�g	
	//���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g
	Sprite m_copyToFrameBufferSprite;
	int m_refreshRate = 59;						//���������Ɏg�����t���b�V�����[�g
	std::vector< Model* > m_zprepassModels;		// ZPrepass�̕`��p�X�ŕ`�悳��郂�f���̃��X�g
	RenderTarget m_zprepassRenderTarget;		// ZPrepass�`��p�̃����_�����O�^�[�Q�b�g
	Texture m_skyCubeTexture;					//�X�J�C�L���[�u�̃e�N�X�`��
	CPostEffect m_postEffect;					//�|�X�g�G�t�F�N�g
	CShadowMap m_shadowMap;						//�V���h�E�}�b�v
	CSceneChange m_sceneChange;					//��ʓ]��

public:		//�����o�֐�

	/**
	 * @brief ���C�������_�����O�^�[�Q�b�g�̎擾�B
	 * @return ���C�������_�����O�^�[�Q�b�g
	*/
	RenderTarget& GetMainRenderTarget()
	{
		return m_mainRenderTarget;
	}

	/**
	 * @brief �t���[���o�b�t�@�ɕ`�悷��Ƃ��̃r���[�|�[�g���擾�B
	 * @return �r���[�|�[�g
	*/
	D3D12_VIEWPORT& GetFrameBufferViewport()
	{
		return m_viewport;
	}

	/**
	 * @brief ���C�������_�[�^�[�Q�b�g���g�p�ł���悤�ɂ���
	*/
	void UseMainRenderTarget();

	/**
	 * @brief ���C�������_�[�^�[�Q�b�g�̏������ݏI���҂�
	*/
	void WaitDrawingMainRenderTarget();

	/**
	 * @brief ��ʓ]���̕`��
	*/
	void SceneChangeRender();


	/**
	 * @brief ���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�[�ɃR�s�[����
	*/
	void CopyToFrameBuffer();

	/**
	 * @brief ���������̃��t���b�V�����[�g�̐ݒ�
	 * @param rate ���t���b�V�����[�g
	 * @note �X�s�����b�N��FPS���Œ肵�����߁A���͎g���Ă��Ȃ��B
	*/
	void SetRefreshRate(const int rate)
	{
		m_refreshRate = rate;
	};
	
	/**
	 * @brief ZPrepass�̕`��p�X�Ƀ��f����ǉ�
	 * @param [in,out] ���f��
	*/
	void Add3DModelToZPrepass(Model& model)
	{
		m_zprepassModels.push_back(&model);
	}

	/**
	 * @brief ZPrepass�ɓo�^����Ă���3D���f�����N���A
	*/
	void ClearZPrepassModels();

	/**
	 * @brief ZPrepass�ō��ꂽ�[�x�l�̃e�N�X�`�����擾
	 * @return �[�x�l�̃e�N�X�`��
	*/
	Texture& GetZPrepassDepthTexture()
	{
		return m_zprepassRenderTarget.GetRenderTargetTexture();
	}

	/**
	 * @brief ZPrepass
	 * @details ���f�������̐[�x�l����������
	 * @param [in,out] rc �����_�����O�R���e�L�X�g
	*/
	void ZPrepass(RenderContext& rc);


	/**
	 * @brief �X�J�C�L���[�u�p�̃e�N�X�`�����擾
	 * @return �X�J�C�L���[�u�p�̃e�N�X�`��
	*/
	Texture& GetSkyCubeTexture()
	{
		return m_skyCubeTexture;
	}

	/**
	 * @brief �|�X�g�G�t�F�N�g��`�悷��
	 * @param [in,out] rc �����_�����O�R���e�L�X�g
	*/
	void DrawPostEffect(RenderContext& rc)
	{
		m_postEffect.Draw(rc);
	}

	/**
	 * @brief �V���h�E�}�b�v�̎Q�Ƃ𓾂�
	 * @return �V���h�E�}�b�v�̎Q��
	*/
	CShadowMap& GetShadowMap()
	{
		return m_shadowMap;
	}


	/**
	 * @brief ��ʓ]���̎Q�Ƃ𓾂�
	 * @return ��ʓ]���̎Q��
	*/
	CSceneChange& GetSceneChange()
	{
		return m_sceneChange;
	}

private:	//private�ȃ����o�֐�

	/**
	 * @brief ���C�������_�[�^�[�Q�b�g�̏�����
	 * @return �������ł������H
	*/
	const bool InitMainRenderTarget();

	/**
	 * @brief �t���[���o�b�t�@�ɃR�s�[����X�v���C�g�̏�����
	*/
	void InitCopyToFrameBufferSprite();

	/**
	 * @brief ZPrepass�p�̃����_�����O�^�[�Q�b�g��������
	*/
	void InitZPrepassRenderTarget();

};
extern GraphicsEngine* g_graphicsEngine;	//�O���t�B�b�N�X�G���W��
extern Camera* g_camera2D;					//2D�J�����B
extern Camera* g_camera3D;					//3D�J�����B


