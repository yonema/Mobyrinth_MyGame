#include "stdafx.h"
#include "Player.h"
#include "LightManager.h"
#include "LevelObjectManager.h"
#include "ReversibleObject.h"

//スタート関数
bool Player::Start()
{
	/////////////////////////////////////////////////////////
	//お試しプレイヤー
	/////////////////////////////////////////////////////////

	////アニメーションクリップの初期化
	////Idleのアニメーションクリップをロードする
	//m_animationClips[enAnimClip_Idle].Load("Assets/animData/idle.tka");
	////ループ再生をtrueにする
	//m_animationClips[enAnimClip_Idle].SetLoopFlag(true);
	////Runのアニメーションクリップをロードする
	//m_animationClips[enAnimClip_Run].Load("Assets/animData/walk.tka");
	////ループ再生をtrueにする
	//m_animationClips[enAnimClip_Run].SetLoopFlag(true);

	//モデルレンダラーを生成する
	m_modelRender = NewGO<CModelRender>(0);
	//モデルレンダラーの初期化をする
	//この時にアニメーションクリップを一緒に引数に渡しておく
	m_modelRender->Init
		/*("Assets/modelData/unityChan.tkm",m_animationClips,enAnimClip_Num, enModelUpAxisY);*/
		("Assets/modelData/player.tkm");

	/////////////////////////////////////////////////////////////
	//ここまで
	/////////////////////////////////////////////////////////////

	//モデルの場所と回転を設定
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
	//ウェイポイント上の座標にキャラの座標を入れておく
	m_onWayPosition = m_position;

	//ステージのメビウスの輪の参照を得る
	m_mobius = FindGO<Mobius>("Mobius");


	//初期化処理
	Init();

	//OBB初期化
	//OBBの初期化用データ構造体
	SInitOBBData initData;
	//なんかいい感じのOBBの大きさにする
	initData.width = 100.0f;
	initData.length = 200.0f;
	initData.height = 200.0f;
	//Playerの場所と回転を入れる
	initData.position = m_position;
	initData.rotation = m_rotation;
	//ピボットは底面の中央
	initData.pivot = { 0.5f,0.0f,0.5f };
	//キャラクターコントローラーを初期化する
	m_myCharaCon.Init(initData);

	//プレイヤーを照らすライトの方向
	m_lightDirection = { 1.0f,-1.0f,0.0f };

	//デバック用
	//後で消す

	////レイの視点と終点と交差点を見るためのデバック用モデルの生成
	//m_dbgModel = NewGO<CModelRender>(0);
	//m_dbgModel->Init("Assets/modelData/yuka.tkm");
	//m_dbgModel2 = NewGO<CModelRender>(0);
	//m_dbgModel2->Init("Assets/modelData/yuka.tkm");
	//m_dbgModel3 = NewGO<CModelRender>(0);
	//m_dbgModel3->Init("Assets/modelData/yuka.tkm");
	
	////OBBの頂点の座標の配列の先頭アドレスを取得
	//Vector3* vertPos = m_myCharaCon.GetOBB().GetBoxVertex();
	////OBBの頂点の数だけ繰り返す
	//for (int i = 0; i < m_myCharaCon.GetOBB().GetBoxVertexNum(); i++)
	//{
	//	//OBBの頂点の座標を見るためのモデルの生成
	//	m_dbgObbModel[i] = NewGO<CModelRender>(0);
	//	m_dbgObbModel[i]->Init("Assets/modelData/dbgBox.tkm");
	//	m_dbgObbModel[i]->SetPosition(vertPos[i]);
	//}

	//m_dbgStunMoveModel = NewGO<CModelRender>(0);
	//m_dbgStunMoveModel->Init("Assets/modelData/dbgBox.tkm");
	//m_dbgStunMoveModel->SetPosition(g_vec3Zero);


	//デバック用ここまで

	return true;
}

//デストラクタ
Player::~Player()
{
	//プレイヤーのモデルレンダラーの破棄
	DeleteGO(m_modelRender);

	//デバック用
	//後で消す
	//DeleteGO(m_dbgModel);
	//DeleteGO(m_dbgModel2);
	//DeleteGO(m_dbgModel3);
	//for (int i = 0; i < m_myCharaCon.GetOBB().GetBoxVertexNum(); i++)
	//{
	//	DeleteGO(m_dbgObbModel[i]);
	//}
	//for (int i = 0; i < 32; i++)
	//{
	//	for (int j = 0; j < 8; j++)
	//	{
	//		DeleteGO(m_dbgObbModel2[i][j]);
	//	}
	//}
	//デバック用ここまで

}

/// <summary>
/// プレイヤーの初期設定
/// </summary>
void Player::Init()
{
	//ゲームパッドの左スティックのX軸の入力情報を取得
	m_padLStickXF = 0.0f;

	//ウェイポイントの更新処理
	CheckWayPoint();
	//移動処理
	Move();
	//モデルの回転処理
	Rotation();

	//道の上の座標を移動させる	//デルタタイムを掛ける
	m_onWayPosition += m_moveSpeed * GameTime().GetFrameDeltaTime();

	//ステージ（メビウスの輪）の上に乗る処理
	//GetOnStage();

	//モデルの場所と回転を設定
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
}


/// <summary>
/// ウェイポイントの更新処理
/// </summary>
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
	m_rpIndex = m_wayPointState;	//右のウェイポイントはとm_wayPointStateは同じ値
	m_lpIndex = m_rpIndex + 1;		//左のウェイポイントは右のウェイポイントの1つ上の値

	if (m_lpIndex > m_maxWayPointState)
	{
		//左のウェイポイントがMAXより大きかったら
		//一周したということだから、スタートの0にする
		m_lpIndex = 0;
	}


	//2.m_wayPointStateの更新。

	//左のウェイポイントから右のウェイポイントへのベクトル
	Vector3 LpToRpVec = (*m_wayPointPos)[m_rpIndex] - (*m_wayPointPos)[m_lpIndex];
	LpToRpVec.Normalize();
	//左のウェイポイントからプレイヤーへのベクトル
	Vector3 LpToPlayerVec = /*m_position*/m_onWayPosition - (*m_wayPointPos)[m_lpIndex];
	LpToPlayerVec.Normalize();
	//右のウェイポイントからプレイヤーへのベクトル
	Vector3 RpToPlayerVec = /*m_position*/m_onWayPosition - (*m_wayPointPos)[m_rpIndex];
	RpToPlayerVec.Normalize();

	//左側の内積
	float LpDotPlayer = Dot(LpToRpVec, LpToPlayerVec);
	//右側の内積
	float RpDotPlayer = Dot(LpToRpVec, RpToPlayerVec);

	//デバック用
	//後で消す
	//m_dbgDot1 = LpDotPlayer;
	//m_dbgDot2 = RpDotPlayer;
	//デバック用ここまで
	if (!m_stunFlag)
	{
		//OBBで調べる
		//if (m_padLStickXF < 0.0f &&
		//	CollisionOBBs(m_myCharaCon.GetOBB(), m_wayPointOBB[m_lpIndex]))
		//{
		//	//今のウェイポイントの間から、左側に出ていった
		//	m_wayPointState += 1;
		//	if (m_wayPointState > m_maxWayPointState)
		//	{
		//		//m_wayPointStateがMAXより大きかったら
		//		//一周したということだから、スタートの0にする
		//		m_wayPointState = 0;
		//	}
		//}
		//else if (m_padLStickXF > 0.0f &&
		//	CollisionOBBs(m_myCharaCon.GetOBB(), m_wayPointOBB[m_rpIndex]))
		//{
		//	//今のウェイポイントの間から、右側から出ていった
		//	//m_wayPointStateを減算して右に進める。
		//	m_wayPointState -= 1;
		//	if (m_wayPointState < 0)
		//	{
		//		//m_wayPointStateが0より小さかったら
		//		//一周したということだから、MAXの値にする
		//		m_wayPointState = m_maxWayPointState;
		//	}
		//}

		//内積で調べる
		//左右のウェイポイントとの距離を調べる
		float f = 0.35f;
		if (LpDotPlayer > f && RpDotPlayer < -f)
		{
			//今のウェイポイントの間にいる
		}
		else if (LpDotPlayer <= f && m_padLStickXF < 0.0f)
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
		else if (RpDotPlayer >= -f && m_padLStickXF > 0.0f)
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
		//OBBで調べる
		//if (m_leftOrRight == enRight &&
		//	CollisionOBBs(m_myCharaCon.GetOBB(), m_wayPointOBB[m_lpIndex]))
		//{
		//	//今のウェイポイントの間から、左側に出ていった
		//	m_wayPointState += 1;
		//	if (m_wayPointState > m_maxWayPointState)
		//	{
		//		//m_wayPointStateがMAXより大きかったら
		//		//一周したということだから、スタートの0にする
		//		m_wayPointState = 0;
		//	}
		//}
		//else if (m_leftOrRight == enLeft &&
		//	CollisionOBBs(m_myCharaCon.GetOBB(), m_wayPointOBB[m_rpIndex]))
		//{
		//	//今のウェイポイントの間から、右側から出ていった
		//	//m_wayPointStateを減算して右に進める。
		//	m_wayPointState -= 1;
		//	if (m_wayPointState < 0)
		//	{
		//		//m_wayPointStateが0より小さかったら
		//		//一周したということだから、MAXの値にする
		//		m_wayPointState = m_maxWayPointState;
		//	}
		//}

		//内積で調べる
		//左右のウェイポイントとの距離を調べる
		float f = 0.35f;
		if (LpDotPlayer > f && RpDotPlayer < -f)
		{
			//今のウェイポイントの間にいる
		}
		else if (LpDotPlayer <= f && m_leftOrRight == enRight)
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
		else if (RpDotPlayer >= -f && m_leftOrRight == enLeft)
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
	
	//左右のウェイポイントとの距離を調べる
	//float f = 0.35f;
	//if (LpDotPlayer > f && RpDotPlayer < -f)
	//{
	//	//今のウェイポイントの間にいる
	//}
	//else if (LpDotPlayer <= f && m_padLStickXF < 0.0f)
	//{
	//	//今のウェイポイントの間から、左側に出ていった
	//	m_wayPointState += 1;
	//	if (m_wayPointState > m_maxWayPointState)
	//	{
	//		//m_wayPointStateがMAXより大きかったら
	//		//一周したということだから、スタートの0にする
	//		m_wayPointState = 0;
	//	}
	//}
	//else if (RpDotPlayer >= -f && m_padLStickXF > 0.0f)
	//{
	//	//今のウェイポイントの間から、右側から出ていった
	//	//m_wayPointStateを減算して右に進める。
	//	m_wayPointState -= 1;
	//	if (m_wayPointState < 0)
	//	{
	//		//m_wayPointStateが0より小さかったら
	//		//一周したということだから、MAXの値にする
	//		m_wayPointState = m_maxWayPointState;
	//	}
	//}

	return;

}


/// <summary>
/// 移動処理
/// </summary>
void Player::Move()
{
	//移動する向きは毎フレーム計算した方がいいのかな？
	//それとも、m_wayPointStateの切り替の時にした方がいいのかな？
	//いや、今のやり方だと毎フレームやらなくてはいけない気がする

	//1.左右への移動する方向を計算する。

	//左へ移動する方向
	Vector3 moveToLeft = (*m_wayPointPos)[m_lpIndex] - m_onWayPosition;
	moveToLeft.Normalize();
	//右へ移動する方向
	Vector3 moveToRight = (*m_wayPointPos)[m_rpIndex] - m_onWayPosition;
	moveToRight.Normalize();


	//2.移動処理

	//とりあえずの処理
	//重力や、加速度、抵抗を実装するときは別のやり方で
	m_moveSpeed = g_vec3Zero;

	//移動する長さ
	float moveLen = 1000.0f;


	//通常の移動処理
	if (!m_stunFlag)
	{
		//ゲームパッドのR1ボタンの入力情報を取得(ダッシュ状態)
		if (g_pad[0]->IsPress(enButtonRB1) == true) {
			moveLen = 3000.0f;
		}


		if (m_padLStickXF < 0.0f)
		{
			//左への移動の入力があったら
			//左への移動の計算する
			m_moveSpeed += moveToLeft * m_padLStickXF * -moveLen;
		}
		else if (m_padLStickXF > 0.0f)
		{
			//右への移動の入力があったら
			//右への移動の計算をする
			m_moveSpeed += moveToRight * m_padLStickXF * moveLen;
		}
	}
	//スタン中
	else
	{

		//スタン中の移動処理
		StunMove();
	}


	return;
}

/// <summary>
/// スタン中の移動処理
/// </summary>
void Player::StunMove()
{
	//スタン状態の最大時間
	const float maxTime = 3.0f;

	//スタン中タイマーが0.0fの時
	//つまりスタン中に、最初の一回ずつしか呼ばれない
	if (m_stunTimer == 0.0f)
	{
		//プレイヤーから衝突したOBBへのベクトル
		Vector3 playerToHitOBB = m_hitOBB->GetPosition() - m_onWayPosition;
		//右向きのベクトル
		Vector3 rightVec = g_vec3Right;
		//右向きのベクトルを回して、プレイヤーの右向きのベクトルにする
		m_finalWPRot.Apply(rightVec);
		//OBBへのベクトルと、右向きのベクトルの内積
		float hitOBBDotRight = Dot(playerToHitOBB, rightVec);
		//左に飛ばされるか、右に飛ばされるか
		int leftOrRight = enLeft;
		if (hitOBBDotRight >= 0.0f)
		{
			//内積が正だったら
			//反対側にする
			leftOrRight = enRight;
		}
		//最終的に吹っ飛ばされる先の座標を取ってくる
		m_stunMoveSpeed = CLevelObjectManager::GetInstance()->CalcWayPointNextPos
		(m_rpIndex, m_onWayPosition, 1000.0f, leftOrRight);
		//デバック用
		//後で消す
		//m_dbgStunMoveModel->SetPosition(m_stunMoveSpeed);
		//デバック用ここまで
		//現在の座標から移動先のベクトルを出す
		m_stunMoveSpeed -= m_onWayPosition;
		//少し小さくする
		m_stunMoveSpeed *= 0.7f;
		//スタン中に吹っ飛び中のフラグを立てる
		m_stunMoveFlag = true;
		//現在のUpベクトルをとってくる
		Vector3 upVec = m_upVec;
		//大きくする
		upVec.Scale(1000.0f);
		//吹っ飛ぶときの上下に動くベクトルに上向きのベクトルを入れる
		m_stunDownVec = upVec;
	}

	//吹っ飛び中のフラグが立っていたら
	if (m_stunMoveFlag)
	{
		//下に加えるベクトル、現在のUpベクトルを入れる
		Vector3 down = m_upVec;
		//Upベクトルを下向き大きくする
		down.Scale(-20.0f);
		down.Scale(90.0f);
		//デルタタイムを掛ける
		down.Scale(GameTime().GetFrameDeltaTime());
		//吹っ飛び中の上下に動くベクトルに下向きのベクトルを加える
		m_stunDownVec += down;
	}
	else
	{
		//吹っ飛び中のフラグが折れていたら
		//吹っ飛ぶ力をゼロにする
		m_stunMoveSpeed = g_vec3Zero;
		m_stunDownVec = g_vec3Zero;

	}

	//点滅処理
	//点滅の間隔	//スタン状態の最大時間のうち24回切り替わるようにする
	const float blinkInterval = maxTime / 24;
	//点滅タイマーが、点滅の間隔より小さかったら
	if (m_blinkTimer < blinkInterval)
	{
		//少し白く光らせる
		float color = 0.3f;
		m_modelRender->SetEmissionColor({ color,color,color,color });
	}
	//点滅タイマーが、点滅の間隔の2倍より小さかったら
	else if (m_blinkTimer < blinkInterval * 2)
	{
		//元の明るさに戻す
		float color = 0.0f;
		m_modelRender->SetEmissionColor({ color,color,color,color });
	}
	//点滅タイマーが、点滅の間隔の2倍を過ぎたら
	else
	{
		//点滅タイマーを0.0fに戻す
		m_blinkTimer = 0.0f;
	}

	//点滅タイマーを進める
	m_blinkTimer += GameTime().GetFrameDeltaTime();
	//スタン中のタイマーを進める
	m_stunTimer += GameTime().GetFrameDeltaTime();

	//スタン中のタイマーが、スタン中の最大時間を過ぎたら
	if (m_stunTimer >= maxTime)
	{
		//スタン中ではなくする
		m_stunFlag = false;
		//解除していたOBBを登録しなおす
		COBBWorld::GetInstance()->AddOBB(m_hitOBB);
		//衝突したOBBをnullptrにする
		m_hitOBB = nullptr;
		//スタン中タイマーをゼロにする
		m_stunTimer = 0.0f;
		//点滅タイマーをゼロにする
		m_blinkTimer = 0.0f;
		//明るさを元に戻す
		m_modelRender->SetEmissionColor({ 0.0f,0.0f,0.0f,0.0f });

	}

	//移動スピードにスタン中のスピードを加算する
	m_moveSpeed += m_stunMoveSpeed + m_stunDownVec;
}


/// <summary>
/// ステージに乗る
/// </summary>
void Player::GetOnStage()
{
	//Upベクトルをイイ感じに伸ばす。
	//伸ばした先がレイの始点となる
	Vector3 upVec = m_upVec;
	upVec.Scale(300.0f);

	if (m_mobius)
	{
		//メビウスの輪が見つかっていたら
		//メビウスの輪のモデルのポリゴンと、レイの当たり判定を取る
		if (m_mobius->GetModel()
			->InIntersectLine(m_onWayPosition + upVec, m_onWayPosition - upVec))
		{
			//ポリゴンとレイの交差点をを取ってきてモデルの座標に入れる
			m_position = m_mobius->GetModel()->GetIntersectPos();
			//デバック用
			//後で消す
			//m_dbgHit = true;
		}
		else
		{
			m_position = m_onWayPosition;
			//デバック用
			//後で消す
			//m_dbgHit = false;
		}
	}
	else
	{
		//メビウスの輪（ステージ）が見つかっていなかったら
		//探してreturnする
		m_mobius = FindGO<Mobius>("Mobius");
		return;
	}


	//デバック用
	//後で消す
	//レイの始点と終点と交差点を場所を入れる
	//auto hitPos = m_mobius->GetModel()->GetIntersectPos();
	//m_dbgModel->SetPosition(m_onWayPosition + upVec);
	//m_dbgModel2->SetPosition(m_onWayPosition - upVec);
	//m_dbgModel3->SetPosition(hitPos);
	//デバック用ここまで

	return;

}

/// <summary>
/// スタン中のステージに乗る処理
/// </summary>
void Player::StunGetOnStage()
{
	//現在のUpべクトル
	Vector3 upVec = m_upVec;
	//レイの長さ分大きくする
	upVec.Scale(300.0f);

	//メビウスの輪のモデルのポリゴンと、レイの当たり判定を取る
	if (m_mobius->GetModel()
		->InIntersectLine(m_onWayPosition + upVec, m_onWayPosition - upVec))
	{
		//ポリゴンとレイの交差点
		Vector3 intersectPos = m_mobius->GetModel()->GetIntersectPos();
		//交差点からプレイヤーの道上の座標のベクトル
		Vector3 intersectToOnWay = m_onWayPosition - intersectPos;
		//正規化する
		intersectToOnWay.Normalize();
		//さっきのベクトルとUpベクトルとの内積
		float vec1DotUp = Dot(intersectToOnWay, m_upVec);
		//内積が負なら
		if (vec1DotUp <= 0.0f)
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
}

/// <summary>
/// モデルの回転処理
/// </summary>
void Player::Rotation()
{
	//左のウェイポイントから右のウェイポイントへのベクトル
	Vector3 lpToRpLen = (*m_wayPointPos)[m_rpIndex] - (*m_wayPointPos)[m_lpIndex];

	//左のウェイポイントからプレイヤーへのベクトル
	Vector3 lpToPlayerLen = m_onWayPosition - (*m_wayPointPos)[m_lpIndex];

	//補完率
	float ComplementRate = lpToPlayerLen.Length() / lpToRpLen.Length();

	//球面線形補完
	m_finalWPRot.Slerp(ComplementRate, (*m_wayPointRot)[m_lpIndex], (*m_wayPointRot)[m_rpIndex]);

	//キャラクターの左右の向きに合わせて回転
	if (m_leftOrRight == enLeft)
	{
		m_rotation.SetRotationDegY(90.0f);
	}
	else if (m_leftOrRight == enRight)
	{
		m_rotation.SetRotationDegY(-90.0f);
	}
	m_rotation.Multiply(m_finalWPRot);

	//プレイヤーのUpベクトルにYUpベクトルを入れる
	m_upVec = g_vec3AxisY;
	//Yupを補完済みの回転で回す
	m_finalWPRot.Apply(m_upVec);
	m_upVec.Normalize();

	return;
}

/// <summary>
/// 衝突したOBBのタグを調べる
/// </summary>
void Player::CheckHitOBBTag()
{
	if (m_myCharaCon.GetTag() == COBB::enBigFire)
	{
		if (m_hitOBB != m_myCharaCon.GetHitOBB())
		{
			m_stunFlag = true;
			m_stunTimer = 0.0f;

			if (m_hitOBB)
				COBBWorld::GetInstance()->AddOBB(m_hitOBB);

			m_hitOBB = m_myCharaCon.GetHitOBB();
			COBBWorld::GetInstance()->RemoveOBB(m_hitOBB);
		}

	}
	else if (m_myCharaCon.GetTag() == COBB::enWall)
	{
		const COBB* hit = m_myCharaCon.GetHitOBB();
		Vector3 vec = hit->GetPosition() - m_position;
		vec.Normalize();
		Vector3 up = m_upVec;
		up.Normalize();
		float vecDotUp = Dot(up, vec);
		if (vecDotUp >= 0.8)
		{
			m_stunFlag = true;
			if (m_hitOBB)
				COBBWorld::GetInstance()->AddOBB(m_hitOBB);
			m_hitOBB = m_myCharaCon.GetHitOBB();
			COBBWorld::GetInstance()->RemoveOBB(m_hitOBB);
		}
	}
}

/// <summary>
/// ライトのデータを更新する
/// </summary>
void Player::UpdateLightData()
{
	//影を生成するライトの更新
	SetShadowParam();
	//ディレクションライトの更新
	SetDirectionLight();
}


/// <summary>
/// プレイヤーを照らす影を生成するライトを更新する
/// </summary>
void Player::SetShadowParam()
{
	//ライトの照らす方向
	Vector3 dir = m_lightDirection;
	//プレイヤー回転で方向をまわす
	m_finalWPRot.Apply(dir);
	//シャドウのつくるライトのパラメータを設定
	g_shadowMap->SetShadowParam(dir, 1000.0f, m_position);

	//デバック用
	//後で消す
	//dir.Normalize();
	//dir.Scale(1000.0f);
	//Vector3 pos = m_position - dir;
	//m_dbgModel3->SetPosition(pos);
}

/// <summary>
/// ディレクションライトを更新する
/// </summary>
void Player::SetDirectionLight()
{
	//ディレクションライトが見つかっていなかったら
	if (!m_gameDirectionLight)
	{
		//探す
		m_gameDirectionLight = FindGO<CDirectionLight>("GameDirectionLight");
		//まだ見つからなかったら何もせずにreturn
		if (!m_gameDirectionLight)
			return;
	}

	//ライトの照らす方向
	Vector3 dir = m_lightDirection;
	//プレイヤー回転で方向をまわす
	m_finalWPRot.Apply(dir);
	//ディレクションライトの方向を
	m_gameDirectionLight->SetDirection(dir);


}

//アップデート関数
void Player::Update()
{
	if (m_titleMove == true) {
		TitleMove();
	}
	else {
		GameMove();
	}
}

void Player::TitleMove()
{
	m_padLStickXF = 1.0f;

	//ウェイポイントの更新処理
	CheckWayPoint();
	//移動処理
	Move();
	//モデルの回転処理
	Rotation();

	//タイトルの時の移動は遅くする
	const float titleMoveSpeed = 0.5f;
	//デルタタイムを掛ける
	//m_onWayPosition += m_moveSpeed * titleMoveSpeed * GameTime().GetFrameDeltaTime();
	m_onWayPosition = 
		m_myCharaCon.Execute(m_moveSpeed * titleMoveSpeed, GameTime().GetFrameDeltaTime());
	GetOnStage();

	//ライトのデータを更新する
	UpdateLightData();


	//モデルの場所と回転を更新する
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);


}

void Player::GameMove()
{
	if (m_operationFlag == false) {
		return;
	}

	//UFOに捕まっているか？
	if (m_capturedUFOFlag == true)
	{
		//捕まっていたら

		//捕まっている時の処理
		CapturedUFO();

		//そのままreturn
		return;
	}

	//ゲームパッドの左スティックのX軸の入力情報を取得
	m_padLStickXF = g_pad[0]->GetLStickXF();

	//左右の向きを設定
	if (m_padLStickXF < 0.0f)
		m_leftOrRight = enLeft;		//左向き
	else if (m_padLStickXF > 0.0f)
		m_leftOrRight = enRight;	//右向き

	//ウェイポイントの更新処理
	CheckWayPoint();
	//移動処理
	Move();
	//モデルの回転処理
	Rotation();

	//道の上の座標を移動させる	//デルタタイムを掛ける
	m_onWayPosition = m_myCharaCon.Execute(m_moveSpeed, GameTime().GetFrameDeltaTime());
	//衝突したOBBのタグを調べる
	CheckHitOBBTag();

	//ステージ（メビウスの輪）の上に乗る処理
	if (!m_stunFlag)
		GetOnStage();
	else
		//スタン中のステージの上に乗る処理
		StunGetOnStage();



	//モデルの場所と回転を設定
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
	//OBBの場所と回転を設定
	m_myCharaCon.SetRotation(m_rotation);


	////左右の向き
	//if (!m_stunFlag)
	//{
	//	if (m_padLStickXF < 0.0f)
	//		m_myCharaCon.SetRotation((*m_wayPointRot)[m_lpIndex]);	//左向き
	//	else if (m_padLStickXF > 0.0f)
	//		m_myCharaCon.SetRotation((*m_wayPointRot)[m_rpIndex]);
	//}
	//else
	//{
	//	if (m_leftOrRight == enLeft)
	//		m_myCharaCon.SetRotation((*m_wayPointRot)[m_rpIndex]);
	//	else
	//		m_myCharaCon.SetRotation((*m_wayPointRot)[m_lpIndex]);
	//}

	//ライトのデータを更新する
	UpdateLightData();

	//デバック用
	//後で消す

	////OBBの頂点の座標の配列の先頭アドレスを取得
	//Vector3* boxVertex = m_myCharaCon.GetOBB().GetBoxVertex();
	////OBBの頂点の数だけ繰り返す
	//for (int i = 0; i < m_myCharaCon.GetOBB().GetBoxVertexNum(); i++)
	//{
	//	//OBBの頂点を見るためのモデルの場所を設定
	//	m_dbgObbModel[i]->SetPosition(boxVertex[i]);
	//	m_dbgObbModel[i]->SetRotation(m_rotation);
	//}
	//for (int i = 0; i < 32; i++)
	//{
	//	Vector3* vertPos2 = m_wayPointOBB[i].GetBoxVertex();
	//	for (int j = 0; j < 8; j++)
	//	{
	//		m_dbgObbModel2[i][j]->SetPosition(vertPos2[j]);
	//		m_dbgObbModel2[i][j]->SetRotation((*m_wayPointRot)[i]);
	//	}
	//}
	//デバックここまで
}

/// <summary>
/// UFOに捕まっている時の処理
/// </summary>
void Player::CapturedUFO()
{
	//ウェイポイントの更新処理
	//CheckWayPoint();
	m_onWayPosition = m_position;
	m_myCharaCon.GetOBB().SetPosition(m_position);
	//モデルの回転処理
	//Rotation();
	//モデルの場所と回転を設定
	m_modelRender->SetPosition(m_capturedPosition);
	m_modelRender->SetRotation(m_capturedRotation);
	m_finalWPRot = m_rotation;
	//OBBの場所と回転を設定
	m_myCharaCon.SetRotation(m_rotation);
	//ライトのデータを更新する
	UpdateLightData();
}


/// <summary>
/// ウェイポイントの「場所」を取得
/// </summary>
/// <param name="vecSize">ウェイポイントのサイズ</param>
/// <param name="posMap">場所のベクター</param>
void Player::SetWayPointPos
(const std::size_t vecSize, std::vector<Vector3>*const posMap)
{
	//ウェイポイントステートの最大の値を設定
	m_maxWayPointState = vecSize - 1;
	//m_wayPointPosにウェイポイントの「場所」を格納する
	m_wayPointPos = posMap;
	m_wayPointOBB.resize(m_wayPointPos->size());
	
}

/// <summary>
/// ウェイポイントの「回転」を取得
/// </summary>
/// <param name="vecSize">ウェイポイントのサイズ</param>
/// <param name="rotMap">回転のベクター</para
void Player::SetWayPointRot
(const std::size_t vecSize, std::vector<Quaternion>* rotMap)
{
	//ウェイポイントステートの最大の値を設定
	m_maxWayPointState = vecSize - 1;
	//m_wayPointRotにウェイポイントの「回転」を格納する
	m_wayPointRot = rotMap;
}

void Player::SetWayPointOBB()
{
	int vecSize = m_wayPointPos->size();
	m_wayPointOBB.resize(vecSize);
	for (int i = 0; i < vecSize; i++)
	{
		SInitOBBData initOBBData;
		initOBBData.position = (*m_wayPointPos)[i];
		initOBBData.rotation = (*m_wayPointRot)[i];
		initOBBData.width = 10.0f;
		initOBBData.length = 400.0f;
		initOBBData.height = 600.0f;
		initOBBData.pivot = { 0.5f,0.0f,0.5f };
		m_wayPointOBB[i].Init(initOBBData);

		Vector3* vert = m_wayPointOBB[i].GetBoxVertex();
		int vertNum = m_wayPointOBB[i].GetBoxVertexNum();

		//for (int v = 0; v < vertNum; v++)
		//{
		//	m_dbgObbModel2[i][v] = NewGO<CModelRender>(0);
		//	m_dbgObbModel2[i][v]->Init("Assets/modelData/dbgBox.tkm");
		//	m_dbgObbModel2[i][v]->SetPosition(vert[v]);
		//	m_dbgObbModel2[i][v]->SetRotation(initOBBData.rotation);
		//}

	}
}

//デバック用
//後で消す
////デバック用のフォントを表示するため
//void Player::PostRender(RenderContext& rc)
//{
//	//テキスト用意
//	wchar_t text[256];
//
//	//描画開始
//	m_font.Begin(rc);
//
//	//ウェイポイントステートの表示
//	swprintf(text, L"wayPointState:%02d", m_wayPointState);
//	//描画
//	m_font.Draw(text,
//		{ -600.0f, 300.0f },
//		{ 0.0f,0.0f,0.0f,1.0f },
//		0.0f,
//		1.0f,
//		{ 0.0f,0.0f }
//	);
//
//	//プレイヤーの左右のウェイポイントの表示
//	//左側のウェイポイント
//	swprintf(text, L"[%02d]", m_lpIndex);
//	m_font.Draw(text,
//		{ -110.0f, 50.0f },
//		{ 1.0f,0.0f,0.0f,1.0f },
//		0.0f,
//		1.0f,
//		{ 0.0f,0.0f }
//	);
//	//右側のウェイポイント
//	swprintf(text, L"[%02d]", m_rpIndex);
//	m_font.Draw(text,
//		{ 10.0f,50.0f },
//		{ 1.0f,0.0f,0.0f,1.0f },
//		0.0f,
//		1.0f,
//		{ 0.0f,0.0f }
//	);
//
//	//ステージとの当たり判定
//	swprintf(text, L"Hit%d", m_dbgHit);
//	m_font.Draw(text,
//		{ 110.0f, 150.0f },
//		{ 1.0f,0.0f,0.0f,1.0f },
//		0.0f,
//		1.0f,
//		{ 0.0f,0.0f }
//	);
//	//ステージとプレイヤーのポジションとの補完率
//	swprintf(text, L"rate%05f", m_mobius->GetModel()->getDbg());
//	m_font.Draw(text,
//		{ 110.0f, 120.0f },
//		{ 1.0f,0.0f,0.0f,1.0f },
//		0.0f,
//		1.0f,
//		{ 0.0f,0.0f }
//	);
//
//
//	//ウェイポイントの切り替え
//	swprintf(text, L"Right:%02.2f", m_dbgDot1);
//	m_font.Draw(text,
//		{ -310.0f, 150.0f },
//		{ 1.0f,0.0f,0.0f,1.0f },
//		0.0f,
//		1.0f,
//		{ 0.0f,0.0f }
//	);
//	swprintf(text, L"Left:%02.2f", m_dbgDot2);
//	m_font.Draw(text,
//		{ -310.0f, 120.0f },
//		{ 1.0f,0.0f,0.0f,1.0f },
//		0.0f,
//		1.0f,
//		{ 0.0f,0.0f }
//	);
//
//
//	//描画終了
//	m_font.End(rc);
//}
