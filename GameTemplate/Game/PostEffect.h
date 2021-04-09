#pragma once
#include "Bloom.h"

/// <summary>
/// ポストエフェクトクラス
/// </summary>
class CPostEffect
{
public:		//自動で呼ばれるメンバ関数
	CPostEffect();		//コンストラクタ
	~CPostEffect();		//デストラクタ

public:		//メンバ関数
	void Init();	//初期化関数
	void Draw(RenderContext& renderContext);	//描画する関数

private:	//データメンバ
	CBloom m_bloom;		//ブルームクラス
};

