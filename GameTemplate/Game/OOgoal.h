#pragma once
#include "ObstacleObject.h"
#include "SpriteRender.h"
#include "effect/Effect.h"

/// <summary>
/// 障害オブジェクト
/// ゴールオブジェクト
/// </summary>
class OOgoal : public CObstacleObject
{
public:		//自動で呼ばれるメンバ関数
	bool StartSub()override final;	//スタート関数
	~OOgoal();						//デストラクタ
	void UpdateSub()override final;	//アップデート関数

public:		//メンバ関数

	/// <summary>
	/// ゴールしているかどうかを得る
	/// </summary>
	/// <returns>ゴールしているか？</returns>
	const bool GetIsGoal()const
	{
		return m_isGoal;
	}

private:	//データメンバ
	//CSpriteRender* m_spriteRender = nullptr;	//ゴールした時のスプライト
	bool m_isGoal = false;						//ゴール判定
};

