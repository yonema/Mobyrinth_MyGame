#include "stdafx.h"
#include "ReversibleObject.h"



//スタート関数
bool CReversibleObject::PureVirtualStart()
{
	//モデルの回転を、現在の場所とイイ感じに合わせる
	CheckWayPoint();

	//changeSEのサウンドキューを生成する
	m_changeSE = NewGO<CSoundCue>(0);
	//changeSEのサウンドキューを、waveファイルを指定して初期化する。
	m_changeSE->Init(L"Assets/sound/change.wav");
	//音量調節
	m_changeSE->SetVolume(0.5f);

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

}


/// <summary>
/// 初期化関数
/// 最初に読んでね。trueを戻してね。
/// 表のモデルとそのタイプ、裏のモデルとそのタイプ
/// を設定する。
/// タイプ一覧はLevelObjectBase.hを参照
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
	m_modelRender[enFront]->Init(filePath_front);
	m_modelRender[enBack]->Init(filePath_back);
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
	case enThrownSide:	//持っているオブジェクトを横に投げる関数
		ThrownSide();
		break;
	case enQuery: //クエリしてほしいタイミング
		Query();
		break;
	case enOverlap: //障害オブジェクトに重なっているかの確認
		CheckObjectsOverlap();
		break;
	case enOverlapThrownDown:
		OverlapThrownDown();
		break;
	case enOverlapThrownSide:
		OverlapThrownSide();
		break;
	default:
		break;
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
			if (!m_pPlayer->GetHoldObject())
			{
				//ステートをプレイヤーに持たれている状態へ
				m_objectState = enHeldPlayer;
				//プレイヤーをオブジェクトを持ってる状態にする
				m_pPlayer->SetHoldObject(true);
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
	//オブジェクトを横に投げる処理
	else if (g_pad[0]->IsTrigger(enButtonX))
	{
		if (m_objectState != enThrownSide)
		{
			//CalcTargetPos();
			m_throwRot = m_pPlayer->GetFinalWPRot();
			//投げる位置をプレイヤーの少し上に設定
			Vector3 dir = { 0.0f,10.0f,0.0f };
			m_throwRot.Apply(dir);
			dir.Scale(7.0f);
			m_position += dir;
			//プレイヤーの左右の向きを調べる
			m_playerLeftOrRight = m_pPlayer->GetEnLeftOrRight();
		}
		//ステートを横に投げる状態へ
		m_objectState = enThrownSide;
		m_objectAction = enThrownSide;
	}
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
	dir.Scale(7.0f);

	test = dir;
	test.Scale(0.9f);

	//モデルの場所を下に下げる
	m_position += dir;
	//モデルの回転を、逆さ向きに向かってちょっとずつ回す
	m_rotation.SetRotationDegX(180.0f * m_throwCounter / maxThrowCounter);
	m_rotation.Multiply(m_throwRot);

	//投げている時のカウンターを進める
	m_throwCounter += GameTime().GetFrameDeltaTime();

	//投げている時のカウンターが最大値まで来たら
	if (m_throwCounter >= maxThrowCounter)
	{
		//モデルの回転を完全に逆さ向きに回す。
		m_rotation.SetRotationDegX(180.0f);
		m_rotation.Multiply(m_throwRot);

		//プレイヤーがオブジェクトを持っていない状態にする
		m_pPlayer->SetHoldObject(false);

		//投げている時のカウンターを0に戻す
		m_throwCounter = 0.0f;

		m_changeObject = true;

		//ステートをクエリへ移行する
		m_objectState = enQuery;
	}
	//投げている時のカウンターが最大値の半分まで来たら
	else if (m_throwCounter >= maxThrowCounter / 2.0f && m_changeObject == true)
	{
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
void CReversibleObject::ThrownSide()
{
	//投げている時のカウンターの最大値
	const float maxThrowCounter = 0.5f;

	//左方向に投げる
	if (m_playerLeftOrRight == enLeft) {
		//オブジェクトが横方向に移動するベクトルの作成
		Vector3 dir = g_vec3Right;
		m_throwRot.Apply(dir);
		dir.Scale(10.0f);
		m_position += dir;

		//投げ終わったオブジェクトが地面と良い感じの距離になるように調整する。
		Vector3 dir2 = { 0.0f,-0.5f,0.0f };
		m_throwRot.Apply(dir2);
		dir2.Scale(7.0f);
		m_position += dir2;

		//
		m_rotation.SetRotationDegZ(-360.0f * m_throwCounter / maxThrowCounter);
		m_rotation.Multiply(m_throwRot);
		m_throwCounter += GameTime().GetFrameDeltaTime();
		m_pPlayer->SetHoldObject(false);

		if (m_throwCounter >= maxThrowCounter)
		{
			m_rotation.SetRotationDegZ(0.0f);
			m_rotation.Multiply(m_throwRot);
			m_objectState = enQuery;
			m_throwCounter = 0;
		}
	}
	//右方向に投げる
	else if (m_playerLeftOrRight == enRight) {
		//オブジェクトが横方向に移動するベクトルの作成
		Vector3 dir = g_vec3Left;
		m_throwRot.Apply(dir);
		dir.Scale(10.0f);
		m_position += dir;

		//投げ終わったオブジェクトが地面と良い感じの距離になるように調整する。
		Vector3 dir2 = { 0.0f,-0.5f,0.0f };
		m_throwRot.Apply(dir2);
		dir2.Scale(7.0f);
		m_position += dir2;

		//
		m_rotation.SetRotationDegZ(360.0f * m_throwCounter / maxThrowCounter);
		m_rotation.Multiply(m_throwRot);
		m_throwCounter += GameTime().GetFrameDeltaTime();
		m_pPlayer->SetHoldObject(false);

		if (m_throwCounter >= maxThrowCounter)
		{
			m_rotation.SetRotationDegZ(0.0f);
			m_rotation.Multiply(m_throwRot);
			m_objectState = enQuery;
			m_throwCounter = 0;
		}
	}
	
}

/// <summary>
/// オブジェクトを横に投げる際の、投げる先を計算する関数
/// </summary>
//void CReversibleObject::CalcTargetPos()
//{
//	m_pPlayer->GetLeftPointIndex();
//}


/// <summary>
/// クエリしてほしいタイミングで呼ばれる関数
/// オーバーライドしてほしい関数、QuerySub()はここで呼ばれる。
/// enOverlapへステート（状態）を移行
/// </summary>
void CReversibleObject::Query()
{
	//オーバーライドしてほしい関数QuerySub()
	QuerySub();

	//ステートをプレイヤーに持たれるかどうか調べる状態に移行する
	m_objectState = enOverlap;
}





void CReversibleObject::CheckObjectsOverlap()
{
	if (m_flagCheckOverlap == false) {
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
		case enThrownSide:
			m_objectState = enOverlapThrownSide;

			if (m_playerLeftOrRight == enLeft) {
				m_playerLeftOrRight = enRight;
			}
			else if (m_playerLeftOrRight == enRight) {
				m_playerLeftOrRight = enLeft;
			}

			//投げる位置をプレイヤーの少し上に設定
			{
				Vector3 dir = { 0.0f,12.0f,0.0f };
				m_throwRot.Apply(dir);
				dir.Scale(7.0f);
				m_position += dir;
			}
			break;
		default:
			break;
		}

		return;
	}

	m_objectState = enCheckPlayer;
}


void CReversibleObject::OverlapThrownDown()
{
	//投げている時のカウンターの最大値
	const float maxThrowCounter = 0.5f;

	//モデルの場所を下に下げる
	m_position += test;
	//モデルの回転を、逆さ向きに向かってちょっとずつ回す
	m_rotation.SetRotationDegX(-180.0f * m_throwCounter / maxThrowCounter);
	m_rotation.Multiply(m_throwRot);

	//投げている時のカウンターを進める
	m_throwCounter += GameTime().GetFrameDeltaTime();

	//投げている時のカウンターが最大値まで来たら
	if (m_throwCounter >= maxThrowCounter)
	{
		//モデルの回転を完全に逆さ向きに回す。
		m_rotation.SetRotationDegX(0.0f);
		m_rotation.Multiply(m_throwRot);

		//プレイヤーがオブジェクトを持っていない状態にする
		m_pPlayer->SetHoldObject(false);

		//投げている時のカウンターを0に戻す
		m_throwCounter = 0.0f;

		m_changeObject = true;

		//ステートをクエリへ移行する
		m_objectState = enQuery;
	}
	//投げている時のカウンターが最大値の半分まで来たら
	else if (m_throwCounter >= maxThrowCounter / 2.0f && m_changeObject == true)
	{
		//反転させる
		Reverse();

		m_changeObject = false;
	}
}

void CReversibleObject::OverlapThrownSide()
{
	//投げている時のカウンターの最大値
	const float maxThrowCounter = 0.5f;

	//左方向に投げる
	if (m_playerLeftOrRight == enLeft) {
		//オブジェクトが横方向に移動するベクトルの作成
		Vector3 dir = g_vec3Right;
		m_throwRot.Apply(dir);
		dir.Scale(10.0f);
		m_position += dir;

		//投げ終わったオブジェクトが地面と良い感じの距離になるように調整する。
		Vector3 dir2 = { 0.0f,-0.5f,0.0f };
		m_throwRot.Apply(dir2);
		dir2.Scale(7.0f);
		m_position += dir2;

		//
		m_rotation.SetRotationDegZ(-360.0f * m_throwCounter / maxThrowCounter);
		m_rotation.Multiply(m_throwRot);
		m_throwCounter += GameTime().GetFrameDeltaTime();
		m_pPlayer->SetHoldObject(false);

		if (m_throwCounter >= maxThrowCounter)
		{
			m_rotation.SetRotationDegZ(0.0f);
			m_rotation.Multiply(m_throwRot);
			m_objectState = enQuery;
			m_throwCounter = 0;
		}
	}
	//右方向に投げる
	else if (m_playerLeftOrRight == enRight) {
		//オブジェクトが横方向に移動するベクトルの作成
		Vector3 dir = g_vec3Left;
		m_throwRot.Apply(dir);
		dir.Scale(10.0f);
		m_position += dir;

		//投げ終わったオブジェクトが地面と良い感じの距離になるように調整する。
		Vector3 dir2 = { 0.0f,-0.5f,0.0f };
		m_throwRot.Apply(dir2);
		dir2.Scale(7.0f);
		m_position += dir2;

		//
		m_rotation.SetRotationDegZ(360.0f * m_throwCounter / maxThrowCounter);
		m_rotation.Multiply(m_throwRot);
		m_throwCounter += GameTime().GetFrameDeltaTime();
		m_pPlayer->SetHoldObject(false);

		if (m_throwCounter >= maxThrowCounter)
		{
			m_rotation.SetRotationDegZ(0.0f);
			m_rotation.Multiply(m_throwRot);
			m_objectState = enQuery;
			m_throwCounter = 0;
		}
	}
}