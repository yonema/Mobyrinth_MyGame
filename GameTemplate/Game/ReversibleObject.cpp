#include "stdafx.h"
#include "ReversibleObject.h"

//反転オブジェクトの定数を使えるようにする
using namespace reversibleObjectConstData;


//コンストラクタ
CReversibleObject::CReversibleObject()
{
	//ウェイポイントからの奥行の距離を設定
	SetZPosLen(LENGHT_Z_FROMT_WEYPOINT);

	return;
}

//スタート関数
bool CReversibleObject::PureVirtualStart()
{
	//自身が表側にあるか裏側にあるかを調べる
	CheckFrontOrBackSide();

	//サウンドの初期化
	InitSound();

	//エフェクトの初期化
	InitEffect();

	//OBBの初期化
	InitOBB();

	//透明オブジェクト用のパラメーターの初期化
	InitTransparentObjectParam();


	//オーバーライドしてほしい関数StartSub()はここで呼ばれる。
	return StartSub();
}

//デストラクタ
CReversibleObject::~CReversibleObject()
{
	//LevelObjectManagerにこのオブジェクトは消すよ！って伝える。
	Delete();

	//表と裏のモデルを消去
	for (int i = 0; i < EN_FRONT_AND_BACK_NUM; i++)
	{
		DeleteGO(m_modelRender[i]);
	}

	//表側か裏側かの反転オブジェクトの数を減算する
	CLevelObjectManager::GetInstance()->RemoveReversibleObjectNum(GetFrontOrBackSide());

	//m_changeSEの削除
	DeleteGO(m_changeSE);

	//m_throwSEの削除
	DeleteGO(m_throwSE);

	//キャパシティオーバー時のSEの破棄
	DeleteGO(m_capacityOverSE);


	//全反転時のエフェクトの破棄
	DeleteGO(m_reverseAllEffect);

	return;
}

/**
 * @brief サウンドの初期化
*/
void CReversibleObject::InitSound()
{
	//changeSEのサウンドキューを生成する
	m_changeSE = NewGO<CSoundCue>(PRIORITY_FIRST);
	//changeSEのサウンドキューを、waveファイルを指定して初期化する。
	m_changeSE->Init(SOUND_FILEPATH_SE_CHANGE);
	//音量調節
	m_changeSE->SetVolume(SOUND_VOLUME_SE_CHANGE);

	//throwSEのサウンドキューを生成する
	m_throwSE = NewGO<CSoundCue>(PRIORITY_FIRST);
	//throwSEのサウンドキューを、waveファイルを指定して初期化する。
	m_throwSE->Init(SOUND_FILEPATH_SE_THROW);
	//音量調節
	m_throwSE->SetVolume(SOUND_VOLUME_SE_THROW);


	//m_capacity_overSEのサウンドキューを生成する
	m_capacityOverSE = NewGO<CSoundCue>(PRIORITY_FIRST);
	//m_capacity_overSEのサウンドキューを、waveファイルを指定して初期化する。
	m_capacityOverSE->Init(SOUND_FILEPATH_SE_CAPACITY_OVER);
	//音量調節
	m_capacityOverSE->SetVolume(SOUND_VOLUME_SE_CAPACITY_OVER);

	return;
}

/**
 * @brief エフェクトの初期化
*/
void CReversibleObject::InitEffect()
{
	//m_reverseall2エフェクトの作成
	m_reverseAllEffect = NewGO<Effect>(PRIORITY_FIRST);
	m_reverseAllEffect->Init(u"Assets/effect/reverseall2.efk");
	//小さいので大きくしておく
	m_reverseAllEffect->SetScale(EFFECT_SCALE_REVERSE_ALL);
	m_reverseAllEffect->SetPosition(m_position);				//座標を渡す
	m_reverseAllEffect->SetRotation(m_rotation);

	return;
}

/**
 * @brief OBBの初期化
*/
void CReversibleObject::InitOBB()
{
	//OBBのサイズを設定
	Vector3 obbSize;
	obbSize = OBB_SIZE_DEFAULT_REVERSIBLE_OBJECT;
	//OBBの方向ベクトルの長さを設定
	SetOBBDirectionLength(obbSize);

	return;
}

/**
 * @brief 透明オブジェクト用のパラメーターの初期化
*/
void CReversibleObject::InitTransparentObjectParam()
{
	//リセット時に使用する、座標、回転、表裏の初期値を設定
	m_startRotation = m_rotation;
	m_startPosition = m_position;
	m_startFrontOrBack = m_frontOrBack;

	return;
}


/**
 * @brief 反転オブジェクトのスイッチをOFFにするときの処理
*/
void CReversibleObject::ReversibleSwitchOff()
{
	//オブジェクトを持ち上げられないようにする。
	m_heldFlag = false;
	//位置、回転情報を初期状態に戻す。
	m_rotation = m_startRotation;
	m_position = m_startPosition;
	//表裏情報を初期状態に戻す。
	m_frontOrBack = m_startFrontOrBack;
	SetFrontOrBack(m_frontOrBack);

	//ステートをプレイヤーに持たれているか調べるステートにする
	m_objectState = EN_CHECK_HELD_PLAYER;
	//プレイヤーが持っている反転オブジェクトは自身か？
	if (m_player->GetReversibleObject() == this)
	{
		//プレイヤーが自身を持っている

		//プレイヤーがオブジェクトを持っていないようにする
		m_player->SetHoldObject(false);
	}

	return;
}


/// <summary>
/// 初期化関数
/// 最初に読んでね。trueを戻してね。
/// 表のモデルとそのタイプ、裏のモデルとそのタイプ
/// を設定する。
/// タイプ一覧はObjectType.hを参照
/// </summary>
/// <param name="filePath_front">表のモデルのtkmファイルパス</param>
/// <param name="type_front">表のタイプ</param>
/// <param name="filePath_back">裏のモデルのtkmファイルパス</param>
/// <param name="type_back">裏のタイプ</param>
/// <returns>true戻してね</returns>
bool CReversibleObject::Init
(const char* filePath_front, const int type_front,
	const char* filePath_back, const int type_back)
{
	//表と裏の分、モデルレンダラーを生成
	for (int i = 0; i < EN_FRONT_AND_BACK_NUM; i++)
	{
		m_modelRender[i] = NewGO<CModelRender>(PRIORITY_FIRST);
	}
	//モデルレンダラーを初期化
	m_modelRender[EN_FRONT]->Init(filePath_front, D3D12_CULL_MODE_NONE);
	m_modelRender[EN_BACK]->Init(filePath_back, D3D12_CULL_MODE_NONE);
	//シャドウレシーバーをOFFにする
	m_modelRender[EN_FRONT]->SetShadowReceiverFlag(false);
	m_modelRender[EN_BACK]->SetShadowReceiverFlag(false);

	//タイプを設定
	m_reversibleType[EN_FRONT] = type_front;
	m_reversibleType[EN_BACK] = type_back;

	//今が表の状態か裏の状態か設定する
	SetObjectType(m_reversibleType[m_frontOrBack]);
	SetFrontOrBack(m_frontOrBack);

	//モデルの輪郭線を描画するようにする
	m_modelRender[EN_FRONT]->SetDrawOutLineFlag(true);
	m_modelRender[EN_BACK]->SetDrawOutLineFlag(true);

	return true;
}



/// <summary>
/// 反転させる
/// </summary>
void CReversibleObject::Reverse()
{
	//現在の表か裏の、逆の設定にする
	SetFrontOrBack(!m_frontOrBack);

	//changeSEをワンショット再生で再生する。
	m_changeSE->Play(false);

	//throwSEをワンショット再生で再生する。
	m_throwSE->Play(false);

	return;
	
}

/// <summary>
/// 特定の場所を通過した際にすべてのオブジェクトを反転させる処理に使用する関数
/// </summary>
void CReversibleObject::AllReverse()
{
	//ロック中か？
	if (GetLock())
		//ロック中なら反転しない
		return;

	//現在の表か裏の、逆の設定にする
	SetFrontOrBack(!m_frontOrBack);

	//高さの座標
	Vector3 positionHeight = g_VEC3_UP;
	//自身の回転で回す
	m_rotation.Apply(positionHeight);
	//高さの座標を大きくする
	positionHeight.Scale(EFFECT_POSITION_HEIGHT_REVERSE_ALL);
	//全反転のエフェクトの座標を回転を設定する
	m_reverseAllEffect->SetPosition(m_position + positionHeight);
	m_reverseAllEffect->SetRotation(m_rotation);
	//再生する
	m_reverseAllEffect->Play();

	return;
}

/// <summary>
/// 表か裏かを設定するする
/// </summary>
/// <param name="frontOrBack">enFrontかenBackを入れる</param>
void CReversibleObject::SetFrontOrBack(bool frontOrBack)
{
	//表か裏を設定する
	m_frontOrBack = frontOrBack;

	//表裏のモデルが有効か？
	if (m_bothModelActiveFlag)
	{
		//有効

		//タイプを設定する
		SetObjectType(m_reversibleType[m_frontOrBack]);

		//モデルが生成されているか？
		if (m_modelRender[m_frontOrBack])
		{
			//現在の表か裏を有効化して
			m_modelRender[m_frontOrBack]->Activate();
		}
		//モデルが生成されているか？
		if (m_modelRender[!m_frontOrBack])
		{
			//他方を無効化する
			m_modelRender[!m_frontOrBack]->Deactivate();
		}
	}
	else
	{
		//無効

		//デフォルトの処理ではなく特別な処理をしたいときに使う関数。
		//オーバーライドされて使われる
		SetFrontOrBackSub();
	}

	return;
}

/// <summary>
/// 表と裏の両方のモデルの有効化フラグを設定する。
/// trueを入れた場合、現在の表か裏どちらかを有効化し、他方を無効化する。
/// falseを入れた場合、両方を無効化する
/// </summary>
/// <param name="activeFlag">有効化フラグ</param>
void CReversibleObject::SetBothModelActiveFlag(const bool activeFlag)
{
	//表裏両方の有効化フラグを設定する
	m_bothModelActiveFlag = activeFlag;

	//表裏両方の有効化フラグが立っていたら
	if (m_bothModelActiveFlag)
	{
		//現在の表か裏どちらかを有効化する
		//モデルが生成されているか？
		if (m_modelRender[m_frontOrBack])
			m_modelRender[m_frontOrBack]->Activate();
		if (m_modelRender[!m_frontOrBack])
			m_modelRender[!m_frontOrBack]->Deactivate();
	}
	else
	{
		//折れていた時

		//両方を無効化する
		m_modelRender[EN_FRONT]->Deactivate();
		m_modelRender[EN_BACK]->Deactivate();
	}
	
	return;
}

//アップデート関数
void CReversibleObject::PureVirtualUpdate()
{
	//プレイヤーが見つかっていなかったら
	if (!m_player)
	{
		//プレイヤーを探す
		m_player = CLevelObjectManager::GetInstance()->GetPlayer();

		//それでも見つからなかったら何もずにreturn
		if (!m_player)
			return;
	}



	//オブジェクトが持てない状態なら、この関switch文の処理を行わない。
	if (m_heldFlag == true) {
		/// <summary>
		/// ステート（状態）アップデートを割り振る
		/// </summary>
		switch (m_objectState)
		{
		case EN_CHECK_HELD_PLAYER:	//プレイヤーに持たれるかどうか調べる
			CheckPlayer();
			break;
		case EN_LIFTED_PLAYER:		//プレイヤーに持ち上げられ中
			LiftedPlayer();
			break;
		case EN_HELD_PLAYER:		//プレイヤーに持たれている状態
			HeldPlayer();
			break;
		case EN_THROWN_DOWN:		//持っているオブジェクトを下に投げる関数
			ThrownDown();
			break;
		case EN_CANCEL:				//持っているオブジェクトをその場に置く
			Cancel();
			break;
		case EN_REPELLED:			//横に弾かれる
			Repelled();
			break;
		case EN_QUERY:				//クエリしてほしいタイミング
			Query();
			break;
		case EN_OVERLAP:			//障害オブジェクトに重なっているかの確認
			CheckObjectsOverlap();
			break;
		default:
			break;
		}

	}


	//モデルレンダラーの更新
	for (int i = 0; i < EN_FRONT_AND_BACK_NUM; i++)
	{
		//モデルレンダラーの場所と回転と拡大
		m_modelRender[i]->SetPosition(m_position);
		m_modelRender[i]->SetRotation(m_rotation);
		m_modelRender[i]->SetScale(m_scale);
	}


	//オーバーライドしてほしい関数UpdateSub()
	UpdateSub();

	return;
}


/// <summary>
/// プレイヤーに持たれるかどうか調べる関数
/// 持たれたらHeldPlayerへステート（状態）移行
/// </summary>
void CReversibleObject::CheckPlayer()
{
	//プレイヤーとの当たり判定
	if (IsHitPlayer())
	{
		//プレイヤーと衝突していて
		//Aボタンを押したら
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//プレイヤーが別のオブジェクトを持っていなかったら
			//かつ、UFOに捕まっていなかったら
			//かつ、スタン中ではなかったら
			if (!m_player->GetHoldObject() && !m_player->GetCapturedUFOFlag() && 
				!m_player->GetStunFlag())
			{
				//ステートをプレイヤーに持ち上げられている状態へ
				m_objectState = EN_LIFTED_PLAYER;
				//プレイヤーをオブジェクトを持ってる状態にする
				m_player->SetHoldObject(true, this);
				//オブジェクトが重なっているかを判定する処理を動かすフラグをtrueにする
				m_OverlapFlag = true;
				//タイマーを初期化する
				m_timer = 0.0f;
			}
		}
	}

	return;
}


/// <summary>
/// プレイヤーに持ち上げられ中の状態の関数
/// 持ち上げ終わったらHeldPlayerへステート（状態）移行
/// </summary>
void CReversibleObject::LiftedPlayer()
{
	//次の座標
	Vector3 nextPos = m_player->GetPosition();

	//横方向に加えるベクトル
	Vector3 addVecHorizontal = g_VEC3_RIGHT;
	//プレイヤーの向きが右向きなら
	if (m_player->GetEnLeftOrRight() == EN_RIGHT)
		//逆向きのベクトルにする
		addVecHorizontal = g_VEC3_LEFT;

	//縦方向に加えるベクトル
	Vector3 addVecVertical = g_VEC3_UP;

	//プレイヤーの回転
	Quaternion playerQRot = m_player->GetFinalWPRot();
	//モデルの回転をプレイヤーの回転にする
	m_rotation = playerQRot;
	//加えるベクトルをプレイヤーの回転で回転させる
	playerQRot.Apply(addVecHorizontal);
	playerQRot.Apply(addVecVertical);

	//横方向に加えるベクトルの大きさ
	float addVecHorizontalScale = ADD_VECTOR_SCALE_HORIZONTAL_LIFTED_PLAYER;
	//縦方向に加えるベクトルの大きさ
	float addVecVerticalScale = HEIGHT_HELD_UP;

	//タイマーが持ち上げられ中のタイムより小さいか
	if (m_timer < TIME_LIFTED_PLAYER)
	{
		//小さい時

		//持ち上げられ中のタイムの半分のタイム
		const float halfLiftedTime = TIME_LIFTED_PLAYER / 2.0f;

		//動く前に少しモデルの移動を待つ時間
		const float waitTimeBeforeMoving = TIME_LIFTED_PLAYER_WAIT_BEFORE_MOVING;

		//タイマーによる拡大
		float timeScale = 0.0f;

		//タイマーが待つ時間より大きかったら
		if (m_timer > waitTimeBeforeMoving)
		{
			//タイマーに応じて徐々に大きくする
			timeScale = (m_timer - waitTimeBeforeMoving) /
				(TIME_LIFTED_PLAYER - waitTimeBeforeMoving);
			//縦方向に移動するベクトルを大きくする
			addVecVerticalScale *= timeScale;
			addVecVertical.Scale(addVecVerticalScale);
		}
		else
		{
			//大きいとき、待ち時間中の時

			//縦方向の移動を0にする
			addVecVertical.Scale(0.0f);
		}

		//タイマーが半分の時間より大きいか？
		if (m_timer >= halfLiftedTime)
		{
			//大きいとき

			//タイマーに応じて拡大する、徐々に小さくする
			timeScale = 1.0f - (m_timer - halfLiftedTime) / (TIME_LIFTED_PLAYER - halfLiftedTime);
			//横方向に移動するベクトルを徐々に小さくする
			addVecHorizontalScale *= timeScale;
			addVecHorizontal.Scale(addVecHorizontalScale);
		}
		else
		{
			//小さいとき

			//横方向に移動するベクトルを最大にしておく
			addVecHorizontal.Scale(addVecHorizontalScale);
		}

		//移動する座標にベクトルを進める
		nextPos += addVecHorizontal;
		nextPos += addVecVertical;

		//タイマーを進める
		m_timer += GameTime().GetFrameDeltaTime();
	}
	else
	{
		//大きいとき

		//縦方向に加えるベクトルを最大値にする
		addVecVertical.Scale(addVecVerticalScale);
		//次の座標にベクトルを加える
		nextPos += addVecVertical;
		//タイマーを初期化する
		m_timer = 0.0f;
		//ステートをプレイヤーに持たれている状態へ
		m_objectState = EN_HELD_PLAYER;
	}

	//座標を次の座標にする
	m_position = nextPos;

	return;
}


/// <summary>
/// プレイヤーに持たれている状態の関数。
/// プレイヤーに持たれている場所に移動する処理。
/// enCancelか、enThrownDownか、enThrownSideへ
/// ステート（状態）を移行
/// </summary>
void CReversibleObject::HeldPlayer()
{
	/*
	* キャパシティオーバーしていないか調べる
	*/

	//プレイヤーの左側のウェイポイントのインデックスを取得
	int lpIndex = m_player->GetLeftPointIndex();
	//自身の左側のウェイポイントのインデックスを設定する
	SetLeftWayPointIndex(lpIndex);
	//自身が表側にあるか裏側にあるかを調べる関数
	CheckFrontOrBackSide();

	//キャパシティがオーバーしているか？
	if (IsCapacityOver(GetFrontOrBackSide()))
	{
		//オーバーしていたら

		//ステートを横に弾かれる状態へ
		m_objectState = EN_REPELLED;
		//タイマーを初期化する
		m_timer = 0.0f;
	}

	
	/*
	* 座標と回転を更新
	*/

	//プレイヤーの回転を保持
	Quaternion qRot = m_player->GetFinalWPRot();
	//上方向ベクトルを保持
	Vector3 up = g_VEC3_UP;
	//上方向のベクトルをプレイヤーの回転で回す
	qRot.Apply(up);
	//ベクトルを伸ばす
	up.Scale(HEIGHT_HELD_UP);

	//更新前の座標
	Vector3 oldPosition = m_position;

	//モデルの場所をプレイヤーの上にする
	m_position = m_player->GetPosition() + up;
	//モデルの回転をプレイヤーと同じにする
	m_rotation = qRot;

	//ステートが弾かれる状態か？
	if (m_objectState == EN_REPELLED)
	{
		//弾かれる状態である

		//更新前から更新後の座標へのベクトル
		Vector3 oldToNext = m_position - oldPosition;
		//右へのベクトル
		Vector3 rightVec = g_VEC3_LEFT;
		//自身の回転で右へのベクトルを回す
		m_rotation.Apply(rightVec);
		//更新前から更新後の座標へのベクトルと
		//右へのベクトルの内積を取る
		float inner = Dot(oldToNext, rightVec);

		//内積が正か負か
		if (inner >= 0.0f)
			//正
			//右に移動しているから、左に弾く
			m_leftOrRight = EN_LEFT;
		else
			//負
			//左に移動しているから、右に弾く
			m_leftOrRight = EN_RIGHT;
	}
	else if (!m_player->GetStunFlag())
	{
		//弾かれる状態ではない
		//かつ、プレイヤーがスタン中ではない

		//オブジェクトを裏側に投げて、オブジェクトの性質を反転させる。
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//プレイヤーの回転を保持する
			m_throwRot = m_player->GetFinalWPRot();
			//プレイヤーを投げている状態にする
			m_player->SetThrowing(true);

			//ステートを下に投げる状態へ
			m_objectState = EN_THROWN_DOWN;
			//最初になげられたときの状態にする
			m_trownDownOrUp = EN_START_THROWN;
		}
		//オブジェクトをプレイヤーの足元に置く。
		else if (g_pad[0]->IsTrigger(enButtonB))
		{
			//ステートをキャンセル状態へ
			m_objectState = EN_CANCEL;

			//キャンセルした時にキャパオーバーした時に弾かれる方向を決める

			//表側の左端のウェイポイントの番号
			const int leftEndWayPointNumForFrontSide =
				CLevelObjectManager::GetInstance()->GetFrontOrBackSideThresholdBig();

			//裏側の左端のウェイポイントの番号
			const int leftEndWayPointNumForBackSide =
				CLevelObjectManager::GetInstance()->GetFrontOrBackSideThresholdSmall() - 1;

			//ウェイポイントの番号が左端か？
			if (m_player->GetLeftPointIndex() == leftEndWayPointNumForFrontSide ||
				m_player->GetLeftPointIndex() == leftEndWayPointNumForBackSide)
			{
				//左端
				//右側に弾く
				m_leftOrRight = EN_RIGHT;
			}
			else
			{
				//左端ではない
				//左側に弾く
				m_leftOrRight = EN_LEFT;
			}
		}
	}

	return;
}

/// <summary>
/// 持っているオブジェクトを下に投げる関数
/// enQueryへステート（状態）を移行
/// </summary>
void CReversibleObject::ThrownDown()
{
	//下に投げるベクトル
	Vector3 thorwnDownHorizontalVec = g_VEC3_DOWN;
	//上に投げるときは上向きのベクトルを入れる
	if (m_trownDownOrUp == EN_UP_THROWN)
		thorwnDownHorizontalVec = g_VEC3_UP;

	//プレイヤーの回転で下方向のべクトルを回す
	m_throwRot.Apply(thorwnDownHorizontalVec);


	//ベクトルを伸ばす
	//投げられる状態が、最初に投げられたか？
	if (m_trownDownOrUp == EN_START_THROWN)
		//最初になげられたとき
		thorwnDownHorizontalVec.Scale(ADD_VECTOR_SCALE_HORIZONTAL_THORWN_DOWN);
	else
		//それ以外の時（跳ね返ったとき、キャパオーバーの時など）
		thorwnDownHorizontalVec.Scale(ADD_VECTOR_SCALE_HORIZONTAL_THORWN_DOWN_OVERLAP);


	//手前に動かすベクトル
	Vector3 backwardVec = g_VEC3_FRONT;
	//手前に動かすベクトルを拡大する
	backwardVec.Scale(ADD_VECTOR_SCALE_DEPTH_THROWN_DOWN);
	//自身の回転で回す
	m_throwRot.Apply(backwardVec);

	//下に投げるタイムの半分の時間
	const double halfThrownDownTime = TIME_THROWN_DOWN / 2.0;


	//二次関数的な動きにする
	backwardVec *= pow(m_thrownDownTimer - halfThrownDownTime,
			POW_NUM_DEPTH_SCALE_THROWN_DOWN);

	//モデルの回転を、逆さ向きに向かってちょっとずつ回す
	m_rotation.SetRotationDegX(180.0f * m_thrownDownTimer / TIME_THROWN_DOWN);
	m_rotation.Multiply(m_throwRot);



	//半分の時間が過ぎたら下に下げる
	if (m_thrownDownTimer >= halfThrownDownTime)
		backwardVec *= -1.0f;

	//モデルの場所を下に下げる
	m_position += (thorwnDownHorizontalVec + backwardVec) * GameTime().GetFrameDeltaTime();


	//投げている時のカウンターを進める
	m_thrownDownTimer += GameTime().GetFrameDeltaTime();

	//投げている時のカウンターが最大値まで来たら
	if (m_thrownDownTimer >= TIME_THROWN_DOWN)
	{
		//モデルの回転を完全に逆さ向きに回す。
		m_rotation.SetRotationDegX(180.0f);
		m_rotation.Multiply(m_throwRot);


		//プレイヤーがオブジェクトを持っていない状態にする
		m_player->SetHoldObject(false);

		//投げている時のカウンターを0に戻す
		m_thrownDownTimer = 0.0f;

		//反転するか？のフラグを立てる
		//二重反転防止をリセット
		m_changeObject = true;


		//ステートをクエリへ移行する
		m_objectState = EN_QUERY;
		//投げた後、重なったら縦に戻ってくる
		m_overlapAction = EN_THROWN_DOWN;

	}
	//投げている時のカウンターが最大値の半分まで来たら
	else if (m_thrownDownTimer >= halfThrownDownTime && m_changeObject == true)
	{
		//表側か裏側か？
		int frontOrBackSide = EN_FRONT_SIDE;
		//自分がいる側の反対側にする
		if (GetFrontOrBackSide() == EN_FRONT_SIDE)
			frontOrBackSide = EN_BACK_SIDE;

		//反転させる
		Reverse();

		//反転するか？フラグを折る
		//二重反転の防止
		m_changeObject = false;

	}
	return;
}

/// <summary>
/// 持っているオブジェクトをその場に置く関数
/// enQueryへステート（状態）を移行
/// </summary>
void CReversibleObject::Cancel()
{
	//プレイヤーの現在の回転を持ってくる
	Quaternion qRot = m_player->GetFinalWPRot();
	//座標の高さにYUpベクトルを入れる
	Vector3 positionHeight = g_VEC3_UP;
	//座標の高さを回す
	qRot.Apply(positionHeight);
	//座標の高さを上に伸ばす
	positionHeight.Scale(POSITION_HEIGHT_CANCEL);

	//プレイヤーの場所のちょっと上に場所を設定する
	m_position = m_player->GetPosition() + positionHeight;
	//プレイヤーがオブジェクトを持っていないようにする
	m_player->SetHoldObject(false);

	//キャンセルの時、重なったら弾かれる処理
	//ステートをクエリへ移行する
	m_objectState = EN_QUERY;

	//移動先でオブジェクト同士が重なったときの処理を設定
	m_overlapAction = EN_REPELLED;

	return;
}



/// <summary>
/// 横に弾かれる
/// </summary>
void CReversibleObject::Repelled()
{
	//タイマーが0.0fの時
	//つまり最初の1回だけ呼ばれる
	if (m_timer == 0.0f)
	{
		//ウェイポイント上の次の座標を計算する
		m_addPosition = CLevelObjectManager::GetInstance()->CalcWayPointNextPos
		(GetRightWayPointIndex(), m_position, ADD_VECTOR_SCALE_HORIZONTAL_REPELLED, m_leftOrRight);

		//自身の座標から次の座標へのベクトルにする
		m_addPosition -= m_position;
		//切り替え時間いっぱいで次の座標へ行けるようにする
		m_addPosition /= TIME_REPELLED;

		//プレイヤーがオブジェクトを持っていない状態にする
		m_player->SetHoldObject(false);
	}

	//ウェイポイントと回転の更新
	CheckWayPoint(true, false);

	//タイマーが切り替え時間より小さいか？
	if (m_timer < TIME_REPELLED)
	{
		//小さい

		//縦に動かすベクトル
		Vector3 verticalPower = g_VEC3_UP;
		verticalPower.Scale(ADD_VECTOR_SCALE_VERTICAL_REPELLED);
		//自身の回転で回す
		m_rotation.Apply(verticalPower);
		//二次関数的な動きにする
		verticalPower *= pow(static_cast<double>(m_timer - TIME_REPELLED / 2.0), 2.0);
		//半分の時間が過ぎたら下に下げる
		if (m_timer >= TIME_REPELLED / 2.0f)
			verticalPower *= -1.0f;

		//Y軸中心にくるくる回転させる
		Quaternion qRot = m_rotation;
		m_rotation.SetRotationDegY(360.0f * 2.0f * m_timer / TIME_REPELLED);
		m_rotation.Multiply(qRot);

		//座標に次へのベクトルを加算する	//デルタタイムを掛けておく
		m_position += (m_addPosition + verticalPower) * GameTime().GetFrameDeltaTime();
		//タイマーを進める
		m_timer += GameTime().GetFrameDeltaTime();
	}
	else
	{
		//大きい

		//タイマーを初期化
		m_timer = 0.0f;
		//ステートをクエリ状態へ
		m_objectState = EN_QUERY;
		//移動先でオブジェクト同士が重なったときの処理を設定
		m_overlapAction = EN_REPELLED;
	}

	return;
}


/// <summary>
/// クエリしてほしいタイミングで呼ばれる関数
/// オーバーライドしてほしい関数、QuerySub()はここで呼ばれる。
/// enOverlapへステート（状態）を移行
/// </summary>
void CReversibleObject::Query()
{
	//ウェイポイントの場所を更新する
	CheckWayPoint();

	//表側か裏側かを更新する
	CheckFrontOrBackSide();


	//キャパシティがオーバーしているか？
	if (!IsCapacityOver(GetFrontOrBackSide()))
	{
		//オーバーしていない
		//通常のクエリの処理

		//オーバーライドしてほしい関数QuerySub()
		QuerySub();
		//オーバーラップをチェックする状態へ
		m_objectState = EN_OVERLAP;
	}
	else
	{

		//オーバーしている

		//クエリしない
		
		//オーバーラップのアクションが横に弾くだったら
		if (m_overlapAction == EN_REPELLED)
			//横に弾く
			m_objectState = EN_REPELLED;
		else
		{
			//投げられた状態

			m_objectState = EN_THROWN_DOWN;
			if (m_trownDownOrUp != EN_UP_THROWN)
				m_trownDownOrUp = EN_UP_THROWN;
			else
				m_trownDownOrUp = EN_DOWN_THROWN;

		}
		
	}
	return;
}



void CReversibleObject::CheckObjectsOverlap()
{
	//オーバーラップの処理を行うか？
	if (m_OverlapFlag == false) {
		//行わない場合は、プレイヤーに持たれているか調べる状態にする
		m_objectState = EN_CHECK_HELD_PLAYER;
		return;
	}

	//何かと衝突していないか調べる
	bool checkOverlap = IsHitAllLOs(*this);

	//衝突しているか？
	if (checkOverlap == true) 
	{
		//投げた後の行動を割り振る
		switch (m_overlapAction)
		{
		case EN_THROWN_DOWN:
			//下に投げた時は、縦に跳ね返る
			m_objectState = EN_THROWN_DOWN;
			//上に返るか下に返るか？
			if (m_trownDownOrUp != EN_UP_THROWN)
				m_trownDownOrUp = EN_UP_THROWN;
			else
				m_trownDownOrUp = EN_DOWN_THROWN;
			break;

		case EN_REPELLED:
			//横に弾いた先で重なってたら、もう一回横に弾く
			m_objectState = EN_REPELLED;
			break;

		default:
			break;
		}

	}
	else 
	{
		//衝突していない時
		//プレイヤーに持たれているか調べる状態にする
		m_objectState = EN_CHECK_HELD_PLAYER;
	}

	return;
}


/// <summary>
/// 表側か裏側のキャパシティがオーバーしているか？
/// を調べる関数
/// 調整値には、自身はまだ対象の側にいないが、行ったとしたら
/// キャパシティはオーバーするのか？を調べるときに、1を入れる。
/// </summary>
/// <param name="frontOrBackSide">表側か裏側か？</param>
/// <returns>オーバーしているか？</returns>
const bool CReversibleObject::IsCapacityOver(const int frontOrBackSide)
{
	//反転オブジェクトの表面と裏面のそれぞれの数
	const int* num = CLevelObjectManager::GetInstance()->GetReversibleObjectNum();
	//反転オブジェクトの表面と裏面のそれぞれの最大数
	const int* maxNum = CLevelObjectManager::GetInstance()->GetReversibleObjectMaxNum();
	//次の表面か裏面かの数
	int nextSideNum = num[frontOrBackSide];
	//次の表面か裏面かの最大数
	int nextSideMaxNum = maxNum[frontOrBackSide];

	//数が最大数以下だったら
	return nextSideNum > nextSideMaxNum;
}

