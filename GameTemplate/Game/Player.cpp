#include "stdafx.h"
#include "Player.h"
#include "LightManager.h"
#include "LevelObjectManager.h"

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
	
	//OBBの頂点の座標の配列の先頭アドレスを取得
	Vector3* vertPos = m_myCharaCon.GetOBB().GetBoxVertex();
	//OBBの頂点の数だけ繰り返す
	for (int i = 0; i < m_myCharaCon.GetOBB().GetBoxVertexNum(); i++)
	{
		//OBBの頂点の座標を見るためのモデルの生成
		m_dbgObbModel[i] = NewGO<CModelRender>(0);
		m_dbgObbModel[i]->Init("Assets/modelData/dbgBox.tkm");
		m_dbgObbModel[i]->SetPosition(vertPos[i]);
	}
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
	for (int i = 0; i < m_myCharaCon.GetOBB().GetBoxVertexNum(); i++)
	{
		DeleteGO(m_dbgObbModel[i]);
	}
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

	//道の上の座標を移動させる
	m_onWayPosition += m_moveSpeed * 1.0 / 60.0f;

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
	Vector3 LpToPlayerVec = m_position - (*m_wayPointPos)[m_lpIndex];
	LpToPlayerVec.Normalize();
	//右のウェイポイントからプレイヤーへのベクトル
	Vector3 RpToPlayerVec = m_position - (*m_wayPointPos)[m_rpIndex];
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


	return;
}


/// <summary>
/// ステージに乗る
/// </summary>
void Player::GetOnStage()
{
	//プレイヤーのUpベクトルにYUpベクトルを入れる
	m_upVec = g_vec3AxisY;
	//Yupを補完済みの回転で回す
	m_finalWPRot.Apply(m_upVec);
	//Upベクトルをイイ感じに伸ばす。
	//伸ばした先がレイの始点となる
	m_upVec.Scale(150.0f);


	if (m_mobius)
	{
		//メビウスの輪が見つかっていたら
		//メビウスの輪のモデルのポリゴンと、レイの当たり判定を取る
		if (m_mobius->GetModel()
			->InIntersectLine(m_onWayPosition + m_upVec, m_onWayPosition - m_upVec))
		{
			//ポリゴンとレイの交差点をを取ってきてモデルの座標に入れる
			m_position = m_mobius->GetModel()->GetIntersectPos();
			//デバック用
			//後で消す
			//m_dbgHit = true;
		}
		else
		{
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
	//m_dbgModel->SetPosition(m_onWayPosition + m_upVec);
	//m_dbgModel2->SetPosition(m_onWayPosition - m_upVec);
	//m_dbgModel3->SetPosition(hitPos);
	//デバック用ここまで

	return;

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

	return;
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
	Vector3 dir = m_lightDirection;
	m_finalWPRot.Apply(dir);
	g_shadowMap->SetShadowParam(dir, 1000.0f, m_position);

	dir.Normalize();
	dir.Scale(1000.0f);
	Vector3 pos = m_position - dir;

	//デバック用
	//後で消す
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

	Vector3 dir = m_lightDirection;
	m_finalWPRot.Apply(dir);

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
	m_onWayPosition += m_moveSpeed * titleMoveSpeed * 1.0f / 60.0f;
	GetOnStage();

	//ライトのデータを更新する
	UpdateLightData();


	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
}

void Player::GameMove()
{
	if (m_operationFlag == false) {
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

	//道の上の座標を移動させる
	m_onWayPosition = m_myCharaCon.Execute(m_moveSpeed, 1.0 / 60.0f);
	//m_onWayPosition += m_moveSpeed * 1.0 / 60.0f;
	//ステージ（メビウスの輪）の上に乗る処理
	GetOnStage();

	//モデルの場所と回転を設定
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
	//OBBの場所と回転を設定
	m_myCharaCon.SetRotation(m_finalWPRot);
	//m_myCharaCon.GetOBB().SetPosition(m_position);

	//ライトのデータを更新する
	UpdateLightData();


	//デバック用
	//後で消す

	//OBBの頂点の座標の配列の先頭アドレスを取得
	Vector3* boxVertex = m_myCharaCon.GetOBB().GetBoxVertex();
	//OBBの頂点の数だけ繰り返す
	for (int i = 0; i < m_myCharaCon.GetOBB().GetBoxVertexNum(); i++)
	{
		//OBBの頂点を見るためのモデルの場所を設定
		m_dbgObbModel[i]->SetPosition(boxVertex[i]);
		m_dbgObbModel[i]->SetRotation(m_rotation);
	}
	//デバックここまで
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
