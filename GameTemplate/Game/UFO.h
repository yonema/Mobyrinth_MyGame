#pragma once
#include "LevelObjectBase.h"
#include "ModelRender.h"
#include "SpotLight.h"
#include "UFOLandingPoint.h"

/// <summary>
/// UFOクラス
/// </summary>
class CUFO : public ILevelObjectBase
{
public:		//自動で呼ばれるメンバ関数
	bool PureVirtualStart() override final;		//スタート関数
	~CUFO();									//デストラクタ
	void PureVirtualUpdate() override final;	//アップデート関数

public:		//メンバ関数

private:	//privateなメンバ関数

	void Search();		//プレイヤーを探す処理
	void Capture();		//プレイヤーを見つけた時の処理
	void Transport();	//プレイヤーを運ぶ処理
	void Landing();
	void Leave();

	void Move();		//移動処理
	void GetOnStage();	//メビウスの輪のステージにイイ感じに合わせる処理

private:	//列挙型

	//アップデートのステート（状態）
	enum EnUpdateState
	{
		enSearch,		//プレイヤーを探す
		enCapture,		//プレイヤーを見つけた
		enTransport,	//プレイヤーを運ぶ
		enLanding,		//着地
		enLeave,
	};

	enum EnLeftOrRight
	{
		enLeft,
		enRight,
	};
private:	//データメンバ
	CModelRender* m_modelRender = nullptr;	//モデルレンダラー
	Vector3 m_onWayPosition = g_vec3Zero;	//ウェイポイント上の座標
	Mobius* m_mobius = nullptr;				//ステージのメビウスの輪のポインタ
	Vector3 m_upVec = g_vec3Up;				//アップベクトル
	bool m_leftOrRight = enRight;
	float m_defaultSpeed = 1300.0f;			//デフォルトの移動速度
	float m_moveSpeed = m_defaultSpeed;		//移動速度
	int m_updateState = enSearch;			//アップデートのステート（状態）
	int m_searchFlag = true;				//プレイヤーを探しているか？
	float m_timer = 0.0f;					//タイマー
	CSpotLight* m_spotLight = nullptr;		//スポットライト
	CUFOLandingPoint* m_ufoLandingPoint = nullptr;	//着地点
	bool m_getOnStageFlag = true;

	//デバック用
private:
	static const int m_vertNum = 8;
	CModelRender* m_dbgVertPosMR[m_vertNum];	//OBBの頂点を見るためのモデル
	CModelRender* m_dbgRay[2] = { nullptr,nullptr };	//レイの始点と終点を見るためのモデル
};

