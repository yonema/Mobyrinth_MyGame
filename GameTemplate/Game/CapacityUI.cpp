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
	const float capacityX = -490.0f;
	const float capacityY = 270.0f;
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
		m_RONumFR[i] = NewGO<CFontRender>(1);
		m_RONumFR[i]->SetPostRenderFlag(true);
		//最大数を設定い
		CLevelObjectManager::GetInstance()->SetReversibleObjectMaxNum(i, 5);
	}

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
	//表側のフォントの初期化
	m_RONumFR[enFrontSide]->Init(L"",
		{ capacityNumX,capacityNumY }, { fontColor.x,fontColor.y,fontColor.z,1.0f },
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
	const float capacityNumDiff = 70.0f;
	//裏側のフォントの初期化
	m_RONumFR[enBackSide]->Init(L"",
		{ capacityNumX,capacityNumY - capacityNumDiff }, { fontColor.x,fontColor.y,fontColor.z,1.0f },
		0.0f, scale, { 0.5f,0.5f });

	//タイトル画面か？
	if (m_titleFlag)
	{
		//タイトル画面なら

		//非表示にする
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
	const int* maxNum = CLevelObjectManager::GetInstance()->GetReversibleObjectMaxNum();
	//テキストをセット
	std::wstring wString = L"あ";
	swprintf(text, L"表:%d /%d", num[enFrontSide], maxNum[enFrontSide]);
	m_RONumFR[enFrontSide]->SetText(text);
	swprintf(text, L"裏:%d /%d", num[enBackSide], maxNum[enBackSide]);
	m_RONumFR[enBackSide]->SetText(text);

	//どの演出を起こすかチェックする
	CheckDirecting(num, maxNum);


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
void CCapacityUI::CheckDirecting(const int* num, const int* maxNum)
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
			if (num[i] > maxNum[i])
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

/// <summary>
/// 減った演出
/// </summary>
/// <param name="frontOrBackSide">表側か裏側か？</param>
void CCapacityUI::Decreased(const int frontOrBackSide)
{
	//切り替え時間
	const float switchingTime = 2.0f;

	//フォントのカラーを減った時のカラーにする
	m_RONumFR[frontOrBackSide]->SetColor({ 0.0f,0.0f,20.0f,1.0f });

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