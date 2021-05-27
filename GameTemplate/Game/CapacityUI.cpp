#include "stdafx.h"
#include "CapacityUI.h"

//スタート関数
bool CCapacityUI::Start()
{
	//スプライトの初期化
	InitSprite();

	//フォントの初期化
	InitFont();

	//m_capacity_overのサウンドキューを生成する
	m_capacity_over = NewGO<CSoundCue>(0);
	//m_buttonpushSEのサウンドキューを、waveファイルを指定して初期化する。
	m_capacity_over->Init(L"Assets/sound/capacity_over.wav");
	//音量調節
	m_capacity_over->SetVolume(0.5f);

	//プレイヤーを探す
	m_player = FindGO<Player>("Player");

	return true;

}

//スプライトの初期化
void CCapacityUI::InitSprite()
{
	//キャパシティを表示するスプライトの生成と初期化
	m_capacityDisplaySR = NewGO<CSpriteRender>(1);
	//スプライトのX座標
	const float capacityX = -490.0f;
	//スプライトのY座標S
	const float capacityY = 270.0f;
	//スプライトの初期化
	m_capacityDisplaySR->Init
	("Assets/image/Capacity_display.DDS", 512.0f * 1.1f, 256.0f * 1.1f, { 0.5f,0.5f }, AlphaBlendMode_Trans);
	m_capacityDisplaySR->SetPosition({ capacityX, capacityY,0.0f });
	m_capacityDisplaySR->SetPostRenderFlag(true);
	//タイトル画面か？
	if (m_titleFlag)
	{
		//タイトル画面なら
		//非表示にする
		m_capacityDisplaySR->Deactivate();
	}
}


//フォントの初期化
void CCapacityUI::InitFont()
{
	//表側と裏側の反転オブジェクトの数を表示する
	//フォントレンダラーの初期化
	for (int i = 0; i < enFrontAndBackSideNum; i++)
	{
		//表側と裏側にあるアイテムの数のフォント
		m_RONumFR[i] = NewGO<CFontRender>(1);
		m_RONumFR[i]->SetPostRenderFlag(true);
		m_RONumFR[i]->SetShadowParam(true, 0.4f, { 0.0f,0.0f,0.0f,1.0f });
		//キャパシティのフォント
		m_capacityFR[i] = NewGO<CFontRender>(1);
		m_capacityFR[i]->SetPostRenderFlag(true);
		m_capacityFR[i]->SetShadowParam(true, 0.4f, { 0.0f,0.0f,0.0f,1.0f });
		//最大数を設定
		CLevelObjectManager::GetInstance()->SetReversibleObjectMaxNum(i, m_maxCapacityNum[i]);
	}

	//キャパシティからアイテムの数のフォントへの差
	m_diffCapacityToNum = 100.0f;
	//テキスト用意
	wchar_t text[256];
	//現在の表側と裏側の反転オブジェクトの数を取得
	const int* maxNum = m_maxCapacityNum;


	//＊＊表側のフォントの設定＊＊
	//フォントの拡大
	const float scale = 1.5f;
	m_defaultScale = scale;
	//フォントのカラー
	Vector3 fontColor;
	//表側のタイルの色
	//fontColor = { 214.0f,85.0f,0.0f };
	//明るすぎるため正規化
	fontColor.Normalize();
	fontColor = { 1.0f,1.0f,1.0f };
	m_defaultFontColor[enFrontSide] = fontColor;
	m_activeFontColor = fontColor;
	//フォントのX座標
	const float capacityNumX = -615.0f;
	//フォントのY座標
	const float capacityNumY = 332.5f;
	m_capacityPos[enFrontSide] = { capacityNumX ,capacityNumY };
	//テキストをセット
	swprintf(text, L"表:  /%d", maxNum[enFrontSide]);
	//表側の数のキャパシティのフォントの初期化
	m_capacityFR[enFrontSide]->Init(text,
		m_capacityPos[enFrontSide],
		{ fontColor.x,fontColor.y,fontColor.z,1.0f },
		0.0f, scale, { 0.5f,0.5f });
	//表側の数フォントの初期化
	m_RONumFR[enFrontSide]->Init(L"",
		{ capacityNumX + m_diffCapacityToNum,capacityNumY }, { fontColor.x,fontColor.y,fontColor.z,1.0f },
		0.0f, scale, { 0.5f,0.5f });


	//＊＊裏側のフォントの設定＊＊
	//フォントのカラー
	//裏側のタイルの色
	//fontColor = { 99.0f,97.0f,99.0f };
	//明るすぎるため正規化
	fontColor.Normalize();
	//色が一緒過ぎて見えにくかったから、暗くする
	fontColor = { 1.0f,1.0f,1.0f };
	fontColor.Scale(0.0);
	m_defaultFontColor[enBackSide] = fontColor;
	m_inactiveFontColor = fontColor;
	//表側のフォントとのY座標の差
	const float diffFrontToBackY = 70.0f;
	m_capacityPos[enBackSide] = { capacityNumX ,capacityNumY - diffFrontToBackY };
	//テキストをセット
	swprintf(text, L"裏:  /%d", maxNum[enBackSide]);
	//裏側のキャパシティのフォントの初期化
	m_capacityFR[enBackSide]->Init(text,
		m_capacityPos[enBackSide],
		{ fontColor.x,fontColor.y,fontColor.z,1.0f },
		0.0f, scale, { 0.5f,0.5f });
	//裏側の数のフォントの初期化
	m_RONumFR[enBackSide]->Init(L"",
		{ capacityNumX + m_diffCapacityToNum,capacityNumY - diffFrontToBackY },
		{ fontColor.x,fontColor.y,fontColor.z,1.0f },
		0.0f, scale, { 0.5f,0.5f });

	//タイトル画面か？
	if (m_titleFlag)
	{
		//タイトル画面なら

		//非表示にする
		m_capacityFR[enFrontSide]->Deactivate();
		m_capacityFR[enBackSide]->Deactivate();
		m_RONumFR[enFrontSide]->Deactivate();
		m_RONumFR[enBackSide]->Deactivate();
	}
}

//デストラクタ
CCapacityUI::~CCapacityUI()
{
	//キャパシティを表示するスプライトの破棄
	DeleteGO(m_capacityDisplaySR);

	//表側と裏側にあるアイテムの数のフォントの破棄
	for (int i = 0; i < enFrontAndBackSideNum; i++)
	{
		DeleteGO(m_RONumFR[i]);
		DeleteGO(m_capacityFR[i]);
	}
}

void CCapacityUI::Update()
{
	//演出の割り振り
	Directing();


	//テキスト用意
	wchar_t text[256];
	//現在の表側と裏側の反転オブジェクトの数を取得
	const int* num = CLevelObjectManager::GetInstance()->GetReversibleObjectNum();
	//テキストをセット
	swprintf(text, L"%d", num[enFrontSide]);
	m_RONumFR[enFrontSide]->SetText(text);
	swprintf(text, L"%d", num[enBackSide]);
	m_RONumFR[enBackSide]->SetText(text);

	//最初のフレームか？
	if (m_firstFrame)
	{
		//最初のフレーム

		//最初だけ、古い番号を更新
		for (int i = 0; i < enFrontAndBackSideNum; i++)
		{
			m_oldRONum[i] = num[i];
		}
		//最初ではなくする
		m_firstFrame = false;
	}

	//どの演出を起こすかチェックする
	CheckDirecting(num);

	CheckActiveFontColor();


	//今の表と裏の数を、前の表と裏の数に代入する
	for (int i = 0; i < enFrontAndBackSideNum; i++)
	{
		m_oldRONum[i] = num[i];
	}
}

/// <summary>
/// どの演出を起こすかチェックする
/// </summary>
/// <param name="num">表側と裏側のアイテムの数</param>
/// <param name="maxNum">表側と裏側のアイテムの最大数</param>
void CCapacityUI::CheckDirecting(const int* num)
{
	//どれかがオーバー中か？
	if (m_directingState[enFrontSide] == enOver ||
		m_directingState[enBackSide] == enOver)
		//オーバー中なら
		//ほかの演出はしない
		return;

	//表側と裏側両方調べる
	for (int i = 0; i < enFrontAndBackSideNum; i++)
	{
		//前の数と現在の数が変わっているか？
		if (num[i] != m_oldRONum[i])
		{
			//変わっている

			//タイマーを初期化する
			m_timer[i] = 0.0f;

			//現在の数を調べる
			if (num[i] > m_maxCapacityNum[i])
				//現在の数が最大値より大きい
				//オーバーした
				m_directingState[i] = enOver;
			else if (num[i] > m_oldRONum[i])
				//現在の数が前の数より大きい
				//増えた
				m_directingState[i] = enIncreased;
			else if (num[i] < m_oldRONum[i])
				//現在の数が前の数より小さい
				//減った
				m_directingState[i] = enDecreased;

		}
	}

	//どれかがオーバーの演出になったら、他の演出はキャンセルする
	if (m_directingState[enFrontSide] == enOver)
		m_directingState[enBackSide] = enNormal;
	else if (m_directingState[enBackSide] == enOver)
		m_directingState[enFrontSide] = enNormal;
}


//演出の割り振り
void CCapacityUI::Directing()
{
	//表側と裏側両方調べる
	for (int i = 0; i < enFrontAndBackSideNum; i++)
	{
		//演出のステートで割り振る
		switch (m_directingState[i])
		{
		case enNormal:
			//通常
			//何もしない
			break;
		case enIncreased:
			//増えた
			Increased(i);
			break;
		case enDecreased:
			//減った
			Decreased(i);
			break;
		case enOver:
			//オーバーした
			Over(i);
			break;
		}
	}
}

/// <summary>
/// 増えた演出
/// </summary>
/// <param name="frontOrBackSide">表側か裏側か？</param>
void CCapacityUI::Increased(const int frontOrBackSide)
{
	//切り替え時間
	const float switchingTime = 1.0f;
	//フォントが特有の動きをする時間
	const float actionTime = 0.7f;

	//タイマーが0.0fか？、つまり最初の一回目のループか？
	if (m_timer[frontOrBackSide] == 0.0f)
	{
		//最初の一回目

		//通常の値に戻す
		DefaultParam(frontOrBackSide);

		//フォントのカラーを増えた時のカラーにする
		//if ()
		m_RONumFR[frontOrBackSide]->SetColor({ 1.0f,1.0f,0.0f,1.0f });
		
	}

	//数のフォントの座標
	//現在の数のフォントの座標を取ってくる
	Vector2 numFontPos = m_RONumFR[frontOrBackSide]->GetPosition();
	//Y座標には最初の座標を入れておく
	numFontPos.y = m_capacityPos[frontOrBackSide].y;

	//タイマーがアクションタイム以内か？
	if (m_timer[frontOrBackSide] <= actionTime)
	{
		//以内の時

		//二次式の動きでフォントをはねさせる
		//y = ax^2+b
		//a = (y-b)/x^2

		//アクションタイムの半分のタイム
		const float halfActionTime = actionTime / 2.0f;
		//二次式のxの値、半分のタイムで頂点に行くようにする
		float x = m_timer[frontOrBackSide] - halfActionTime;
		//二次式のbの値、ここがはねる最大の高さになる
		const float b = 25.0f;
		//二次式のaの値、
		//タイマーが最初の時、高さが0になるaの値を求める、つまり
		//x = 0.0f - halfActionTimeかつ、y = 0.0fの時のaを求める
		const float a = (0.0f - b) / pow(-halfActionTime, 2.0f);
		//二次式のyの値、今までの値で計算して、ここがはねている高さになる
		const float y = a * pow(x, 2.0f) + b;


		//はねる分だけ高くする
		numFontPos.y += y;

	}
	else
	{
		//以外の時

		//通常の座標に戻しておく

	}


	//数のフォントの座標を設定する
	m_RONumFR[frontOrBackSide]->SetPosition(numFontPos);

	//タイマーが切り替え時間以上か
	if (m_timer[frontOrBackSide] >= switchingTime)
	{
		//以上

		//タイマーを初期化する
		m_timer[frontOrBackSide] = 0.0f;
		//フォントのカラーを初期化する
		m_RONumFR[frontOrBackSide]->SetColor(m_defaultFontColor[frontOrBackSide]);
		//演出のステートを通常にする
		m_directingState[frontOrBackSide] = enNormal;
	}
	else
	{
		//未満

		//タイマーを進める
		m_timer[frontOrBackSide] += GameTime().GetFrameDeltaTime();
	}
}

/// <summary>
/// 減った演出
/// </summary>
/// <param name="frontOrBackSide">表側か裏側か？</param>
void CCapacityUI::Decreased(const int frontOrBackSide)
{
	//切り替え時間
	const float switchingTime = 1.0f;
	//フォントが特有の動きをする時間
	const float actionTime = 0.7f;


	//タイマーが0.0fか？、つまり最初の一回目のループか？
	if (m_timer[frontOrBackSide] == 0.0f)
	{
		//最初の一回目

		//通常の値に戻す
		DefaultParam(frontOrBackSide);

		//フォントのカラーを減った時のカラーにする
		m_RONumFR[frontOrBackSide]->SetColor({ 0.0f,0.0f,2.0f,1.0f });
		
	}


	//数のフォントの座標
//
	Vector2 numFontPos = m_capacityPos[frontOrBackSide];
	//X座標をそろえる
	numFontPos.x += m_diffCapacityToNum;

	//タイマーがアクションタイム以内か？
	if (m_timer[frontOrBackSide] <= actionTime)
	{
		//以内の時

		//アクションタイムから分割されたタイム
		const float separationTime = actionTime / 4.0f;

		//タイマーと分割されたタイムに応じた状態
		const int state = m_timer[frontOrBackSide] / separationTime;

		//目的地への距離
		const float destination = 10.0f;

		//移動させる長さ
		float moveLen = 0.0f;

		//タイマーに応じた拡大
		const float timeScale = 
			(m_timer[frontOrBackSide] - separationTime * state) / separationTime;

		//状態に応じて割り振る
		switch (state)
		{
		case 0:
		case 4:
			//真ん中から左端に進む
			moveLen = -destination * timeScale;
			break;
		case 1:
		case 5:
			//左端から真ん中へ進む
			moveLen = -destination + destination * timeScale;
			break;
		case 2:
		case 6:
			//真ん中から右端へ進む
			moveLen = destination * timeScale;
			break;
		case 3:
		case 7:
			//右端から真ん中へ進む
			moveLen = destination - destination * timeScale;
			break;

		}

		//数のフォントの座標を移動させる
		numFontPos.x += moveLen;


	}
	else
	{
		//以外の時


	}

	//数のフォントの座標を設定する
	m_RONumFR[frontOrBackSide]->SetPosition(numFontPos);

	//タイマーが切り替え時間以上か
	if (m_timer[frontOrBackSide] >= switchingTime)
	{
		//以上

		//タイマーを初期化する
		m_timer[frontOrBackSide] = 0.0f;
		//フォントのカラーを初期化する
		m_RONumFR[frontOrBackSide]->SetColor(m_defaultFontColor[frontOrBackSide]);
		//演出のステートを通常にする
		m_directingState[frontOrBackSide] = enNormal;
	}
	else
	{
		//未満

		//タイマーを進める
		m_timer[frontOrBackSide] += GameTime().GetFrameDeltaTime();
	}
}

/// <summary>
/// オーバーした演出
/// </summary>
/// <param name="frontOrBackSide">表側か裏側か？</param>
void CCapacityUI::Over(const int frontOrBackSide)
{
	//切り替え時間
	const float switchingTime = 1.1f;
	//フォントが特有の動きをする時間
	const float actionTime = 0.5f;

	//タイマーが0.0fか？、つまり最初の一回目のループか？
	if (m_timer[frontOrBackSide] == 0.0f)
	{
		//最初の一回目

		//通常の値に戻す
		DefaultParam(frontOrBackSide);

		//フォントのカラーをオーバー時のカラーにする
		m_RONumFR[frontOrBackSide]->SetColor({ 2.0f,0.0f,0.0f,1.0f });

		m_capacity_over->Play(false);		//再生
		
	}

	//加える拡大
	const float addScale = 0.8f;
	//数のフォントの拡大
	float numFontScale = m_defaultScale + addScale;
	//数のフォントの座標
	Vector2 numFontPosition = m_capacityPos[frontOrBackSide];
	//X座標を合わセル
	numFontPosition.x += m_diffCapacityToNum;

	//タイマーがアクションタイム以内か？
	if (m_timer[frontOrBackSide] <= actionTime)
	{
		//以内の時

		//加える座標
		Vector2 addPosition = { -20.0f,15.0f };

		//2次元ベクトルを回す方法
		//Vector2 bufPos = addPosition;
		//addPosition.x = 
		//	bufPos.x * std::cos(radian) - bufPos.y * std::sin(radian);
		//addPosition.y =
		//	bufPos.x * std::sin(radian) + bufPos.y * std::cos(radian);

		//数のフォントの座標を加算する
		numFontPosition.Add(addPosition);
	}
	else
	{
		//以外の時

		//通常の値に戻す
		//座標を初期化する
		numFontPosition = m_capacityPos[frontOrBackSide];
		numFontPosition.x += m_diffCapacityToNum;
		//拡大を初期化する
		numFontScale = m_defaultScale;
		//フォントのカラーを初期化する
		m_RONumFR[frontOrBackSide]->SetColor(m_defaultFontColor[frontOrBackSide]);

		//m_RONumFR[frontOrBackSide]->SetScale(m_defaultScale);
		////m_RONumFR[frontOrBackSide]->SetRotation(numFontRotation);
		//m_RONumFR[frontOrBackSide]->SetPosition(numFontPosition);
	}

	//数のフォントの拡大を設定する
	m_RONumFR[frontOrBackSide]->SetScale(numFontScale);
	//数のフォントの座標を設定する
	m_RONumFR[frontOrBackSide]->SetPosition(numFontPosition);

	//タイマーが切り替え時間以上か
	if (m_timer[frontOrBackSide] >= switchingTime)
	{
		//以上

		//タイマーを初期化する
		m_timer[frontOrBackSide] = 0.0f;
		//フォントのカラーを初期化する
		m_RONumFR[frontOrBackSide]->SetColor(m_defaultFontColor[frontOrBackSide]);
		//演出のステートを通常にする
		m_directingState[frontOrBackSide] = enNormal;


	}
	else
	{
		//未満

		//タイマーを進める
		m_timer[frontOrBackSide] += GameTime().GetFrameDeltaTime();
	}
}


/// <summary>
/// 通常の値に戻す
/// </summary>
void CCapacityUI::DefaultParam(const int frontOrBackSide)
{
	//通常の座標を計算する
	//キャパシティのUIの座標を持ってくる
	Vector2 numFontPos = m_capacityPos[frontOrBackSide];
	//X座標をそろえる
	numFontPos.x += m_diffCapacityToNum;

	//座標を初期化する
	m_RONumFR[frontOrBackSide]->SetPosition(numFontPos);

	//回転を初期化する
	m_RONumFR[frontOrBackSide]->SetRotation(0.0f);

	//拡大を初期化する
	m_RONumFR[frontOrBackSide]->SetScale(m_defaultScale);

	//カラーを初期化する
	m_RONumFR[frontOrBackSide]->SetColor(m_defaultFontColor[frontOrBackSide]);

}


void CCapacityUI::CheckActiveFontColor()
{
	//プレイヤーのウェイポイント
	const int lpIndex = m_player->GetLeftPointIndex();


	//左側のウェイポイントを調べて表側か裏側か調べる
	if ((25 <= lpIndex && lpIndex <= 31) ||
		(0 <= lpIndex && lpIndex <= 8))
	{
		//表側
		m_defaultFontColor[enFrontSide] = m_activeFontColor;
		m_defaultFontColor[enBackSide] = m_inactiveFontColor;

	}
	else if (9 <= lpIndex <= 24)
	{
		//裏側
		m_defaultFontColor[enFrontSide] = m_inactiveFontColor;
		m_defaultFontColor[enBackSide] = m_activeFontColor;
	}
	
	for (int frontOrBack = 0; frontOrBack < enFrontAndBackSideNum; frontOrBack++)
	{
		if (m_directingState[frontOrBack] == enNormal)
		{
			m_RONumFR[frontOrBack]->SetColor(m_defaultFontColor[frontOrBack]);
			m_RONumFR[frontOrBack]->SetColor(m_defaultFontColor[frontOrBack]);
		}
		m_capacityFR[frontOrBack]->SetColor(m_defaultFontColor[frontOrBack]);
		m_capacityFR[frontOrBack]->SetColor(m_defaultFontColor[frontOrBack]);
	}
}
