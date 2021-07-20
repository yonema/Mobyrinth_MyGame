#include "stdafx.h"
#include "SpriteRender.h"

//スプライトレンダラーの定数データを使用可能にする
using namespace spriteRenderConstData;

//スタート関数
bool CSpriteRender::Start()
{
	return true;
}

//デストラクタ
CSpriteRender::~CSpriteRender()
{

}

/// <summary>
/// 初期化用関数
/// 最初に呼んでね。
/// </summary>
/// <param name="texFilePath">ddsファイルパス</param>
/// <param name="w">スプライトの横幅</param>
/// <param name="h">スプライトの縦幅</param>
/// <param name="pivot">ピボット（基点）</param>
void CSpriteRender::Init(const char* texFilePath, const float w, const float h, const Vector2& pivot,
	AlphaBlendMode alphaBlendMode)
{
	//スプライトの初期化用データ
	SpriteInitData initData;
	initData.m_ddsFilePath[0] = texFilePath;
	initData.m_width = w;
	initData.m_height = h;
	initData.m_fxFilePath = SHADER_FILEPATH_DEFAULT;
	initData.m_psEntryPoinFunc = ENTRY_POINT_FUNC_PS_DEFAULT;
	initData.m_vsEntryPointFunc = ENTRY_POINT_FUNC_VS_DEFAULT;
	initData.m_alphaBlendMode = alphaBlendMode;

	m_pivot = pivot;

	//スプライトの初期化
	m_sprite.Init(initData);

	return;
}

//アップデート関数
void CSpriteRender::Update()
{
	//スプライトのアップデート
	m_sprite.Update(m_position, m_rotation, m_scale, m_pivot);

	return;
}

//描画用関数
void CSpriteRender::Render(RenderContext& rc)
{
	//PostRenderFlagがtrueなら何もせずにreturn
	if (m_postRenderFlag)
		return;

	//スプライトの描画用関数
	m_sprite.Draw(rc);

	return;
}

//一番上に描画する関数
void CSpriteRender::PostRender(RenderContext& rc)
{
	//PostRenderFlagがfalseなら何もせずにreturn
	if (!m_postRenderFlag)
		return;

	//スプライトの描画用関数
	m_sprite.Draw(rc);

	return;
}