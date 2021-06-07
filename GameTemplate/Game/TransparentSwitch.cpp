#include "stdafx.h"

#include "ROmizu_kori.h"

#include "TransparentSwitch.h"

//コンストラクタ
OOTransparentSwitch::OOTransparentSwitch()
{
	//先にカラーを設定する

	//信号の青の色
	Vector3 color = { 71.0f,225.0f,241.0f };
	//明るすぎるから正規化する
	color.Normalize();
	//こんどは暗すぎるからちょっと明るくする
	color.Scale(1.5f);
	//通常のフォントのカラーを設定
	m_normalColor = { color.x,color.y,color.z,1.0f };

	//点滅時のフォントのカラーを設定
	m_blinkColor = { 1.0f,0.0f,0.0f,1.0f };


	//ウェイポイントからの奥行の距離を設定
	SetZPosLen(100.0f);
}
//スタート関数
bool OOTransparentSwitch::StartSub()
{
	Init("Assets/modelData/transparentSwitch.tkm", enTransparentSwitch);

	//OBBWorldに自身のOBBの登録を消去させる
	COBBWorld::GetInstance()->RemoveOBB(&GetOBB());

	//透明オブジェクトを半透明にする。
	ChangeTransparent();

	//ボタンが押されたとき用のモデルレンダラーの生成と初期化
	m_modelRender = NewGO<CModelRender>(0);
	m_modelRender->Init("Assets/modelData/transparentSwitch2.tkm", D3D12_CULL_MODE_NONE);
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
	m_modelRender->SetDrawOutLineFlag(true);
	//非表示にする
	m_modelRender->Deactivate();

	//カメラを探す	//これは確実に見つける
	m_gameCamera = FindGO<CGameCamera>("GameCamera");

	//UFOを探す	//こっちは見つかるか分からない。
	m_ufo = FindGO<CUFO>("UFO");

	////フェードに使うスプライトの生成と初期化
	//m_fadeSR = NewGO<CSpriteRender>(0);
	//m_fadeSR->Init("Assets/Image/black.DDS", 1280.0f, 780.0f, { 0.5f,0.5f }, AlphaBlendMode_Trans);
	//m_fadeSR->SetPosition({ 0.0f,0.0f,0.0f });
	//m_fadeSR->SetPostRenderFlag(true);
	////透明にしておく
	//m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
	////非表示にする
	//m_fadeSR->Deactivate();

	//OBBのサイズを設定
	Vector3 obbSize;
	obbSize = { 100.0f,200.0f,600.0f };
	//OBBの方向ベクトルの長さを設定
	GetOBB().SetDirectionLength(obbSize);

	
	//m_buttonpushSEのサウンドキューを生成する
	m_buttonpushSE = NewGO<CSoundCue>(0);
	//m_buttonpushSEのサウンドキューを、waveファイルを指定して初期化する。
	m_buttonpushSE->Init(L"Assets/sound/buttonpush.wav");
	//音量調節
	m_buttonpushSE->SetVolume(0.5f);

	//m_activationSEのサウンドキューを生成する
	m_activationSE = NewGO<CSoundCue>(0);
	//m_activationSEのサウンドキューを、waveファイルを指定して初期化する。
	m_activationSE->Init(L"Assets/sound/activation.wav");
	//音量調節
	m_activationSE->SetVolume(0.5f);

	//m_invalidationSEのサウンドキューを生成する
	m_invalidationSE = NewGO<CSoundCue>(0);
	//m_activationSEのサウンドキューを、waveファイルを指定して初期化する。
	m_invalidationSE->Init(L"Assets/sound/invalidation.wav");
	//音量調節
	m_invalidationSE->SetVolume(0.5f);

	//m_activationエフェクトの作成
	m_activation = NewGO<Effect>(0);
	m_activation->Init(u"Assets/effect2/activation.efk");
	float scale = 200.0f;								//小さいので大きくしておく
	m_activation->SetScale({ scale ,scale ,scale });
	m_activation->SetPosition(m_position);				//座標を渡す
	m_activation->SetRotation(m_rotation);

	//ポーズを探す
	m_pause = FindGO<CPause>("Pause");
	
	//タイマーのサウンドの生成と初期化
	m_timerSC = NewGO<CSoundCue>(0);
	m_timerSC->Init(L"Assets/sound/timer.wav");
	m_timerSC->SetVolume(1.0f);

	//警告タイマーのサウンドの生成と初期化
	m_timerWarningSC = NewGO<CSoundCue>(0);
	m_timerWarningSC->Init(L"Assets/sound/timer_warning.wav");
	m_timerWarningSC->SetVolume(1.0f);

	return true;
}

//デストラクタ
OOTransparentSwitch::~OOTransparentSwitch()
{
	//モデルレンダラーの破棄
	DeleteGO(m_modelRender);

	//m_activationSEの削除
	DeleteGO(m_activationSE);

	//m_buttonpushSEの削除
	DeleteGO(m_buttonpushSE);

	//タイマーのサウンドの削除
	DeleteGO(m_timerSC);

	//警告タイマーのサウンドの削除
	DeleteGO(m_timerWarningSC);

	DeleteGO(m_invalidationSE);

	DeleteGO(m_activation);
}

//アップデート関数
void OOTransparentSwitch::UpdateSub()
{
	//スイッチの切り替え中か？
	if (m_switchingFlag)
	{
		//切り替え中

		//透明、実体化を切り替える処理
		Switching();
		return;
	}

	//スイッチのタイマーをカウントするか
	if (m_timerCountFlag)
	{
		//カウントをする


		//リセットタイマーが０になったとき
		//透明オブジェクトをすべて持ち上げられないようにする。
		//透明オブジェクトを持っている場合、そのオブジェクトを持っていない状態にする。
		//透明オブジェクトを初期位置に戻す。
		if (m_flagSwitchOn == true) {
			m_resetTimer -= GameTime().GetFrameDeltaTime();

			//タイマーのフォントの更新
			UpdateTimerFR();
			///--m_resetTimer;

			//警告のタイム
			const float warningTime = 3.1f;

			//タイマーが警告のタイム以下か？
			if (m_resetTimer <= warningTime)
			{
				//以下の時

				//警告タイマーのサウンドを流す
				m_timerWarningSC->Play(true);

				//通常のタイマーのサウンドが流れていたら
				if (m_timerSC->IsPlaying())
					//停止する
					m_timerSC->Stop();
			}
			else
			{
				//より大きい

				//通常のタイマーのサウンドを流す
				m_timerSC->Play(true);
			}

			if (m_resetTimer <= 0.0f) {
				m_flagSwitchOn = false;


				//透明オブジェクトを半透明にする。
				ChangeTransparent();

				//押されていない時のモデルレンダラーを表示する
				GetModelRender()->Activate();
				//押されたときのモデルレンダラーを非常時にする
				m_modelRender->Deactivate();

				//m_invalidationSEをループ再生をオフで再生する。
				m_invalidationSE->Play(false);

			}
		}
		//リセットタイマーが０のときに下の文の処理を作動させる。
		else if (m_flagSwitchOn == false) {

			//タイマーのサウンドが流れていたら
			if (m_timerSC->IsPlaying())
				//タイマーのサウンドを停止する
				m_timerSC->Stop();
			//警告タイマーのサウンドが流れていたら
			if (m_timerWarningSC->IsPlaying())
				//警告タイマーのサウンドを停止する
				m_timerWarningSC->Stop();


			//スイッチが押されたとき
			//透明オブジェクトをすべて持ち上げられるようにする。
			//スイッチのオブジェクトの範囲内でAボタンが押されたとき
			if (IsHitPlayer() && g_pad[0]->IsTrigger(enButtonA)) {

				//切り替え中フラグを立てる
				m_switchingFlag = true;
				//切り替え中タイマーを初期化する
				m_switchingTimer = 0.0f;



				//プレイヤーを操作不能にする
				m_pPlayer->SetOperationFlag(false);

				//UFOがあるか？
				if (m_ufo)
					//あるとき
					//UFOを動かなくする
					m_ufo->SetMoveSpeed(0.0f);

				//ポーズできなくする
				m_pause->SetCanPause(false);

				//リセットタイマーに開始する値を代入
				m_resetTimer = m_resetTimerStartValue;
				//押されていない時のモデルレンダラーを非表示にする
				GetModelRender()->Deactivate();
				//押されたときのモデルレンダラーを表示する
				m_modelRender->Activate();

				//m_fadeSR->Activate();

				//m_buttonpushSEをループ再生をオフで再生する。
				m_buttonpushSE->Play(false);
			}
		}
	}
	else
	{
		//カウントをしない

		//タイマーのサウンドが流れていたら
		if (m_timerSC->IsPlaying())
			//止める
			m_timerSC->Stop();
		//警告タイマーのサウンドが流れていたら
		if (m_timerWarningSC->IsPlaying())
			//止める
			m_timerWarningSC->Stop();
	}
}

/// <summary>
/// 透明、実体化を切り替える処理
/// </summary>
void OOTransparentSwitch::Switching()
{
	const float startWaitTime = 0.5f;							//最初の待つ時間		 
	const float startFadeOutTime = startWaitTime + 0.5f;		//最初のフェードアウト時間
	const float startFadeWaitTime = startFadeOutTime + 0.5f;	//最初のフェード中間時間
	const float startFadeInTime = startFadeWaitTime + 0.5f;		//最初のフェードイン時間
	const float switchingTime = startFadeInTime + 2.0f;			//切り替え中時間
	const float endFadeOutTime = switchingTime + 0.5f;			//終わりのフェードアウト時間
	const float endFadeWaitTime = endFadeOutTime + 0.5f;		//終わりのフェード中間時間
	const float endFadeInTime = endFadeWaitTime + 0.5f;			//終わりのフェードイン時間

	//切り替え中のタイマーの経過を調べる
	if (m_switchingTimer < startWaitTime)
	{
		//最初の待つ時間

		//何もせずに待つ
	}
	else if (m_switchingTimer < startFadeOutTime)
	{
		//フェードアウトしていく

		//アルファ値
		float alphaValue = 1.0f;
		//タイマーに経過具合によって補完
		float timeScale = (m_switchingTimer - startWaitTime) / (startFadeOutTime - startWaitTime);
		alphaValue *= timeScale;
		//フェードを徐々に暗くしていく
		//m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,alphaValue });
		g_graphicsEngine->GetSceneChange().GetFadeSprite()->SetMulColor({ 1.0f,1.0f,1.0f,alphaValue });
	}
	else if (m_switchingTimer < startFadeWaitTime)
	{
		//フェードアウトとフェードインの間
		//何もせずに待つ

		//フェードは真っ暗
		//m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });
		g_graphicsEngine->GetSceneChange().GetFadeSprite()->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });


		//カメラがプレイヤーを見なくする
		m_gameCamera->SetLookPlayerFlag(false);

		//カメラをリフレッシュして、一時的にばねカメラを無効にし、
		//一気にカメラを移動できるようにする
		m_gameCamera->Refresh();
		//カメラへの座標
		Vector3 toCameraPos = { 0.0f,0.0f,3500.0f };
		//カメラの始点を設定
		m_gameCamera->SetPosition(toCameraPos);
		//カメラの注視点を設定
		m_gameCamera->SetTarget({ 0.0f,0.0f,0.0f });

	}
	else if (m_switchingTimer < startFadeInTime)
	{
		//フェードインしていく

		//アルファ値
		float alphaValue = 1.0f;
		//タイマーに経過具合によって補完
		float timeScale = (m_switchingTimer - startFadeWaitTime) / (startFadeInTime - startFadeWaitTime);
		alphaValue -= 1.0f * timeScale;
		//フェードを徐々に明るくしていく
		//m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,alphaValue });
		g_graphicsEngine->GetSceneChange().GetFadeSprite()->SetMulColor({ 1.0f,1.0f,1.0f,alphaValue });
	}
	else if (m_switchingTimer < switchingTime)
	{
		//透明オブジェクトの切り替え中

		//切り替え中時間の半分の時間
		const float halfSwitchingTime = 
			startFadeInTime + (switchingTime - startFadeInTime) / 2.0f;

		//半分の時間より小さいか？
		if (m_switchingTimer < halfSwitchingTime)
		{
			//小さいとき

			//フェードを透明にする
			//m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
			g_graphicsEngine->GetSceneChange().GetFadeSprite()->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
		}
		else
		{
			//大きいとき
			
			//一つ目のタイマーのフォントは有効化ではないか？
			if (!m_timerFRIsActive)
			{
				//有効化ではない


				//点滅タイマーを初期化する
				m_blinkTimer = FLT_MAX;
				//フォントのカラーを通常時のカラーにする
				m_fontColor = m_normalColor;

				//透明オブジェクトを実体にする。
				ChangeEntity();
				//m_activationSEをループ再生をオフで再生する。
				m_activationSE->Play(false);
			}
			else
			{
				//有効化である

				//何もせずに待つ
			}
		}
	}
	else if (m_switchingTimer < endFadeOutTime)
	{
		//フェードアウトしていく

		//アルファ値
		float alphaValue = 1.0f;
		//タイマーに経過具合によって補完
		float timeScale = (m_switchingTimer - switchingTime) / (endFadeOutTime - switchingTime);
		alphaValue *= timeScale;
		//フェードを徐々に暗くしていく
		//m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,alphaValue });
		g_graphicsEngine->GetSceneChange().GetFadeSprite()->SetMulColor({ 1.0f,1.0f,1.0f,alphaValue });
	}
	else if (m_switchingTimer < endFadeWaitTime)
	{
		//フェードアウトとフェードインの間
		//何もせずに待つ

		//フェードは真っ暗
		//m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });
		g_graphicsEngine->GetSceneChange().GetFadeSprite()->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });
		//カメラがプレイヤーを見るようにする
		m_gameCamera->SetLookPlayerFlag(true);

		//カメラをリフレッシュして、一時的にばねカメラを無効にし、
		//一気にカメラを移動できるようにする
		m_gameCamera->Refresh();
	}
	else if (m_switchingTimer < endFadeInTime)
	{
		//フェードインしてく

		//アルファ値
		float alphaValue = 1.0f;
		//タイマーに経過具合によって補完
		float timeScale = (m_switchingTimer - endFadeWaitTime) / (endFadeInTime - endFadeWaitTime);
		alphaValue -= 1.0f * timeScale;
		//フェードを徐々に明るくしていく
		//m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,alphaValue });
		g_graphicsEngine->GetSceneChange().GetFadeSprite()->SetMulColor({ 1.0f,1.0f,1.0f,alphaValue });

		//タイマーのフォントの更新
		UpdateTimerFR();
	}
	else
	{
		//終了

		//切り替え中フラグを折る
		m_switchingFlag = false;
		//切り替え中のタイマーを初期化する
		m_switchingTimer = 0.0f;
		//フェードを透明にする
		//m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
		g_graphicsEngine->GetSceneChange().GetFadeSprite()->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
		//フェードを非表示にする
		//m_fadeSR->Deactivate();

		//スイッチオンフラグを立てる
		m_flagSwitchOn = true;

		//プレイヤーを操作可能にする
		m_pPlayer->SetOperationFlag(true);

		//UFOがあるか？
		if (m_ufo)
			//あるとき
			//UFOを動かす
			m_ufo->SetMoveSpeed();

		//ポーズできるようにする
		m_pause->SetCanPause(true);

	}

	//切り替え中か？
	if (m_switchingFlag)
		//切り替え中
		//切り替え中タイマーを進める
		m_switchingTimer += GameTime().GetFrameDeltaTime();

}



/// <summary>
/// タイマーのフォントの更新
/// </summary>
void OOTransparentSwitch::UpdateTimerFR()
{
	//レベルオブジェクトを取ってくる
	std::vector<ILevelObjectBase*> levelObjects
		= CLevelObjectManager::GetInstance()->GetLevelObjects();


	//点滅し始めるタイム
	const float blinkStartTime = 3.1f;

	//点滅し始めるタイムよりタイマーが小さいか？
	if (m_resetTimer <= blinkStartTime)
	{
		//小さい
		//点滅させる

		//点滅するタイム
		const float blinkTime = 0.1f;

		//点滅タイマーが点滅するタイムより大きいか？
		if (m_blinkTimer > blinkTime)
		{
			//大きかったら
			//点滅させる

			//フォントのカラーは通常のカラーか？
			if (m_fontColor.x == m_normalColor.x)
				//通常のカラーなら
				//点滅時のカラーにする
				m_fontColor = m_blinkColor;
			else
				//違うなら
				//通常のカラーにする
				m_fontColor = m_normalColor;

			//点滅タイマーを0.0fにする
			m_blinkTimer = 0.0f;
		}
		else
		{
			//小さいとき

			//点滅タイマーを進める
			m_blinkTimer += GameTime().GetFrameDeltaTime();
		}
	}

	//全てのレベルオブジェクトに検索
	for (int i = 0; i < levelObjects.size(); i++)
	{
		//透明オブジェクトではない場合、次のオブジェクトを検索
		if (levelObjects[i]->GetFlagTransparentObject() == false) {
			continue;
		}

		//タイマーのフォントのパラメーターを設定する
		SetTimerFRParam(levelObjects[i]);


	}
}

/// <summary>
/// タイマーのフォントのパラメーターを設定する
/// </summary>
/// <param name="itr">タイマーのフォントのイテレーター</param>
/// <param name="levelObject">対応する透明オブジェクト</param>
void OOTransparentSwitch::SetTimerFRParam
(ILevelObjectBase* levelObject)
{
	//タイマーのフォントの座標
	Vector2 timerPos;
	//対応する透明オブジェクトの回転を持ってくる
	Quaternion qRot = levelObject->GetRotation();
	//アップベクトル
	Vector3 upVec = g_vec3Up;
	//回転で回す
	qRot.Apply(upVec);
	//アップベクトルの長さ
	const float upLen = 300.0f;
	//アップベクトルを拡大する
	upVec.Scale(upLen);
	//フォントはピボットが効いてないから自分でずらす
	const Vector2 offset = { -40.0f,40.0f };
	//ワールド座標からスクリーン座標へ変換する
	g_camera3D->CalcScreenPositionFromWorldPosition
	(timerPos, levelObject->GetPosition() + upVec);
	//ピボットの分ずらす
	timerPos.x += offset.x;
	timerPos.y += offset.y;
	//タイマーのフォントの座標を設定する
	levelObject->GetTimerFR()->SetPosition(timerPos);

	//タイマーのフォントに設定するテキスト
	wchar_t text[32];
	//そのままタイマーを入れると0.9秒～0.0秒の間ずっと0が表示されてしまうので
	//1.0f加算して、0もちょっとは表示したいから0.1f減算する
	swprintf_s(text, L"%d", static_cast<int>(m_resetTimer + 0.9f));
	//タイマーのフォントにテキストを設定する
	levelObject->GetTimerFR()->SetText(text);
	//タイマーのフォントのカラーを設定する
	levelObject->GetTimerFR()->SetColor(m_fontColor);
}

/// <summary>
	/// オブジェクトを半透明にする
	/// </summary>
void OOTransparentSwitch::ChangeTransparent()
{
	//レベルオブジェクトを取ってくる
	std::vector<ILevelObjectBase*> levelObjects
		= CLevelObjectManager::GetInstance()->GetLevelObjects();
	//全てのレベルオブジェクトに検索
	for (int i = 0; i < levelObjects.size(); i++)
	{
		//透明オブジェクトではない場合、次のオブジェクトを検索
		if (levelObjects[i]->GetFlagTransparentObject() == false) {
			continue;
		}

		//モデルの参照を得てから、SetMulColor()を呼ぶ
		//Obstacleの場合は無駄に二回呼ばれるけど、我慢しよう。
		levelObjects[i]->GetModelRender(CReversibleObject::enFront)->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
		levelObjects[i]->GetModelRender(CReversibleObject::enBack)->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
		//オブジェクトの輪郭線を書くようにする
		levelObjects[i]->GetModelRender(CReversibleObject::enFront)->SetDrawOutLineFlag(true);
		levelObjects[i]->GetModelRender(CReversibleObject::enBack)->SetDrawOutLineFlag(true);

		levelObjects[i]->TransparentSwitchOff();

		//フォントのカラーを通常のカラーに設定する
		m_fontColor = m_normalColor;
		levelObjects[i]->GetTimerFR()->SetColor(m_fontColor);
	}

	//タイマーのフォントを無効化した
	m_timerFRIsActive = false;

}

/// <summary>
/// オブジェクトを実体にする
/// </summary>
void OOTransparentSwitch::ChangeEntity()
{
	//レベルオブジェクトを取ってくる
	std::vector<ILevelObjectBase*> levelObjects
		= CLevelObjectManager::GetInstance()->GetLevelObjects();


	//全てのレベルオブジェクトに検索
	for (int i = 0; i < levelObjects.size(); i++)
	{
		//透明オブジェクトではない場合、次のオブジェクトを検索
		if (levelObjects[i]->GetFlagTransparentObject() == false) {
			continue;
		}

		//モデルの参照を得てから、SetMulColor()を呼ぶ
		//Obstacleの場合は無駄に二回呼ばれるけど、我慢しよう。
		levelObjects[i]->GetModelRender(CReversibleObject::enFront)->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });
		levelObjects[i]->GetModelRender(CReversibleObject::enBack)->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });
		//オブジェクトの輪郭線を書かないようにする
		levelObjects[i]->GetModelRender(CReversibleObject::enFront)->SetDrawOutLineFlag(false);
		levelObjects[i]->GetModelRender(CReversibleObject::enBack)->SetDrawOutLineFlag(false);

		levelObjects[i]->TransparentSwitchOn();

		//タイマーのフォントのパラメーターを更新する
		SetTimerFRParam(levelObjects[i]);
	}

	//タイマーのフォントを有効化した
	m_timerFRIsActive = true;
}

