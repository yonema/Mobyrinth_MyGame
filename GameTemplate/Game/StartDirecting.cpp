#include "stdafx.h"
#include "StartDirecting.h"
#include "GameCamera.h"

//開始演出の定数データを使えるようにする
using namespace startDirectingConstData;

/**
 * @brief 開始演出の更新。使うときにアップデート関数で呼ぶ。
*/
void CStartDirecting::StartDirectingUpdate()
{
	//ステージの周りをぐるぐる回るか？
	if (m_goAroundStageFlag)
	{
		GoAroundStage();
	}

	//フェードを行うか？
	if (m_fadeFlag)
	{
		FadeDirectingCamera();
	}

	//プレイヤーが落ちてくる演出を行うか？
	if (m_fallPlayerFlag)
	{
		FallPlayer();
	}

	return;
}


/**
 * @brief ステージ開始時のカメラ
*/
void CStartDirecting::GoAroundStage()
{
	//ワイプが終わっていないか？
	if (!m_wipeEndFlag)
	{
		//終わっていない
		//何もせずreturn
		return;
	}

	//ちょっと待ってから演出を開始する
	if (m_firstWaitTimer < TIME_FIRST_WAIT_START_DIRECTING)
	{
		//タイマーを進める
		m_firstWaitTimer += GameTime().GetFrameDeltaTime();
		return;
	}

	//ステージの周りを回るクォータニオン
	Quaternion goAroundStageQRot;
	//ステージの周りを回る角度を増やす	//デルタタイムを掛ける
	m_goAroundStageAngle += ANGLE_MAX_GO_AROUND_STAGE / TIME_GO_AROUND_STAGE * 
		CGameTime::GetInstance().GetFrameDeltaTime();
	//クォータニオンに角度をセットする
	goAroundStageQRot.SetRotationDegY(m_goAroundStageAngle);
	//カメラの視点への座標
	Vector3 toCameraPos = POSITION_TO_CAMERA;
	//カメラの視点への座標を回す
	goAroundStageQRot.Apply(toCameraPos);
	//ステージ全体が見渡せるように視点と注視点を設定
	m_gameCamera->SetPosition(POSITION_TARGET_CAMERA + toCameraPos);
	m_gameCamera->SetTarget(POSITION_TARGET_CAMERA);

	//ステージの周りを回る角度が一定の角度に達する、または
	//Aボタンを押す
	if (m_goAroundStageAngle >= ANGLE_MAX_GO_AROUND_STAGE * SCALE_ANGLE_FINISH_GO_AROUND_STAGE ||
		g_pad[0]->IsTrigger(enButtonA) == true)
	{
		//フェードのフラグを立てる
		m_fadeFlag = true;
	}

	return;
}

/**
 * @brief フェードの処理
*/
void CStartDirecting::FadeDirectingCamera()
{
	//フェードアウトの時間か？
	if (m_fadeTimer < TIME_FADE_OUT)
	{
		//アルファ値
		float alphaValue = 1.0f;
		//タイマーに経過具合によってアルファ値を補完
		float timeScale = (m_fadeTimer) / (TIME_FADE_OUT);
		alphaValue *= timeScale;
		//フェードを徐々に暗くしていく
		g_graphicsEngine->GetHUD().SetFadeSpriteAlphaValue(alphaValue);
	}
	//フェードアウトとフェードインの中間の時間か？
	else if (m_fadeTimer < TIME_FADE_WAIT)
	{
		//カメラがステージの周りを回転しないようにする。
		m_goAroundStageFlag = false;

		//フェードは真っ暗
		g_graphicsEngine->GetHUD().
			SetFadeSpriteAlphaValue(spriteRenderConstData::ALPHA_VALUE_OPACITY);
	}
	//フェードインの時間か？
	else if (m_fadeTimer < TIME_FADE_IN)
	{
		//プレイヤーが落ちてくる演出を始める
		m_fallPlayerFlag = true;

		//アルファ値
		float alphaValue = 1.0f;
		//タイマーに経過具合によって補完
		float timeScale = (m_fadeTimer - TIME_FADE_WAIT) / (TIME_FADE_IN - TIME_FADE_WAIT);
		alphaValue -= 1.0f * timeScale;
		//フェードを徐々に明るくしていく
		g_graphicsEngine->GetHUD().SetFadeSpriteAlphaValue(alphaValue);
	}
	else
	{
		//全ての工程が終わったら
		//フェードを完全に透明にして
		g_graphicsEngine->GetHUD().
			SetFadeSpriteAlphaValue(spriteRenderConstData::ALPHA_VALUE_TRANSPARENT);
		//フェードを行わないようにする
		m_fadeFlag = false;
	}

	//フェードのタイマーを進める
	m_fadeTimer += GameTime().GetFrameDeltaTime();

	return;
}

/**
 * @brief プレイヤーの演出時の落下処理
*/
void CStartDirecting::FallPlayer()
{
	//プレイヤーがみつかっていないか？
	if (m_player == nullptr)
	{
		//見つかっていない
		//プレイヤーを探す
		m_player = FindGO<Player>(GetGameObjectName(EN_GO_TYPE_PLAYER));
		//まだ見つかっていなかったら
		if (m_player == nullptr)
		{
			//何もせずにreturn
			return;
		}
	}

	//プレイヤーを落ちている状態にする
	m_player->SetFallFlag(true);

	//落下中のプレイヤーの座標
	Vector3 fallpos = m_player->GetPosition();
	//落下させる	//デルタタイムを掛ける
	fallpos.y -= SPEED_FALL_PLAYER * GameTime().GetFrameDeltaTime();
	//高さが地面より低いか？
	if (fallpos.y < HEIGHT_GROUND) 
	{
		//高さを地面に合わせる
		fallpos.y = HEIGHT_GROUND;

		//落下後に待つ時間か？
		if (m_waitTimerAfterFell < TIME_WAIT_AFTER_FELL) 
		{
			//落下後に少し待機する
			m_waitTimerAfterFell += GameTime().GetFrameDeltaTime();
		}
		//少し待った後
		else 
		{
			//開始演出を終える
			m_startDirectingFlag = false;

			//プレイヤーを落ちている状態ではなくする
			m_player->SetFallFlag(false);
			//プレイヤーの操作フラグをtrueにする。
			m_player->SetOperationFlag(true);

			//ポーズの操作フラグをtrueにする。
			CPause* pause = FindGO<CPause>(GetGameObjectName(EN_GO_TYPE_PAUSE));
			pause->SetCanPause(true);
			//UFOがあったら動かす
			CUFO* ufo = FindGO<CUFO>(GetGameObjectName(EN_GO_TYPE_UFO));
			if (ufo)
				ufo->SetMoveSpeed();
		}
	}

	//プレイヤーに落下中の座標を設定する
	m_player->SetPosition(fallpos);
	//カメラをリフレッシュして、一時的にばねカメラを無効にし、
	//一気にカメラを移動できるようにする
	m_gameCamera->Refresh();
	//カメラの視点を設定する
	m_gameCamera->SetPosition(m_player->GetPosition() + gameCameraConstData::POSITION_TO_CAMERA);
	//カメラの注視点を設定する
	m_gameCamera->SetTarget(m_player->GetPosition());

	return;
}
