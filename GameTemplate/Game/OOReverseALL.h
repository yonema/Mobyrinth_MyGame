#pragma once
#include "ObstacleObject.h"
#include "ReversibleObject.h"
#include "FontRender.h"

/// <summary>
/// 障害オブジェクト
/// 全反転オブジェクト
/// </summary>
class OOReverseALL : public CObstacleObject
{
public:		//自動で呼ばれるメンバ関数
	bool StartSub() override final;		//スタート関数
	~OOReverseALL();					//デストラクタ
	void UpdateSub() override final;	//アップデート関数

private:	//privateなメンバ関数

	void BeforeHitPlayer();	//プレイヤーと衝突前の処理

	void HitPlayer();		//プレイヤーと衝突時の処理

	void AfterHitPlayer();	//プレイヤーと衝突後の処理


private:	//列挙型

	//アップデートステート
	enum EnUpdateState
	{
		enBeforHitPlayer,	//プレイヤーと衝突前の状態
		enHitPlayer,		//プレイヤーと衝突時の状態
		enAfterHitPlayer,	//プレイヤーと衝突後の状態
	};

private:	//データメンバ

	int m_updateState = enBeforHitPlayer;			//アップデートステート
	Vector3 m_playerBeforePosition = g_vec3Zero;	//衝突前のプレイヤーの座標
	Vector3 m_playerHitPosition = g_vec3Zero;		//衝突時のプレイヤーの座標
	Vector3 m_playerAfterPosition = g_vec3Zero;		//衝突後のプレイヤーの座標
	CSoundCue* m_changeSE = nullptr; //m_changeSEのサウンドキュー

private:	//デバック用

#ifdef MY_DEBUG
//後で消す
	static const int m_vertNum = 8;		//頂点の数
	CModelRender* m_dbgVertPosMR[m_vertNum] = { nullptr };	//頂点を見るためのモデル
#endif
};

