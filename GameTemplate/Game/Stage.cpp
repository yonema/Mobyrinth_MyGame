#include "stdafx.h"
#include "Stage.h"

/// <summary>
/// 初期化関数、NewGOした後で必ずよんでね。
/// </summary>
/// <param name="levelFilePath">レベルのファイルパス</param>
void CStage::Init(const char* levelFilePath, const int stageNum)
{
	sprintf(m_levelFilePath, levelFilePath);
	SetStageNum(stageNum);
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
