#pragma once
#include "SpriteRender.h"
#include "Player.h"
#include "StartDirecting.h"
#include "SpringCamera.h"

/// <summary>
/// ゲーム内カメラクラス
/// </summary>
class GameCamera : public IGameObject
{
public:		//自動で呼ばれるメンバ関数
	bool Start()override final;		//スタート関数
	void Update()override final;	//アップデート関数

private:	//メンバ関数
	/// <summary>
	/// ステージ開始時のカメラ
	/// </summary>
	void StartDirectingCamera();
	/// <summary>
	/// ステージ開始時のカメラのフェード
	/// </summary>
	void FadeDirectingCamera();
	/// <summary>
	/// ゲーム中のカメラ
	/// </summary>
	void InGameCamera();

public:	//インライン関数
	void SetStartDirectingZoomInCamera(const bool b)
	{
		m_startDirectingZoomInCamera = b;
	}

	/// <summary>
	/// ズームが終了した
	/// </summary>
	void FinishZoom()
	{
		m_zoomFinished = true;
	}

	/// <summary>
	/// カメラの注視点を設定する
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const Vector3& target)
	{
		g_camera3D->SetTarget(target);
	}

	/// <summary>
	/// カメラの視点を設定する
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3& pos)
	{
		g_camera3D->SetPosition(pos);
	}

	/// <summary>
	/// カメラの視点を取得する
	/// </summary>
	/// <returns>カメラの視点</returns>
	const Vector3 GetPosition() const
	{
		return g_camera3D->GetPosition();
	}

	/// <summary>
	/// タイトルモードにする
	/// </summary>
	void TitleMode()
	{
		m_titleMode = true;
	}

	/// <summary>
	/// プレイヤーを見るか？を設定する
	/// </summary>
	/// <param name="lookPlayerFlag">プレイヤーを見るか？</param>
	void SetLookPlayerFlag(const bool lookPlayerFlag)
	{
		m_lookPlayerFlag = lookPlayerFlag;
	}

private:	//データメンバ
	Player* m_pPlayer = nullptr;				//プレイヤーのポインタ
	Vector3 m_toCameraPos;						//注視点から視点へのベクトル
	bool m_lookPlayerFlag = true;				//プレイヤーを見るか？
	StartDirecting* m_startDirecting = nullptr;
	bool m_startDirectingZoomInCamera = false;	//カメラを寄せる処理のフラグ

	bool m_zoomFinished = false;				//Zoomが終わったか

	bool m_titleMode = false;					//タイトル画面か？



	float m_switchingTimer = 0.0f;				//スイッチの切り替え中のタイマー
	CSpriteRender* m_fadeSR = nullptr;			//フェードイン、アウトで使うスプライトレンダラー
	float m_test = 0.0f;
	bool m_flagRotationCamera = true;			//カメラがステージの周りを回転しているかのフラグ
	bool m_wipeEndFlag = false;					//ワイプが終わった？
	float m_startTimer = 0;						//演出開始までのタイマー
	CSpringCamera m_springCamera;				//ばねカメラ


public:
	/// <summary>
	/// ワイプが終わったかを設定
	/// </summary>
	/// <param name="wipeEndFlag">ワイプが終わった？</param>
	void SetWipeEndFlag(const bool wipeEndFlag)
	{
		m_wipeEndFlag = wipeEndFlag;
	}
};

