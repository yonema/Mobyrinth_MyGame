#include "stdafx.h"
#include "ReversibleObject.h"

//コンストラクタ
CReversibleObject::CReversibleObject()
{
	//ウェイポイントからの奥行の距離を設定
	SetZPosLen(100.0f);
}

//スタート関数
bool CReversibleObject::PureVirtualStart()
{
	//モデルの回転を、現在の場所とイイ感じに合わせる
	//CheckWayPoint();
	//自身が表側にあるか裏側にあるかを調べる
	CheckFrontOrBackSide();

	//changeSEのサウンドキューを生成する
	m_changeSE = NewGO<CSoundCue>(0);
	//changeSEのサウンドキューを、waveファイルを指定して初期化する。
	m_changeSE->Init(L"Assets/sound/change.wav");
	//音量調節
	m_changeSE->SetVolume(0.5f);

	//throwSEのサウンドキューを生成する
	m_throwSE = NewGO<CSoundCue>(0);
	//throwSEのサウンドキューを、waveファイルを指定して初期化する。
	m_throwSE->Init(L"Assets/sound/throw.wav");
	//音量調節
	m_throwSE->SetVolume(0.5f);

	//OBBのサイズを設定
	Vector3 obbSize;
	obbSize = { 200.0f,200.0f,600.0f };
	//OBBの方向ベクトルの長さを設定
	GetOBB().SetDirectionLength(obbSize);

	//m_reverseall2エフェクトの作成
	m_reverseall2 = NewGO<Effect>(0);
	m_reverseall2->Init(u"Assets/effect/reverseall2.efk");
	float scale2 = 200.0f;								//小さいので大きくしておく
	m_reverseall2->SetScale({ scale2 ,scale2 ,scale2 });
	m_reverseall2->SetPosition(m_position);				//座標を渡す
	m_reverseall2->SetRotation(m_rotation);


	//オーバーライドしてほしい関数StartSub()はここで呼ばれる。
	return StartSub();
}

//デストラクタ
CReversibleObject::~CReversibleObject()
{
	//LevelObjectManagerにこのオブジェクトは消すよ！って伝える。
	Delete();

	//表と裏のモデルを消去
	for (int i = 0; i < enFrontAndBackNum; i++)
	{
		DeleteGO(m_modelRender[i]);
	}

	//表側か裏側かの反転オブジェクトの数を減算する
	CLevelObjectManager::GetInstance()->RemoveReversibleObjectNum(GetFrontOrBackSide());

	//m_changeSEの削除
	DeleteGO(m_changeSE);

	//m_throwSEの削除
	DeleteGO(m_throwSE);

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
	for (int i = 0; i < enFrontAndBackNum; i++)
	{
		m_modelRender[i] = NewGO<CModelRender>(0);
	}
	//モデルレンダラーを初期化
	m_modelRender[enFront]->Init(filePath_front, D3D12_CULL_MODE_NONE);
	m_modelRender[enBack]->Init(filePath_back, D3D12_CULL_MODE_NONE);
	m_modelRender[enFront]->SetShadowReceiverFlag(false);
	m_modelRender[enBack]->SetShadowReceiverFlag(false);

	//タイプを設定
	m_reversibleType[enFront] = type_front;
	m_reversibleType[enBack] = type_back;
	//今が表の状態か裏の状態か設定する
	SetObjectType(m_reversibleType[m_frontOrBack]);
	SetFrontOrBack(m_frontOrBack);

	return true;
}



/// <summary>
/// 反転させる
/// </summary>
void CReversibleObject::Reverse()
{
	//現在の表か裏の、逆の設定にする
	SetFrontOrBack(!m_frontOrBack);

	//changeSEをループ再生をオフで再生する。
	m_changeSE->Play(false);

	//throwSEをループ再生をオフで再生する。
	m_throwSE->Play(false);
	
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

	Vector3 upVec = g_vec3Up;
	m_rotation.Apply(upVec);
	//この値を変更して高さを調節する
	const float upVecLne = 100.0f;
	upVec.Scale(upVecLne);
	m_reverseall2->SetPosition(m_position + upVec);		//座標を渡す
	m_reverseall2->SetRotation(m_rotation);
	m_reverseall2->Play();								//再生

}

/// <summary>
/// 表か裏かを設定するする
/// </summary>
/// <param name="frontOrBack">enFrontかenBackを入れる</param>
void CReversibleObject::SetFrontOrBack(bool frontOrBack)
{
	//表か裏を設定する
	m_frontOrBack = frontOrBack;
	if (m_bothModelactiveFlag)
	{
		//タイプを設定する
		SetObjectType(m_reversibleType[m_frontOrBack]);
		//現在の表か裏を有効化して
		if (m_modelRender[m_frontOrBack])
			m_modelRender[m_frontOrBack]->Activate();
		//他方を無効化する
		if (m_modelRender[!m_frontOrBack])
			m_modelRender[!m_frontOrBack]->Deactivate();
	}
	else
	{
		SetFrontOrBackSub();
	}
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
	m_bothModelactiveFlag = activeFlag;


	if (m_bothModelactiveFlag)
	{
		//現在の表か裏どちらかを有効化する
		if (m_modelRender[m_frontOrBack])
			m_modelRender[m_frontOrBack]->Activate();
		if (m_modelRender[!m_frontOrBack])
			m_modelRender[!m_frontOrBack]->Deactivate();
	}
	else
	{
		//両方を無効化する
		m_modelRender[enFront]->Deactivate();
		m_modelRender[enBack]->Deactivate();
	}
	
}

//アップデート関数
void CReversibleObject::PureVirtualUpdate()
{
	//プレイヤーが見つかっていなかったら
	if (!m_pPlayer)
	{
		//プレイヤーを探す
		m_pPlayer = CLevelObjectManager::GetInstance()->GetPlayer();

		//それでも見つからなかったら何もずにreturn
		if (!m_pPlayer)
			return;
	}

	//オブジェクトが持てない状態のとき、m_objectStateの値をenCheckPlayerに変更
	if (GetFlagHeld() == false) {
		m_objectState = enCheckPlayer;
		//プレイヤーがオブジェクトを持っていないようにする
		m_pPlayer->SetHoldObject(false);
	}


	//オブジェクトが持てない状態なら、この関switch文の処理を行わない。
	if (GetFlagHeld() == true) {
		/// <summary>
		/// ステート（状態）アップデートを割り振る
		/// </summary>
		switch (m_objectState)
		{
		case enCheckPlayer:	//プレイヤーに持たれるかどうか調べる
			CheckPlayer();
			break;
		case enHeldPlayer:	//プレイヤーに持たれている状態
			HeldPlayer();
			break;
		case enThrownDown: //持っているオブジェクトを下に投げる関数
			ThrownDown();
			break;
		case enCancel: //持っているオブジェクトをその場に置く
			Cancel();
			break;
			//case enThrownSide: //持っているオブジェクトを横に投げる関数
			//	ThrownSide();
			//	break;
		case enRepelled:	//横に弾かれる
			Repelled();
			break;
		case enQuery: //クエリしてほしいタイミング
			Query();
			break;
		case enOverlap: //障害オブジェクトに重なっているかの確認
			CheckObjectsOverlap();
			break;
		case enOverlapThrownDown: //オブジェクトが戻ってくる処理（縦）
			OverlapThrownDown();
			break;
			//case enOverlapThrownSide: //オブジェクトが戻ってくる処理（横）
			//	OverlapThrownSide();
			//	break;
		default:
			break;
		}

	}


	//モデルレンダラーの更新
	for (int i = 0; i < enFrontAndBackNum; i++)
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
		//Aボタンを押したら
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//プレイヤーが別のオブジェクトを持っていなかったら
			//かつ、UFOに捕まっていなかったら
			//かつ、スタン中ではなかったら
			if (!m_pPlayer->GetHoldObject() && !m_pPlayer->GetCapturedUFOFlag() && 
				!m_pPlayer->GetStunFlag())
			{
				//ステートをプレイヤーに持たれている状態へ
				m_objectState = enHeldPlayer;
				//プレイヤーをオブジェクトを持ってる状態にする
				m_pPlayer->SetHoldObject(true, this);
				//オブジェクトが重なっているかを判定する処理を動かすフラグをtrueにする
				m_flagOverlap = true;
			}
		}
	}
}


/// <summary>
/// プレイヤーに持たれている状態の関数。
/// プレイヤーに持たれている場所に移動する処理。
/// enCancelか、enThrownDownか、enThrownSideへ
/// ステート（状態）を移行
/// </summary>
void CReversibleObject::HeldPlayer()
{
	//プレイヤーの左側のウェイポイントのインデックスを取得
	int lpIndex = m_pPlayer->GetLeftPointIndex();

	//自身の左側のウェイポイントのインデックスを設定する
	SetLeftWayPointIndex(lpIndex);

	//更新前の表面か裏面か？
	int oldFrontOrBackSide = GetFrontOrBackSide();
	//更新前の座標
	Vector3 oldPosition = m_position;

	//自身が表側にあるか裏側にあるかを調べる関数
	CheckFrontOrBackSide();

	//更新前と更新後の表門か裏面か？が違っていたら
	if (oldFrontOrBackSide != GetFrontOrBackSide())
	{
		//キャパシティがオーバーしているか？
		if (IsCapacityOver(GetFrontOrBackSide()))
		{
			//オーバーしていたら

			//ステートを横に弾かれる状態へ
			m_objectState = enRepelled;
			//タイマーを初期化する
			m_timer = 0.0f;
		}
	}
	


	//プレイヤーの回転を保持
	Quaternion qRot = m_pPlayer->GetFinalWPRot();
	//上方向ベクトルを保持
	Vector3 up = g_vec3Up;
	//上方向のベクトルをプレイヤーの回転で回す
	qRot.Apply(up);
	//ベクトルを伸ばす
	up.Scale(m_heldUpLen);

	//モデルの場所をプレイヤーの上にする
	m_position = m_pPlayer->GetPosition() + up;
	//モデルの回転をプレイヤーと同じにする
	m_rotation = qRot;

	//ステートが弾かれる状態か？
	if (m_objectState == enRepelled)
	{
		//弾かれる状態である

		//更新前から更新後の座標へのベクトル
		Vector3 oldToNext = m_position - oldPosition;
		//右へのベクトル
		Vector3 rightVec = g_vec3Left;
		//自身の回転で右へのベクトルを回す
		m_rotation.Apply(rightVec);
		//更新前から更新後の座標へのベクトルと
		//右へのベクトルの内積を取る
		float inner = Dot(oldToNext, rightVec);

		//内積が正か負か
		if (inner >= 0.0f)
			//正
			//右に移動しているから、左に弾く
			m_leftOrRight = enLeft;
		else
			//負
			//左に移動しているから、右に弾く
			m_leftOrRight = enRight;
	}
	else if (!m_pPlayer->GetStunFlag())
	{
		//弾かれる状態ではない
		//かつ、プレイヤーがスタン中ではない

		//オブジェクトを裏側に投げて、オブジェクトの性質を反転させる。
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//プレイヤーの回転を保持する
			m_throwRot = m_pPlayer->GetFinalWPRot();

			//ステートを下に投げる状態へ
			m_objectState = enThrownDown;
			m_objectAction = enThrownDown;
		}
		//オブジェクトをプレイヤーの足元に置く。
		else if (g_pad[0]->IsTrigger(enButtonB))
		{
			//ステートをキャンセル状態へ
			m_objectState = enCancel;
			m_objectAction = enCancel;
		}
	}
	//オブジェクトを横に投げる処理
	//else if (g_pad[0]->IsTrigger(enButtonX))
	//{
	//	if (m_objectState != enThrownSide)
	//	{
	//		//CalcTargetPos();
	//		m_throwRot = m_pPlayer->GetFinalWPRot();
	//		//投げる位置をプレイヤーの少し上に設定
	//		Vector3 dir = { 0.0f,10.0f,0.0f };
	//		m_throwRot.Apply(dir);
	//		dir.Scale(7.0f);
	//		m_position += dir;
	//		//プレイヤーの左右の向きを調べる
	//		m_playerLeftOrRight = m_pPlayer->GetEnLeftOrRight();
	//	}
	//	//ステートを横に投げる状態へ
	//	m_objectState = enThrownSide;
	//	m_objectAction = enThrownSide;
	//}
}

/// <summary>
/// 持っているオブジェクトを下に投げる関数
/// enQueryへステート（状態）を移行
/// </summary>
void CReversibleObject::ThrownDown()
{
	//投げている時のカウンターの最大値
	const float maxThrowCounter = 0.5f;

	//下方向のベクトルを保持する
	Vector3 dir = g_vec3Down;

	//プレイヤーの回転で下方向のべクトルを回す
	m_throwRot.Apply(dir);
	//ベクトルを伸ばす
	dir.Scale(400.0f);

	test = dir;
	test.Scale(0.9f);

	//手前に動かすベクトル
	Vector3 backwardPower = { 0.0f,0.0f,1.0f };
	//手前に動かすベクトルの大きさ
	const float backwardPowerScale = 60000.0f;
	//手前に動かすベクトルを拡大する
	backwardPower.Scale(backwardPowerScale);
	//自身の回転で回す
	m_throwRot.Apply(backwardPower);

	//カウンターを調節する変数
	float counterAdjust = 0.0f;

	//縦に弾かれるか？
	if (!m_virticalRepelledFlag)
	{
		//弾かれない
		//通常の処理

		//二次関数的な動きにする
		backwardPower *= pow(static_cast<double>(m_throwCounter - maxThrowCounter / 2.0), 2.0);

		//モデルの回転を、逆さ向きに向かってちょっとずつ回す
		m_rotation.SetRotationDegX(180.0f * m_throwCounter / maxThrowCounter);
		m_rotation.Multiply(m_throwRot);
	}
	else
	{
		//弾かれる
		//跳ね返る処理

		//手前に弾くベクトルを弱くする
		float toWeaken = 0.042f;
		backwardPower.Scale(toWeaken);
		//二次関数的な動きにする
		backwardPower *= pow(static_cast<double>
			((maxThrowCounter * 2.0 - maxThrowCounter / 2.0) - 
				(m_throwCounter - maxThrowCounter / 2.0))
			, 2.0);

		//下方向のベクトルを上方向にする
		dir.Scale(-1.0f);
		//弱める
		toWeaken = 0.5f;
		dir.Scale(toWeaken);
		//モデルの回転の値
		float rotValue = 90.0f - 360.0f * 
			(m_throwCounter - maxThrowCounter / 2) / (maxThrowCounter * 2 - maxThrowCounter / 2);
		m_rotation.SetRotationDegX(rotValue);
		m_rotation.Multiply(m_throwRot);
		//カウンターを調節する時間にカウンターの最大値を入れる
		counterAdjust = maxThrowCounter;
	}

	//半分の時間が過ぎたら下に下げる
	if (m_throwCounter >= maxThrowCounter / 2.0f)
		backwardPower *= -1.0f;

	//モデルの場所を下に下げる
	m_position += (dir + backwardPower) * GameTime().GetFrameDeltaTime();


	//投げている時のカウンターを進める
	m_throwCounter += GameTime().GetFrameDeltaTime();

	//投げている時のカウンターが最大値まで来たら
	if (m_throwCounter >= maxThrowCounter + counterAdjust)
	{
		//縦に弾かれるか？
		if (!m_virticalRepelledFlag)
		{
			//弾かれないとき（通常時）

			//モデルの回転を完全に逆さ向きに回す。
			m_rotation.SetRotationDegX(180.0f);
			m_rotation.Multiply(m_throwRot);
		}
		else
			test.Scale(-1.0f);

		//プレイヤーがオブジェクトを持っていない状態にする
		m_pPlayer->SetHoldObject(false);

		//投げている時のカウンターを0に戻す
		m_throwCounter = 0.0f;

		m_changeObject = true;

		//縦に弾かれるか？を初期化する
		m_virticalRepelledFlag = false;

		//ステートをクエリへ移行する
		m_objectState = enQuery;
	}
	//投げている時のカウンターが最大値の半分まで来たら
	else if (m_throwCounter >= maxThrowCounter / 2.0f && m_changeObject == true)
	{
		//表側か裏側か？
		int frontOrBackSide = CLevelObjectManager::enFrontSide;
		//自分がいる側の反対側にする
		if (GetFrontOrBackSide() == CLevelObjectManager::enFrontSide)
			frontOrBackSide = CLevelObjectManager::enBackSide;

		//キャパシティオーバーか？
		//自分がいない側のキャパシティを調べる
		//自分はまだそっち側にいないから、調整値に1を入れている
		if (IsCapacityOver(frontOrBackSide, 1))
			//キャパオーバーしてる
			m_virticalRepelledFlag = true;
		else
			//キャパオーバーではない
			//反転させる
			Reverse();

		m_changeObject = false;

	}
}

/// <summary>
/// 持っているオブジェクトをその場に置く関数
/// enQueryへステート（状態）を移行
/// </summary>
void CReversibleObject::Cancel()
{
	//プレイヤーの現在の回転を持ってくる
	Quaternion qRot = m_pPlayer->GetFinalWPRot();
	//UpベクトルにYUpベクトルを入れる
	Vector3 up = g_vec3Up;
	//Upベクトルを回す
	qRot.Apply(up);
	//Upベクトルを上に伸ばす
	up.Scale(20.0f);

	//プレイヤーの場所のちょっと上に場所を設定する
	m_position = m_pPlayer->GetPosition() + up;
	//プレイヤーがオブジェクトを持っていないようにする
	m_pPlayer->SetHoldObject(false);

	//ステートをクエリへ移行する
	m_objectState = enQuery;
}

/// <summary>
/// 持っているオブジェクトを横に投げる関数
/// enQueryへステート（状態）を移行
/// </summary>
//void CReversibleObject::ThrownSide()
//{
//	//投げている時のカウンターの最大値
//	const float maxThrowCounter = 0.5f;
//
//	//左方向に投げる
//	if (m_playerLeftOrRight == enLeft) {
//		//オブジェクトが横方向に移動するベクトルの作成
//		Vector3 dir = g_vec3Right;
//		m_throwRot.Apply(dir);
//		dir.Scale(700.0f);
//		m_position += dir * GameTime().GetFrameDeltaTime();
//
//		//投げ終わったオブジェクトが地面と良い感じの距離になるように調整する。
//		Vector3 dir2 = { 0.0f,-0.5f,0.0f };
//		m_throwRot.Apply(dir2);
//		dir2.Scale(250.0f);
//		m_position += dir2 * GameTime().GetFrameDeltaTime();
//
//		//
//		m_rotation.SetRotationDegZ(-360.0f * m_throwCounter / maxThrowCounter);
//		m_rotation.Multiply(m_throwRot);
//		m_throwCounter += GameTime().GetFrameDeltaTime();
//		m_pPlayer->SetHoldObject(false);
//
//		if (m_throwCounter >= maxThrowCounter)
//		{
//			m_rotation.SetRotationDegZ(0.0f);
//			m_rotation.Multiply(m_throwRot);
//			m_objectState = enQuery;
//			m_throwCounter = 0;
//		}
//	}
//	//右方向に投げる
//	else if (m_playerLeftOrRight == enRight) {
//		//オブジェクトが横方向に移動するベクトルの作成
//		Vector3 dir = g_vec3Left;
//		m_throwRot.Apply(dir);
//		dir.Scale(700.0f);
//		m_position += dir * GameTime().GetFrameDeltaTime();
//
//		//投げ終わったオブジェクトが地面と良い感じの距離になるように調整する。
//		Vector3 dir2 = { 0.0f,-0.5f,0.0f };
//		m_throwRot.Apply(dir2);
//		dir2.Scale(250.0f);
//		m_position += dir2 * GameTime().GetFrameDeltaTime();
//
//		//
//		m_rotation.SetRotationDegZ(360.0f * m_throwCounter / maxThrowCounter);
//		m_rotation.Multiply(m_throwRot);
//		m_throwCounter += GameTime().GetFrameDeltaTime();
//		m_pPlayer->SetHoldObject(false);
//
//		if (m_throwCounter >= maxThrowCounter)
//		{
//			m_rotation.SetRotationDegZ(0.0f);
//			m_rotation.Multiply(m_throwRot);
//			m_objectState = enQuery;
//			m_throwCounter = 0;
//		}
//	}
//	
//}

/// <summary>
/// オブジェクトを横に投げる際の、投げる先を計算する関数
/// </summary>
//void CReversibleObject::CalcTargetPos()
//{
//	m_pPlayer->GetLeftPointIndex();
//}

/// <summary>
/// 横に弾かれる
/// </summary>
void CReversibleObject::Repelled()
{
	//切り替えタイマー
	const float switchingTimer = 1.0f;

	//タイマーが0.0fの時
	//つまり最初の1回だけ呼ばれる
	if (m_timer == 0.0f)
	{
		//弾く距離
		const float addLen = 500.0f;
		//ウェイポイント上の次の座標を計算する
		m_addPosition = CLevelObjectManager::GetInstance()->CalcWayPointNextPos
		(GetRightWayPointIndex(), m_position, addLen, m_leftOrRight);

		//自身の座標から次の座標へのベクトルにする
		m_addPosition -= m_position;
		//切り替え時間いっぱいで次の座標へ行けるようにする
		m_addPosition /= switchingTimer;

		//プレイヤーがオブジェクトを持っていない状態にする
		m_pPlayer->SetHoldObject(false);
	}

	//ウェイポイントと回転の更新
	CheckWayPoint(true, false);

	//タイマーが切り替え時間より小さいか？
	if (m_timer < switchingTimer)
	{
		//小さい

		//縦に動かすベクトル
		Vector3 verticalPower = { 0.0f,4000.0f,0.0f };
		//自身の回転で回す
		m_rotation.Apply(verticalPower);
		//二次関数的な動きにする
		verticalPower *= pow(static_cast<double>(m_timer - switchingTimer / 2.0), 2.0);
		//半分の時間が過ぎたら下に下げる
		if (m_timer >= switchingTimer / 2.0f)
			verticalPower *= -1.0f;

		//Y軸中心にくるくる回転させる
		Quaternion qRot = m_rotation;
		m_rotation.SetRotationDegY(360.0f * 2.0f * m_timer / switchingTimer);
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
		m_objectState = enQuery;
		//移動先でオブジェクト同士が重なったときの処理を設定
		m_objectAction = enRepelled;
	}

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

	//オーバーライドしてほしい関数QuerySub()
	QuerySub();


	//オーバーラップをチェックする状態へ
	m_objectState = enOverlap;
	////キャパシティがオーバーしているか？
	//if (!IsCapacityOver(GetFrontOrBackSide()))
	//{
	//	//オーバーしていない
	//	//通常のクエリの処理

	//	//オーバーライドしてほしい関数QuerySub()
	//	QuerySub();
	//	//オーバーラップをチェックする状態へ
	//	m_objectState = enOverlap;
	//}
	//else
	//{
	//	//念のため書いてるけど、本来ここを通る前に、
	//	//ThrownDown()でキャパシティオーバーを調べて
	//	//オーバーしてたらRepelled()に行っているはず

	//	//オーバーしている

	//	//クエリせずに
	//	//下に投げた時のオーバーラップ
	//	m_objectState = enOverlapThrownDown;
	//	test.Scale(-1.0f);
	//}
}





void CReversibleObject::CheckObjectsOverlap()
{
	if (m_flagOverlap == false) {
		m_objectState = enCheckPlayer;
		return;
	}

	m_checkOverlap = QueryAllLOs(*this, GetObjectType());

	if (m_checkOverlap == true) {
		switch (m_objectAction)
		{
		case enThrownDown:
			m_objectState = enOverlapThrownDown;
			test.Scale(-1.0f);
			break;
		case enCancel:
			m_objectState = enHeldPlayer;
			break;
		case enRepelled:
			//横に弾いた先で重なってたら、もう一回横に弾く
			m_objectState = enRepelled;
			break;
		//case enThrownSide:
		//	m_objectState = enOverlapThrownSide;

		//	if (m_playerLeftOrRight == enLeft) {
		//		m_playerLeftOrRight = enRight;
		//	}
		//	else if (m_playerLeftOrRight == enRight) {
		//		m_playerLeftOrRight = enLeft;
		//	}

		//	//投げる位置をプレイヤーの少し上に設定
		//	{
		//		Vector3 dir = { 0.0f,12.0f,0.0f };
		//		m_throwRot.Apply(dir);
		//		dir.Scale(7.0f);
		//		m_position += dir;
		//	}
		//	break;
		default:
			break;
		}

		return;
	}
	else {
		m_objectState = enCheckPlayer;
	}
}


void CReversibleObject::OverlapThrownDown()
{
	//投げている時のカウンターの最大値
	const float maxThrowCounter = 0.5f;

	//手前に動かすベクトル
	Vector3 backwardPower = { 0.0f,0.0f,1.0f };
	//手前に動かすベクトルの大きさ
	const float backwardPowerScale = 60000.0f;
	//手前に動かすベクトルを拡大する
	backwardPower.Scale(backwardPowerScale);
	//自身の回転で回す
	m_throwRot.Apply(backwardPower);

	//カウンターを調節する変数
	float counterAdjust = 0.0f;

	//縦に弾かれるか？
	if (!m_virticalRepelledFlag)
	{
		//弾かれない
		//通常の処理

		//二次関数的な動きにする
		backwardPower *= pow(static_cast<double>(m_throwCounter - maxThrowCounter / 2.0), 2.0);

		//モデルの回転を、逆さ向きに向かってちょっとずつ回す
		m_rotation.SetRotationDegX(180.0f * m_throwCounter / maxThrowCounter);
		m_rotation.Multiply(m_throwRot);
	}
	else
	{
		//弾かれる
		//跳ね返る処理

		//手前に弾くベクトルを弱くする
		float toWeaken = 0.042f;
		backwardPower.Scale(toWeaken);
		//二次関数的な動きにする
		backwardPower *= pow(static_cast<double>
			((maxThrowCounter * 2.0 - maxThrowCounter / 2.0) -
				(m_throwCounter - maxThrowCounter / 2.0))
			, 2.0);

		//下方向のベクトルを上方向にする
		test.Scale(-1.0f);
		//弱める
		toWeaken = 0.5f;
		test.Scale(toWeaken);
		//モデルの回転の値
		float rotValue = 90.0f - 360.0f *
			(m_throwCounter - maxThrowCounter / 2) / (maxThrowCounter * 2 - maxThrowCounter / 2);
		m_rotation.SetRotationDegX(rotValue);
		m_rotation.Multiply(m_throwRot);
		//カウンターを調節する時間にカウンターの最大値を入れる
		counterAdjust = maxThrowCounter;
	}

	//半分の時間が過ぎたら下に下げる
	if (m_throwCounter >= maxThrowCounter / 2.0f)
		backwardPower *= -1.0f;

	//モデルの場所を下に下げる
	m_position += (test + backwardPower) * GameTime().GetFrameDeltaTime();


	//投げている時のカウンターを進める
	m_throwCounter += GameTime().GetFrameDeltaTime();

	//投げている時のカウンターが最大値まで来たら
	if (m_throwCounter >= maxThrowCounter + counterAdjust)
	{
		//縦に弾かれるか？
		if (!m_virticalRepelledFlag)
		{
			//弾かれないとき（通常時）

			//モデルの回転を完全に逆さ向きに回す。
			m_rotation.SetRotationDegX(180.0f);
			m_rotation.Multiply(m_throwRot);
		}

		//プレイヤーがオブジェクトを持っていない状態にする
		//m_pPlayer->SetHoldObject(false);

		//投げている時のカウンターを0に戻す
		m_throwCounter = 0.0f;

		m_changeObject = true;

		//縦に弾かれるか？を初期化する
		m_virticalRepelledFlag = false;

		//ステートをクエリへ移行する
		m_objectState = enQuery;
	}
	//投げている時のカウンターが最大値の半分まで来たら
	else if (m_throwCounter >= maxThrowCounter / 2.0f && m_changeObject == true)
	{
		//表側か裏側か？
		int frontOrBackSide = CLevelObjectManager::enFrontSide;
		//自分がいる側の反対側にする
		if (GetFrontOrBackSide() == CLevelObjectManager::enFrontSide)
			frontOrBackSide = CLevelObjectManager::enBackSide;

		//キャパシティオーバーか？
		//自分がいない側のキャパシティを調べる
		//自分はまだそっち側にいないから、調整値に1を入れている
		if (IsCapacityOver(frontOrBackSide, 1))
			//キャパオーバーしてる
			m_virticalRepelledFlag = true;
		else
			//キャパオーバーではない
			//反転させる
			Reverse();

		m_changeObject = false;

	}
}

/// <summary>
/// 表側か裏側のキャパシティがオーバーしているか？
/// を調べる関数
/// 調整値には、自身はまだ対象の側にいないが、行ったとしたら
/// キャパシティはオーバーするのか？を調べるときに、1を入れる。
/// </summary>
/// <param name="frontOrBackSide">表側か裏側か？</param>
/// <param name="adjust">調整値</param>
/// <returns>オーバーしているか？</returns>
const bool CReversibleObject::IsCapacityOver(const int frontOrBackSide, const int adjust)
{
	//反転オブジェクトの表面と裏面のそれぞれの数
	const int* num = CLevelObjectManager::GetInstance()->GetReversibleObjectNum();
	//反転オブジェクトの表面と裏面のそれぞれの最大数
	const int* maxNum = CLevelObjectManager::GetInstance()->GetReversibleObjectMaxNum();
	//次の表面か裏面かの数
	int nextSideNum = num[frontOrBackSide] + adjust;
	//次の表面か裏面かの最大数
	int nextSideMaxNum = maxNum[frontOrBackSide];

	//数が最大数以下だったら
	return nextSideNum > nextSideMaxNum;
}

//void CReversibleObject::OverlapThrownSide()
//{
//	//投げている時のカウンターの最大値
//	const float maxThrowCounter = 0.5f;
//
//	//左方向に投げる
//	if (m_playerLeftOrRight == enLeft) {
//		//オブジェクトが横方向に移動するベクトルの作成
//		Vector3 dir = g_vec3Right;
//		m_throwRot.Apply(dir);
//		dir.Scale(700.0f);
//		m_position += dir * GameTime().GetFrameDeltaTime();
//
//		//投げ終わったオブジェクトが地面と良い感じの距離になるように調整する。
//		Vector3 dir2 = { 0.0f,-0.5f,0.0f };
//		m_throwRot.Apply(dir2);
//		dir2.Scale(450.0f);
//		m_position += dir2 * GameTime().GetFrameDeltaTime();
//
//		//
//		m_rotation.SetRotationDegZ(-360.0f * m_throwCounter / maxThrowCounter);
//		m_rotation.Multiply(m_throwRot);
//		m_throwCounter += GameTime().GetFrameDeltaTime();
//		m_pPlayer->SetHoldObject(false);
//
//		if (m_throwCounter >= maxThrowCounter)
//		{
//			m_rotation.SetRotationDegZ(0.0f);
//			m_rotation.Multiply(m_throwRot);
//			m_objectState = enQuery;
//			m_throwCounter = 0;
//		}
//	}
//	//右方向に投げる
//	else if (m_playerLeftOrRight == enRight) {
//		//オブジェクトが横方向に移動するベクトルの作成
//		Vector3 dir = g_vec3Left;
//		m_throwRot.Apply(dir);
//		dir.Scale(700.0f);
//		m_position += dir * GameTime().GetFrameDeltaTime();
//
//		//投げ終わったオブジェクトが地面と良い感じの距離になるように調整する。
//		Vector3 dir2 = { 0.0f,-0.5f,0.0f };
//		m_throwRot.Apply(dir2);
//		dir2.Scale(450.0f);
//		m_position += dir2 * GameTime().GetFrameDeltaTime();
//
//		//
//		m_rotation.SetRotationDegZ(360.0f * m_throwCounter / maxThrowCounter);
//		m_rotation.Multiply(m_throwRot);
//		m_throwCounter += GameTime().GetFrameDeltaTime();
//		m_pPlayer->SetHoldObject(false);
//
//		if (m_throwCounter >= maxThrowCounter)
//		{
//			m_rotation.SetRotationDegZ(0.0f);
//			m_rotation.Multiply(m_throwRot);
//			m_objectState = enQuery;
//			m_throwCounter = 0;
//		}
//	}
//}