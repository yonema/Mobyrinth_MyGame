#include "stdafx.h"
#include "Player.h"
#include "LightManager.h"
#include "ReversibleObject.h"
#include "GameCamera.h"

/**
 * @brief メビリンス
*/
namespace nsMobyrinth
{
	/**
	 * @brief プレイヤー
	*/
	namespace nsPlayer
	{

//プレイヤーの定数データを使えるようにする
using namespace playerConstData;
using namespace nsCommonData;
//OBBを使用可能にする
using namespace nsOBB;


//スタート関数
bool Player::Start()
{
	//アニメーションの初期化
	InitAnimation();

	//モデルの初期化
	InitModel();

	//キャラクターコントローラーの初期化
	InitMyCharacterController();

	//サウンドを初期化
	InitSound();


	//カメラを探す
	m_gameCamera = FindGO<nsCamera::CGameCamera>(GetGameObjectName(EN_GO_TYPE_GAME_CAMERA));

	//ウェイポイント上の座標にキャラの座標を入れておく
	m_onWayPosition = m_position;

	//ステージのメビウスの輪の参照を得る
	m_mobius = nsLevelObject::CLevelObjectManager::GetInstance()->GetMobius();


	return true;
}

//デストラクタ
Player::~Player()
{
	//プレイヤーのモデルレンダラーの破棄
	DeleteGO(m_modelRender);
	//ゴール時のプレイヤーのモデルレンダラーを破棄
	DeleteGO(m_goalPlayerMR);
	

	//落ちてくるときのSEを破棄
	DeleteGO(m_fallstartSE);
	//歩いているときのSEを破棄
	DeleteGO(m_walkSE);
	//走っているときのSEを破棄
	DeleteGO(m_runSE);
	//炎に当たったときのSEを破棄
	DeleteGO(m_flameHitSE);

	return;
}

//アップデート関数
void Player::Update()
{
	//タイトルか？
	if (m_titleMove == true) {
		//タイトル
		//タイトル中の動きをする
		TitleMove();
		Player* player = FindGO<Player>("");
	}
	else {
		//ゲーム中
		//ゲーム中の動きをする
		GameMove();
	}

	return;
}

/**
 * @brief タイトル中の動き
*/
void Player::TitleMove()
{
	//入力情報を調べる
	CheckInput();
	//タイトルでのパッドの入力は定数
	m_padLStickXF = TITLE_PAD_STICK_VALUE;
	//タイトルでの向きは右向き
	m_leftOrRight = EN_RIGHT;

	//タイトル画面では、歩きのアニメーションを再生
	m_modelRender->PlayAnimation(EN_ANIM_CLIP_WALK);

	//タイトル画面では走らない。
	m_isDush = false;

	//ウェイポイントの更新処理
	CheckWayPoint();
	//移動処理
	Move();
	//モデルの回転処理
	Rotation();


	//デルタタイムを掛ける
	//キャラコンを使ってウェイポイント上の座標を進める
	m_onWayPosition = 
		m_myCharaCon.Execute(m_moveSpeed, nsTimer::GameTime().GetFrameDeltaTime());

	//メビウスの輪の上に乗る処理
	GetOnStage();

	//ライトのデータを更新する
	UpdateLightData();

	//モデルの場所と回転を更新する
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);

	return;
}

/**
 * @brief ゲーム中の動き
*/
void Player::GameMove()
{
	//プレイヤーのフラグを調べる
	//特定のフラグが立っているか
	if (CheckFlag())
	{
		//立っていたら
		//これ以降の処理を行わない
		return;
	}	

	//入力情報を調べる
	CheckInput();

	//右向きか左向きかを調べる
	CheckLeftOrRight();

	//ウェイポイントの更新処理
	CheckWayPoint();
	//移動処理
	Move();
	//モデルの回転処理
	Rotation();

	//キャラコンを使ってウェイポイント上の座標を移動させる	//デルタタイムを掛ける
	m_onWayPosition = 
		m_myCharaCon.Execute(m_moveSpeed, nsTimer::GameTime().GetFrameDeltaTime());

	//衝突したOBBのタグを調べる
	CheckHitOBBTag();

	//ステージ（メビウスの輪）の上に乗る処理
	GetOnStage();

	//アニメーションの制御
	AnimationController();

	//SEの制御
	SoundController();

	//ライトのデータを更新する
	UpdateLightData();

	//モデルの場所と回転を設定
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
	//OBBの場所と回転を設定
	m_myCharaCon.SetRotation(m_rotation);

	return;
}


/**
 * @brief アニメーションの初期化
*/
void Player::InitAnimation()
{
	/*
	* 通常のモデルのアニメーションクリップの初期化
	*/
	for (int animClipNo = 0; animClipNo < EN_ANIM_CLIP_NUM; animClipNo++)
	{
		//アニメーションのファイルパス
		const char* filepath = nullptr;
		//ループフラグ
		bool loopFlag = true;
		//通常のアニメーションのファイルパスとループフラグを取得
		GetAnimationFilePath(animClipNo, &filepath, &loopFlag);
		//アニメーションクリップをロードする
		m_animationClips[animClipNo].Load(filepath);
		//アニメーションのループフラグを設定する
		m_animationClips[animClipNo].SetLoopFlag(loopFlag);
	}



	/*
	* ゴール時のモデルのアニメーションクリップの初期化
	*/
	for (int animClipNo = 0; animClipNo < EN_GOAL_ANIM_CLIP_NUM; animClipNo++)
	{
		//アニメーションのファイルパス
		const char* filepath = nullptr;
		//ループフラグ
		bool loopFlag = true;
		//通常のアニメーションのファイルパスとループフラグを取得
		GetGoalAnimationFilePath(animClipNo, &filepath, &loopFlag);
		//アニメーションクリップをロードする
		m_goalAnimationClips[animClipNo].Load(filepath);
		//アニメーションのループフラグを設定する
		m_goalAnimationClips[animClipNo].SetLoopFlag(loopFlag);
	}


	return;
}

/**
 * @brief モデルの初期化
*/
void Player::InitModel()
{
	//通常のモデルレンダラーを生成する
	m_modelRender = NewGO<nsGraphic::nsModel::CModelRender>(PRIORITY_FIRST);
	//通常のモデルレンダラーの初期化をする
	//この時にアニメーションクリップを一緒に引数に渡しておく
	m_modelRender->Init(
		MODEL_FILEPATH_PLAYER,			//ファイルパス
		D3D12_CULL_MODE_BACK,			//カリング設定
		m_animationClips,				//アニメーションクリップ
		EN_ANIM_CLIP_NUM,				//アニメーションクリップの数
		enModelUpAxisZ					//アップ軸
	);
	//シャドウキャスター有効
	m_modelRender->SetShadowCasterFlag(true);
	//シャドウレシーバーを無効
	m_modelRender->SetShadowReceiverFlag(false);
	m_modelRender->PlayAnimation(EN_ANIM_CLIP_IDLE);
	//モデルの場所と回転を設定
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);



	//ゴール用のモデルの生成
	m_goalPlayerMR = NewGO<nsGraphic::nsModel::CModelRender>(PRIORITY_FIRST);
	//ゴール用のモデルの初期化
	m_goalPlayerMR->Init
	(
		MODEL_FILEPATH_PLAYER_GOAL,		//ファイルパス
		D3D12_CULL_MODE_BACK,			//カリング設定
		m_goalAnimationClips,			//アニメーションクリップ
		EN_GOAL_ANIM_CLIP_NUM,			//アニメーションクリップの数
		enModelUpAxisZ					//アップ軸
	);
	//ゴール用のモデルを非表示にする
	m_goalPlayerMR->Deactivate();

	return;
}

/**
 * @brief キャラクターコントローラーの初期化
*/
void Player::InitMyCharacterController()
{
	//OBB初期化
	//OBBの初期化用データ構造体
	SInitOBBData initData;
	//なんかいい感じのOBBの大きさにする
	initData.width = OBB_SIZE_PLAYER.x;
	initData.height = OBB_SIZE_PLAYER.y;
	initData.length = OBB_SIZE_PLAYER.z;
	//Playerの場所と回転を入れる
	initData.position = m_position;
	initData.rotation = m_rotation;
	//ピボットをデフォルトのピボットにする
	initData.pivot = OBBConstData::OBB_PIVOT_DEFAULT;
	//キャラクターコントローラーを初期化する
	m_myCharaCon.Init(initData);

	return;
}


/**
 * @brief サウンドを初期化
*/
void Player::InitSound()
{
	//この関数限定で
	//サウンドを使用可能にする
	using namespace nsSound;

	//m_fallstartSEのサウンドキューを生成する
	m_fallstartSE = NewGO<CSoundCue>(PRIORITY_FIRST);
	//m_fallstartSEのサウンドキューを、waveファイルを指定して初期化する。
	m_fallstartSE->Init(SE_FILEPATH_FALL_START);
	//音量調節
	m_fallstartSE->SetVolume(SE_VOLUME_FALL_START);

	//m_walkSEのサウンドキューを生成する
	m_walkSE = NewGO<CSoundCue>(PRIORITY_FIRST);
	//m_walkSEのサウンドキューを、waveファイルを指定して初期化する。
	m_walkSE->Init(SE_FILEPATH_WALK);
	//音量調節
	m_walkSE->SetVolume(SE_VOLUME_WALK);

	//m_runSEのサウンドキューを生成する
	m_runSE = NewGO<CSoundCue>(PRIORITY_FIRST);
	//m_runSEのサウンドキューを、waveファイルを指定して初期化する。
	m_runSE->Init(SE_FILEPATH_RUN);
	//音量調節
	m_runSE->SetVolume(SE_VOLUME_RUN);

	//炎に当たったときのサウンドの生成と初期化
	m_flameHitSE = NewGO<CSoundCue>(PRIORITY_FIRST);
	m_flameHitSE->Init(SE_FILEPATH_FLAME_HIT);
	//音量調節
	m_flameHitSE->SetVolume(SE_VOLUME_FLAME_HIT);

	return;
}

/**
 * @brief フラグを調べる
 * @return フラグが立っているか？
 * @retval true 立っている
 * @retval false 立っていない
*/
bool Player::CheckFlag()
{
	//プレイヤーが落ちている状態か？
	if (m_fallFlag == true)
	{
		//落ちている
		//落ちている時の処理
		Fall();
		//これ以降の処理を行わない
		//フラグが立っていると戻す
		return true;
	}

	//操作できるか？
	if (m_operationFlag == false) {
		//できない
		//アニメーションの遷移をリセットする
		AnimationReset();
		//アイドル状態のアニメーションを再生する
		m_modelRender->PlayAnimation(EN_ANIM_CLIP_IDLE);
		//これ以降の処理を行わない
		//フラグが立っていると戻す
		return true;
	}

	//UFOに捕まっているか？
	if (m_capturedUFOFlag == true)
	{
		//捕まっている
		//捕まっている時の処理
		CapturedUFO();
		//これ以降の処理を行わない
		//フラグが立っていると戻す
		return true;
	}

	//ゴール状態か？
	if (m_isGoal)
	{
		//ゴール状態
		//ゴール状態の処理
		Goal();
		//これ以降の処理を行わない
		//フラグが立っていると戻す
		return true;
	}

	//フラグが立っていないと戻す
	return false;
}

/**
 * @brief 入力情報を調べる
*/
void Player::CheckInput()
{
	//ゲームパッドの左スティックのX軸の入力情報を取得
	m_padLStickXF = g_pad[PAD_ONE]->GetLStickXF();

	//投げ中または持ち上げ中は移動できないようにする
	if (m_throwing || m_lifting)
		m_padLStickXF = 0.0f;

	//ゲームパッドのR1ボタンが押されているか？かつ
	//ゲームパッドの左スティックのX軸の入力は0ではないか？
	if (g_pad[0]->IsPress(enButtonRB1) && m_padLStickXF != 0.0f)
	{
		//押されている、かつ、入力が0ではない。
		//ダッシュ状態
		m_isDush = true;
	}
	else
	{
		//押されていない、もしくは、入力が0
		//ダッシュ状態ではない
		m_isDush = false;
	}

	return;
}

/**
 * @brief 右向きか左向きかを調べる
*/
void Player::CheckLeftOrRight()
{
	//左右の向きを設定
	//左スティックの入力値が正なら
	if (m_padLStickXF < 0.0f)
	{
		//左向き
		m_leftOrRight = EN_LEFT;
	}
	//左スティックの入力値が負なら
	else if (m_padLStickXF > 0.0f)
	{
		//右向き
		m_leftOrRight = EN_RIGHT;
	}

	return;
}

/**
 * @brief ウェイポイントの更新処理
*/
void Player::CheckWayPoint()
{
	///
	///
	/// m_wayPointStateは左周りで0から増えていく。
	/// m_wayPointStateが0の時、m_lpIndexは0,m_rpIndexは1になる。
	/// つまり、m_lpIndexは常にm_wayPointStateと同じ値になり、
	/// m_rpIndexはm_wayPointStateに1加算した値になる。
	/// そしてm_maxWayPointStateはm_wayPointStateの最大数を表す。
	/// m_maxWayPointStateが31だったらm_wayPointStateは31まで存在する。
	
	//1.プレイヤー自身の左右のウェイポイントを設定する
	//m_wayPointStateをもとにウェイポイントを設定する。
	CalcLeftAndRightWayPoint();

	//更新前のウェイポイントステートを保持しておく
	int oldWayPointState = m_wayPointState;

	//2.m_wayPointStateの更新。

	//左のウェイポイントから右のウェイポイントへのベクトル
	Vector3 LpToRpVec = (*m_wayPointPos)[m_rpIndex] - (*m_wayPointPos)[m_lpIndex];
	//正規化しておく
	LpToRpVec.Normalize();
	//左のウェイポイントからプレイヤーへのベクトル
	Vector3 LpToPlayerVec = m_onWayPosition - (*m_wayPointPos)[m_lpIndex];
	//正規化しておく
	LpToPlayerVec.Normalize();
	//右のウェイポイントからプレイヤーへのベクトル
	Vector3 RpToPlayerVec = m_onWayPosition - (*m_wayPointPos)[m_rpIndex];
	//正規化しておく
	RpToPlayerVec.Normalize();

	//左側の内積
	float LpDotPlayer = Dot(LpToRpVec, LpToPlayerVec);
	//右側の内積
	float RpDotPlayer = Dot(LpToRpVec, RpToPlayerVec);


	//スタン中か？
	if (!m_stunFlag)
	{
		//スタン中ではない

		//内積で調べる
		//左右のウェイポイントとの距離を調べる
		if (LpDotPlayer > 0.0f && RpDotPlayer < 0.0f)
		{
			//今のウェイポイントの間にいる
		}
		else if (LpDotPlayer <= 0.0f && m_padLStickXF < 0.0f)
		{
			//今のウェイポイントの間から、左側に出ていった
			m_wayPointState += 1;
			if (m_wayPointState > m_maxWayPointState)
			{
				//m_wayPointStateがMAXより大きかったら
				//一周したということだから、スタートの0にする
				m_wayPointState = 0;
			}
		}
		else if (RpDotPlayer >= 0.0f && m_padLStickXF > 0.0f)
		{
			//今のウェイポイントの間から、右側から出ていった
			//m_wayPointStateを減算して右に進める。
			m_wayPointState -= 1;
			if (m_wayPointState < 0)
			{
				//m_wayPointStateが0より小さかったら
				//一周したということだから、MAXの値にする
				m_wayPointState = m_maxWayPointState;
			}
		}
	}
	else
	{
		//スタン中

		//内積で調べる
		//左右のウェイポイントとの距離を調べる
		if (LpDotPlayer > 0.0f && RpDotPlayer < 0.0f)
		{
			//今のウェイポイントの間にいる
		}
		else if (LpDotPlayer <= 0.0f && !m_stunLeftOrRight == EN_RIGHT)
		{
			//今のウェイポイントの間から、左側に出ていった
			m_wayPointState += 1;
			if (m_wayPointState > m_maxWayPointState)
			{
				//m_wayPointStateがMAXより大きかったら
				//一周したということだから、スタートの0にする
				m_wayPointState = 0;
			}
		}
		else if (RpDotPlayer >= 0.0f && !m_stunLeftOrRight == EN_LEFT)
		{
			//今のウェイポイントの間から、右側から出ていった
			//m_wayPointStateを減算して右に進める。
			m_wayPointState -= 1;
			if (m_wayPointState < 0)
			{
				//m_wayPointStateが0より小さかったら
				//一周したということだから、MAXの値にする
				m_wayPointState = m_maxWayPointState;
			}
		}
	}

	//更新前と更新後のウェイポイントステートは違っているか？
	if (m_wayPointState != oldWayPointState)
	{
		//違ってたら
		//もう一度左側と右側のウェイポイントを計算する
		CalcLeftAndRightWayPoint();
	}

	return;
}

/**
 * @brief ウェイポイントステートから
 * 左側のウェイポイントと右側のウェイポイントを計算する
*/
void Player::CalcLeftAndRightWayPoint()
{
	//1.プレイヤー自身の左右のウェイポイントを設定する
	//m_wayPointStateをもとにウェイポイントを設定する。
	m_rpIndex = m_wayPointState;	//右のウェイポイントはとm_wayPointStateは同じ値
	m_lpIndex = m_rpIndex + 1;		//左のウェイポイントは右のウェイポイントの1つ上の値

	if (m_lpIndex > m_maxWayPointState)
	{
		//左のウェイポイントがMAXより大きかったら
		//一周したということだから、スタートの0にする
		m_lpIndex = 0;
	}

	return;
}

/**
 * @brief 移動処理
*/
void Player::Move()
{
	//1.左右への移動する方向を計算する。

	//左へ移動するベクトル
	Vector3 moveToLeft = (*m_wayPointPos)[m_lpIndex] - m_onWayPosition;
	//正規化しておく
	moveToLeft.Normalize();
	//右へ移動するベクトル
	Vector3 moveToRight = (*m_wayPointPos)[m_rpIndex] - m_onWayPosition;
	//正規化しておく
	moveToRight.Normalize();


	//2.移動処理

	//スピードをリセットする
	//重力や、加速度、抵抗を実装するときは別のやり方で
	m_moveSpeed = g_VEC3_ZERO;


	//スタン中でないか？
	if (!m_stunFlag)
	{
		//スタン中ではない
		//通常の移動処理

		//移動する長さ
		//歩くスピードを入れる
		float moveLen = MOVE_SPEED_WALK;

		//ダッシュ状態か？
		if (m_isDush)
		{
			//ダッシュ状態
			//走るスピードを入れる
			moveLen = MOVE_SPEED_RUN;
		}


		//左右の入力情報を調べる
		if (m_padLStickXF < 0.0f)
		{
			//左への移動の入力があったら
			//左への移動の計算する
			m_moveSpeed += moveToLeft * -m_padLStickXF * moveLen;
		}
		else if (m_padLStickXF > 0.0f)
		{
			//右への移動の入力があったら
			//右への移動の計算をする
			m_moveSpeed += moveToRight * m_padLStickXF * moveLen;
		}
	}
	else
	{
		//スタン中

		//スタン中の移動処理
		StunMove();
	}

	return;
}

/**
 * @brief スタン中の移動処理
*/
void Player::StunMove()
{
	//スタン中タイマーが0.0fの時
	//つまりスタン中に、最初の一回ずつしか呼ばれない
	if (m_stunTimer == 0.0f)
	{
		//プレイヤーから衝突したOBBへのベクトル
		Vector3 playerToHitOBB = m_hitOBB->GetPosition() - m_onWayPosition;
		//右向きのベクトル
		Vector3 rightVec = g_VEC3_RIGHT;
		//右向きのベクトルを回して、プレイヤーの右向きのベクトルにする
		m_finalWPRot.Apply(rightVec);
		//OBBへのベクトルと、右向きのベクトルの内積
		float hitOBBDotRight = Dot(playerToHitOBB, rightVec);
		//左に飛ばされるか、右に飛ばされるか
		m_stunLeftOrRight = EN_LEFT;
		//内積が正か？
		if (hitOBBDotRight >= 0.0f)
		{
			//正だったら
			//反対側にする
			m_stunLeftOrRight = EN_RIGHT;
		}
		//最終的に吹っ飛ばされる先の座標を取ってくる
		m_stunMoveSpeedHorizontal = nsLevelObject::CLevelObjectManager::GetInstance()->CalcWayPointNextPos
		(m_rpIndex, m_onWayPosition, MOVE_SPEED_STUN_HORIZONTAL, m_stunLeftOrRight);
		//現在の座標から移動先のベクトルを出す
		m_stunMoveSpeedHorizontal -= m_onWayPosition;
		//スタン中に吹っ飛び中のフラグを立てる
		m_stunMoveFlag = true;
		//現在のUpベクトルをとってくる
		Vector3 upVec = m_upVec;
		//上方向に大きくする
		upVec.Scale(MOVE_SPEED_STUN_UP);
		//吹っ飛ぶときの上下に動くベクトルに上向きのベクトルを入れる
		m_stunMoveSpeedVertical = upVec;
	}

	//吹っ飛び中のフラグが立っているか？
	if (m_stunMoveFlag)
	{
		//吹っ飛び中のフラグが立っている

		//下に加えるベクトル、現在のUpベクトルを入れる
		Vector3 down = m_upVec;
		//Upベクトルを下向き大きくする
		down.Scale(MOVE_ACCELERATION_STUN_DOWN);
		//デルタタイムを掛ける
		down.Scale(nsTimer::GameTime().GetFrameDeltaTime());
		//吹っ飛び中の上下に動くベクトルに下向きのベクトルを加える
		m_stunMoveSpeedVertical += down;
	}
	else
	{
		//吹っ飛び中のフラグが折れている

		//吹っ飛ぶ力をゼロにする
		m_stunMoveSpeedHorizontal = g_VEC3_ZERO;
		m_stunMoveSpeedVertical = g_VEC3_ZERO;
	}

	//点滅処理

	//点滅の間隔
	//スタン状態の最大時間のうち一定回数切り替わるようにする
	const float blinkInterval = TIME_STUN / NUMBER_OF_BLINKS_STUN;
	//点滅タイマーが、点滅の間隔より小さかったら
	if (m_blinkTimer < blinkInterval)
	{
		//点滅のカラーに光らせる
		m_modelRender->SetModelEmissionColor(BLINK_COLOR_STUN);
	}
	//点滅タイマーが、点滅の間隔の2倍より小さかったら
	else if (m_blinkTimer < blinkInterval * 2)
	{
		//元の明るさに戻す
		m_modelRender->SetModelEmissionColor(g_VEC4_BLACK);
	}
	//点滅タイマーが、点滅の間隔の2倍を過ぎたら
	else
	{
		//点滅タイマーを0.0fに戻す
		m_blinkTimer = 0.0f;
	}

	//点滅タイマーを進める
	m_blinkTimer += nsTimer::GameTime().GetFrameDeltaTime();
	//スタン中のタイマーを進める
	m_stunTimer += nsTimer::GameTime().GetFrameDeltaTime();

	//スタン中のタイマーが、スタン中の最大時間を過ぎたら
	if (m_stunTimer >= TIME_STUN)
	{
		//スタン中ではなくする
		m_stunFlag = false;
		//解除していたOBBを登録しなおす
		COBBWorld::GetInstance()->AddOBB(*m_hitOBB);
		//衝突したOBBをnullptrにする
		m_hitOBB = nullptr;
		//スタン中タイマーをゼロにする
		m_stunTimer = 0.0f;
		//点滅タイマーをゼロにする
		m_blinkTimer = 0.0f;
		//明るさを元に戻す
		m_modelRender->SetModelEmissionColor(g_VEC4_BLACK);

	}

	//移動スピードにスタン中のスピードを加算する
	m_moveSpeed += m_stunMoveSpeedHorizontal + m_stunMoveSpeedVertical;

	return;
}

/**
 * @brief ステージに乗る
*/
void Player::GetOnStage()
{
	//スタン中か？
	if (m_stunFlag)
	{
		//スタン中
		//スタン中のステージの上に乗る処理
		StunGetOnStage();
		//これ以降の処理は行わない
		return;
	}

	//Upベクトルをイイ感じに伸ばす。
	//伸ばした先がレイの始点となる
	Vector3 intersectLine = m_upVec;
	intersectLine.Scale(INTERSECT_LINE_FOR_MOBIUS_LEN);

	//ステージのメビウスの輪が有効か？
	if (m_mobius)
	{
		//有効

		//メビウスの輪が見つかっていたら
		//メビウスの輪のモデルのポリゴンと、レイの当たり判定を取る
		m_mobius->GetIntersectPosWithMobius(
			m_onWayPosition + intersectLine, m_onWayPosition - intersectLine, &m_position
		);

	}
	else
	{
		//無効

		//メビウスの輪（ステージ）が見つかっていなかったら探す
		m_mobius = FindGO<nsLevelObject::nsMobius::CMobius>(GetGameObjectName(EN_GO_TYPE_MOBIUS));
	}

	return;
}

/**
 * @brief スタン中のステージに乗る処理
*/
void Player::StunGetOnStage()
{
	//現在のUpべクトル
	Vector3 upVec = m_upVec;
	//レイの長さ分大きくする
	upVec.Scale(INTERSECT_LINE_FOR_MOBIUS_LEN);

	//ポリゴンとレイの交差点
	Vector3 intersectPos = g_VEC3_ZERO;
	//メビウスの輪のモデルのポリゴンと、レイの当たり判定を取る
	if (m_mobius->
		GetIntersectPosWithMobius(m_onWayPosition + upVec, m_onWayPosition - upVec, &intersectPos))
	{

		//交差点からプレイヤーの道上の座標のベクトル
		Vector3 intersectToOnWay = m_onWayPosition - intersectPos;
		//正規化する
		intersectToOnWay.Normalize();
		//交差点からプレイヤーの道上のベクトルとUpベクトルとの内積
		float intersectToOnWayDotUp = Dot(intersectToOnWay, m_upVec);
		//内積が負なら
		if (intersectToOnWayDotUp <= 0.0f)
		{
			//メビウスの輪よりプレイヤーの座標が下に来てしまったら
			//プレイヤーの座標を交差点にする
			m_onWayPosition = intersectPos;
			//スタン中でももう吹っ飛ばないようにする
			m_stunMoveFlag = false;
		}
	}
	//プレイヤーの場所を更新する
	m_position = m_onWayPosition;

	return;
}

/**
 * @brief モデルの回転処理
*/
void Player::Rotation()
{
	//左のウェイポイントから右のウェイポイントへのベクトル
	Vector3 lpToRpLen = (*m_wayPointPos)[m_rpIndex] - (*m_wayPointPos)[m_lpIndex];

	//左のウェイポイントからプレイヤーへのベクトル
	Vector3 lpToPlayerLen = m_onWayPosition - (*m_wayPointPos)[m_lpIndex];

	//補完率
	const float ComplementRate = lpToPlayerLen.Length() / lpToRpLen.Length();

	//球面線形補完
	m_finalWPRot.Slerp(
		ComplementRate, (*m_wayPointRot)[m_lpIndex], (*m_wayPointRot)[m_rpIndex]
	);

	//キャラクターの左右の向きに合わせて回転
	if (m_leftOrRight == EN_LEFT)
	{
		//左向き
		m_rotation.SetRotationDegY(90.0f);
	}
	else
	{
		//右向き
		m_rotation.SetRotationDegY(-90.0f);
	}

	//補完済みの回転と掛け合わせる
	m_rotation.Multiply(m_finalWPRot);

	//プレイヤーのUpベクトルにYUpベクトルを入れる
	m_upVec = g_VEC3_AXIS_Y;
	//Yupを補完済みの回転で回す
	m_finalWPRot.Apply(m_upVec);
	//正規化する
	m_upVec.Normalize();

	return;
}

/**
 * @brief 衝突したOBBのタグを調べる
*/
void Player::CheckHitOBBTag()
{
	//キャラコンから、衝突したOBBのタグを調べる
	if (m_myCharaCon.GetTag() == COBB::EN_OO_TYPE_FLAME)
	{
		//炎衝突していたら

		//すでに衝突中の炎でないか？
		if (m_hitOBB != m_myCharaCon.GetHitOBB())
		{
			//新しく衝突した炎

			//炎の衝突した時の処理
			HitFlame();
		}

	}
	else if (m_myCharaCon.GetTag() == COBB::EN_OO_TYPE_WALL)
	{
		//壁と衝突していたら

		//壁と衝突した時の処理
		HitWall();
	}

	return;
}

/**
 * @brief 炎と衝突した時の処理
*/
void Player::HitFlame()
{
	//スタンフラグを立てる
	m_stunFlag = true;
	//スタンタイマーを初期化する
	m_stunTimer = 0.0f;

	//前に衝突したOBBがまだ残っていたら
	if (m_hitOBB)
	{
		//OBBをOBBワールドに登録しなおす
		COBBWorld::GetInstance()->AddOBB(*m_hitOBB);
	}

	//衝突したOBBを入れる
	m_hitOBB = m_myCharaCon.GetHitOBB();
	//衝突したOBBをOBBワールドから解除する
	COBBWorld::GetInstance()->RemoveOBB(*m_hitOBB);

	//炎と衝突したSEをワンショット再生で再生する
	m_flameHitSE->Play(false);

	return;
}

/**
 * @brief 壁と衝突した時の処理
*/
void Player::HitWall()
{
	//自身の座標から衝突したOBBの座標へのベクトル
	Vector3 toHitOBBVec = m_myCharaCon.GetHitOBB()->GetPosition() - m_position;
	//正規化する
	toHitOBBVec.Normalize();
	//自身のアップベクトル
	Vector3 up = m_upVec;
	//正規化しておく
	up.Normalize();
	//OBBの座標へのベクトルとアップベクトルとの内積を取る
	float toHitOBBVecDotUp = Dot(up, toHitOBBVec);
	//内積がしきい値以上なら
	if (toHitOBBVecDotUp >= THRESHOLD_UNDER_WALL)
	{
		//壁の下にいる状態

		//スタンフラグを立てる
		m_stunFlag = true;
		//前に衝突したOBBがまだ残っていたら
		if (m_hitOBB)
		{
			//OBBをOBBワールドに登録しなおす
			COBBWorld::GetInstance()->AddOBB(*m_hitOBB);
		}
		//衝突したOBBを入れる
		m_hitOBB = m_myCharaCon.GetHitOBB();
		//衝突したOBBをOBBワールドから解除する
		COBBWorld::GetInstance()->RemoveOBB(*m_hitOBB);
	}

	return;
}

/**
 * @brief ライトのデータを更新する
*/
void Player::UpdateLightData()
{
	//影を生成するライトの更新
	SetShadowParam();
	//ディレクションライトの更新
	SetDirectionLight();

	return;
}


/**
 * @brief プレイヤーを照らす影を生成するライトを更新する
*/
void Player::SetShadowParam()
{
	//ライトの照らす方向
	Vector3 dir = LIGHT_DIRECTION;
	//プレイヤー回転で方向をまわす
	m_finalWPRot.Apply(dir);
	//シャドウのつくるライトのパラメータを設定
	g_graphicsEngine->GetShadowMap().SetShadowParam(
		dir, nsGraphic::nsShadow::shadowConstData::SHADOW_INIT_LENGTH, m_position
	);

	return;
}

/**
 * @brief ディレクションライトを更新する
*/
void Player::SetDirectionLight()
{
	//ディレクションライトが見つかっていなかったら
	if (!m_gameDirectionLight)
	{
		//探す
		m_gameDirectionLight = 
			FindGO<nsLight::CDirectionLight>(GetGameObjectName(EN_GO_TYPE_GAME_DIRECTION_LIGHT));
		//まだ見つからなかったら何もせずにreturn
		if (!m_gameDirectionLight)
			return;
	}

	//ライトの照らす方向
	Vector3 dir = LIGHT_DIRECTION;
	//プレイヤー回転で方向をまわす
	m_finalWPRot.Apply(dir);
	//ディレクションライトの方向を設定する
	m_gameDirectionLight->SetDirection(dir);

	return;
}


/**
 * @brief UFOに捕まっている時の処理
*/
void Player::CapturedUFO()
{
	//歩きのSEが再生されていたら
	if (m_walkSE->IsPlaying()) 
	{
		//停止する
		m_walkSE->Stop();
	}
	//走りのSEが再生されていたら
	if (m_runSE->IsPlaying())
	{
		//停止する
		m_runSE->Stop();
	}

	//アニメーションの遷移をリセットする
	AnimationReset();
	//アイドル状態のアニメーションを再生する
	m_modelRender->PlayAnimation(EN_ANIM_CLIP_IDLE);

	//ウェイポイント上の座標とモデルの座標を同じにする
	m_onWayPosition = m_position;
	//キャラコンの座標とモデルの座標を同じにする
	m_myCharaCon.GetOBB().SetPosition(m_position);
	//モデルの場所と回転を設定
	m_modelRender->SetPosition(m_capturedPosition);
	m_modelRender->SetRotation(m_capturedRotation);
	//補完済みの回転をモデルの回転と同じにする
	m_finalWPRot = m_rotation;
	//キャラコンの回転とモデルの回転を同じにする
	m_myCharaCon.SetRotation(m_rotation);
	//ライトのデータを更新する
	UpdateLightData();

	return;
}

/**
 * @brief スタートの落ちるときの処理
*/
void Player::Fall()
{

	//落下中のカウンターが1未満のとき
	//つまり最初の一回だけ呼ばれる
	if (m_fallcount < 1)
	{
		//落下中のSEをワンショット再生で再生する
		m_fallstartSE->Play(false);

		//アニメーションの遷移をリセットする
		AnimationReset();
		//アイドル状態のアニメーションを再生する
		m_modelRender->PlayAnimation(EN_ANIM_CLIP_FALL);

		//落下中のカウンターを進める
		m_fallcount++;
	}
	
	//ウェイポイント上の座標
	Vector3 onWayPos = m_position;
	//高さが1800未満にならないようにする
	if (onWayPos.y < MIN_HEIGHT_FALL)
		onWayPos.y = MIN_HEIGHT_FALL;
	//ウェイポイント上の座標を設定する
	m_onWayPosition = onWayPos;
	//キャラコンの座標をウェイポイント上の座標と同じにする
	m_myCharaCon.SetPosition(onWayPos);
	//直角の回転を作る
	Quaternion rightAngleYQRot;
	rightAngleYQRot.SetRotationDegY(90.0f);
	//モデルの回転を直角にする
	m_rotation = rightAngleYQRot;
	//モデルの座標と回転を設定する
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
	//補完済みの回転を初期化する
	m_finalWPRot = g_QUAT_IDENTITY;


	return;
}


/**
 * @brief ゴールしている時の処理
*/
void Player::Goal()
{
	//ゴール用のモデルが非アクティブ状態だったら
	//つまり最初の一回だけ呼ばれる
	if (!m_goalPlayerMR->IsActive())
	{
		//歩きのSEが再生されていたら
		if (m_walkSE->IsPlaying())
		{
			//停止する
			m_walkSE->Stop();
		}
		//走りのSEが再生されていたら
		if (m_runSE->IsPlaying())
		{
			//停止する
			m_runSE->Stop();
		}
		//ゴール用のモデルをアクティブ状態
		m_goalPlayerMR->Activate();
		//通常のモデルを非アクティブ状態にする
		m_modelRender->Deactivate();
		//ゴール用のモデルの座標を更新する
		m_goalPlayerMR->SetPosition(m_position);
		//ゴール用のモデルの回転を更新する
		m_goalPlayerMR->SetRotation(m_finalWPRot);

		//ゴール時のタイマーを初期化する
		m_goalTimer = 0.0f;
		//カメラをプレイヤーを追いかけないようにする
		m_gameCamera->SetLookPlayerFlag(false);
	}


	//ゴール時のタイマーがカメラが動く時間より小さいか？
	if (m_goalTimer < TIME_GOAL_CAMERA_MOVE)
	{
		//小さいとき

		//注視点のアップベクトル
		Vector3 targetUp = m_upVec;
		//タイマーに応じた拡大率	//徐々に大きくする
		const float timeScale = m_goalTimer / TIME_GOAL_CAMERA_MOVE;
		//注視点のアップベクトルを拡大する	//徐々に大きくする
		targetUp.Scale(CAMERA_TRGET_UPVEC_LEN_GOAL * timeScale);

		//カメラからプレイヤーへのベクトル
		Vector3 cameraToPlayerVec = m_position - m_gameCamera->GetPosition();
		//正規化する
		cameraToPlayerVec.Normalize();
		//カメラからプレイヤーへのベクトルを拡大する
		cameraToPlayerVec.Scale(CAMERA_TO_PLAYER_VEC_LEN_GOAL);
		//デルタタイムを掛けておく
		cameraToPlayerVec.Scale(nsTimer::GameTime().GetFrameDeltaTime());
		
		//カメラの注視点を更新
		m_gameCamera->SetTarget(m_position + targetUp);
		//カメラの視点を更新
		m_gameCamera->SetPosition(m_gameCamera->GetPosition() + cameraToPlayerVec);

		//ゴール時のタイマーを進める
		m_goalTimer += nsTimer::GameTime().GetFrameDeltaTime();
	}
	else
	{
		//大きいとき

		//ゴール時のモデルのアニメーションを再生する
		m_goalPlayerMR->PlayAnimation(1);

	}

	return;
}


/**
 * @brief アニメーションを制御する
*/
void Player::AnimationController()
{
	//アニメーションを選択

	//スタン中か？
	if (m_stunFlag)
	{
		//スタン中
		//アイドル状態のアニメーションにする
		m_animState = EN_ANIM_CLIP_IDLE;
		//アニメーションの遷移をリセットする
		AnimationReset();
	}
	//持ち上げ中か？
	else if (m_lifting)
	{
		//持ち上げ中

		//アニメーションステートがが持ち上げ中か？
		if (m_animState == EN_ANIM_CLIP_CARRY)
		{
			//持ち上げ中
			//アニメーションの再生が終了したら
			if (!m_modelRender->IsPlayingAnimation())
				//持ち上げ中ではなくする
				m_lifting = false;
		}

		//持ち上げ中のアニメーションにする
		m_animState = EN_ANIM_CLIP_CARRY;
	}
	//投げ中か？
	else if (m_throwing)
	{
		//投げ中

		//アニメーションステートが右向きか左向きの投げる状態だったら。
		if (m_animState == EN_ANIM_CLIP_THROW_L || m_animState == EN_ANIM_CLIP_THROW_R)
		{
			//投げる状態
			//アニメーションの再生が終了したら
			if (!m_modelRender->IsPlayingAnimation())
				//投げ中ではなくする
				m_throwing = false;
		}

		//右向きか左向きか？
		if (m_leftOrRight == EN_LEFT)
		{
			//左向き
			//左向きの時の投げるアニメーションにする
			m_animState = EN_ANIM_CLIP_THROW_L;
		}
		else
		{
			//右向き
			//右向きの時の投げるアニメーションにする
			m_animState = EN_ANIM_CLIP_THROW_R;
		}


	}
	//ダッシュ中か？
	else if (m_isDush)
	{
		//ダッシュ中
		//走るアニメーションにする
		m_animState = EN_ANIM_CLIP_RUN;
	}
	else if (m_padLStickXF != 0.0f)
	{
		//歩きで移動中
		//歩きのアニメーションにする
		m_animState = EN_ANIM_CLIP_WALK;
	}
	else
	{
		//どれでもない時
		//アイドル状態のアニメーションにする
		m_animState = EN_ANIM_CLIP_IDLE;
	}


	//何か物を持っている状態か？	//持っている場合は上書き
	if (m_holdObject)
	{
		//持っている

		//持っている状態でのアニメーションを割り振る
		switch (m_animState)
		{
		case EN_ANIM_CLIP_IDLE:
			//アイドル状態の時は、運んでいる時のアイドル状態のアニメーション
			m_animState = EN_ANIM_CLIP_CARRY_IDEL;
			break;
		case EN_ANIM_CLIP_WALK:
			//歩いている時は、運んでいる時の歩いているアニメーション
			m_animState = EN_ANIM_CLIP_CARRY_WALK;
			break;
		case EN_ANIM_CLIP_RUN:
			//走っている時は、運んでいる時の走っているアニメーション
			m_animState = EN_ANIM_CLIP_CARRY_RUN;
			break;
		default:
			//どれでもない時は、そのままのアニメーション
			break;
		}
	}

	//選択されたアニメーションを流す
	m_modelRender->PlayAnimation(m_animState);

	return;
}

/**
 * @brief アニメーションの遷移をリセットする
*/
void Player::AnimationReset()
{
	//持ち上げ中ではなくする
	m_lifting = false;

	//投げ中ではなくする
	m_throwing = false;

	return;
}

/**
 * @brief SEの制御
*/
void Player::SoundController()
{
	//アニメーションのステートでSEを制御する
	switch (m_animState)
	{
	case EN_ANIM_CLIP_WALK:
	case EN_ANIM_CLIP_CARRY_WALK:
		//歩き中か
		//持ちながら歩いている時

		//走りのSEが再生中か？
		if (m_runSE->IsPlaying()) 
		{
			//再生中
			//停止する。
			m_runSE->Stop();
		}
		//歩きのSEをループ再生で再生する
		m_walkSE->Play(true);
		break;

	case EN_ANIM_CLIP_RUN:
	case EN_ANIM_CLIP_CARRY_RUN:
		//走り中か
		//持ちながら走っている時

		//歩きのSEが再生中か？
		if (m_walkSE->IsPlaying())
		{
			//再生中
			//停止する
			m_walkSE->Stop();
		}
		//走りのSEをループ再生で再生する
		m_runSE->Play(true);
		break;

	default:
		//それ以外の時

		//歩きのSEが再生中か？
		if (m_walkSE->IsPlaying()) 
		{
			//再生中
			//停止する
			m_walkSE->Stop();
		}
		//歩きのSEが再生中か？
		if (m_runSE->IsPlaying()) 
		{
			//再生中
			//停止する
			m_runSE->Stop();
		}
		break;
	}

	return;
}

/**
 * @brief ウェイポイントの「座標」を取得
 * @param posVec 座標のベクター
*/
void Player::SetWayPointPos
(std::vector<Vector3>*const posVec)
{
	//m_wayPointPosにウェイポイントの「場所」を格納する
	m_wayPointPos = posVec;
	//ウェイポイントステートの最大の値を設定
	//ステートは0から始まるから、サイズより1小さくする
	m_maxWayPointState = static_cast<int>(m_wayPointPos->size()) - 1;

	return;
	
}

/**
 * @brief ウェイポイントの「回転」を取得
 * @param rotVec 回転のベクター
*/
void Player::SetWayPointRot
(std::vector<Quaternion>* rotVec)
{
	//m_wayPointRotにウェイポイントの「回転」を格納する
	m_wayPointRot = rotVec;
	//ウェイポイントステートの最大の値を設定
	//ステートは0から始まるから、サイズより1小さくする
	m_maxWayPointState = static_cast<int>(m_wayPointRot->size()) - 1;

	return;
}



}
}