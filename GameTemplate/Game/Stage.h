#pragma once
#include "StageBase.h"

/// <summary>
/// ステージクラス
/// ステージを生成するときに使うクラス
/// </summary>
class CStage : public IStageBase
{
public:		//自動で呼ばれるメンバ関数
	bool StartSub() override final;	//スタート関数
	~CStage();						//デストラクタ

	/// <summary>
	/// リトライした時の処理
	/// </summary>
	void RetryStage()override final
	{
		//ステージを生成し直す
		CStage* stage = NewGO<CStage>(0);
		//スイッチのタイムを設定する
		stage->SetSwitchTime(GetSwitchTime());
		//同じレベルのファイルパスで初期化する
		stage->Init(GetLevelFilePath(), GetHighestClearStageNum());
	}


public:		//メンバ関数

	/// <summary>
	/// 初期化関数、NewGOした後で必ずよんでね。
	/// </summary>
	/// <param name="levelFilePath">レベルのファイルパス</param>
	void Init(const char* levelFilePath, const int stageNum);

	/// <summary>
	/// レベルのファイルパスを得る
	/// </summary>
	/// <returns>レベルのファイルパス</returns>
	const char* GetLevelFilePath() const
	{
		return m_levelFilePath;
	}

private:	//データメンバ
	char m_levelFilePath[256];		//レベルのファイルパス
};
