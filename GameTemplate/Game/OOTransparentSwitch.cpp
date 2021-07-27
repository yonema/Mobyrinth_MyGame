#include "stdafx.h"

#include "OOTransparentSwitch.h"

//ObstacleObjectのモデルのファイルパスとOBBのサイズの定数データを使用可能にする
using namespace OOsFilepathAndObbSizeConstData;
//透明アイテム切り替えスイッチの定数データを使用可能にする
using namespace transparentSwitchConstData;

//コンストラクタ
OOTransparentSwitch::OOTransparentSwitch():
	m_entityMaxTime(TIME_ENTITY_DEFAULT),	//実体化中の最大タイムをデフォルトの値にする
	m_blinkTimer(FLT_MAX)					//最大値で初期化する
{
	//Start関数より先にしておいてほしい処理

	//ウェイポイントからの奥行の距離を設定
	SetZPosLen(LENGTH_Z_FROM_WAYPOINT);
	
	return;
}

//スタート関数
bool OOTransparentSwitch::StartSub()
{
	//初期化処理
	Init(MODEL_FILEPATH_SWITCH, EN_OO_TYPE_TRANSPARENT_SWITCH);

	//OBBのサイズを設定
	SetOBBDirectionLength(MODEL_SCALE_SWITCH);

	//OBBWorldに自身のOBBの登録を消去させて、プレイヤーと衝突しないようにする
	COBBWorld::GetInstance()->RemoveOBB(GetOBB());

	//透明オブジェクトを半透明にする。
	ChangeTransparent();

	//モデルの初期化処理
	InitModel();

	//サウンドの初期化処理
	InitSound();

	//必要なオブジェクトを探す処理
	FindObjects();

	return true;
}

/**
 * @brief モデルの初期化処理
*/
void OOTransparentSwitch::InitModel()
{
	//ボタンが押されたとき用のモデルレンダラーの生成と初期化
	m_pressedSwitchMR = NewGO<CModelRender>(PRIORITY_FIRST);
	m_pressedSwitchMR->Init(MODEL_FILEPATH_SWITCH_PRESSED, D3D12_CULL_MODE_NONE);
	//座標と回転を設定
	m_pressedSwitchMR->SetPosition(m_position);
	m_pressedSwitchMR->SetRotation(m_rotation);
	//アウトラインを描画する
	m_pressedSwitchMR->SetDrawOutLineFlag(true);
	//非表示にする
	m_pressedSwitchMR->Deactivate();

	return;
}

/**
 * @brief サウンドの初期化処理
*/
void OOTransparentSwitch::InitSound()
{

	//スイッチを押したときのサウンドを生成する
	m_pressSwitchSC = NewGO<CSoundCue>(PRIORITY_FIRST);
	//スイッチを押したときのサウンドを、waveファイルを指定して初期化する。
	m_pressSwitchSC->Init(SOUND_FILEPATH_PRESS_SWITCH);
	//音量調節
	m_pressSwitchSC->SetVolume(SOUND_VOLUME_PRESS_SWITCH);

	//透明アイテムが実体化した時のサウンドを生成する
	m_entitySC = NewGO<CSoundCue>(PRIORITY_FIRST);
	//透明アイテムが実体化した時のサウンドを、waveファイルを指定して初期化する。
	m_entitySC->Init(SOUND_FILEPATH_ENTITY);
	//音量調節
	m_entitySC->SetVolume(SOUND_VOLUME_ENTITY);

	//実体化していた透明アイテムが透明になるときのサウンドを生成する
	m_transparentSC = NewGO<CSoundCue>(PRIORITY_FIRST);
	//実体化していた透明アイテムが透明になるときのサウンドを、waveファイルを指定して初期化する。
	m_transparentSC->Init(SOUND_FILEPATH_TRANSPARENT);
	//音量調節
	m_transparentSC->SetVolume(SOUND_VOLUME_TRANSPARENT);

	//タイマーのサウンドの生成と初期化
	m_timerSC = NewGO<CSoundCue>(PRIORITY_FIRST);
	m_timerSC->Init(SOUND_FILTPATH_TIMER);
	m_timerSC->SetVolume(SOUND_VOLUME_TIMER);

	//警告タイマーのサウンドの生成と初期化
	m_timerWarningSC = NewGO<CSoundCue>(PRIORITY_FIRST);
	m_timerWarningSC->Init(SOUND_FILTPATH_TIMER_WARNING);
	m_timerWarningSC->SetVolume(SOUND_VOLUME_TIMER_WARNING);

	return;
}

/**
 * @brief 必要なオブジェクトを探す処理
*/
void OOTransparentSwitch::FindObjects()
{

	//カメラを探す	//これは確実に見つける
	m_gameCamera = FindGO<CGameCamera>(GetGameObjectName(EN_GO_TYPE_GAME_CAMERA));

	//UFOを探す	//こっちは見つかるか分からない。
	m_ufo = FindGO<CUFO>(GetGameObjectName(EN_GO_TYPE_UFO));

	//ポーズを探す
	m_pause = FindGO<CPause>(GetGameObjectName(EN_GO_TYPE_PAUSE));

	return;
}

//デストラクタ
OOTransparentSwitch::~OOTransparentSwitch()
{
	//モデルレンダラーの破棄
	DeleteGO(m_pressedSwitchMR);

	//透明アイテムが実体化した時のサウンドの破棄
	DeleteGO(m_entitySC);
	//実体化していた透明アイテムが透明になるときのサウンドの破棄
	DeleteGO(m_transparentSC);
	//スイッチを押したときのサウンドの破棄
	DeleteGO(m_pressSwitchSC);
	//タイマーのサウンドの削除
	DeleteGO(m_timerSC);
	//警告タイマーのサウンドの削除
	DeleteGO(m_timerWarningSC);

	return;
}

//アップデート関数
void OOTransparentSwitch::UpdateSub()
{
	//スイッチの切り替え中か？
	if (m_switchingFlag)
	{
		//切り替え中

		//透明、実体化を切り替え演出処理
		SwitchingProduction();
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
			m_entityTimer -= GameTime().GetFrameDeltaTime();

			//タイマーのフォントの更新
			UpdateTimerFR();
			///--m_entityTimer;

			//警告のタイム
			const float warningTime = 3.1f;

			//タイマーが警告のタイム以下か？
			if (m_entityTimer <= warningTime)
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

			if (m_entityTimer <= 0.0f) {
				m_flagSwitchOn = false;


				//透明オブジェクトを半透明にする。
				ChangeTransparent();

				//押されていない時のモデルレンダラーを表示する
				GetModelRender()->Activate();
				//押されたときのモデルレンダラーを非常時にする
				m_pressedSwitchMR->Deactivate();

				//m_invalidationSEをループ再生をオフで再生する。
				m_transparentSC->Play(false);

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
				m_player->SetOperationFlag(false);

				//UFOがあるか？
				if (m_ufo)
					//あるとき
					//UFOを動かなくする
					m_ufo->SetMoveSpeed(0.0f);

				//ポーズできなくする
				m_pause->SetCanPause(false);

				//リセットタイマーに開始する値を代入
				m_entityTimer = m_entityMaxTime;
				//押されていない時のモデルレンダラーを非表示にする
				GetModelRender()->Deactivate();
				//押されたときのモデルレンダラーを表示する
				m_pressedSwitchMR->Activate();

				//m_fadeSR->Activate();

				//m_buttonpushSEをループ再生をオフで再生する。
				m_pressSwitchSC->Play(false);
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

	return;
}

/**
 * @brief 透明、実体化を切り替え演出処理
*/
void OOTransparentSwitch::SwitchingProduction()
{
	//切り替え中タイマーを進める
	m_switchingTimer += GameTime().GetFrameDeltaTime();

	//切り替え中のタイマーの経過を調べる
	if (m_switchingTimer < TIME_WAIT_START)
	{
		//最初の待つ時間
		//何もせずに待つ
		//早期リターン
		return;
	}
	else if (m_switchingTimer < TIME_FADE_OUT_START)
	{
		//フェードアウトしていく
		
		//タイマーに経過具合によって補完。徐々に0.0f～1.0fに変化する
		const float timeScale = 
			(m_switchingTimer - TIME_WAIT_START) / (TIME_FADE_OUT_START - TIME_WAIT_START);
		//フェードアウトさせる
		FadeOut(timeScale);

		//早期リターン
		return;
	}
	else if (m_switchingTimer < TIME_FADE_WAIT_START)
	{
		//フェードアウトとフェードインの間

		//フェードは真っ暗
		g_graphicsEngine->GetSceneChange().
			SetFadeSpriteAlphaValue(spriteRenderConstData::ALPHA_VALUE_OPACITY);
		//カメラをステージ全体を見渡す位置に移動させる処理
		CameraSetStageView();

		//早期リターン
		return;
	}
	else if (m_switchingTimer < TIME_FADE_IN_START)
	{
		//フェードインしていく

		//タイマーに経過具合によって補完
		const float timeScale = 
			(m_switchingTimer - TIME_FADE_WAIT_START) / (TIME_FADE_IN_START - TIME_FADE_WAIT_START);
		//フェードインさせる
		FadeIn(timeScale);

		//早期リターン
		return;
	}
	else if (m_switchingTimer < TIME_SWITCHING)
	{
		//透明オブジェクトの切り替え中

		//透明、実体化の切り替え時の処理
		Switching();

		//早期リターン
		return;
	}
	else if (m_switchingTimer < TIME_FADE_OUT_END)
	{
		//フェードアウトしていく

		//タイマーに経過具合によって補完
		const float timeScale = (m_switchingTimer - TIME_SWITCHING) / (TIME_FADE_OUT_END - TIME_SWITCHING);
		//フェードアウトさせる
		FadeOut(timeScale);

		//早期リターン
		return;
	}
	else if (m_switchingTimer < TIME_FADE_WAIT_END)
	{
		//フェードアウトとフェードインの間

		//フェードは真っ暗
		g_graphicsEngine->GetSceneChange().
			SetFadeSpriteAlphaValue(spriteRenderConstData::ALPHA_VALUE_OPACITY);
		//カメラがプレイヤーを見るようにする
		CameraSetLookPlayer();

		//早期リターン
		return;
	}
	else if (m_switchingTimer < TIME_FADE_IN_END)
	{
		//フェードインしてく

		//タイマーに経過具合によって補完
		const float timeScale = (m_switchingTimer - TIME_FADE_WAIT_END) / (TIME_FADE_IN_END - TIME_FADE_WAIT_END);
		//フェードインさせる
		FadeIn(timeScale);
		//タイマーのフォントの更新
		UpdateTimerFR();

		//早期リターン
		return;
	}
	else
	{
		//終了

		//透明、実体化の切り替え処理の最後の処理
		SwitchingEnd();
	}

	return;
}


/**
 * @brief フェードアウトする処理
 * @param [in] fadeRate フェードアウト率。0.0f:普通。1.0f:真っ暗。
*/
void OOTransparentSwitch::FadeOut(const float fadeRate)
{
	//アルファ値
	const float alphaValue = Math::Lerp<float>(fadeRate, 0.0f, 1.0f);
	//フェードを徐々に暗くしていく
	g_graphicsEngine->GetSceneChange().SetFadeSpriteAlphaValue(alphaValue);

	return;
}

/**
 * @brief フェードインする処理
 * @param [in] fadeRate フェードイン率。0.0f:真っ暗。1.0f:透明。
*/
void OOTransparentSwitch::FadeIn(const float fadeRate)
{
	//アルファ値
	const float alphaValue = Math::Lerp(fadeRate, 1.0f, 0.0f);
	//フェードを徐々に明るくしていく
	g_graphicsEngine->GetSceneChange().SetFadeSpriteAlphaValue(alphaValue);

	return;
}

/**
 * @brief 透明、実体化の切り替え時の処理
*/
void OOTransparentSwitch::Switching()
{
	//半分の時間より小さいか？
	if (m_switchingTimer < TIME_SWITCHING_HALF)
	{
		//小さいとき
		//フェードを透明にする
		g_graphicsEngine->GetSceneChange().
			SetFadeSpriteAlphaValue(spriteRenderConstData::ALPHA_VALUE_TRANSPARENT);

		//早期リターン
		return;
	}

	//大きいとき

	//透明状態か？一回の切り替えで一回ずつ呼ばれる。
	if (m_isTransparentFlag)
	{
		//透明状態
		//点滅タイマーを初期化する
		m_blinkTimer = FLT_MAX;
		//フォントのカラーを通常時のカラーにする
		m_fontColor = GetTimerFontNormalColor();

		//透明オブジェクトを実体にする。
		ChangeEntity();
	}

	return;
}

/**
 * @brief 透明、実体化の切り替え処理の最後の処理
*/
void OOTransparentSwitch::SwitchingEnd()
{
	//切り替え中フラグを折る
	m_switchingFlag = false;
	//スイッチオンフラグを立てる
	m_flagSwitchOn = true;
	//切り替え中のタイマーを初期化する
	m_switchingTimer = 0.0f;

	//フェードを透明にする
	g_graphicsEngine->GetSceneChange().
		SetFadeSpriteAlphaValue(spriteRenderConstData::ALPHA_VALUE_TRANSPARENT);

	//プレイヤーを操作可能にする
	m_player->SetOperationFlag(true);

	//UFOがあるか？
	if (m_ufo)
		//あるとき
		//UFOを動かす
		m_ufo->SetMoveSpeed();

	//ポーズできるようにする
	m_pause->SetCanPause(true);

	return;
}


/**
 * @brief カメラをステージ全体を見渡す位置に移動させる処理
*/
void OOTransparentSwitch::CameraSetStageView()
{
	//カメラがプレイヤーを見なくする
	m_gameCamera->SetLookPlayerFlag(false);

	//カメラをリフレッシュして、一時的にばねカメラを無効にし、
	//一気にカメラを移動できるようにする
	m_gameCamera->Refresh();

	//カメラの視点を設定
	m_gameCamera->SetPosition(POSITION_VIEW_STAGE_CAMERA);
	//カメラの注視点を設定
	m_gameCamera->SetTarget(g_VEC3_ZERO);

	return;
}

/**
 * @brief カメラがプレイヤーを見るようにする
*/
void OOTransparentSwitch::CameraSetLookPlayer()
{
	//カメラがプレイヤーを見るようにする
	m_gameCamera->SetLookPlayerFlag(true);

	//カメラをリフレッシュして、一時的にばねカメラを無効にし、
	//一気にカメラを移動できるようにする
	m_gameCamera->Refresh();

	return;
}



/// <summary>
/// タイマーのフォントの更新
/// </summary>
void OOTransparentSwitch::UpdateTimerFR()
{
	//点滅し始めるタイムよりタイマーが小さいか？
	if (m_entityTimer <= TIME_START_BLINK)
	{
		//小さい
		//点滅させる

		//タイマーのフォントの点滅処理
		TimerFontBlink();
	}

	//全てのレベルオブジェクトに検索
	for (ILevelObjectBase* levelObject : CLevelObjectManager::GetInstance()->GetLevelObjects())
	{
		//透明オブジェクトではない場合、次のオブジェクトを検索
		if (levelObject->GetFlagTransparentObject() == false)
			continue;
		
		//タイマーのフォントのパラメーターを設定する
		SetTimerFRParam(levelObject);
	}

	return;
}

/**
 * @brief タイマーのフォントの点滅処理
*/
void OOTransparentSwitch::TimerFontBlink()
{
	//点滅タイマーが点滅するタイムより大きいか？
	if (m_blinkTimer > TIME_BLINK)
	{
		//大きかったら
		//点滅させる

		//フォントのカラーは通常のカラーか？
		if (m_fontColor.x == GetTimerFontNormalColor().x)
			//通常のカラーなら
			//点滅時のカラーにする
			m_fontColor = FONT_COLOR_BLINK_TIMER;
		else
			//違うなら
			//通常のカラーにする
			m_fontColor = GetTimerFontNormalColor();

		//点滅タイマーを0.0fにする
		m_blinkTimer = 0.0f;
	}
	else
	{
		//小さいとき

		//点滅タイマーを進める
		m_blinkTimer += GameTime().GetFrameDeltaTime();
	}

	return;
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
	Vector2 timerPos = g_VEC2_ZERO;
	//対応する透明オブジェクトの回転を持ってくる
	Quaternion qRot = levelObject->GetRotation();
	//アップベクトル
	Vector3 upVec = g_VEC3_UP;
	//回転で回す
	qRot.Apply(upVec);
	//アップベクトルを拡大する
	upVec.Scale(LENGHT_UP_POSITION_FONT_TIMER);
	//ワールド座標からスクリーン座標へ変換する
	g_camera3D->CalcScreenPositionFromWorldPosition
	(timerPos, levelObject->GetPosition() + upVec);
	//ピボットの分ずらす
	timerPos.x += OFFSET_FONT_TIMER.x;
	timerPos.y += OFFSET_FONT_TIMER.y;
	//タイマーのフォントの座標を設定する
	levelObject->SetTimerFontPosition(timerPos);

	//タイマーのフォントに設定するテキスト
	wchar_t text[TEXT_SIZE_FONT_TIMER];
	//そのままタイマーを入れると0.9秒～0.0秒の間ずっと0が表示されてしまうので
	//1.0f加算して、0もちょっとは表示したいから0.1f減算する
	swprintf_s(text, TEXT_FONT_TIMER, static_cast<int>(m_entityTimer + TIME_ADJUST));
	//タイマーのフォントにテキストを設定する
	levelObject->SetTimerFontText(text);
	//タイマーのフォントのカラーを設定する
	levelObject->SetTimerFontColor(m_fontColor);

	return;
}

/// <summary>
/// オブジェクトを半透明にする
/// </summary>
void OOTransparentSwitch::ChangeTransparent()
{
	//全てのレベルオブジェクトに検索
	for (ILevelObjectBase* levelObject : CLevelObjectManager::GetInstance()->GetLevelObjects())
	{
		//透明オブジェクトではない場合、次のオブジェクトを検索
		if (levelObject->GetFlagTransparentObject() == false) {
			continue;
		}

		//モデルの参照を得てから、SetMulColor()を呼ぶ
		//Obstacleの場合は無駄に二回呼ばれるけど、我慢しよう。
		levelObject->GetModelRender(EN_FRONT)->SetMulColor(g_VEC4_TRANSPARENT);
		levelObject->GetModelRender(EN_BACK)->SetMulColor(g_VEC4_TRANSPARENT);
		//オブジェクトの輪郭線を書くようにする
		levelObject->GetModelRender(EN_FRONT)->SetDrawOutLineFlag(true);
		levelObject->GetModelRender(EN_BACK)->SetDrawOutLineFlag(true);

		levelObject->TransparentSwitchOff();

		//フォントのカラーを通常のカラーに設定する
		m_fontColor = GetTimerFontNormalColor();
		levelObject->SetTimerFontColor(m_fontColor);
	}

	//透明オブジェクトになった
	m_isTransparentFlag = true;

	return;
}

/// <summary>
/// オブジェクトを実体にする
/// </summary>
void OOTransparentSwitch::ChangeEntity()
{
	//全てのレベルオブジェクトに検索
	for (ILevelObjectBase* levelObject : CLevelObjectManager::GetInstance()->GetLevelObjects())
	{
		//透明オブジェクトではない場合、次のオブジェクトを検索
		if (levelObject->GetFlagTransparentObject() == false) {
			continue;
		}

		//モデルの参照を得てから、SetMulColor()を呼ぶ
		//Obstacleの場合は無駄に二回呼ばれるけど、我慢しよう。
		levelObject->GetModelRender(EN_FRONT)->SetMulColor(g_VEC4_OPACITY);
		levelObject->GetModelRender(EN_BACK)->SetMulColor(g_VEC4_OPACITY);
		//オブジェクトの輪郭線を書かないようにする
		levelObject->GetModelRender(EN_FRONT)->SetDrawOutLineFlag(false);
		levelObject->GetModelRender(EN_BACK)->SetDrawOutLineFlag(false);

		levelObject->TransparentSwitchOn();

		//タイマーのフォントのパラメーターを更新する
		SetTimerFRParam(levelObject);
	}

	//透明アイテムが実体化した時のサウンドをワンショット再生で再生
	m_entitySC->Play(false);

	//透明状態ではなくなった
	m_isTransparentFlag = false;

	return;
}

