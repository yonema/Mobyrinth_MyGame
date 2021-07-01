#pragma once

/**
 * @brief 開始演出の定数データ
*/
namespace startDirectingConstData
{
	/*
	* ステージの周りを回る演出関連の定数
	*/

	//開始演出の最初の少し待つタイム
	const float TIME_FIRST_WAIT_START_DIRECTING = 0.5f;
	//ステージの周りを回る最大の角度
	const float ANGLE_MAX_GO_AROUND_STAGE = 360.0f;
	//ステージの周りを回るタイム
	const float TIME_GO_AROUND_STAGE = 10.0f;
	//開始演出時のカメラの視点への座標
	const Vector3 POSITION_TO_CAMERA = { 0.0f,0.0f,4000.0f };
	//カメラの注視点の座標
	const Vector3 POSITION_TARGET_CAMERA = { 0.0f,0.0f,0.0f };
	//ステージの周りを回り終える角度の割合
	const float SCALE_ANGLE_FINISH_GO_AROUND_STAGE = 0.9f;



	/*
	* フェード関連の定数
	*/

	//フェードアウトのタイム
	const float TIME_FADE_OUT = 0.5f;
	//フェードアウトとフェードインの中間のタイム
	const float TIME_FADE_WAIT = TIME_FADE_OUT + 0.5f;
	//フェードインのタイム
	const float TIME_FADE_IN = TIME_FADE_WAIT + 0.5f;


	/*
	* プレイヤーの落下演出関連の定数
	*/

	//プレイヤーの落下速度
	const float SPEED_FALL_PLAYER = 1200.0f;
	//地面の高さ
	const float HEIGHT_GROUND = 1630.0f;
	//落下後に待つタイム
	const float TIME_WAIT_AFTER_FELL = 1.0f;
	//プレイヤーからカメラの座標
	const Vector3 POSITION_PLAYER_TO_CAMERA = { 0.0f,0.0f,1200.0f };

}