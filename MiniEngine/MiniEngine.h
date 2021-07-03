#pragma once

#pragma comment( lib, "xinput.lib")
#pragma comment(lib, "dxcompiler.lib")

#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <math.h>
#include <vector>
#include <list>
#include <D3Dcompiler.h>
#include <Windows.h>
#include <d3d12.h>
#include <map>
#include <functional>
#include <algorithm>
#include <DirectXMath.h>
#include <Xinput.h>
#include <array>
#include <dxcapi.h>
#include <atlbase.h>
#include <dxgi.h>
#include <dxgitype.h>
#include <dxgiformat.h>

#include "DirectXTK/Inc/CommonStates.h"
#include "DirectXTK/Inc/SpriteBatch.h"
#include "DirectXTK/Inc/SpriteFont.h"

#include "d3dx12.h"
//#include "debug/MyAssert.h"
#define MY_ASSERT

#include "math/Math.h"
#include "math/Vector.h"
#include "math/Matrix.h"

#include "tkEngine.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "IShaderResource.h"
#include "IUnorderAccessResource.h"
#include "GPUBuffer.h"
#include "Texture.h"
#include "RWStructuredBuffer.h"
#include "StructuredBuffer.h"
#include "Shader.h"
#include "PipelineState.h"
#include "ConstantBuffer.h"
#include "RootSignature.h"
#include "raytracing/Raytracing.h"
#include "GraphicsEngine.h"
#include "DescriptorHeap.h"
#include "RenderTarget.h"
#include "Animation.h"
#include "font/Font.h"

#include "RenderContext_inline.h"
#include "DescriptorHeap_inline.h"

#include "Sprite.h"
#include "Model.h"
#include "HID/GamePad.h"


//追加
#include <xaudio2fx.h>
#include <xaudio2.h>
#include <x3daudio.h>



//追加ここまで





extern const UINT g_FRAME_BUFFER_W;	//フレームバッファの幅。
extern const UINT g_FRAME_BUFFER_H;	//フレームバッファの高さ。
static const int g_MAX_BONE = 512;	//!<ボーンの最大数。