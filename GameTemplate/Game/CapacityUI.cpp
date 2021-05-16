#include "stdafx.h"
#include "CapacityUI.h"

//スタート関数
bool CCapacityUI::Start()
{
	//スプライトの初期化
	InitSprite();

	//フォントの初期化
	InitFont();

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
		//キャパシティのフォント
		m_capacityFR[i] = NewGO<CFontRender>(1);
		m_capacityFR[i]->SetPostRenderFlag(true);
		//最大数を設定
		CLevelObjectManager::GetInstance()->SetReversibleObjectMaxNum(i, m_maxCapacityNum[i]);
	}

	//キャパシティからアイテムの数のフォントへの差
	const float diffCapacityToNum = 100.0f;
	//テキスト用意
	wchar_t text[256];
	//現在の表側と裏側の反転オブジェクトの数を取得
	const int* maxNum = m_maxCapacityNum;


	//＊＊表側のフォントの設定＊＊
	//フォントの拡大
	const float scale = 1.5f;
	//フォントのカラー
	Vector3 fontColor;
	//表側のタイルの色
	fontColor = { 214.0f,85.0f,0.0f };
	//明るすぎるため正規化
	fontColor.Normalize();
	m_normalFontColor[enFrontSide] = fontColor;
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
		{ capacityNumX + diffCapacityToNum,capacityNumY }, { fontColor.x,fontColor.y,fontColor.z,1.0f },
		0.0f, scale, { 0.5f,0.5f });


	//＊＊裏側のフォントの設定＊＊
	//フォントのカラー
	//裏側のタイルの色
	fontColor = { 99.0f,97.0f,99.0f };
	//明るすぎるため正規化
	fontColor.Normalize();
	//色が一緒過ぎて見えにくかったから、暗くする
	fontColor.Scale(0.1);
	m_normalFontColor[enBackSide] = fontColor;
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
		{ capacityNumX + diffCapacityToNum,capacityNumY - diffFrontToBackY },
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

	if (m_firstFrame)
	{
		for (int i = 0; i < enFrontAndBackSideNum; i++)
		{
			m_oldRONum[i] = num[i];
		}
		m_firstFrame = false;
	}
	//どの演出を起こすかチェックする
	CheckDirecting(num);


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
	const float switchingTime = 2.0f;
	//フォントが特有の動きをする時間
	const float actionTime = 1.0f;

	//タイマーが0.0fか？、つまり最初の一回目のループか？
	if (m_timer[frontOrBackSide] == 0.0f)
	{
		//最初の一回目

		//フォントのカラーを増えた時のカラーにする
		m_RONumFR[frontOrBackSide]->SetColor({ 10.0f,0.0f,0.0f,1.0f });
	}

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

		//数のフォントの座標
		//現在の数のフォントの座標を取ってくる
		Vector2 numFontPos = m_RONumFR[frontOrBackSide]->GetPosition();
		//Y座標には最初の座標を入れておく
		numFontPos.y = m_capacityPos[frontOrBackSide].y;
		//はねる分だけ高くする
		numFontPos.y += y;

		//数のフォントの座標を設定する
		m_RONumFR[frontOrBackSide]->SetPosition(numFontPos);
	}
	else
	{
		//以外の時

		//通常の座標に戻しておく

		//数のフォントの座標
		//現在の数のフォントの座標を取ってくる
		Vector2 numFontPos = m_RONumFR[frontOrBackSide]->GetPosition();
		//Y座標には最初の座標を入れておく
		numFontPos.y = m_capacityPos[frontOrBackSide].y;
		//数のフォントの座標を設定する
		m_RONumFR[frontOrBackSide]->SetPosition(numFontPos);
	}

	//タイマーが切り替え時間以上か
	if (m_timer[frontOrBackSide] >= switchingTime)
	{
		//以上

		//タイマーを初期化する
		m_timer[frontOrBackSide] = 0.0f;
		//フォントのカラーを初期化する
		m_RONumFR[frontOrBackSide]->SetColor(m_normalFontColor[frontOrBackSide]);
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
	const float switchingTime = 2.0f;
	//フォントが特有の動きをする時間
	const float actionTime = 1.0f;

	//キャパシティからアイテムか数のフォントへの差
	const float diffCapacityToNum = 100.0f;

	//タイマーが0.0fか？、つまり最初の一回目のループか？
	if (m_timer[frontOrBackSide] == 0.0f)
	{
		//最初の一回目

		//フォントのカラーを減った時のカラーにする
		m_RONumFR[frontOrBackSide]->SetColor({ 0.0f,0.0f,20.0f,1.0f });
	}


	//タイマーがアクションタイム以内か？
	if (m_timer[frontOrBackSide] <= actionTime)
	{
		//以内の時

		const float oneEightTime = actionTime / 4.0f;

		const int state = m_timer[frontOrBackSide] / oneEightTime;

		const float destination = 10.0f;

		float moveLen = 0.0f;

		const float timeScale = 
			(m_timer[frontOrBackSide] - oneEightTime * state) / oneEightTime;


		switch (state)
		{
		case 0:
		case 4:
			moveLen = -destination * timeScale;
			break;
		case 1:
		case 5:
			moveLen = -destination + destination * timeScale;
			break;
		case 2:
		case 6:
			moveLen = destination * timeScale;
			break;
		case 3:
		case 7:
			moveLen = destination - destination * timeScale;
			break;

		}



		//数のフォントの座標
		//
		Vector2 numFontPos = m_capacityPos[frontOrBackSide];
		//X座標をそろえる
		numFontPos.x += diffCapacityToNum;
		numFontPos.x += moveLen;

		//数のフォントの座標を設定する
		m_RONumFR[frontOrBackSide]->SetPosition(numFontPos);
	}
	else
	{
		//以外の時

		//通常の座標に戻しておく

		//数のフォントの座標
		Vector2 numFontPos = m_capacityPos[frontOrBackSide];
		numFontPos.x += diffCapacityToNum;
		//数のフォントの座標を設定する
		m_RONumFR[frontOrBackSide]->SetPosition(numFontPos);
	}

	//タイマーが切り替え時間以上か
	if (m_timer[frontOrBackSide] >= switchingTime)
	{
		//以上

		//タイマーを初期化する
		m_timer[frontOrBackSide] = 0.0f;
		//フォントのカラーを初期化する
		m_RONumFR[frontOrBackSide]->SetColor(m_normalFontColor[frontOrBackSide]);
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
	const float switchingTime = 2.0f;

	//フォントのカラーを増えた時のカラーにする
	m_RONumFR[frontOrBackSide]->SetColor({ 10.0f,0.0f,0.0f,1.0f });

	//タイマーが切り替え時間以上か
	if (m_timer[frontOrBackSide] >= switchingTime)
	{
		//以上

		//タイマーを初期化する
		m_timer[frontOrBackSide] = 0.0f;
		//フォントのカラーを初期化する
		m_RONumFR[frontOrBackSide]->SetColor(m_normalFontColor[frontOrBackSide]);
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