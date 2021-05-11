#include "stdafx.h"
#include "MapChip2D.h"

/// <summary>
/// 初期化。
/// </summary>
/// <param name="caslData">Caslデータ。</param>
void MapChip2D::Init(CaslData* caslData)
{
	//スプライトレンダラーの生成
	m_spriteRender = NewGO<CSpriteRender>(1);
	//スプライトレンダラーの初期化
	m_spriteRender->Init(caslData->ddsFilePath.get(), caslData->width, caslData->height);

	//座標をスプライトレンダラーに設定
	Vector3 position = { 0.0f,0.0f,0.0f };
	position.x = caslData->position.x;
	position.y = caslData->position.y;
	m_spriteRender->SetPosition(position);

	//拡大をスプライトレンダラーに設定
	Vector3 scale = { 1.0f,1.0f,1.0f };
	scale.x = caslData->scale.x;
	scale.y = caslData->scale.y;
	m_spriteRender->SetScale(scale);

}


MapChip2D::~MapChip2D()
{
	if (m_spriteRender)
		DeleteGO(m_spriteRender);
}