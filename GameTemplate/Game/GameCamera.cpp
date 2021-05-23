#include "stdafx.h"
#include "GameCamera.h"
class Player;

bool GameCamera::Start()
{
	//プレイヤーを探す
	m_pPlayer = FindGO<Player>("Player");
	//注視点から視点へのベクトルを設定する
	m_toCameraPos = { 0.0f,0.0f,1000.0f };

	//開始演出用のベクトル
	m_toCameraPos = { 0.0f,0.0f,4000.0f };

	//ばねカメラの初期化
	m_springCamera.Init(*g_camera3D, 5000.0f, false, 0.0f);

	if (m_pPlayer)
	{
		//プレイヤーが見つかったら
		//プレイヤーの位置をもとにカメラの場所を決める
		m_springCamera.SetPosition(m_pPlayer->GetPosition() + m_toCameraPos);
		m_springCamera.SetTarget(m_pPlayer->GetPosition());
	}
	else
	{
		//見つからなかったら
		//デフォルトの位置にする
		m_springCamera.SetPosition({ 0.0f,1800.0f,1000.0f });
		m_springCamera.SetTarget({ 0.0f,1800.0f,0.0f });
	}


	//近平面、遠平面の設定
	m_springCamera.SetNear(0.5f);
	m_springCamera.SetFar(50000.0f);

	//ステージ開始時の演出を探す
	m_startDirecting = FindGO<StartDirecting>("StartDirecting");

	//カメラの上方向の修正
	//const Quaternion qRot = (m_pPlayer->GetFinalWPRot());
	//qRot.Apply(m_toCameraPos);
	Vector3 vecUp = g_vec3AxisY;
	//qRot.Apply(vecUp);

	m_springCamera.SetUp(vecUp);


	//フェードに使うスプライトの生成と初期化
	m_fadeSR = NewGO<CSpriteRender>(0);
	m_fadeSR->Init("Assets/Image/black.DDS", 1280.0f, 780.0f, { 0.5f,0.5f }, AlphaBlendMode_Trans);
	m_fadeSR->SetPosition({ 0.0f,0.0f,0.0f });
	m_fadeSR->SetPostRenderFlag(true);
	//透明にしておく
	m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
	//非表示にする
	//m_fadeSR->Deactivate();


	//開始演出用の位置情報
	m_springCamera.SetPosition(m_toCameraPos);
	m_springCamera.SetTarget({ 0.0f,0.0f,0.0f });

	return true;
}

void GameCamera::Update()
{
	if (m_startDirecting->GetStartDirecting() == true) {
		if (m_startDirecting->GetCheckAButton() == false) {
			StartDirectingCamera();
		}
		if (m_startDirectingZoomInCamera == true) {
			FadeDirectingCamera();
		}
	}
	else {
		InGameCamera();
	}

	//ばねカメラの更新
	m_springCamera.Update();
}

void GameCamera::StartDirectingCamera()
{
	if (m_startDirecting->GetStartDirecting() == false || !m_wipeEndFlag) {
		return;
	}

	//ちょっと待ってから演出を開始する
	if (m_startTimer < /*0.5f*/0.5f)
	{
		m_startTimer += GameTime().GetFrameDeltaTime();
		return;
	}



	if (m_flagRotationCamera == true) {
		const Quaternion qRot = { 0.0f,0.005f,0.0f,1.0f };
		//カメラへのベクトルを回す
		qRot.Apply(m_toCameraPos);
		//ステージ全体が見渡せる設定
		m_springCamera.SetPosition(m_toCameraPos);
		m_springCamera.SetTarget({ 0.0f,0.0f,0.0f });
	}


	if (m_startDirecting) {
		//ステージの周りを回転する。
		if (m_startDirectingZoomInCamera == false) {
			//処理的にマジックナンバーになるけど、時間がないのでこのままでいきます。
			m_test = m_test + 0.0017f;

			if (m_test >= 1.0f || g_pad[0]->IsTrigger(enButtonA) == true) {
				m_startDirectingZoomInCamera = true;
			}

		}
		//キャラクターの位置にカメラを合わせる。
		else {
			

			//此処から下は、多分使わない。

			//if (m_zoomFinished == false) {
			//	Vector3 differenceCameraPos = { 0.0f,0.0f,1000.0f / m_startDirecting->GetStartDirectingTime() };
			//	//Vector3 cameraPos1 = { 0.0f,0.0f,2000.0f };
			//	//Vector3 cameraPos2= { 0.0f,0.0f,1000.0f };

			//	m_toCameraPos = m_toCameraPos - differenceCameraPos;

			//	g_camera3D->SetTarget(m_startDirecting->GetPosition());

			//	g_camera3D->SetPosition(m_startDirecting->GetPosition() + m_toCameraPos);
			//}
			//else
			//{
			//	g_camera3D->SetTarget({ 0.0f,1800.0f,0.0f });
			//	g_camera3D->SetPosition({ 0.0f,1800.0f,1200.0f });
			//}
		}
	}
	else {
		//ステージ開始時の演出が見つかっていなかったら
		//ステージ開始時の演出を探す
		m_startDirecting = FindGO<StartDirecting>("StartDirecting");
	}

}

void GameCamera::FadeDirectingCamera()
{
	const float startWaitTime = 0.5f;							//最初の待つ時間		 
	const float startFadeOutTime = startWaitTime + 0.5f;		//最初のフェードアウト時間
	const float startFadeWaitTime = startFadeOutTime + 0.5f;	//最初のフェード中間時間
	const float startFadeInTime = startFadeWaitTime + 0.5f;		//最初のフェードイン時間

	//切り替え中のタイマーの経過を調べる
	if (m_switchingTimer < startWaitTime)
	{
		//最初の待つ時間
		int test = 10;
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

		//カメラがステージの周りを回転しないようにする。
		m_flagRotationCamera = false;

		//フェードは真っ暗
		m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });
	}
	else if (m_switchingTimer < startFadeInTime)
	{
		//フェードインしていく

		//Aボタンを押した扱いにする。
		m_startDirecting->SetCheckAButton(true);

		//カメラがプレイヤーを見るようにする。
		/*SetTarget(m_pPlayer->GetPosition());
		SetPosition({ m_pPlayer->GetPositionX(),
					m_pPlayer->GetPositionY(),
					m_pPlayer->GetPositionZ() + 1200.0f });*/

		//アルファ値
		float alphaValue = 1.0f;
		//タイマーに経過具合によって補完
		float timeScale = (m_switchingTimer - startFadeWaitTime) / (startFadeInTime - startFadeWaitTime);
		alphaValue -= 1.0f * timeScale;
		//フェードを徐々に明るくしていく
		m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,alphaValue });
	}

	//カメラをリフレッシュして、一時的にばねカメラを無効にし、
	//一気にカメラを移動できるようにする
	m_springCamera.Refresh();

	m_switchingTimer += GameTime().GetFrameDeltaTime();
}

void GameCamera::InGameCamera()
{
	if (m_lookPlayerFlag)
	{
		if (m_pPlayer)
		{
			//プレイヤーが見つかっていたら

			//注視点から視点へのベクトルを設定する
			m_toCameraPos = { 0.0f,0.0f,1200.0f };
			//if (m_titleMode)
			//	m_toCameraPos = { 0.0f,600.0f,1200.0f };
			const float upLen = 400.0f;
			Vector3 upVec = { 0.0f,upLen,0.0f };



			//プレイヤーの回転を得る
			const Quaternion qRot = (m_pPlayer->GetFinalWPRot());
			//カメラへのベクトルを回す
			qRot.Apply(m_toCameraPos);
			//アップベクトル
			Vector3 vecUp = g_vec3AxisY;
			//アップベクトルを回す
			qRot.Apply(vecUp);

			qRot.Apply(upVec);

			if (!m_titleMode)
				upVec = g_vec3Zero;

			//注視点を設定する
			m_springCamera.SetTarget(m_pPlayer->GetPosition() + upVec);
			//視点を設定する
			m_springCamera.SetPosition(m_pPlayer->GetPosition() + m_toCameraPos + upVec);
			//アップ軸を設定する
			m_springCamera.SetUp(vecUp);
		}
		else
		{
			//プレイヤーが見つかっていなかったら
			//プレイヤーを探す
			m_pPlayer = FindGO<Player>("Player");
		}
	}


	return;
}