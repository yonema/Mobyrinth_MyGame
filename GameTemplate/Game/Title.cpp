#include "stdafx.h"
#include "Title.h"

//タイトルの定数データを使えるようにする
using namespace titleConstData;

//スタート関数
bool CTitle::Start()
{
	//背景の生成と初期化
	m_stageTitle = NewGO<CStage_title>(PRIORITY_FIRST);
	//開始演出をしないようにする
	m_stageTitle->TitleMode();
	//ワイプをするかしないか設定する
	m_stageTitle->SetWipeInFlag(m_wipeInFlag);

	//タイトルで使うスプライトの初期化
	InitTitleSprite();

	//ステージセレクトで使うスプライトの初期化
	InitStageSelectSprite();
	
	// SEの初期化
	InitSE();


	return true;
}

//デストラクタ
CTitle::~CTitle()
{
	//ステージタイトルクラスの破棄
	DeleteGO(m_stageTitle);


	//画像データ
	for (int i = 0; i < EN_STAGE_NUM; i++)
	{
		//ステージのアイコンの破棄
		DeleteGO(m_stageIcon[i]);
		//ステージリアの破棄
		DeleteGO(m_stageClear[i]);
	}
	DeleteGO(m_title);					//タイトルの破棄
	DeleteGO(m_pressAButton);			//「Aボタンをおしてね」の破棄
	DeleteGO(m_cursor);					//カーソルの破棄
	DeleteGO(m_stageSelection);			//ステージセレクトの破棄
	DeleteGO(m_stageSelectionBase);		//ステージ選択背景の破棄

	//サウンドデータ
	DeleteGO(m_bgmTitle);	//タイトルのBGMの破棄
	DeleteGO(m_buttonASE);	//AボタンのSEを破棄
	DeleteGO(m_buttonBSE);	//BボタンのSEを破棄
	DeleteGO(m_selectSE);	//カーソル移動のSEの破棄

}

//アップデート関数
void CTitle::Update()
{
	//現在のステージのステート（状態）で処理を振り分ける
	switch (m_stageState)
	{
	case EN_START_TITLE:
		//起動時の演出の処理
		StartTitle();
		break;
	case EN_TITLE_SCREEN:
		//タイトル画面
		TitleScreen();
		break;
	case EN_STAGE_SELECT:
		//ステージセレクト
		StageSelect();
		break;
	case EN_STAGE_DESICION:
		//ステージを決定した時の処理
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

	return;
}

/**
 * @brief タイトルで使うスプライトの初期化
*/
void CTitle::InitTitleSprite()
{
	//この関数内で
	//スプライトの定数データを使えるようにする
	using namespace spriteRenderConstData;

	//タイトル画面の表示
	m_level2D.Init(LEVEL_2D_FILEPATH_TITLE, [&](Level2DObjectData& objdata)
		{
			//名前が一致でフックする

			//タイトルのスプライトだったら
			if (objdata.EqualObjectName(LEVEL_2D_OBJECT_NAME_TITLE))
			{
				//タイトルの生成と初期化
				m_title = NewGO<CSpriteRender>(PRIORITY_SECOND);
				m_title->Init(
					objdata.ddsFilePath,	//ファイルパス
					objdata.width,			//横幅
					objdata.height,			//縦幅
					DEFAULT_PIVOT,			//ピボット
					AlphaBlendMode_Trans	//透過合成モード
				);
				//座標と拡大を設定
				m_title->SetScale(objdata.scale);
				m_title->SetPosition(objdata.position);
				//透明にする
				m_title->SetMulColor(g_VEC4_TRANSPARENT);
				//フックしたらtrueを戻す
				return true;
			}
			//「Aボタンをおしてね」のスプライト
			if (objdata.EqualObjectName(LEVEL_2D_OBJECT_NAME_PRESS_A_BUTTON))
			{
				//「Aボタンをおしてね」の生成と初期化
				m_pressAButton = NewGO<CSpriteRender>(PRIORITY_SECOND);
				m_pressAButton->Init(
					objdata.ddsFilePath,
					objdata.width,
					objdata.height,
					DEFAULT_PIVOT,
					AlphaBlendMode_Trans
				);
				//座標と拡大を設定
				m_pressAButton->SetScale(objdata.scale);
				m_pressAButton->SetPosition(objdata.position);
				//透明にする
				m_pressAButton->SetMulColor(g_VEC4_TRANSPARENT);
				//フックしたらtrueを戻す
				return true;
			}
			//そのまま表示するからfalseを戻す
			return false;
		});

	return;
}

/**
 * @brief ステージセレクトで使うスプライトの初期化
*/
void CTitle::InitStageSelectSprite()
{
	//この関数内で
	//スプライトの定数データを使えるようにする
	using namespace spriteRenderConstData;

	//レベル2Dの初期化
	m_level2D.Init(LEVEL_2D_FILEPATH_STAGE_SELECTION, [&](Level2DObjectData& objdata)
		{
			//名前が一致でフックする

			//オブジェクトネームに"Stage_icon"があったら
			if (std::strstr(objdata.name, LEVEL_2D_OBJECT_NAME_STAGE_ICON) != NULL)
			{
				//Stage_icon1みたいな名前になっているから
				// "n" の文字列があるアドレスを返す
				//そして、その次のポインタを指して、数字を得る
				std::string buff =
					(std::strstr(objdata.name, CHAR_GET_STAGE_ICON_NUM)) + 1;
				//stringをintに変換
				int num = atoi(buff.c_str());
				//ステージ名は1から始まるけど、配列は0から始まるから
				//デクリメントする
				num--;

				//すでに作られていないかチェック
				if (m_stageIcon[num] == nullptr)
				{
					//作られていないなら

					//読み込んだ番号にステージのアイコンを生成
					m_stageIcon[num] = NewGO<CSpriteRender>(PRIORITY_SECOND);
					m_stageIcon[num]->Init(
						objdata.ddsFilePath,	//ファイルパス
						objdata.width,			//横幅
						objdata.height,			//縦幅
						DEFAULT_PIVOT,			//ピボット
						AlphaBlendMode_Trans	//透過合成モード
					);
					//座標と拡大を設定する
					m_stageIcon[num]->SetScale(objdata.scale);
					m_stageIcon[num]->SetPosition(objdata.position);
				}
				else
				{
					//すでに作られていたらエラーを出す
					MessageBoxA(
						nullptr, "ステージのアイコンが重複しています", "エラー", MB_OK
					);
				}

				//フックしたためtrueを戻す
				return true;
			}
			//オブジェクトネームに"Stage_clear"があったら
			else if (std::strstr(objdata.name, LEVEL_2D_OBJECT_NAME_STAGE_CLEAR) != NULL)
			{
				//番号（"r"）の文字列があるアドレスを返す
				//さらに、その次のポインタを指して、数字を得る
				std::string buff =
					(std::strstr(objdata.name, STAGE_CLEAR_GET_NUM_CHAR)) + 1;
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
					m_stageClear[num] = NewGO<CSpriteRender>(PRIORITY_SECOND);
					m_stageClear[num]->Init(
						objdata.ddsFilePath,
						objdata.width,
						objdata.height,
						DEFAULT_PIVOT,
						AlphaBlendMode_Trans);
					//座標と拡大を設定
					m_stageClear[num]->SetScale(objdata.scale);
					m_stageClear[num]->SetPosition(objdata.position);
				}
				else
				{
					//すでに作られていたらエラーを出す
					MessageBoxA(
						nullptr, "ステージのアイコンが重複しています", "エラー", MB_OK
					);
				}

				return true;
			}
			//カーソル
			else if (objdata.EqualObjectName(LEVEL_2D_OBJECT_NAME_CURSOR))
			{
				//カーソルのスプライトの生成と初期化
				m_cursor = NewGO<CSpriteRender>(PRIORITY_SECOND);
				m_cursor->Init(
					objdata.ddsFilePath,
					objdata.width,
					objdata.height,
					DEFAULT_PIVOT,
					AlphaBlendMode_Trans);
				//座標と拡大を設定
				m_cursor->SetScale(objdata.scale);
				m_cursor->SetPosition(objdata.position);
				//フックしたらtrueを戻す
				return true;
			}
			//ステージセレクト
			if (objdata.EqualObjectName(LEVEL_2D_OBJECT_NAME_STAGE_SELECTION))
			{
				//ステージセレクトのスプライトの生成と初期化
				m_stageSelection = NewGO<CSpriteRender>(PRIORITY_SECOND);
				m_stageSelection->Init(
					objdata.ddsFilePath,
					objdata.width,
					objdata.height,
					DEFAULT_PIVOT,
					AlphaBlendMode_Trans);
				//座標と拡大を設定する
				m_stageSelection->SetScale(objdata.scale);
				m_stageSelection->SetPosition(objdata.position);
				//フックしたらtrueを戻す
				return true;
			}
			//ステージ選択背景
			if (objdata.EqualObjectName(LEVEL_2D_OBJECT_NAME_STAGE_SELECTION_BASE))
			{
				//ステージ選択背景の生成と初期化
				m_stageSelectionBase = NewGO<CSpriteRender>(PRIORITY_SECOND);
				m_stageSelectionBase->Init(
					objdata.ddsFilePath,
					objdata.width,
					objdata.height,
					DEFAULT_PIVOT,
					AlphaBlendMode_Trans);
				//座標と拡大を設定する
				m_stageSelectionBase->SetScale(objdata.scale);
				m_stageSelectionBase->SetPosition(objdata.position);
				//スプライトに色を乗算する。
				//少し透明にする
				m_stageSelectionBase->SetMulColor(SPRITE_MUL_COLOR_STAGE_SELECTION_BASE);
				//フックしたらtrueを戻す
				return true;
			}


			//そのまま表示するからfalseを戻す
			return false;
		});

	//タイトル画面では、ここのスプライト達は表示しない

	for (int i = 0; i < EN_STAGE_NUM; i++)
	{
		//ステージのアイコンを無効化
		m_stageIcon[i]->Deactivate();
		//ステージクリアのアイコンを無効化
		m_stageClear[i]->Deactivate();
	}

	//カーソルを無効化
	m_cursor->Deactivate();
	//ステージセレクトを無効化
	m_stageSelection->Deactivate();
	//ステージ選択背景を無効化
	m_stageSelectionBase->Deactivate();

	//ステージのアイコンからカーソルへのベクトルを設定
	m_stageIconToCursorVec = m_cursor->GetPosition() - m_stageIcon[EN_STAGE_1]->GetPosition();

	return;
}

/**
 * @brief SEの初期化
*/
void CTitle::InitSE()
{
	//buttonASEのサウンドキューを生成する
	m_buttonASE = NewGO<CSoundCue>(PRIORITY_FIRST);
	//buttonASEのサウンドキューを、waveファイルを指定して初期化する。
	m_buttonASE->Init(SOUND_FILEPATH_SE_BUTTON_A);
	//音量調節
	m_buttonASE->SetVolume(SOUND_VOLUME_SE_BUTTON_A);

	//buttonBのサウンドキューを生成する
	m_buttonBSE = NewGO<CSoundCue>(PRIORITY_FIRST);
	//buttonBのサウンドキューを、waveファイルを指定して初期化する。
	m_buttonBSE->Init(SOUND_FILEPATH_SE_BUTTON_B);
	//音量調節
	m_buttonBSE->SetVolume(SOUND_VOLUME_SE_BUTTON_B);

	//selectSEのサウンドキューを生成する
	m_selectSE = NewGO<CSoundCue>(PRIORITY_FIRST);
	//selectSEのサウンドキューを、waveファイルを指定して初期化する。
	m_selectSE->Init(SOUND_FILEPATH_SE_SELECT);
	//音量調節
	m_selectSE->SetVolume(SOUND_VOLUME_SE_SELECT);

	return;
}

//BGMの初期化
void CTitle::InitBGM()
{
	//BGMのサウンドキューを生成する
	m_bgmTitle = NewGO<CSoundCue>(PRIORITY_FIRST);
	//BGMのサウンドキューを、waveファイルを指定して初期化する。
	m_bgmTitle->Init(SOUND_FILEPATH_BGM_TITLE);
	//サウンドタイプをBGMに設定する
	m_bgmTitle->SetSoundType(CSoundCue::enBGM);
	//BGMをループ再生をオンで再生する。
	m_bgmTitle->Play(true);
	//音量を設定する
	m_bgmTitle->SetVolume(SOUND_VOLUME_BGM_TITLE);
	
	//BGMを初期化済みにする
	m_initedBGM = true;

	return;
}

//起動時の演出
void CTitle::StartTitle()
{
	//カウンターを進める
	++m_countStartTitle;

	//透明率。カウントが進むにつれて不透明になっていく
	const float transparentRate = static_cast<float>(m_countStartTitle) / MAX_COUNT_START_TITLE;

	//タイトルのスプライトの乗算カラーの設定する
	//だんだん透明から不透明にしていく
	m_title->SetMulColor({ 1.0f,1.0f,1.0f,transparentRate });

	//ワイプが終了したか？
	if (g_graphicsEngine->GetSceneChange().IsWipeFinished())
	{
		//ワイプが終了している状態で

		//開始の演出のカウンターが最大以上になるか
		//Aボタンを押すか？
		if (m_countStartTitle >= MAX_COUNT_START_TITLE || g_pad[0]->IsTrigger(enButtonA)) 
		{
			//ステートをタイトル状態に移行する
			m_stageState = EN_TITLE_SCREEN;
			//ここでタイトルアイコンの乗算カラーを不透明で固定する
			m_title->SetMulColor(g_VEC4_OPACITY);
			//ボタンを押すことができないようにする（連続入力防止用）
			m_buttonFlag = false;
		}
	}
}

//タイトル画面
void CTitle::TitleScreen()
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
		m_stageState = EN_STAGE_SELECT;
		//ステージ選択状態を初期設定にする。
		m_stageSelectState = EN_STAGE_1;

		//タイトル画面用のスプライトレンダラーを無効化して非表示にする
		m_title->Deactivate();
		m_pressAButton->Deactivate();


		//ステージセレクト用のスプライトレンダラーを有効化して表示できるようにする
		for (int i = 0; i < EN_STAGE_NUM; i++)
		{
			//ステージのアイコンを有効化する
			m_stageIcon[i]->Activate();

			//一番高いクリアしたステージの番号より大きいか？
			//ステージの番号は1から始まるが、セーブデータは0から始まるため
			//1加算して比較する
			if (i > m_stageTitle->GetHighestClearStageNum() + 1)
			{
				//大きいとき
				//まだ遊べないステージのため、カラーを薄暗くする
				m_stageIcon[i]->SetMulColor(SPRITE_MUL_COLOR_STAGE_PLAY_CANNOT);
			}
			//以下の時は、遊べるステージのため、そのまま表示


			//一番高いクリアしたステージの番号以下か？
			if (i <= m_stageTitle->GetHighestClearStageNum())
			{
				//以下の時
				//クリアしたアイコンを有効化する
				m_stageClear[i]->Activate();
			}
		}

		//カーソルを有効化
		m_cursor->Activate();
		//ステージセレクトを有効化
		m_stageSelection->Activate();
		//ステージ選択背景を有効化
		m_stageSelectionBase->Activate();
	}


	//Press_A_Buttonの点滅処理
	//点滅のフラグが立っているか？
	if (m_blinkingFlag == true) 
	{
		//立っている
		//だんだんと透明にする
		m_pressAButton->SetMulColor(
			{ 1.0f,1.0f,1.0f,m_pressAButton->GetMulColor().w - SPEED_CHANGE_ALPHA_PRESS_A_BUTTON }
		);

		//完全に透明になったら
		if (m_pressAButton->GetMulColor().w <= 0.0f) 
		{
			//フラグを折る
			m_blinkingFlag = false;
		}
	}
	else 
	{
		//折れている
		//だんだんと不透明にする
		m_pressAButton->SetMulColor(
			{ 1.0f,1.0f,1.0f,m_pressAButton->GetMulColor().w + SPEED_CHANGE_ALPHA_PRESS_A_BUTTON }
		);

		//不透明になったら
		if (m_pressAButton->GetMulColor().w >= THRESHOLD_OPACITY_PRESS_A_BUTTON)
		{
			//フラグを立てる
			m_blinkingFlag = true;
		}
	}

	return;

}

//ステージセレクト
void CTitle::StageSelect()
{
	//ボタンの入力を調べる
	if (g_pad[0]->GetLStickXF() == 0.0f && g_pad[0]->GetLStickYF() == 0.0f &&
		!g_pad[0]->IsPressAnyKey())
	{
		//何も入力がない状態
		//ボタンを押すことができるようにする（連続入力防止用）
		m_buttonFlag = true;
	}

	if (g_pad[0]->GetLStickXF() > THRESHOLD_STICK_L_VALUE && m_buttonFlag)
	{
		//右を入力

		//次のステージを選択する
		SelectNextStage();
	}
	else if (g_pad[0]->GetLStickXF() < -THRESHOLD_STICK_L_VALUE && m_buttonFlag)
	{
		//左を入力

		//前のステージを選択
		SelectPrevStage();
	}
	else if (g_pad[0]->GetLStickYF() > THRESHOLD_STICK_L_VALUE && m_buttonFlag)
	{
		//上を入力

		//前のステージを選択
		SelectPrevStage();
	}
	else if (g_pad[0]->GetLStickYF() < -THRESHOLD_STICK_L_VALUE && m_buttonFlag)
	{
		//下を入力

		//次のステージを選択する
		SelectNextStage();
	}
	else if (g_pad[0]->IsTrigger(enButtonA) && m_buttonFlag)
	{
		//Aボタンを入力
		//ボタンを押すことができないようにする（連続入力防止用）
		m_buttonFlag = false;

		//ランダムワイプアウトをする
		g_graphicsEngine->GetSceneChange().RandomWipeOut();

		//ステージのステートをステージを決定した状態にする
		m_stageState = EN_STAGE_DESICION;

		//buttonAをワンショット再生で再生する。
		m_buttonASE->Play(false);
	}
	else if (g_pad[0]->IsTrigger(enButtonB) && m_buttonFlag)
	{
		//Bボタンを入力
		//ボタンを押すことができないようにする（連続入力防止用）
		m_buttonFlag = false;


		//buttonBをワンショット再生で再生する。
		m_buttonBSE->Play(false);

		//ステージのステートをタイトル画面にする
		m_stageState = EN_TITLE_SCREEN;

		//ステージセレクトの画像無効化して表示できないようにする
		for (int i = 0; i < EN_STAGE_NUM; i++)
		{
			//ステージのアイコンを無効化する
			m_stageIcon[i]->Deactivate();
			//ステージクリアのアイコンを無効化する
			m_stageClear[i]->Deactivate();
		}
		//カーソルを無効化する
		m_cursor->Deactivate();
		//ステージセレクトを無効化する
		m_stageSelection->Deactivate();
		//ステージ選択背景を無効化する。
		m_stageSelectionBase->Deactivate();


		//タイトル画面の画像を有効化して表示する
		//タイトルを有効化する
		m_title->Activate();
		//「Aボタンをおしてね」を有効化する
		m_pressAButton->Activate();

		//点滅のフラグを折る
		m_blinkingFlag = false;
		//「Aボタンをおしてね」を透明にする
		m_pressAButton->SetMulColor(g_VEC4_TRANSPARENT);
	}
	//デバック用
	//全部のステージが選べるようになる
	else if (g_pad[0]->IsTrigger(enButtonStart) && g_pad[0]->IsTrigger(enButtonRB1))
	{
		//スタートボタン（ポーズボタン）とRボタン（ダッシュボタン）を同時に入力

		//一番高いクリアしたステージを最大にする
		m_stageTitle->SetHighestClearStageNum(STAGE_NUM_LAST);
		//ステージセレクト用のスプライトレンダラーを有効化して表示できるようにする
		for (int i = 0; i < EN_STAGE_NUM; i++)
		{
			//ステージのアイコンを有効化する
			m_stageIcon[i]->Activate();
			m_stageIcon[i]->SetMulColor(g_VEC4_OPACITY);
			//一番高いクリアしたステージの番号より大きいか？
			if (i > m_stageTitle->GetHighestClearStageNum() + 1)
			{
				//大きいとき
				//カラーを薄暗くする
				m_stageIcon[i]->SetMulColor(SPRITE_MUL_COLOR_STAGE_PLAY_CANNOT);
			}
		}

	}
	//デバック用ここまで


	//カーソルの座標を更新する
	//対象のステージのアイコンから一定の距離離れた座標に設定する
	m_cursor->SetPosition
	(m_stageIcon[m_stageSelectState]->GetPosition() + m_stageIconToCursorVec);

	return;
}

/**
 * @brief 次のステージを選択する
*/
void CTitle::SelectNextStage()
{
	//ボタンを押すことができないようにする（連続入力防止用）
	m_buttonFlag = false;

	//ステージセレクトのステートが特定の時のみ
	if (m_stageSelectState != (STAGE_NUM_LAST) &&
		m_stageSelectState <= m_stageTitle->GetHighestClearStageNum())
	{
		//ステージセレクトのステートが最後まで行っていないとき、かつ
		//一番高いクリアしたステージの番号以下の時

		//ステージセレクトのステートを加算する
		m_stageSelectState++;
		//selectSEをワンショット再生で再生する。
		m_selectSE->Play(false);
	}
	else
	{
		//該当しない時

	}

	return;
}

/**
 * @brief 前のステージを選択する
*/
void CTitle::SelectPrevStage()
{
	//ボタンを押すことができないようにする（連続入力防止用）
	m_buttonFlag = false;

	//ステージセレクトのステートが特定の時のみ
	if (m_stageSelectState != STAGE_NUM_FIRST)
	{
		//ステージセレクトのステートが一番小さいステージの番号ではない時

		//ステージセレクトのステートを減算する
		m_stageSelectState--;
		//selectSEをワンショット再生で再生する。
		m_selectSE->Play(false);
	}
	else
	{
		//該当しない時
	}

	return;
}

//ステージを決定した時の処理
void CTitle::StageDecision()
{
	//ワイプが終わるまで処理しない
	if (!g_graphicsEngine->GetSceneChange().IsWipeFinished())
		return;

	////////////////////////////////////////////////////////////
	//ステージを新しく作成した場合、ここでNewGOを行う。
	////////////////////////////////////////////////////////////

	CStage* stage = nullptr;	//ステージのポインタ

	//ステージセレクトのステートが
	//一番最初のステージ番号以上かつ、一番最後のステージ番号以下ならば
	if (STAGE_NUM_FIRST <= m_stageSelectState && m_stageSelectState <= STAGE_NUM_LAST)
	{
		//ステージを生成
		stage = NewGO<CStage>(PRIORITY_FIRST);
		//ステージの初期化、レベルのファイルパスとステージ番号を指定する。
		stage->Init(GetLevelFilePath(m_stageSelectState), m_stageSelectState);

		//ステージ4の場合だけ
		if (m_stageSelectState == EN_STAGE_4)
		{
			//スイッチのタイムをデフォルトから変更する	
			stage->SetSwitchTime(stageBaseConstData::SWITCH_TIME_STAGE_4);
		}
	}


	//自身のオブジェクトを破棄する
	Release();

	return;
}