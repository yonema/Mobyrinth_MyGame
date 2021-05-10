#pragma once
#include "LevelObjectBase.h"
#include "ModelRender.h"
#include "SpotLight.h"
#include "UFOLandingPoint.h"
#include "ReversibleObject.h"

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

	/// <summary>
	/// UFOの移動速度を設定
	/// </summary>
	/// <param name="moveSpeed">移動速度</param>
	void SetMoveSpeed(const float moveSpeed)
	{
		m_moveSpeed = moveSpeed;
	}

	/// <summary>
	/// UFOの移動速度を既定の速度にする
	/// </summary>
	void SetMoveSpeed()
	{
		m_moveSpeed = m_defaultSpeed;
	}

private:	//privateなメンバ関数

	void Search();		//プレイヤーを探す処理
	void Capture();		//プレイヤーを見つけた時の処理
	void Transport();	//プレイヤーを運ぶ処理
	void Landing();		//着地の処理
	void Leave();		//着地点から離れる処理

	void Move();		//移動処理
	void GetOnStage();	//メビウスの輪のステージにイイ感じに合わせる処理
	void UpdateLight();	//ライトの更新

private:	//列挙型

	//アップデートのステート（状態）
	enum EnUpdateState
	{
		enSearch,		//プレイヤーを探す
		enCapture,		//プレイヤーを見つけた
		enTransport,	//プレイヤーを運ぶ
		enLanding,		//着地
		enLeave,		//着地点から離れる
	};

	//右か左
	enum EnLeftOrRight
	{
		enLeft,			//左
		enRight,		//右
	};

private:	//データメンバ
	CModelRender* m_modelRender = nullptr;	//モデルレンダラー
	Vector3 m_onWayPosition = g_vec3Zero;	//ウェイポイント上の座標
	Mobius* m_mobius = nullptr;				//ステージのメビウスの輪のポインタ
	Vector3 m_upVec = g_vec3Up;				//アップベクトル
	bool m_leftOrRight = enRight;			//右か左か？
	float m_defaultSpeed = 1300.0f;			//デフォルトの移動速度
	float m_moveSpeed = 0.0f;				//移動速度
	int m_updateState = enSearch;			//アップデートのステート（状態）
	int m_searchFlag = true;				//プレイヤーを探しているか？
	float m_timer = 0.0f;					//タイマー
	CSpotLight* m_spotLight = nullptr;		//スポットライト
	CModelRender* m_ufoLight = nullptr;		//UFOのライト
	CUFOLandingPoint* m_ufoLandingPoint = nullptr;	//着地点クラス
	bool m_getOnStageFlag = true;			//ステージの上に乗る処理を行うか？

	//デバック用
private:
#ifdef MY_DEBUG
	static const int m_vertNum = 8;				//頂点の数
	CModelRender* m_dbgVertPosMR[m_vertNum];	//OBBの頂点を見るためのモデル
	CModelRender* m_dbgRay[2] = { nullptr,nullptr };	//レイの始点と終点を見るためのモデル
#endif
};

