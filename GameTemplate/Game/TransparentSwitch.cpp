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
	//非表示にする
	m_modelRender->Deactivate();

	//カメラを探す	//これは確実に見つける
	m_gameCamera = FindGO<GameCamera>("GameCamera");

	//UFOを探す	//こっちは見つかるか分からない。
	m_ufo = FindGO<CUFO>("UFO");

	//フェードに使うスプライトの生成と初期化
	m_fadeSR = NewGO<CSpriteRender>(0);
	m_fadeSR->Init("Assets/Image/black.DDS", 1280.0f, 780.0f, { 0.5f,0.5f }, AlphaBlendMode_Trans);
	m_fadeSR->SetPosition({ 0.0f,0.0f,0.0f });
	m_fadeSR->SetPostRenderFlag(true);
	//透明にしておく
	m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
	//非表示にする
	m_fadeSR->Deactivate();

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

	return true;
}

//デストラクタ
OOTransparentSwitch::~OOTransparentSwitch()
{
	//モデルレンダラーの破棄
	DeleteGO(m_modelRender);

	//タイマーのフォントの破棄
	for (auto timerFR : m_timerFR)
	{
		DeleteGO(timerFR);
	}

	//m_activationSEの削除
	DeleteGO(m_activationSE);

	//m_buttonpushSEの削除
	DeleteGO(m_buttonpushSE);
}

//アップデート関数
void OOTransparentSwitch::UpdateSub()
{

	if (m_switchingFlag)
	{
		Switching();
		return;
	}
	//リセットタイマーが０になったとき
	//透明オブジェクトをすべて持ち上げられないようにする。
	//透明オブジェクトを持っている場合、そのオブジェクトを持っていない状態にする。
	//透明オブジェクトを初期位置に戻す。
	if (m_flagSwitchOn == true) {
		m_resetTimer -= GameTime().GetFrameDeltaTime();

		//タイマーのフォントの更新
		UpdateTimerFR();
		///--m_resetTimer;

		if (m_resetTimer <= 0.0f) {
			m_flagSwitchOn = false;

			//ここに透明オブジェクトの処理を追加する。
			//配置してあるすべてのレベルオブジェクトの参照のベクター
			//std::vector<ILevelObjectBase*> levelObjects =
			//	CLevelObjectManager::GetInstance()->GetLevelObjects();

			////レベルオブジェクトたちを一つずつ取り出す
			//for (auto lo : levelObjects)
			//{
			//	//自分自身の時はスキップ
			//	if (lo == this)
			//		continue;

			//	//透明オブジェクトの場合
			//	if (GetFlagTransparentObject() == true)
			//	{
			//		//ここにオブジェクトに対する処理
			//		lo->TransparentSwitchOff();

			//		//オブジェクトを持っている場合
			//		lo->SetFlagHeldPlayer(false);
			//	}
			//}

			//透明オブジェクトを半透明にする。
			ChangeTransparent();

			//タイマーのフォントを非表示にする
			for (auto timerFR : m_timerFR)
			{
				timerFR->Deactivate();
			}

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

			//リセットタイマーに開始する値を代入
			m_resetTimer = m_resetTimerStartValue;
			//押されていない時のモデルレンダラーを非表示にする
			GetModelRender()->Deactivate();
			//押されたときのモデルレンダラーを表示する
			m_modelRender->Activate();

			m_fadeSR->Activate();

			//m_buttonpushSEをループ再生をオフで再生する。
			m_buttonpushSE->Play(false);
		}
	}
}

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
		m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,alphaValue });
	}
	else if (m_switchingTimer < startFadeWaitTime)
	{
		//フェードアウトとフェードインの間
		//何もせずに待つ

		//フェードは真っ暗
		m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });

		//カメラがプレイヤーを見なくする
		m_gameCamera->SetLookPlayerFlag(false);
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
		m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,alphaValue });
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
			m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
		}
		else
		{
			//大きいとき
			
			//一つ目のタイマーのフォントは有効化ではないか？
			if (!(*m_timerFR.begin())->IsActive())
			{
				//有効化ではない

				//タイマーのフォントを有効化する
				for (auto timerFR : m_timerFR)
				{
					timerFR->Activate();
				}
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
		m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,alphaValue });
	}
	else if (m_switchingTimer < endFadeWaitTime)
	{
		//フェードアウトとフェードインの間
		//何もせずに待つ

		//フェードは真っ暗
		m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });
		//カメラがプレイヤーを見るようにする
		m_gameCamera->SetLookPlayerFlag(true);
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
		m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,alphaValue });

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
		m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
		//フェードを非表示にする
		m_fadeSR->Deactivate();

		//スイッチオンフラグを立てる
		m_flagSwitchOn = true;

		//プレイヤーを操作可能にする
		m_pPlayer->SetOperationFlag(true);

		//UFOがあるか？
		if (m_ufo)
			//あるとき
			//UFOを動かす
			m_ufo->SetMoveSpeed();

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

	//タイマーのフォントのイテレーター
	std::list<CFontRender*>::iterator itr = m_timerFR.begin();

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
		SetTimerFRParam(itr, levelObjects[i]);

		//イテレーターを進める
		itr++;
	}
}

/// <summary>
/// タイマーのフォントのパラメーターを設定する
/// </summary>
/// <param name="itr">タイマーのフォントのイテレーター</param>
/// <param name="levelObject">対応する透明オブジェクト</param>
void OOTransparentSwitch::SetTimerFRParam
(std::list<CFontRender*>::iterator itr, const ILevelObjectBase* levelObject)
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
	(*itr)->SetPosition(timerPos);

	//タイマーのフォントに設定するテキスト
	wchar_t text[32];
	//そのままタイマーを入れると0.9秒～0.0秒の間ずっと0が表示されてしまうので
	//1.0f加算して、0もちょっとは表示したいから0.1f減算する
	swprintf_s(text, L"%d", static_cast<int>(m_resetTimer + 0.9f));
	//タイマーのフォントにテキストを設定する
	(*itr)->SetText(text);
	//タイマーのフォントのカラーを設定する
	(*itr)->SetColor(m_fontColor);
}

/// <summary>
/// タイマーのフォントが何個いるのか設定する
/// </summary>
/// <param name="num">何個</param>
void OOTransparentSwitch::SetTimerFRNum(const int num)
{
	//サイズを指定する
	m_timerFR.resize(num);
	//タイマーのフォントのイテレーターを用意する
	std::list<CFontRender*>::iterator itr = m_timerFR.begin();

	//フォントのカラーを通常のカラーに設定する
	m_fontColor = m_normalColor;

	//指定した数、フォントを生成し、初期化する
	for (; itr != m_timerFR.end(); itr++)
	{
		(*itr) = NewGO<CFontRender>(0);
		(*itr)->Init(L"10", { 0.0f,0.0f }, m_fontColor);
		(*itr)->SetPostRenderFlag(true);
		//非表示にする
		(*itr)->Deactivate();
	}
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
		levelObjects[i]->GetModelRender(CReversibleObject::enFront)->SetMulColor({ 1.0f,1.0f,1.0f,0.5f });
		levelObjects[i]->GetModelRender(CReversibleObject::enBack)->SetMulColor({ 1.0f,1.0f,1.0f,0.5f });

		levelObjects[i]->TransparentSwitchOff();
	}
}

/// <summary>
	/// オブジェクトを実体にする
	/// </summary>
void OOTransparentSwitch::ChangeEntity()
{
	//レベルオブジェクトを取ってくる
	std::vector<ILevelObjectBase*> levelObjects
		= CLevelObjectManager::GetInstance()->GetLevelObjects();

	//タイマーのフォントのイテレーター
	std::list<CFontRender*>::iterator itr = m_timerFR.begin();

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

		levelObjects[i]->TransparentSwitchOn();

		//タイマーのフォントのパラメータを設定する
		SetTimerFRParam(itr, levelObjects[i]);



		//イテレーターを進める
		itr++;

	}
}