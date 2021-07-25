#pragma once
#include "ObstacleObject.h"
#include "SoundCue.h"
class ROrunning_stop;

/**
 * @brief 「壁」の定数データ
*/
namespace wallConstData
{
	//ステージからどれくらい上の位置にいるか
	constexpr float LENGHT_POSITION_FROM_STAGE = 50.0f;

	//壁が移動する片道分の時間
	constexpr float TIME_MOVE = 3.0f;
	//壁が移動する片道分の長さ
	constexpr float LENGTH_MOVE = 200.0f;

	//壁が動くときのサウンドのファイルパス
	constexpr const wchar_t* const SOUND_FILEPATH_WALL_MOVE = L"Assets/sound/wallmove.wav";
	//壁が動くときのサウンドの音量
	constexpr float SOUND_VALUME_WALL_MOVE = 1.0f;

	//壁が動くときのサウンドが聞こえる最大距離
	constexpr float DISTANCE_MAX_WALL_MOVE_SOUND = 1500.0f;
}

/// <summary>
/// 障害オブジェクト
/// 壁オブジェクト
/// </summary>
class OOwall :public CObstacleObject
{
public:		//自動で呼ばれるメンバ関数
	OOwall();						//コンストラクタ
	bool StartSub() override final;	//スタート関数
	~OOwall();						//デストラクタ
	void UpdateSub()override final;	//アップデート関数

public:		//メンバ関数

	/// <summary>
	/// 稼働モード
	/// </summary>
	void Running()
	{
		m_moveFlag = true;
	}

	/// <summary>
	/// 停止モード
	/// </summary>
	void Stop()
	{
		m_moveFlag = false;
	}

	/// <summary>
	/// 持っている
	/// 反転オブジェクトの「稼働、停止オブジェクト」を設定する
	/// </summary>
	/// <param name="run_stop">稼働、停止オブジェクト</param>
	void SetRun_stop(ROrunning_stop* run_stop)
	{
		m_pRun_stop = run_stop;
	}

	/// <summary>
	/// 持っている
	/// 反転オブジェクトの「稼働、停止オブジェクト」のポインタを得る
	/// </summary>
	/// <returns></returns>
	ROrunning_stop* GetRun_stop()
	{
		return m_pRun_stop;
	}

private:	//privateなメンバ関数

	/**
	 * @brief 移動前の初期位置と移動先の終端位置の初期化処理
	*/
	void InitStartAndEndPos();

	/**
	 * @brief 稼働、停止オブジェクトの初期化処理
	*/
	void InitRun_stopObject();

	/**
	 * @brief サウンドの初期化処理
	*/
	void InitSound();

	/// <summary>
	/// 一回目のアップデートでだけ呼ばれる関数
	/// </summary>
	void FirstUpdate();

	/**
	 * @brief 座標の更新処理
	*/
	void PositionUpdate();

	/**
	 * @brief サウンドの更新処理
	*/
	void SEUpdate();

private:	//データメンバ
	bool m_moveFlag = false;				//稼働状態か？
	float m_moveTimer = 0.0f;				//稼働中のカウンター
	Vector3 m_startPosition = g_VEC3_ZERO;	//移動前の初期位置
	Vector3 m_endPosition = g_VEC3_ZERO;	//移動先の終端位置
	ROrunning_stop* m_pRun_stop = nullptr;	//稼働、停止オブジェクトのポインタ
	bool m_firstUpdateFlag = true;			//一回目のアップデートか？
	CSoundCue* m_wallmoveSE = nullptr;		//壁が動くときのサウンド
};

