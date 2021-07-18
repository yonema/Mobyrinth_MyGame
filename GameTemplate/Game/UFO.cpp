#include "stdafx.h"
#include "UFO.h"

//スタート関数
bool CUFO::PureVirtualStart()
{
	//モデルレンダラーの生成と初期化
	m_modelRender = NewGO<CModelRender>(0);
	m_modelRender->Init("Assets/modelData/UFO2.tkm");
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);

	m_modelRender->SetDrawOutLineFlag(true);

	//アップベクトルを現在の回転に合わせる
	m_rotation.Apply(m_upVec);
	//ウェイポイント上の座標を更新する
	m_onWayPosition = m_position;

	//OBBを調節する
	SetOBBPivot({ 0.5f,0.0f,0.5f });
	SetOBBDirectionLength({ 10.0f,200.0f,400.0f });

	//上向きのスポットライトの生成と初期化
	//m_spotLightUp = NewGO<CSpotLight>(0);
	//アップベクトル
	Vector3 up = m_upVec;
	//イイ感じに伸ばす
	up.Scale(300.0f);
	//上向きのスポットライトのパラメータを設定
	//m_spotLightUp->SetPosition(m_position + up);
	//m_spotLightUp->SetColor({ 300.0f, 300.0f, 0.0f, 0.0f });
	//m_spotLightUp->SetRange(500.0f);
	//m_spotLightUp->SetAngle(/*90.0f*/3.14f * 0.5f);
	//m_spotLightUp->SetDirection(g_VEC3_DOWN);


	//下向きのスポットライトの生成と初期化
	up.Normalize();
	//イイ感じに伸ばす
	up.Scale(500.0f);
	//下向きのスポットライトのパラメータを設定
	/*m_spotLightDown = NewGO<CSpotLight>(0);
	m_spotLightDown->SetPosition(m_position + up);
	m_spotLightDown->SetColor({ 0.0f,0.0f,0.0f,1.0f });
	m_spotLightDown->SetRange(500.0f);
	m_spotLightDown->SetDirection(g_VEC3_DOWN);
	m_spotLightDown->SetAngle(3.14f * 0.25f);*/

	//UFOの光線のモデルの生成と初期化
	m_ufoLight = NewGO<CModelRender>(1);
	ModelInitData ufoLigInitData;
	ufoLigInitData.m_tkmFilePath = "Assets/modelData/ufoLight.tkm";
	ufoLigInitData.m_fxFilePath = "Assets/shader/ufoLight.fx";
	//ZPrepassで作成された深度テクスチャの登録
	ufoLigInitData.m_expandShaderResoruceView[1] = &g_graphicsEngine->GetZPrepassDepthTexture();
	m_ufoLight->Init(ufoLigInitData);
	m_ufoLight->SetPosition(m_position + up);
	m_ufoLight->SetRotation(m_rotation);
	m_ufoLight->SetDrawOutLineFlag(false);
	//最初は見えないように透明にする
	m_ufoLight->SetAlphaValue(modelRenderConstData::ALPHA_VALUE_TRANSPARENT);

	//UFOの着地点の生成と初期化
	m_ufoLandingPoint = NewGO<CUFOLandingPoint>(0);
	m_ufoLandingPoint->SetPosition(m_position);

	//他のレベルオブジェクトと衝突しなくする
	SetIsHitFlag(false);

	//UFOmoveSEのサウンドキューを生成する
	m_UFOmoveSE = NewGO<CSoundCue>(0);
	//UFOmoveSEのサウンドキューを、waveファイルを指定して初期化する。
	m_UFOmoveSE->Init(L"Assets/sound/UFOmove.wav");
	//音量調節
	m_UFOmoveSE->SetVolume(0.5f);

	//UFOcarrymoveSEのサウンドキューを生成する
	m_UFOcarrymoveSE = NewGO<CSoundCue>(0);
	//UFOcarrymoveSEのサウンドキューを、waveファイルを指定して初期化する。
	m_UFOcarrymoveSE->Init(L"Assets/sound/UFOcarrymove.wav");
	//音量調節
	m_UFOcarrymoveSE->SetVolume(0.5f);

	//UFOyellowlightのサウンドキューを生成する
	m_UFOyellowlightSE = NewGO<CSoundCue>(0);
	//UFOyellowlightのサウンドキューを、waveファイルを指定して初期化する。
	m_UFOyellowlightSE->Init(L"Assets/sound/UFOyellowlight.wav");
	//音量調節
	m_UFOyellowlightSE->SetVolume(0.5f);

	//UFOdiscoverySEのサウンドキューを生成する
	m_UFOdiscoverySE = NewGO<CSoundCue>(0);
	//UFOdiscoverySEのサウンドキューを、waveファイルを指定して初期化する。
	m_UFOdiscoverySE->Init(L"Assets/sound/UFOdiscovery.wav");
	//音量調節
	m_UFOdiscoverySE->SetVolume(0.5f);

	//UFOdiscoverySEのサウンドキューを生成する
	m_UFOredlightSE = NewGO<CSoundCue>(0);
	//UFOredlightのサウンドキューを、waveファイルを指定して初期化する。
	m_UFOredlightSE->Init(L"Assets/sound/UFOredlight.wav");
	//音量調節
	m_UFOredlightSE->SetVolume(0.5f);

#ifdef MY_DEBUG
	//デバック用
	//後で消す
	Vector3* vertPos = GetOBB().GetBoxVertex();
	for (int i = 0; i < m_vertNum; i++)
	{
		//OBBの頂点を見るためのモデル
		m_dbgVertPosMR[i] = NewGO<CModelRender>(0);
		m_dbgVertPosMR[i]->Init("Assets/modelData/dbgBox.tkm");
		m_dbgVertPosMR[i]->SetPosition(vertPos[i]);
		m_dbgVertPosMR[i]->SetRotation(m_rotation);

	}
	for (int i = 0; i < 2; i++)
	{
		//レイの始点と終点を見るためのモデル
		m_dbgRay[i] = NewGO<CModelRender>(0);
		m_dbgRay[i]->Init("Assets/modelData/dbgBox.tkm");
	}
	
	//スポットライトの位置を見るためのモデル
	m_dbgSpotLigPos = NewGO<CModelRender>(0);
	m_dbgSpotLigPos->Init("Assets/modelData/dbgBox.tkm");

#endif
	//デバック用ここまで

	return true;
}

//デストラクタ
CUFO::~CUFO()
{
	//LevelObjectManagerにこのオブジェクトは消すよ！って伝える。
	Delete();

	//モデルレンダラーの破棄
	DeleteGO(m_modelRender);

	//スポットライトの破棄
	//DeleteGO(m_spotLightUp);
	//DeleteGO(m_spotLightDown);
	//UFOの光線のモデルの破棄
	DeleteGO(m_ufoLight);

	//UFOの着地点の破棄
	DeleteGO(m_ufoLandingPoint);

	//UFOmoveSEの破棄
	DeleteGO(m_UFOmoveSE);

	//UFOcarrymoveSEの破棄
	DeleteGO(m_UFOcarrymoveSE);

	//UFOyellowlightSEの破棄
	DeleteGO(m_UFOyellowlightSE);

	//UFOredightSEの破棄
	DeleteGO(m_UFOredlightSE);

	//UFOdiscoverySEの破棄
	DeleteGO(m_UFOdiscoverySE);

#ifdef MY_DEBUG
	//デバック用
	//後で消す
	for (int i = 0; i < m_vertNum; i++)
	{
		//頂点を見るためのモデルの破棄
		DeleteGO(m_dbgVertPosMR[i]);
	}
	for (int i = 0; i < 2; i++)
	{
		//レイを見るためのモデルを破棄
		DeleteGO(m_dbgRay[i]);
	}
	//スポットライトを見るためのモデルを破棄
	DeleteGO(m_dbgSpotLigPos);
#endif
	//デバック用ここまで
}

//アップデート関数
void CUFO::PureVirtualUpdate()
{
	//移動処理
	Move();
	//回転を現在の座標に合わせてた回転にする
	CheckRotation();
	//アップベクトルを回転に合わせる
	m_upVec = g_VEC3_UP;
	m_rotation.Apply(m_upVec);
	m_upVec.Normalize();
	//ステージの上に乗る
	GetOnStage();


	//アップデートのステートで行動を割り振る
	switch (m_updateState)
	{
	case enSearch:
		//プレイヤーを探す処理
		Search();
		break;
	case enCapture:
		//プレイヤーを見つけた時の処理
		Capture();
		break;
	case enTransport:
		//プレイヤーを運ぶ処理
		Transport();
		break;
	case enLanding:
		//着地の処理
		Landing();
		break;
	case enLeave:
		//着地点から離れる処理
		Leave();
		break;
	}

	//ライトの更新
	UpdateLight();

	//モデルレンダラーの更新
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(Rotating());

	//デバック用
#ifdef MY_DEBUG
	//後で消す
	Vector3* vertPos = GetOBB().GetBoxVertex();
	for (int i = 0; i < m_vertNum; i++)
	{
		//頂点を見るためのモデルの更新
		m_dbgVertPosMR[i]->SetPosition(vertPos[i]);
		m_dbgVertPosMR[i]->SetRotation(m_rotation);

	}
#endif
	//デバック用ここまで
}

//プレイヤーがUFOに近づくと音を鳴らす
void CUFO::UFOmove()
{
	Vector3 distance = m_position - m_player->GetPosition();
	const float MaxDist = 1500;
	const float DistLen = distance.Length();

	if (DistLen < MaxDist) {
		float Le = MaxDist - DistLen;
		float SubLe = Le / MaxDist;
		float Vo = 2.0f * SubLe;

		//UFOmoveSEをループ再生をオンで再生する。
		m_UFOmoveSE->Play(true);

		//音量調節
		m_UFOmoveSE->SetVolume(Vo);

	}
	else {
		if (m_UFOmoveSE->IsPlaying()) {
			m_UFOmoveSE->Stop();
		}
	}
}

//プレイヤーがUFOyellowlightに近づくと音を鳴らす
void CUFO::UFOyellowlight()
{
	Vector3 distance = m_position - m_player->GetPosition();
	const float MaxDist = 1500;
	const float DistLen = distance.Length();

	if (DistLen < MaxDist) {
		float Le = MaxDist - DistLen;
		float SubLe = Le / MaxDist;
		float Vo = 2.0f * SubLe;

		//UFOyellowlightSEをループ再生をオンで再生する。
		m_UFOyellowlightSE->Play(true);

		//音量調節
		m_UFOyellowlightSE->SetVolume(Vo);

	}
	else {
		if (m_UFOyellowlightSE->IsPlaying()) {
			m_UFOyellowlightSE->Stop();
		}
	}
}

//プレイヤーを探す処理
void CUFO::Search()
{
	//if (g_pad[0]->IsPress(enButtonA))
	//{
	//	m_moveSpeed = m_defaultSpeed;
	//}
	//else if (g_pad[0]->IsPress(enButtonB))
	//{
	//	m_moveSpeed = -m_defaultSpeed;
	//}
	//else
	//	m_moveSpeed = 0.0f;

	//スピードが0.0fなら探さない
	if (m_moveSpeed == 0.0f)
		return;


	//タイマーにデルタタイムを加算
	m_timer += GameTime().GetFrameDeltaTime();

	//捜索フラグの切り替えと時間
	const float switchingTime = 1.5f;

	//切り替え時間になったら
	if (m_timer >= switchingTime)
	{
		//フラグを切り合える
		m_searchFlag = !m_searchFlag;
		//タイマーを0に戻す
		m_timer = 0.0f;
	}
	//m_searchFlag = true;
	//探索中か？
	if (m_searchFlag)
	{
		//捜索中なら

		//黄色に光る
		//m_modelRender->SetModelEmissionColor({ 0.5f,0.5f,0.0f,1.0f });
		//上下のスポットライトを光らせる
		//m_spotLightUp->SetColor({ 450.0f,450.0f,0.0f,1.0f });
		//m_spotLightDown->SetColor({ 450.0f,450.0f,0.0f,1.0f });
		//UFOの光線を光らせる
		m_ufoLight->SetModelEmissionColor({ 1.5f,1.5f,0.0f,1.0f });
		//UFOの光線を半透明にする
		m_ufoLight->SetAlphaValue(0.5f);
		//UFOyellowlightSE
		UFOyellowlight();
		//輪郭線を書くようにする
		m_ufoLight->SetDrawOutLineFlag(true);


		//プレイヤーを衝突しているか？
		if (IsHitPlayer())
		{
			//プレイヤーを見つけた

			//アップデートステートをプレイヤーを見つけた状態にする
			m_updateState = enCapture;

			//赤色に光る
			//m_modelRender->SetModelEmissionColor({ 1.0f,0.0f,0.0f,1.0f });
			//上下のスポットライトを光らせる
			//m_spotLightUp->SetColor({ 900.0f,0.0f,0.0f,1.0f });
			//m_spotLightDown->SetColor({ 900.0f,0.0f,0.0f,1.0f });
			//UFOの光線を光らせる
			m_ufoLight->SetModelEmissionColor({ 3.0f,0.0f,0.0f,1.0f });
			//UFOの光線を半透明にする
			m_ufoLight->SetAlphaValue(0.5f);
			//輪郭線を書くようにする
			m_ufoLight->SetDrawOutLineFlag(true);
			
			//プレイヤーをUFOに捕まった状態にする
			m_player->SetCapturedUFOFlag(true);
			m_player->SetRotation(m_rotation);
			m_player->SetCapturedRotation(m_rotation);
			//プレイヤーが何か持っていたら、離させる
			if (m_player->GetHoldObject())
				m_player->GetReversibleObject()->StateToCancel();
			//スピードを0にする
			m_moveSpeed = 0.0f;
			//タイマーを初期化する
			m_timer = 0.0f;

			//UFOyellowSEが鳴っていたら止める
			if (m_UFOyellowlightSE->IsPlaying()) {
				m_UFOyellowlightSE->Stop();
			}
			//UFOdiscoverySEをループ再生をオフで再生する。
			m_UFOdiscoverySE->Play(false);

			//UFOredlightSEをループ再生をオフで再生する。
			m_UFOredlightSE->Play(false);
		}
	}
	else
	{
		//捜索中ではない

		//光らない
		//m_modelRender->SetModelEmissionColor({ 0.0f,0.0f,0.0f,1.0f });
		//上下のスポットライトの光を消す
		//m_spotLightUp->SetColor({ 0.0f,0.0f,0.0f,1.0f });
		//m_spotLightDown->SetColor({ 0.0f,0.0f,0.0f,1.0f });
		//UFOの光線の光を消す
		m_ufoLight->SetModelEmissionColor({ 0.0f,0.0f,0.0f,1.0f });
		//UFOの光線の透明にする
		m_ufoLight->SetAlphaValue(modelRenderConstData::ALPHA_VALUE_TRANSPARENT);
		//輪郭線を書かないようにする
		m_ufoLight->SetDrawOutLineFlag(false);
		//UFOyellowSEが鳴っていたら止める
		if(m_UFOyellowlightSE->IsPlaying()) {
			m_UFOyellowlightSE->Stop();
		}

	}
}

//プレイヤーを見つけた時の処理
void CUFO::Capture()
{
	//UFOに見つかってびっくりして止まっている時間
	const float surprisedTimer = 1.0f;
	//タイマーの切り替え時間
	const float switchingTimer = surprisedTimer + 2.0f;

	//タイマーがびっくりしている時間より小さいか
	if (m_timer < surprisedTimer)
	{
		//小さいとき

		//プレイヤーはその場にとどまらせる
		m_player->SetCapturedPosition(m_player->GetPosition());
		m_player->SetCapturedRotation(m_player->GetRotation());
		//タイマーを進める
		m_timer += GameTime().GetFrameDeltaTime();

		//そのまま戻す
		return;
	}

	//アップベクトルを得る
	Vector3 upVec = m_upVec;
	//イイ感じに伸ばす
	const float upVecLen = 550.0f;
	upVec.Scale(upVecLen);

	//UFOに捕まる位置
	Vector3 capturePos = m_position + upVec;

	//プレイヤーに加えるベクトル
	Vector3 addVec = capturePos - m_player->GetPosition();
	//デルタタイムを掛けておく
	addVec.Scale(GameTime().GetFrameDeltaTime());
	//プレイヤーの座標を設定
	Vector3 pos = m_player->GetPosition() + addVec;
	m_player->SetPosition(pos);

	//プレイヤーに渡す回転
	Quaternion qRot;
	//横に向かせる
	Quaternion sideRot;
	sideRot.SetRotationDegY(90.0f);
	//くるくる回転させる回転
	Quaternion spiralRot;
	//タイマーにが進むにつれてスケールが0.0f～1.0fに変化する
	float timeScale = (m_timer - surprisedTimer) / (switchingTimer - surprisedTimer);
	//全部で3週するようにくるくる回す
	float spiralAngel = (360.0f * 3) * timeScale;
	spiralRot.SetRotationDegZ(spiralAngel);
	//横を向かせて
	qRot = sideRot;
	//くるくる回して
	qRot.Multiply(spiralRot);
	//本来の回転に合わせる
	qRot.Multiply(m_rotation);
	//プレイヤーに本来の回転を渡す
	m_player->SetRotation(m_rotation);
	//プレイヤーにUFOに捕まったとき限定の見た目だけの回転を渡す
	m_player->SetCapturedRotation(qRot);

	//タイマーの一定のところで終わる
	float endTimeScale = 0.95f;

	//タイマーにが進むにつれてスケールが0.0f～1.0fに変化する
	timeScale = (m_timer - surprisedTimer) / ((switchingTimer - surprisedTimer) * endTimeScale);
	//変化を1.0f～0.0fにする
	float scale = 1.0f - 1.0f * timeScale;
	//0.0fより小さいか
	if (scale < 0.0f)
		//小さいときは0.0fにする
		scale = 0.0f;

	//プレイヤーの拡大を設定する
	m_player->SetScale({ scale ,scale ,scale });

	//くるくる回す回転を入れる
	qRot = spiralRot;
	//本来の回転に合わせる
	qRot.Multiply(m_rotation);
	//プレイヤーの基点は足元だけど、お腹らへんを中心に回すためのオフセット
	Vector3 offsetVec = g_VEC3_UP;
	//回転でベクトルを回す
	qRot.Apply(offsetVec);
	//プレイヤーの半分の高さ
	const float playerHalfHeight = 150.0f;
	//プレイヤーの拡大に合わせてオフセットも拡大する
	offsetVec.Scale(-playerHalfHeight * scale);
	//プレイヤーにUFOに捕まったとき限定の見た目だけの座標を渡す
	m_player->SetCapturedPosition(m_player->GetPosition() + offsetVec);

	//タイマーが一定のところまでは
	if (m_timer - surprisedTimer < ((switchingTimer - surprisedTimer) * endTimeScale))
	{
		//UFOを横に膨らませる処理

		//タイマーに応じて0.0f～1.0fに変化する
		float scaleXZ = (m_timer - surprisedTimer) /
			((switchingTimer - surprisedTimer) * endTimeScale);
		//指数関数的に変化させる
		//徐々に大きくなるスピードが速くなる
		scaleXZ = pow(scaleXZ, 3.0f);
		//最大で1.5倍にする
		scaleXZ *= 0.5f;
		scaleXZ += 1.0f;
		//XZ方向に拡大する
		m_modelRender->SetScale({ scaleXZ ,1.0f ,scaleXZ });
	}
	else
	{
		//タイマーが一定以降

		//拡大の
		//分母
		float denominator = ((switchingTimer - surprisedTimer) * (1.0f - endTimeScale));
		//分子
		float numerator = m_timer - surprisedTimer -
			((switchingTimer - surprisedTimer) * endTimeScale);

		//徐々に元の倍率に戻す
		float scaleXZ = numerator / denominator;
		//最大の1.5倍から1.0に徐々に戻す
		scaleXZ *= 0.5f;
		scaleXZ = 1.5f - scaleXZ;
		//XZ方向に拡大する
		m_modelRender->SetScale({ scaleXZ ,1.0f ,scaleXZ });
	}

	if (m_timer >= switchingTimer)
	{
		//タイマーが切り替え時間になったら
		//タイマーを初期化して
		m_timer = 0.0f;
		//アップデートステートをプレイヤーを運ぶ処理状態にする
		m_updateState = enTransport;

		m_modelRender->SetScale({ 1.0f ,1.0f ,1.0f });

	}
	else
		//まだならタイマーを進める
		m_timer += GameTime().GetFrameDeltaTime();

}

//プレイヤーを運ぶ処理
void CUFO::Transport()
{
	//タイマーが0.0fの時に
	//つまり最初の一回だけ呼ばれる
	if (m_timer == 0.0f)
	{
		//UFOの回転速度を2倍にする
		m_ufoAngleSpeed = 200.0f;
		//上下のスポットライトの光を消す
		//m_spotLightUp->SetColor({ 0.0f,0.0f,0.0f,1.0f });
		//m_spotLightDown->SetColor({ 0.0f,0.0f,0.0f,1.0f });
		//UFOの光線の光を消す
		m_ufoLight->SetModelEmissionColor({ 0.0f,0.0f,0.0f,1.0f });
		//UFOの光線を透明にする
		m_ufoLight->SetAlphaValue(modelRenderConstData::ALPHA_VALUE_TRANSPARENT);
		//輪郭線を書かないようにする
		m_ufoLight->SetDrawOutLineFlag(false);

		//ウェイポイントの最大値
		const int maxWayPoint = 31;

		//現在の左側のウェイポイントの反対側のウェイポイントを出す。
		//0を含むため最大値に1加算してから、反対側を出すため半分にして、
		//現在の左側のウェイポイントにその値を加算している。
		int reverseLp = GetLeftWayPointIndex() + (maxWayPoint + 1) / 2;

		//右側から回って行くのか左側から行くのか決める
		if (GetLeftWayPointIndex() == m_ufoLandingPoint->GetLeftWayPointIndex())
		{
			//自身の左側のウェイポイントと、着地点の左側のウェイポイントが同じなら

			//右の単位方向ベクトル
			Vector3 rightVec = g_VEC3_RIGHT;
			//自身の回転を掛ける
			m_rotation.Apply(rightVec);
			//自身（UFO）から着地点へのベクトル
			Vector3 UFOToLandPointVec = m_ufoLandingPoint->GetPosition() - m_position;
			//正規化する
			UFOToLandPointVec.Normalize();
			//着地点へのベクトルと右のベクトルの内積を取る
			float landVecDotRightVec = Dot(UFOToLandPointVec, rightVec);
			//内積の正か負かで左右を振り分ける
			if (landVecDotRightVec >= 0.0f)
				//左に進む
				m_leftOrRight = enLeft;
			else
				//右に進む
				m_leftOrRight = enRight;
		}
		//反対側のウェイポイントが最大値を超えているか？
		else if (reverseLp > maxWayPoint)
		{
			//超えている時

			//最大値+1（0を含むため）を減算する
			reverseLp -= ( maxWayPoint + 1 );

			//着地点の位置を調べて、右から行くか左から行くかを振り分ける
			if (GetLeftWayPointIndex() < m_ufoLandingPoint->GetLeftWayPointIndex() ||
				m_ufoLandingPoint->GetLeftWayPointIndex() <= reverseLp)
				//着地点が、現在のウェイポイントよりも大きいか
				//反対側のウェイポイント以下だったら
				//左に進む
				m_leftOrRight = enLeft;
			else
				//それ以外なら
				//右に進む
				m_leftOrRight = enRight;
		}
		else
		{
			//超えていない時

			//着地点の位置を調べて、右から行くか左から行くかを振り分ける
			//if (m_ufoLandingPoint->GetLeftWayPointIndex() <= reverseLp)
			if (m_ufoLandingPoint->GetLeftWayPointIndex() >= GetLeftWayPointIndex())
			{
				//着地点が反対側のウェイポイント以下だったら
				//左に進む

				//着地点が自身のウェイポイント以上だったら
				//左に進む
				m_leftOrRight = enLeft;
			}
			else
			{
				//それ以外なら
				//右に進む
				m_leftOrRight = enRight;
			}
		}
	}

	//最初に運ぶ前に少し停止する時間
	const float stopTimer = 0.5f;
	if (m_timer < stopTimer)
	{
		//停止時間より小さい

		//タイマーを進める
		m_timer += GameTime().GetFrameDeltaTime();
		//何もせずに戻す
		return;
	}
	else
	{
		//移動速度をデフォルトの速度にする
		SetMoveSpeed();
	}

	//アップベクトルを得る
	Vector3 upVec = m_upVec;
	//イイ感じに伸ばす
	const float upVecLen = 550.0f;
	upVec.Scale(upVecLen);

	//UFOに捕まる位置
	Vector3 capturePos = m_position + upVec;

	//自身のOBBと着地点のOBBが衝突しているか？
	if (CollisionOBBs(GetOBB(), m_ufoLandingPoint->GetOBB()))
	{
		//衝突した時

		//アップデートステートを着地状態にする
		m_updateState = enLanding;
		//タイマーを初期化する
		m_timer = 0.0f;
		m_ufoAngleSpeed = 100.0f;

		//UFOcarrymoveSEが鳴っていたら止める
		if (m_UFOcarrymoveSE->IsPlaying()) {
			m_UFOcarrymoveSE->Stop();
		}
	}
	else
		//衝突していなかったら
		//タイマーを進める
		m_timer += GameTime().GetFrameDeltaTime();

	//プレイヤーの情報を更新する
	m_player->SetPosition(capturePos);
	m_player->SetCapturedPosition(capturePos);
	m_player->SetRotation(m_rotation);
	m_player->SetCapturedRotation(m_rotation);
	m_player->SetLeftPointIndex(GetLeftWayPointIndex());
	m_player->SetRightPointIndex(GetRightWayPointIndex());

	//UFOcarrymoveSEをループ再生オンで再生する
	m_UFOcarrymoveSE->Play(true);
}

//着地の処理
void CUFO::Landing()
{
	//膨らむ時間
	const float inflateTimer = 1.0f;
	//プレイヤーを吐き出す時間
	const float spitOutTimer = inflateTimer + 0.25f;
	//しばらく待つ時間
	const float waitTimer = spitOutTimer + 1.0f;

	//アップベクトルを得る
	Vector3 upVec = m_upVec;
	//イイ感じに伸ばす
	const float upVecLen = 550.0f;
	upVec.Scale(upVecLen);
	//UFOに捕まる位置
	Vector3 capturePos = m_position + upVec;
	//UFOに捕まる位置と着地点の少し上の間のベクトル
	Vector3 dist = (m_ufoLandingPoint->GetPosition() + upVec);
	dist -= capturePos;
	//距離を出す
	float distLen = dist.Length();
	//距離の最大値
	const float maxDistLen = 1000.0f;
	//拡大の最大値
	const float maxScale = 1.5f;

	//プレイヤーに渡す回転
	Quaternion qRot;
	//横を向かせる回転
	Quaternion qRotY;
	qRotY.SetRotationDegY(90.0f);
	//仰向けにする回転
	Quaternion qRotZ;
	qRotZ.SetRotationDegZ(90.0f);
	//横向きの回転を渡す
	qRot = qRotY;
	//仰向けの回転を渡す
	qRot.Multiply(qRotZ);
	//本来の回転に合わせる
	qRot.Multiply(m_rotation);
	
	//プレイヤーの基点は足元だけど、お腹らへんを中心に回すためのオフセット
	Vector3 offSet = /*g_VEC3_LEFT*/g_VEC3_RIGHT;
	//自身の回転で回す
	m_rotation.Apply(offSet);
	//プレイヤーの高さの半分
	const float playerHalfHeight = 150.0f;
	offSet.Scale(playerHalfHeight);

	//タイマーが0.0fか？
	//自身のOBBと着地点の端のOBBが衝突しているか？
	if (!CollisionOBBs(GetOBB(), m_ufoLandingPoint->GetSideOBB(m_leftOrRight)) && 
		m_timer == 0.0f)
	{
		//0.0fかつ
		//衝突していない時

		//移動速度をデフォルトにしてから
		SetMoveSpeed();

		//距離の最低値、スピードを一定以下にしないため
		float minDistLen = 200.0f;
		if (distLen < minDistLen)
			//距離が最低値未満だったら
			//最低値にしておく
			distLen = minDistLen;

		//距離が近いほど遅くする
		m_moveSpeed *= distLen / maxDistLen;
	}
	else 
	{
		//0.0fでない、または、
		//衝突している時

		//タイマーを進める
		m_timer += GameTime().GetFrameDeltaTime();

		//タイマーの進具合を調べる
		if (m_timer <= inflateTimer)
		{
			//膨らむ時間以下なら
			
			//スピードを0にする
			m_moveSpeed = 0.0f;

			//膨らむ時間の一定の時間まで
			const float endInflateTimerScale = 0.8f;

			//XZ方向の拡大
			float scaleXZ = 1.0f;
			scaleXZ += (maxScale - 1.0f) * (m_timer / (inflateTimer * endInflateTimerScale));
			if (scaleXZ > maxScale)
				scaleXZ = maxScale;

			//モデルをXZ方向に拡大
			m_modelRender->SetScale({ scaleXZ ,1.0f,scaleXZ });

		}
		else if (m_timer <= spitOutTimer)
		{
			//膨らむ時間より大きい、かつ
			//吐き出す時間以下

			//吐き出す時間の半分の時間
			const float halfSpitOutTimer = spitOutTimer / 2.0f;
			if (m_timer <= halfSpitOutTimer)
			{
				//タイマーが吐き出す時間の半分以下なら

				//タイマーに応じて0.0f～1.0fに変化する
				float timeScale = (m_timer - inflateTimer) / 
					(spitOutTimer - inflateTimer - halfSpitOutTimer);
				//XZ方向の拡大	//最大の拡大を入れる
				float scaleXZ = maxScale;
				//徐々に小さくする
				scaleXZ -= (maxScale - 1.0f) * timeScale;

				//XZ方向に拡大する
				m_modelRender->SetScale({ scaleXZ ,1.0f,scaleXZ });
			}
			else
			{
				//半分の時間を過ぎたら
				//元の大きさにする
				m_modelRender->SetScale({ 1.0f ,1.0f,1.0f });
			}

			//下へのベクトル
			Vector3 downVec = m_upVec;
			downVec.Scale(-1.0f);
			//移動先までベクトルを伸ばす
			const float nextPosDist = 500.0f;
			downVec.Scale(nextPosDist);

			//タイマーに応じて0.0f～1.0fに変化する
			float timeScale = (m_timer - inflateTimer) / (spitOutTimer - inflateTimer);
			//プレイヤーの座標をタイマーに応じて下げる
			downVec *= timeScale;
			capturePos += downVec;

			//プレイヤーの拡大をタイマーに応じて元の大きさに向かって大きくする
			m_player->SetScale({ timeScale ,timeScale ,timeScale });

			//オフセットもタイマーに応じて大きくする
			offSet.Scale(timeScale);
		}
		else if (m_timer <= waitTimer)
		{
			//タイマーが吐き出す時間より大きい、割
			//待ち時間以下

			//しばらく待つ
		}
		else
		{
			//待ち時間まで終わったら

			//アップデートステートを着地点から離れる処理状態にする
			m_updateState = enLeave;
		}
	}

	//タイマーが吐き出す時間以下か？
	if (m_timer <= spitOutTimer)
	{
		//吐き出す時間以下の時

		//プレイヤーのパラメーターを更新する
		m_player->SetPosition(capturePos);
		m_player->SetCapturedPosition(capturePos + offSet);
		m_player->SetRotation(m_rotation);
		m_player->SetCapturedRotation(qRot);
		m_player->SetWayPointState(GetLeftWayPointIndex());
		m_player->SetLeftPointIndex(GetLeftWayPointIndex());
		m_player->SetRightPointIndex(GetRightWayPointIndex());
	}
	else if (m_updateState == enLeave)
	{
		//アップデートパラメータがenLeave状態なら

		//タイマーを0にする
		m_timer = 0.0f;

	}

}

//着地点から離れる処理
void CUFO::Leave()
{
	//進む向きを右向きに戻す。
	m_leftOrRight = enRight;

	//タイマーが0.0fの時に
	//つまり最初の一回だけ呼ばれる
	if (m_timer == 0.0f)
	{
		//ウェイポイント上の座標に今の
		m_onWayPosition = m_position;

		//ステージの上に乗る処理を行うようにする
		m_getOnStageFlag = true;

		//ウェイポイントの最大数
		const int maxWayPointNum = 31;
		//次に進むウェイポイント
		int nextIndex = m_ufoLandingPoint->GetLeftWayPointIndex();
		if (m_leftOrRight == enLeft)
		{
			//左に動いたら、特に何もいじらない
		}
		else
		{
			//右に動いたので、次のウェイポイントを減算する

			nextIndex--;
			//0より小さかったら
			if (nextIndex < 0)
				//一周して最大値にする
				nextIndex = maxWayPointNum;
		}
		//自身の左側のウェイポイントを更新する
		SetLeftWayPointIndex(nextIndex);
	}

	//移動速度をデフォルトの速度にする
	SetMoveSpeed();
	//切り替えタイム
	const float switchingTime = 2.0f;

	//タイマーの調べる
	if (m_timer >= switchingTime)
	{
		//切り替え時間以上になったら

		//アップデートステートをプレイヤーを探す処理状態にする
		m_updateState = enSearch;
		//タイマーを初期化する
		m_timer = 0.0f;
		//プレイヤーをUFOに捕まっていない状態にする
		m_player->SetCapturedUFOFlag(false);

		if (m_UFOcarrymoveSE->IsPlaying()) {
			m_UFOcarrymoveSE->Stop();
		}
	}
	else
		//切り替え時間より小さかったら
		//タイマーを進める
		m_timer += GameTime().GetFrameDeltaTime();
}

//移動処理
void CUFO::Move()
{
	//次のウェイポイント
	int nextIndex = GetRightWayPointIndex();
	//最終的な移動速度	//移動速度にデルタタイムを掛ける
	float finalMoveSpeed = m_moveSpeed * GameTime().GetFrameDeltaTime();
	//ウェイポイント上の次の座標を計算する
	m_onWayPosition = CLevelObjectManager::GetInstance()->CalcWayPointNextPos
	(GetRightWayPointIndex(), m_onWayPosition, finalMoveSpeed, m_leftOrRight, &nextIndex);

	//ウェイポイントの最大数
	const int maxWayPointNum = 31;
	if (m_leftOrRight == enLeft)
	{
		//左に動いたら、特に何もいじらない
	}
	else
	{
		//右に動いたので、次のウェイポイントを加算する

		nextIndex++;
		//ウェイポイントの最大数より大きかったら
		if (nextIndex > maxWayPointNum)
			//一周して0にする
			nextIndex = 0;
	}
	//自身の左側のウェイポイントを更新する
	SetLeftWayPointIndex(nextIndex);
	//UFOmoveSE
	UFOmove();
}

//メビウスの輪のステージにイイ感じに合わせる処理
void CUFO::GetOnStage()
{
	if (!m_getOnStageFlag)
		return;
	//Upベクトルをイイ感じに伸ばす。
	//伸ばした先がレイの始点となる
	Vector3 upVec = g_VEC3_UP;
	m_rotation.Apply(upVec);
	upVec.Scale(500.0f);

	//デバック用
#ifdef MY_DEBUG
	//後で消す
	m_dbgRay[0]->SetPosition(m_onWayPosition + upVec);
	m_dbgRay[1]->SetPosition(m_onWayPosition - upVec);
#endif
	//デバック用ここまで
	if (m_mobius)
	{
		//メビウスの輪が見つかっていたら
		//メビウスの輪のモデルのポリゴンと、レイの当たり判定を取る
		m_mobius->GetIntersectPosWithMobius(
			m_onWayPosition + upVec, m_onWayPosition - upVec, &m_position
		);
	}
	else
	{
		//メビウスの輪（ステージ）が見つかっていなかったら
		//探してreturnする
		m_mobius = FindGO<CMobius>("Mobius");
		return;
	}
}

/// <summary>
/// 回転処理
/// </summary>
/// <returns>計算された回転</returns>
const Quaternion CUFO::Rotating()
{
	//UFOの回転の角度を増やす	//デルタタイムを掛けておく
	m_ufoAngle += m_ufoAngleSpeed * GameTime().GetFrameDeltaTime();

	//角度が360度より大きくか？
	if (m_ufoAngle > 360.0f)
	{
		//大きいときは

		//一周させる
		m_ufoAngle = m_ufoAngle - 360.0f;
	}

	//渡す回転
	Quaternion qRot;
	//Y軸中心にくるくる回す
	qRot.SetRotationDegY(-m_ufoAngle);
	//本来の回転にあわせる
	qRot.Multiply(m_rotation);

	return qRot;
}

//ライトの更新
void CUFO::UpdateLight()
{
	//ライトの更新
	Vector3 upVec = m_upVec;
	upVec.Scale(300.0f);
	//照らす位置
	//m_spotLightUp->SetAngle(10.0f);
	//m_spotLightUp->SetPosition(m_position + upVec);
	m_ufoLight->SetPosition(m_position);
	m_ufoLight->SetRotation(m_rotation);
	//照らす方向
	//m_spotLightUp->SetDirection(m_upVec);

	upVec = m_upVec;
	upVec.Scale(500.0f);
	//m_spotLightDown->SetPosition(m_position + upVec);
	Vector3 downVec = m_upVec;
	downVec.Scale(-1.0f);
	//m_spotLightDown->SetDirection(downVec);



#ifdef MY_DEBUG
	m_dbgSpotLigPos->SetPosition(m_position + upVec);
	m_dbgSpotLigPos->SetRotation(m_rotation);
#endif
}