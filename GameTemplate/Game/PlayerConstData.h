#pragma once

/**
 * @brief Playerの定数値
*/
namespace playerConstData
{
	/*
	* アニメーション関連の定数
	*/

	/**
	* @brief 通常のアニメーションクリップ。
	*/
	enum EN_ANIMATION_CLIP
	{
		EN_ANIM_CLIP_IDLE,		//アイドル状態のアニメーションクリップ
		EN_ANIM_CLIP_WALK,		//歩きのアニメーションクリップ
		EN_ANIM_CLIP_RUN,		//走りのアニメーションクリップ
		EN_ANIM_CLIP_CARRY,		//持ち上げるアニメーションクリップ
		EN_ANIM_CLIP_CARRY_IDEL,//持っているアイドル状態のアニメーションクリップ
		EN_ANIM_CLIP_CARRY_WALK,//持ちながら歩くアニメーションクリップ
		EN_ANIM_CLIP_CARRY_RUN,	//持ちながら走るアニメーションクリップ
		EN_ANIM_CLIP_THROW_L,	//左向きの投げるアニメーションクリップ
		EN_ANIM_CLIP_THROW_R,	//右向きの投げるアニメーションクリップ
		EN_ANIM_CLIP_FALL,		//落ちるアニメーションクリップ
		EN_ANIM_CLIP_NUM,		//アニメーションクリップの総数
	};

	/**
	 * @brief ゴール時のアニメーションクリップ
	*/
	enum EN_GOAL_ANIMATION_CLIP
	{
		EN_GOAL_ANIM_CLIP_IDLE,	//アイドル状態のアニメーションクリップ
		EN_GOAL_ANIM_CLIP_GOAL,	//ゴール時のアニメーションクリップ
		EN_GOAL_ANIM_CLIP_NUM,	//ゴール時のアニメーションクリップの数
	};


	/**
	 * @brief 通常のアニメーションのファイルパスとループフラグを取得
	 * @param [in] clipNo アニメションクリップのナンバー
	 * @param [out] filepath アニメーションのファイルパス
	 * @param [out] loopFlag ループフラグ
	*/
	constexpr void GetAnimationFilePath(const int clipNo, const char** filepath, bool* loopFlag)
	{
		//アニメーションクリップの番号に対応するファイルパスとループフラグを設定する
		switch (clipNo)
		{
		case EN_GOAL_ANIM_CLIP_IDLE:
			*filepath = "Assets/animData/idle.tka";
			*loopFlag = true;
			break;
		case EN_ANIM_CLIP_WALK:
			*filepath = "Assets/animData/walk.tka";
			*loopFlag = true;
			break;
		case EN_ANIM_CLIP_RUN:
			*filepath = "Assets/animData/run.tka";
			*loopFlag = true;
			break;
		case EN_ANIM_CLIP_CARRY:
			*filepath = "Assets/animData/carry.tka";
			*loopFlag = false;
			break;
		case EN_ANIM_CLIP_CARRY_IDEL:
			*filepath = "Assets/animData/carryidle.tka";
			*loopFlag = true;
			break;
		case EN_ANIM_CLIP_CARRY_WALK:
			*filepath = "Assets/animData/carrywalk.tka";
			*loopFlag = true;
			break;
		case EN_ANIM_CLIP_CARRY_RUN:
			*filepath = "Assets/animData/carryrun.tka";
			*loopFlag = true;
			break;
		case EN_ANIM_CLIP_THROW_L:
			*filepath = "Assets/animData/throwL.tka";
			*loopFlag = false;
			break;
		case EN_ANIM_CLIP_THROW_R:
			*filepath = "Assets/animData/throwR.tka";
			*loopFlag = false;
			break;
		case EN_ANIM_CLIP_FALL:
			*filepath = "Assets/animData/fall.tka";
			*loopFlag = false;
			break;
		default:
			//どれでもなかったらエラー
			MessageBoxA(
				nullptr, "アニメーションのファイルパスが登録されていません", "エラー", MB_OK
			);
			break;
		};
		return;
	}

	/**
	 * @brief 
	 * @param [in] clipNo ゴール時のアニメーションのファイルパスとループフラグを取得
	 * @param [out] filepath アニメーションのファイルパス
	 * @param [out] loopFlag アニメーションのループフラグ
	*/
	constexpr void GetGoalAnimationFilePath(const int clipNo, const char** filepath, bool* loopFlag)
	{
		//アニメーションクリップの番号に対応するファイルパスとループフラグを設定する
		switch (clipNo)
		{
		case EN_GOAL_ANIM_CLIP_IDLE:
			*filepath = "Assets/animData/goalidle.tka";
			*loopFlag = true;
			break;
		case EN_GOAL_ANIM_CLIP_GOAL:
			*filepath = "Assets/animData/goal.tka";
			*loopFlag = false;
			break;
		default:
			//どれでもなかったらエラー
			MessageBoxA(
				nullptr, "アニメーションのファイルパスが登録されていません", "エラー", MB_OK
			);
			break;
		}

		return;
	}


	

	/*
	* モデル関連の定数
	*/

	//プレイヤーの通常のモデルのファイルパス
	const char* const MODEL_FILEPATH_PLAYER = "Assets/modelData/player.tkm";

	//プレイヤーのゴール時のモデルのファイルパス
	const char* const MODEL_FILEPATH_PLAYER_GOAL = "Assets/modelData/player2.tkm";



	/*
	* 移動速度関連の定数
	*/

	//歩くときの移動速度
	const float MOVE_SPEED_WALK = 1000.0f;

	//走るときの移動速度
	const float MOVE_SPEED_RUN = 3000.0f;



	/*
	* エフェクト関連の定数
	*/

	//ゴール時のエフェクトのファイルパス
	const char16_t* const EFFECT_FILEPATH_GOAL = u"Assets/effect2/goal.efk";
	//ゴール時のエフェクトの拡大率
	const Vector3 EFFECT_SCALE_GOAL = { 150.0f,150.0f,150.0f };



	/*
	* サウンド関連の定数
	*/

	//落ちるときのSEのファイルパス
	const wchar_t* const SE_FILEPATH_FALL_START = L"Assets/sound/fallstart.wav";
	//落ちるときのSEのボリューム
	const float SE_VOLUME_FALL_START = 0.5f;

	//歩くときのSEのファイルパス
	const wchar_t* const SE_FILEPATH_WALK = L"Assets/sound/walk2.wav";
	//歩くときのSEのボリューム
	const float SE_VOLUME_WALK = 1.0f;

	//走るときのSEのファイルパス
	const wchar_t* const SE_FILEPATH_RUN = L"Assets/sound/run2.wav";
	//走るときのSEのボリューム
	const float SE_VOLUME_RUN = 1.0f;

	//炎に当たったときのSEのファイルパス
	const wchar_t* const SE_FILEPATH_FLAME_HIT = L"Assets/sound/flameHit.wav";
	//炎に当たったときのSEのボリューム
	const float SE_VOLUME_FLAME_HIT = 1.0f;


	/*
	* キャラコン関連の定数
	*/

	//プレイヤーのOBBのサイズ
	const Vector3 OBB_SIZE_PLAYER = { 100.0f ,200.0f, 200.0f };


	/*
	* メビウスの輪との当たり判定関連の定数
	*/

	//メビウスの輪の交差戦の長さ
	const float INTERSECT_LINE_FOR_MOBIUS_LEN = 300.0f;



	/*
	* ライト関連の定数
	*/

	//プレイヤーを照らすライトの方向
	const Vector3 LIGHT_DIRECTION = { 1.0f,-1.0f,0.0f };


	/*
	* 特定のOBBとの衝突関連の定数
	*/

	//壁の下のしきい値
	const float THRESHOLD_UNDER_WALL = 0.8F;



	/*
	* スタン関連の定数
	*/

	//スタン中の横方向の移動速度
	const float MOVE_SPEED_STUN_HORIZONTAL = 675.0f;

	//スタン中の上方向の移動速度
	const float MOVE_SPEED_STUN_UP = 1000.0f;

	//スタン中の下方向の加速度
	const float MOVE_ACCELERATION_STUN_DOWN = -1800.0f;
	
	//スタン中の時間
	const float TIME_STUN = 3.0f;

	//スタン中の点滅回数
	const int NUMBER_OF_BLINKS_STUN = 24;

	//スタン中の点滅のカラー
	const Vector4 BLINK_COLOR_STUN = { 1.5f,0.2f,0.2f,1.0f };

	/*
	* 開始演出関連の定数
	*/

	//ゲームの開始時の座標
	const Vector3 POSITION_GAME_START = { 0.0f, 3000.0f, 0.0f };

	/*
	* ゴール関連の定数
	*/

	//ゴール時にカメラが移動する時間
	const float TIME_GOAL_CAMERA_MOVE = 0.5f;

	//ゴール時のカメラの注視点に使う上向きのベクトルの長さ
	const float CAMERA_TRGET_UPVEC_LEN_GOAL = 200.0f;

	//ゴール時のカメラからプレイヤーへのベクトルの長さ
	const float CAMERA_TO_PLAYER_VEC_LEN_GOAL = 100.0f;



	/*
	* タイトル関連の定数
	*/

	//タイトル時のパッドの左スティックの入力値
	const float TITLE_PAD_STICK_VALUE = 0.5f;



	/*
	* 落下関連の定数
	*/

	//落下時の高さの最小値
	const float MIN_HEIGHT_FALL = 1800.0f;

}