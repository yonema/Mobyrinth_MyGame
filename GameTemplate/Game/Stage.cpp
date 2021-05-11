#include "stdafx.h"
#include "Stage.h"

/// <summary>
/// 初期化関数、NewGOした後で必ずよんでね。
/// </summary>
/// <param name="levelFilePath">レベルのファイルパス</param>
void CStage::Init(const char* levelFilePath)
{
	sprintf(m_levelFilePath, levelFilePath);
}

//スタート関数
bool CStage::StartSub()
{
	//レベルのロード
	LoadLevel(m_levelFilePath);

	return true;
}

//デストラクタ
CStage::~CStage()
{

}
