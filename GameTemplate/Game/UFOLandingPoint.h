#pragma once
#include "ObstacleObject.h"

/// <summary>
/// UFOの着地点クラス
/// </summary>
class CUFOLandingPoint : public CObstacleObject
{
public:		//自動で呼ばれるメンバ関数
	bool StartSub() override final;	//スタート関数
	~CUFOLandingPoint();			//デストラクタ

public:		//メンバ関数

	/// <summary>
	/// 端のOBBの参照を得る
	/// </summary>
	/// <param name="leftOrRight">左端か右端か？</param>
	/// <returns></returns>
	COBB& GetSideOBB(const int leftOrRight)
	{
		return m_sideOBB[leftOrRight];
	}

	/// <summary>
	/// 端のOBBのアップデート
	/// </summary>
	void UpdateSideOBB();

private:
	//左か右か
	enum LeftOrRight
	{
		enLeft,				//左
		enRight,			//右
		enLeftAndRightNum,	//左と右の数
	};

private:	//データメンバ
	COBB m_sideOBB[enLeftAndRightNum];		//端のOBB


private:	//デバック用
#ifdef MY_DEBUG
	//後で消す
	static const int m_vertNum = 8;		//頂点の数
	CModelRender* m_dbgVertPosMR[m_vertNum] = { nullptr };	//頂点を見るためのモデル
	CModelRender* m_dbgSidePosMR[enLeftAndRightNum][m_vertNum];	//端のOBBを見るためのモデル
#endif
};

