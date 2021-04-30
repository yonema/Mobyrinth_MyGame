#include "stdafx.h"
#include "UFO.h"

//スタート関数
bool CUFO::PureVirtualStart()
{
	//モデルレンダラーの生成と初期化
	m_modelRender = NewGO<CModelRender>(0);
	m_modelRender->Init("Assets/modelData/UFO.tkm");
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);

	//アップベクトルを現在の回転に合わせる
	m_rotation.Apply(m_upVec);
	//ウェイポイント上の座標を更新する
	m_onWayPosition = m_position;

	//OBBを調節する
	GetOBB().SetPivot({ 0.5f,0.0f,0.5f });
	GetOBB().SetDirectionLength({ 250.0f,200.0f,400.0f });

	//スポットライトの生成と初期化
	m_spotLight = NewGO<CSpotLight>(0);
	Vector3 up = m_upVec;
	up.Scale(100.0f);
	m_spotLight->SetPosition({ 0.0f,2000.0f, 0.0f });
	m_spotLight->SetColor({ 300.0f, 300.0f, 0.0f, 0.0f });
	m_spotLight->SetRange(500.0f);
	up.Normalize();
	up.Scale(-1.0f);
	m_spotLight->SetDirection(g_vec3Down);

	//UFOの着地点の生成と初期化
	m_ufoLandingPoint = NewGO<CUFOLandingPoint>(0);
	m_ufoLandingPoint->SetPosition(m_position);


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
	//デバック用ここまで

	return true;
}

//デストラクタ
CUFO::~CUFO()
{
	//モデルレンダラーの破棄
	DeleteGO(m_modelRender);

	//スポットライトの破棄
	DeleteGO(m_spotLight);

	//UFOの着地点の破棄
	DeleteGO(m_ufoLandingPoint);

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
	m_upVec = g_vec3Up;
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
	m_modelRender->SetRotation(m_rotation);

	//デバック用
	//後で消す
	Vector3* vertPos = GetOBB().GetBoxVertex();
	for (int i = 0; i < m_vertNum; i++)
	{
		//頂点を見るためのモデルの更新
		m_dbgVertPosMR[i]->SetPosition(vertPos[i]);
		m_dbgVertPosMR[i]->SetRotation(m_rotation);

	}
	//デバック用ここまで
}

//プレイヤーを探す処理
void CUFO::Search()
{
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

	//探索中か？
	if (m_searchFlag)
	{
		//捜索中なら

		//黄色に光る
		m_modelRender->SetEmissionColor({ 0.5f,0.5f,0.0f,1.0f });
		m_spotLight->SetColor({ 300.0f,300.0f,0.0f,1.0f });

		//プレイヤーを衝突しているか？
		if (IsHitPlayer())
		{
			//プレイヤーを見つけた

			//アップデートステートをプレイヤーを見つけた状態にする
			m_updateState = enCapture;

			//赤色に光る
			m_modelRender->SetEmissionColor({ 1.0f,0.0f,0.0f,1.0f });
			m_spotLight->SetColor({ 600.0f,0.0f,0.0f,1.0f });
			
			//プレイヤーをUFOに捕まった状態にする
			m_pPlayer->SetCapturedUFOFlag(true);
			m_pPlayer->SetRotation(m_rotation);
			//プレイヤーが何か持っていたら、離させる
			if (m_pPlayer->GetHoldObject())
				m_pPlayer->GetReversibleObject()->StateToCancel();
			//スピードを0にする
			m_moveSpeed = 0.0f;
			//タイマーを初期化する
			m_timer = 0.0f;

		}
	}
	else
	{
		//捜索中ではない

		//光らない
		m_modelRender->SetEmissionColor({ 0.0f,0.0f,0.0f,1.0f });
		m_spotLight->SetColor({ 0.0f,0.0f,0.0f,1.0f });

	}
}

//プレイヤーを見つけた時の処理
void CUFO::Capture()
{
	//アップベクトルを得る
	Vector3 upVec = m_upVec;
	//イイ感じに伸ばす
	const float upVecLen = 500.0f;
	upVec.Scale(upVecLen);

	//UFOに捕まる位置
	Vector3 capturePos = m_position + upVec;

	//プレイヤーに加えるベクトル
	Vector3 addVec = capturePos - m_pPlayer->GetPosition();
	//デルタタイムを掛けておく
	addVec.Scale(GameTime().GetFrameDeltaTime());
	//プレイヤーの座標を設定
	m_pPlayer->SetPosition(m_pPlayer->GetPosition() + addVec);
	m_pPlayer->SetRotation(m_rotation);

	//タイマーの切り替え時間
	const float switchingTimer = 2.0f;
	if (m_timer >= switchingTimer)
	{
		//タイマーが切り替え時間になったら
		//タイマーを初期化して
		m_timer = 0.0f;
		//アップデートステートをプレイヤーを運ぶ処理状態にする
		m_updateState = enTransport;
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
			Vector3 rightVec = g_vec3Right;
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
			if (m_ufoLandingPoint->GetLeftWayPointIndex() <= reverseLp)
			{
				//着地点が反対側のウェイポイント以下だったら
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

		//移動速度をデフォルトの速度にする
		SetMoveSpeed();
	}

	//アップベクトルを得る
	Vector3 upVec = m_upVec;
	//イイ感じに伸ばす
	const float upVecLen = 500.0f;
	upVec.Scale(upVecLen);

	//UFOに捕まる位置
	Vector3 capturePos = m_position + upVec;

	//自身のOBBと着地点のOBBが衝突しているか？
	if (IsHitObject(*this, *m_ufoLandingPoint))
	{
		//衝突した時

		//アップデートステートを着地状態にする
		m_updateState = enLanding;
		//タイマーを初期化する
		m_timer = 0.0f;
	}
	else
		//衝突していなかったら
		//タイマーを進める
		m_timer += GameTime().GetFrameDeltaTime();

	//プレイヤーの情報を更新する
	m_pPlayer->SetPosition(capturePos);
	m_pPlayer->SetRotation(m_rotation);
	m_pPlayer->SetLeftPointIndex(GetLeftWayPointIndex());
	m_pPlayer->SetRightPointIndex(GetRightWayPointIndex());
}

//着地の処理
void CUFO::Landing()
{
	//下に下がる時間
	const float goDownTimer = 1.0f;
	//上に上がる時間
	const float goUpTimer = goDownTimer * 2.0f;

	//アップベクトルを得る
	Vector3 upVec = m_upVec;
	//イイ感じに伸ばす
	const float upVecLen = 500.0f;
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

	//タイマーが0.0fか？
	//自身のOBBと着地点の端のOBBが衝突しているか？
	if (!CollisionOBBs(GetOBB(), m_ufoLandingPoint->GetSideOBB(m_leftOrRight)) && 
		m_timer == 0.0f)
	{
		//0.0fかつ
		//衝突していない時

		//移動速度をデフォルトにしてから
		SetMoveSpeed();
		//距離が近いほど遅くする
		m_moveSpeed *= distLen / maxDistLen;
	}
	else 
	{
		//0.0fでない、または、
		//衝突している時

		//タイマーを進める
		m_timer += GameTime().GetFrameDeltaTime();

		//上下に動くベクトルの大きさ
		const float vecScale = 400.0f;

		//タイマーの進具合を調べる
		if (m_timer <= goDownTimer)
		{
			//下に下がる時間以下なら

			//ステージの上に乗る処理を行わないようにする
			m_getOnStageFlag = false;
			//移動速度を0にする
			m_moveSpeed = 0.0f;
			//下に下がるベクトル
			Vector3 goDownVec = m_upVec;
			goDownVec.Scale(-vecScale);
			//デルタタイムを掛けておく
			goDownVec.Scale(GameTime().GetFrameDeltaTime());
			//デルタタイムを掛けておく
			m_position += goDownVec;
		}
		else if (m_timer <= goUpTimer)
		{
			//下に下がる時間より大きい、かつ、
			//上に上がる時間以下なら

			//上に上がるベクトル
			Vector3 goUpVec = m_upVec;
			goUpVec.Scale(vecScale);
			//デルタタイムを掛けておく
			goUpVec.Scale(GameTime().GetFrameDeltaTime());
			//座標を上に上げる
			m_position += goUpVec;
		}
		else
		{
			//上下に動く時間が終わったら

			//アップデートステートを着地点から離れる処理状態にする
			m_updateState = enLeave;
		}
	}

	if (m_timer <= goDownTimer)
	{
		//タイマーが下に下がる時間以下のときだけ
		//プレイヤーのパラメーターを更新する
		m_pPlayer->SetPosition(capturePos);
		m_pPlayer->SetRotation(m_rotation);
		m_pPlayer->SetWayPointState(GetLeftWayPointIndex());
		m_pPlayer->SetLeftPointIndex(GetLeftWayPointIndex());
		m_pPlayer->SetRightPointIndex(GetRightWayPointIndex());
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
		m_pPlayer->SetCapturedUFOFlag(false);
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
}

//メビウスの輪のステージにイイ感じに合わせる処理
void CUFO::GetOnStage()
{
	if (!m_getOnStageFlag)
		return;
	//Upベクトルをイイ感じに伸ばす。
	//伸ばした先がレイの始点となる
	Vector3 upVec = g_vec3Up;
	m_rotation.Apply(upVec);
	upVec.Scale(500.0f);

	//デバック用
	//後で消す
	m_dbgRay[0]->SetPosition(m_onWayPosition + upVec);
	m_dbgRay[1]->SetPosition(m_onWayPosition - upVec);
	//デバック用ここまで
	if (m_mobius)
	{
		//メビウスの輪が見つかっていたら
		//メビウスの輪のモデルのポリゴンと、レイの当たり判定を取る
		if (m_mobius->GetModel()
			->InIntersectLine(m_onWayPosition + upVec, m_onWayPosition - upVec))
		{
			//ポリゴンとレイの交差点をを取ってきてモデルの座標に入れる
			m_position = m_mobius->GetModel()->GetIntersectPos();

		}
		else
		{
			m_position = m_onWayPosition;
		}
	}
	else
	{
		//メビウスの輪（ステージ）が見つかっていなかったら
		//探してreturnする
		m_mobius = FindGO<Mobius>("Mobius");
		return;
	}
}

//ライトの更新
void CUFO::UpdateLight()
{
	//ライトの更新
	Vector3 upVec = m_upVec;
	upVec.Scale(300.0f);
	//照らす位置
	m_spotLight->SetPosition(m_position + upVec);
	Vector3 downVec = m_upVec;
	downVec.Scale(-1.0f);
	//照らす方向
	m_spotLight->SetDirection(downVec);
}