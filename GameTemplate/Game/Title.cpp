#include "stdafx.h"
#include "Title.h"


//スタート関数
bool Title::Start()
{
	//背景
	m_stageTitle = NewGO<stage_title>(0, "stage_title");
	m_stageTitle->SetStartUpStartDirecting(false);
	m_stageTitle->SetTitlePlayer(true);
	m_stageTitle->SetStartBGM(false);
	m_stageTitle->SetWipeInFlag(m_wipeInFlag);

	//レベル2Dの初期化
	m_level2D.Init("Assets/level2D/Stage_selection3.casl", [&](Level2DObjectData& objdata)
		{
			//名前が一致でフックする

			//オブジェクトネームに"Stage_icon"があったら
			if (std::strstr(objdata.name, "Stage_icon") != NULL)
			{
				//番号（"n"）の文字列があるアドレスを返す
				//さらに、その次のポインタを指して、数字を得る
				std::string buff = (std::strstr(objdata.name, "n")) + 1;
				//stringをintに変換
				int num = atoi(buff.c_str());
				//ステージ名は1から始まるけど、配列は0から始まるから
				//デクリメントする
				num--;

				//すでに作られていないかチェック
				if (m_stageName[num] == nullptr)
				{
					//作られていないなら

					//読み込んだ番号にステージのアイコンを生成
					m_stageName[num] = NewGO<CSpriteRender>(1);
					m_stageName[num]->Init(objdata.ddsFilePath, objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
					m_stageName[num]->SetScale(objdata.scale);
					m_stageName[num]->SetPosition(objdata.position);
				}
				else
					//すでに作られていたら、そのまま表示
					return false;

				return true;
			}
			//オブジェクトネームに"Stage_clear"があったら
			else if (std::strstr(objdata.name, "Stage_clear") != NULL)
			{
				//番号（"r"）の文字列があるアドレスを返す
				//さらに、その次のポインタを指して、数字を得る
				std::string buff = (std::strstr(objdata.name, "r")) + 1;
				//stringをintに変換
				int num = atoi(buff.c_str());
				//ステージ名は1から始まるけど、配列は0から始まるから
				//デクリメントする
				num--;

				//すでに作られていないかチェック
				if (m_stageClear[num] == nullptr)
				{
					//作られていないなら

					//読み込んだ番号にステージクリアのアイコンを生成
					m_stageClear[num] = NewGO<CSpriteRender>(1);
					m_stageClear[num]->Init(objdata.ddsFilePath, objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
					m_stageClear[num]->SetScale(objdata.scale);
					m_stageClear[num]->SetPosition(objdata.position);
				}
				else
					//すでに作られていたら、そのまま表示
					return false;

				return true;
			}
			//カーソル
			else if (objdata.EqualObjectName("cursor"))
			{
				m_cursor = NewGO<CSpriteRender>(1);
				m_cursor->Init("Assets/level2D/cursor.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_cursor->SetScale(objdata.scale);
				m_cursor->SetPosition(objdata.position);
				//フックしたらtrueを戻す
				return true;
			}
			//ステージセレクト
			if (objdata.EqualObjectName("Stage_selection"))
			{
				m_stageSelection = NewGO<CSpriteRender>(1);
				m_stageSelection->Init("Assets/level2D/Stage_selection.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_stageSelection->SetScale(objdata.scale);
				m_stageSelection->SetPosition(objdata.position);
				//フックしたらtrueを戻す
				return true;
			}
			//ステージ選択背景
			if (objdata.EqualObjectName("Stage_selection_base"))
			{
				m_stageSelectionBase = NewGO<CSpriteRender>(1);
				m_stageSelectionBase->Init("Assets/level2D/Stage_selection_base.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_stageSelectionBase->SetScale(objdata.scale);
				m_stageSelectionBase->SetPosition(objdata.position);
				//スプライトに色を乗算する。
				//乗算だから、0.0fを掛けると色が消えるよ。
				//透過するだけだったら、RGBのところは1.0fを掛けてね。
				m_stageSelectionBase->SetMulColor({ 1.0f,1.0f,1.0f,0.8f });
				//フックしたらtrueを戻す
				return true;
			}
			

			//そのまま表示するからfalseを戻す
			return false;
		});

	//最初はステージ名は表示しないから、無効化して非表示にする
	for (int i = 0; i < enStageNum; i++)
	{
		m_stageName[i]->Deactivate();
		m_stageClear[i]->Deactivate();
	}

	m_cursor->Deactivate();
	m_stageSelection->Deactivate();
	m_stageSelectionBase->Deactivate();


	//タイトル画面の表示
	m_level2D.Init("Assets/level2D/Title.casl", [&](Level2DObjectData& objdata)
		{
			//名前が一致でフックする
			if (objdata.EqualObjectName("Title"))
			{
				m_title = NewGO<CSpriteRender>(1);
				m_title->Init("Assets/level2D/Title.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_title->SetScale(objdata.scale);
				m_title->SetPosition(objdata.position);
				m_title->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
				//フックしたらtrueを戻す
				return true;
			}
			//名前が一致でフックする
			if (objdata.EqualObjectName("Press_A_Button"))
			{
				m_pressAButton = NewGO<CSpriteRender>(1);
				m_pressAButton->Init("Assets/level2D/Press_A_Button.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_pressAButton->SetScale(objdata.scale);
				m_pressAButton->SetPosition(objdata.position);
				m_pressAButton->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
				//フックしたらtrueを戻す
				return true;
			}
			//そのまま表示するからfalseを戻す
			return false;
		});

	//ステージのアイコンからカーソルへのベクトルを設定
	m_stageIconToCursorVec = m_cursor->GetPosition() - m_stageName[enStage1]->GetPosition();




	//buttonASEのサウンドキューを生成する
	m_buttonASE = NewGO<CSoundCue>(0);
	//buttonASEのサウンドキューを、waveファイルを指定して初期化する。
	m_buttonASE->Init(L"Assets/sound/buttonA.wav");
	//音量調節
	m_buttonASE->SetVolume(0.5f);

	//buttonBのサウンドキューを生成する
	m_buttonBSE = NewGO<CSoundCue>(0);
	//buttonBのサウンドキューを、waveファイルを指定して初期化する。
	m_buttonBSE->Init(L"Assets/sound/buttonB.wav");
	//音量調節
	m_buttonBSE->SetVolume(0.5f);

	//selectSEのサウンドキューを生成する
	m_selectSE = NewGO<CSoundCue>(0);
	//selectSEのサウンドキューを、waveファイルを指定して初期化する。
	m_selectSE->Init(L"Assets/sound/select.wav");
	//音量調節
	m_selectSE->SetVolume(0.5f);


	return true;
}

//デストラクタ
Title::~Title()
{
	DeleteGO(m_stageTitle);

	for (int i = 0; i < enStageNum; i++)
	{
		DeleteGO(m_stageName[i]);
		DeleteGO(m_stageClear[i]);
	}
	//DeleteGO(m_arrow);

	//画像データ
	DeleteGO(m_title);
	DeleteGO(m_pressAButton);
	DeleteGO(m_cursor);
	DeleteGO(m_stageSelection);
	DeleteGO(m_stageSelectionBase);

	DeleteGO(m_bgmTitle);

}

//アップデート関数
void Title::Update()
{
	//アニメーションの初期化でエラーが出るのを調べる用のコード
	//デバック用
	//後で消す
	//if (m_stageState == enStartTitle)
	//	//ランダムワイプアウトをする
	//	g_sceneChange->RandomWipeOut();

	//m_stageState = enStageDecision;
	//m_stageSelectState = enStage1;
	//デバック用ここまで


	//現在のステージのステート（状態）で処理を振り分ける
	switch (m_stageState)
	{
	case enStartTitle:
		StartTitle();
		break;
	case enTitleScreen:
		//タイトル画面
		TitleScreen();
		break;
	case enStageSelect:
		//ステージセレクト
		StageSelect();
		break;
	case enStageDecision:
		StageDecision();
		break;
	default:
		break;
	}
	

	//Start関数ではなくUpdate関数でBGMを初期化しているのは
	//ステージをロード中にBGMが再生しないようにするため。

	//BGMを初期化していなかったら
	if (!m_initedBGM)
	{
		//BGMを初期化する
		InitBGM();
	}

}

//BGMの初期化
void Title::InitBGM()
{
	//BGMのサウンドキューを生成する
	m_bgmTitle = NewGO<CSoundCue>(0);
	//BGMのサウンドキューを、waveファイルを指定して初期化する。
	m_bgmTitle->Init(L"Assets/sound/Title.wav");
	//BGMをループ再生をオンで再生する。
	m_bgmTitle->SetSoundType(CSoundCue::enBGM);
	m_bgmTitle->Play(true);
	m_bgmTitle->SetVolume(0.5f);
	
	m_initedBGM = true;
}

//起動時の演出
void Title::StartTitle()
{
	++m_countStartTitle;

	m_title->SetMulColor({ 1.0f,1.0f,1.0f,m_countStartTitle / 120.0f });

	if (g_graphicsEngine->GetSceneChange().IsWipeFinished())
	{
		if (m_countStartTitle == 120 || g_pad[0]->IsTrigger(enButtonA)) {
			m_stageState = enTitleScreen;
			//ここでタイトルアイコンの位置を設定する。
			m_title->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });
			//ボタンを押すことができないようにする（連続入力防止用）
			m_buttonFlag = false;
		}
	}
}

//タイトル画面
void Title::TitleScreen()
{
	//ボタンの入力を調べる
	if (g_pad[0]->GetLStickYF() == 0.0f && !g_pad[0]->IsPressAnyKey())
	{
		//何も入力がない状態
		//ボタンを押すことができるようにする（連続入力防止用）
		m_buttonFlag = true;
	}
	else if (g_pad[0]->IsTrigger(enButtonA) && m_buttonFlag)
	{
		//Aボタンを入力
		//ボタンを押すことができないようにする（連続入力防止用）
		m_buttonFlag = false;
		
		//buttonASEをループ再生をオフで再生する。
		m_buttonASE->Play(false);

		//ステージのステート（状態）をステージセレクトに移行する。
		m_stageState = enStageSelect;
		//ステージ選択状態を初期設定にする。
		m_stageSelectState = enStage1;

		//タイトル画面用のスプライトレンダラーを無効化して非表示にする
		m_title->Deactivate();
		m_pressAButton->Deactivate();


		//ステージセレクト用のスプライトレンダラーを有効化して表示できるようにする
		for (int i = 0; i < enStageNum; i++)
		{
			//ステージのアイコンを有効化する
			m_stageName[i]->Activate();
			//一番高いクリアしたステージの番号より大きいか？
			if (i > m_stageTitle->GetHighestClearStageNum() + 1)
			{
				//大きいとき
				//カラーを薄暗くする
				m_stageName[i]->SetMulColor({ 0.4f,0.4f,0.4f,1.0f });
			}

			//一番高いクリアしたステージの番号以下か？
			if (i <= m_stageTitle->GetHighestClearStageNum())
			{
				//クリアしたアイコンを有効化する
				m_stageClear[i]->Activate();
			}
		}

		m_cursor->Activate();
		m_stageSelection->Activate();
		m_stageSelectionBase->Activate();
	}


	//Press_A_Buttonの点滅処理
	if (m_blinkingFlag == true) {
		m_pressAButton->SetMulColor({ 1.0f,1.0f,1.0f,m_pressAButton->GetMulColorW() - 0.02f });

		if (m_pressAButton->GetMulColorW() <= 0.0f) {
			m_blinkingFlag = false;
		}
	}
	else {
		m_pressAButton->SetMulColor({ 1.0f,1.0f,1.0f,m_pressAButton->GetMulColorW() + 0.02f });

		if (m_pressAButton->GetMulColorW() >= 0.8f) {
			m_blinkingFlag = true;
		}
	}

}

//ステージセレクト
void Title::StageSelect()
{
	//ボタンの入力を調べる
	if (g_pad[0]->GetLStickXF() == 0.0f && g_pad[0]->GetLStickYF() == 0.0f &&
		!g_pad[0]->IsPressAnyKey())
	{
		//何も入力がない状態
		//ボタンを押すことができるようにする（連続入力防止用）
		m_buttonFlag = true;
	}

	if (g_pad[0]->GetLStickXF() > 0.5f && m_buttonFlag)
	{
		//右を入力

		//ボタンを押すことができないようにする（連続入力防止用）
		m_buttonFlag = false;
		
		//ステージセレクトのステートが特定の時のみ
		if (m_stageSelectState != (enStageNum - 1) && 
			m_stageSelectState <= m_stageTitle->GetHighestClearStageNum())
		{
			//ステージセレクトのステートが最後まで行っていないとき、かつ
			//一番高いクリアしたステージの番号以下の時

			//ステージセレクトのステートを加算する
			m_stageSelectState++;
			//selectSEをループ再生をオフで再生する。
			m_selectSE->Play(false);
		}
		else
		{
			//該当しない時

			//SEを鳴らさないか、
			//m_selectSEとは別の音を鳴らしたい
		}


	}
	else if (g_pad[0]->GetLStickXF() < -0.5f && m_buttonFlag)
	{
		//左を入力

		//ボタンを押すことができないようにする（連続入力防止用）
		m_buttonFlag = false;
		
		//ステージセレクトのステートが特定の時のみ
		if (m_stageSelectState != enStage1)
		{
			//ステージセレクトのステートが一番小さいステージの番号ではない時

			//ステージセレクトのステートを減算する
			m_stageSelectState--;
			//selectSEをループ再生をオフで再生する。
			m_selectSE->Play(false);
		}
		else
		{
			//該当しない時

			//SEを鳴らさないか、
			//m_selectSEとは別の音を鳴らしたい
		}

	}
	else if (g_pad[0]->GetLStickYF() > 0.5f && m_buttonFlag)
	{
		//上を入力

		//ボタンを押すことができないようにする（連続入力防止用）
		m_buttonFlag = false;

		//ステージセレクトのステートが特定の時のみ
		if (m_stageSelectState != enStage1)
		{
			//ステージセレクトのステートが一番小さいステージの番号ではない時

			//ステージセレクトのステートを減算する
			m_stageSelectState--;
			//selectSEをループ再生をオフで再生する。
			m_selectSE->Play(false);
		}
		else
		{
			//該当しない時

			//SEを鳴らさないか、
			//m_selectSEとは別の音を鳴らしたい
		}

	}
	else if (g_pad[0]->GetLStickYF() < -0.5f && m_buttonFlag)
	{
		//下を入力

		//ボタンを押すことができないようにする（連続入力防止用）
		m_buttonFlag = false;

		//ステージセレクトのステートが特定の時のみ
		if (m_stageSelectState != (enStageNum - 1) &&
			m_stageSelectState <= m_stageTitle->GetHighestClearStageNum())
		{
			//ステージセレクトのステートが最後まで行っていないとき、かつ
			//一番高いクリアしたステージの番号以下の時

			//ステージセレクトのステートを加算する
			m_stageSelectState++;
			//selectSEをループ再生をオフで再生する。
			m_selectSE->Play(false);
		}
		else
		{
			//該当しない時

			//SEを鳴らさないか、
			//m_selectSEとは別の音を鳴らしたい
		}

	}
	else if (g_pad[0]->IsTrigger(enButtonA) && m_buttonFlag)
	{
		//Aボタンを入力
		//ボタンを押すことができないようにする（連続入力防止用）
		m_buttonFlag = false;

		//ランダムワイプアウトをする
		g_graphicsEngine->GetSceneChange().RandomWipeOut();

		//ステージのステートをステージを決定した状態にする
		m_stageState = enStageDecision;

		//buttonAをループ再生をオフで再生する。
		m_buttonASE->Play(false);
	}
	else if (g_pad[0]->IsTrigger(enButtonB) && m_buttonFlag)
	{
		//Bボタンを入力
		//ボタンを押すことができないようにする（連続入力防止用）
		m_buttonFlag = false;

		
		//buttonBをループ再生をオフで再生する。
		m_buttonBSE->Play(false);

		//ステージのステートをタイトル画面にする
		m_stageState = enTitleScreen;

		//ステージセレクトの画像無効化して表示できないようにする
		for (int i = 0; i < enStageNum; i++)
		{
			m_stageName[i]->Deactivate();
			m_stageClear[i]->Deactivate();
		}
		m_cursor->Deactivate();
		m_stageSelection->Deactivate();
		m_stageSelectionBase->Deactivate();


		//タイトル画面の画像を有効化して表示する。
		m_title->Activate();
		m_pressAButton->Activate();

		m_blinkingFlag = false;
		m_pressAButton->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
	}
	//デバック用
	//全部のステージが選べるようになる
	else if (g_pad[0]->IsTrigger(enButtonStart) && g_pad[0]->IsTrigger(enButtonRB1))
	{
		m_stageTitle->SetHighestClearStageNum(8);
		//ステージセレクト用のスプライトレンダラーを有効化して表示できるようにする
		for (int i = 0; i < enStageNum; i++)
		{
			//ステージのアイコンを有効化する
			m_stageName[i]->Activate();
			m_stageName[i]->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });
			//一番高いクリアしたステージの番号より大きいか？
			if (i > m_stageTitle->GetHighestClearStageNum() + 1)
			{
				//大きいとき
				//カラーを薄暗くする
				m_stageName[i]->SetMulColor({ 0.4f,0.4f,0.4f,1.0f });
			}
		}

	}
	//デバック用ここまで



	m_cursor->SetPosition
	(m_stageName[m_stageSelectState]->GetPosition() + m_stageIconToCursorVec);
}

//ステージを決定した時の処理
void Title::StageDecision()
{
	//ワイプが終わるまで処理しない
	if (!g_graphicsEngine->GetSceneChange().IsWipeFinished())
		return;

	////////////////////////////////////////////////////////////
	//ステージを新しく作成した場合、ここでNewGOを行う。
	////////////////////////////////////////////////////////////

	CStage* stage = nullptr;	//ステージのポインタ

	//ステージセレクトのステートによって生成するステージを振り分ける
	switch (m_stageSelectState)
	{
	case enStage1:
		//ステージを生成
		stage = NewGO<CStage>(0, "stage");
		//ステージの初期化、レベルのファイルパスを指定する。
		stage->Init("Assets/level/O_easy.tkl", m_stageSelectState);
		break;
	case enStage2:
		stage = NewGO<CStage>(0, "stage");
		stage->Init("Assets/level/Y_easy.tkl", m_stageSelectState);
		break;
	case enStage3:
		stage = NewGO<CStage>(0, "stage");
		stage->Init("Assets/level/O_normal.tkl", m_stageSelectState);
		break;
	case enStage4:
		stage = NewGO<CStage>(0, "stage");
		//このステージはスイッチのタイムを12秒に設定する
		stage->SetSwitchTime(12.0f);
		stage->Init("Assets/level/T_hard.tkl", m_stageSelectState);
		break;
	case enStage5:
		stage = NewGO<CStage>(0, "stage");
		stage->Init("Assets/level/T_normal.tkl", m_stageSelectState);
		break;
	case enStage6:
		stage = NewGO<CStage>(0, "stage");
		stage->Init("Assets/level/Y_normal.tkl", m_stageSelectState);
		break;
	case enStage7:
		stage = NewGO<CStage>(0, "stage");
		stage->Init("Assets/level/O_hard.tkl", m_stageSelectState);
		break;
	case enStage8:
		stage = NewGO<CStage>(0, "stage");
		stage->Init("Assets/level/T_easy.tkl", m_stageSelectState);
		break;
	case enStage9:
		stage = NewGO<CStage>(0, "stage");
		stage->Init("Assets/level/Y_hard.tkl", m_stageSelectState);
		break;
	default:
		break;
	}

	//自身のオブジェクトを破棄する
	Release();


}