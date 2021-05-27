/*!
 * @brief	エフェクト
 */

#include "stdafx.h"
#include "Effect.h"


 //コンストラクタ
Effect::Effect()
{
}
//デストラクタ
Effect::~Effect()
{
	
}

//スタート関数
bool Effect::Start()
{
	return true;
}

//アップデート関数
void Effect::UpdateWhenPaused()
{
	EffectEngine::GetInstance()->UpdateEffectWorldMatrix(
		m_handle,
		m_position,
		m_rotation,
		m_scale
	);
}
		
/// <summary>
/// 初期化。
/// </summary>
/// <remark>
/// エフェクト再生のための準備を行います。
/// </remark>
/// <param name="filePath">ファイルパス</param>
void Effect::Init(const char16_t* filePath)
{
	m_effect = EffectEngine::GetInstance()->LoadEffect(filePath);
	
}

/// <summary>
/// 再生開始
/// </summary>
/// <remark>
/// この関数を呼び出すとエフェクトの再生が開始されます。
/// </remark>
void Effect::Play()
{
	//再生中のエフェクトを停止する。
	EffectEngine::GetInstance()->Stop(m_handle);
	//新しく再生。
	m_handle = EffectEngine::GetInstance()->Play(m_effect);
}

