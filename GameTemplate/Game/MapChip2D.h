#pragma once
#include "CaslFile.h"
#include "SpriteRender.h"


/// <summary>
/// マップチップ2D。
/// </summary>
class MapChip2D
{
public:		//自動で呼ばれるメンバ関数
	~MapChip2D();

public:		//ここのメンバ関数を主に使う

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="caslData">Caslデータ。</param>
	void Init(CaslData* caslData);



private:	//データメンバ
	CSpriteRender* m_spriteRender = nullptr;	//スプライト。
};



