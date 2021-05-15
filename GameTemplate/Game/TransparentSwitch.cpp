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
}

//アップデート関数
void OOTransparentSwitch::UpdateSub()
{
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

		}
	}
	//リセットタイマーが０のときに下の文の処理を作動させる。
	else if (m_flagSwitchOn == false) {
		//スイッチが押されたとき
		//透明オブジェクトをすべて持ち上げられるようにする。
		//スイッチのオブジェクトの範囲内でAボタンが押されたとき
		if (IsHitPlayer() && g_pad[0]->IsTrigger(enButtonA)) {
			m_flagSwitchOn = true;
			//リセットタイマーに開始する値を代入
			m_resetTimer = m_resetTimerStartValue;


			//タイマーのフォントを表示する
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

			//押されていない時のモデルレンダラーを非表示にする
			GetModelRender()->Deactivate();
			//押されたときのモデルレンダラーを表示する
			m_modelRender->Activate();

		}
	}
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
