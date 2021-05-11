#pragma once
#include "ObstacleObject.h"

/// <summary>
/// 障害オブジェクト
/// 一方通行オブジェクト
/// </summary>
class OOoneway : public CObstacleObject
{
public:		//自動で呼ばれるメンバ関数
	bool StartSub() override final;		//スタート関数
	~OOoneway();						//デストラクタ
	void UpdateSub() override final;	//アップデート関数

public:		//メンバ関数
	
	/// <summary>
	/// 左か右か？を設定する。
	/// </summary>
	/// <param name="leftOrRight">左か右か？</param>
	void SetLeftOrRight(const int leftOrRight)
	{
		//左か右か？を新しく設定
		m_leftOrRight = leftOrRight;

		//両端のOBBを一度例外にしておく。
		for (int i = 0; i < enLeftAndRightNum; i++)
		{
			m_sideOBB[i].SetExceptionFlag(true);
		}
	}

public:		//publicな列挙型

	/// <summary>
	/// 左か右か？
	/// </summary>
	enum EnLeftOrRight
	{
		enLeft,				//左
		enRight,			//右
		enLeftAndRightNum,	//左と右の数
	};

private:	//データメンバ

	COBB m_sideOBB[enLeftAndRightNum];	//両サイドのOBB
	int m_leftOrRight = enLeft;			//左か右か？
	CModelRender* m_canPassMR = nullptr;	//通れる時のモデル

private:	//デバック用
#ifdef MY_DEBUG
	static const int m_vertNum = 8;
	CModelRender* m_dbgVertMR[enLeftAndRightNum][m_vertNum];
#endif
};

