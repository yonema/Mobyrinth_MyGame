#pragma once

/**
 * @brief タイトルの定数データ
*/
namespace titleConstData
{
	/**
	 * @brief タイトルのステート（状態）
	*/
	enum EN_STATE_TITLE
	{
		EN_START_TITLE,		//起動時演出の状態
		EN_TITLE_SCREEN,	//タイトル画面の状態
		EN_STAGE_SELECT,	//ステージセレクトの状態
		EN_STAGE_DESICION,	//ステージを決定した状態
	};

	/**
	 * @brief ステージの番号
	 * @note 新しいステージを作った場合、ここに番号を追加
	*/
	enum EN_STAGE_LIST
	{
		EN_STAGE_1,		//ステージ1
		EN_STAGE_2,		//ステージ2
		EN_STAGE_3,		//ステージ3
		EN_STAGE_4,		//ステージ4
		EN_STAGE_5,		//ステージ5
		EN_STAGE_6,		//ステージ6
		EN_STAGE_7,		//ステージ7
		EN_STAGE_8,		//ステージ8
		EN_STAGE_9,		//ステージ9
		EN_STAGE_NUM,	//ステージの総数
	};

	/**
	 * @brief ステージのレベルデータのファイルパスを取得
	 * @param [in] stageNum ステージの番号
	 * @return ステージのレベルデータのファイルパス
	*/
	constexpr const char* const GetLevelFilePath(const int stageNum)
	{
		//ステージの番号に対応するファイルパスを戻す
		switch (stageNum)
		{
		case EN_STAGE_1:
			return "Assets/level/O_easy.tkl";
			break;
		case EN_STAGE_2:
			return "Assets/level/Y_easy.tkl";
			break;
		case EN_STAGE_3:
			return "Assets/level/O_normal.tkl";
			break;
		case EN_STAGE_4:
			return "Assets/level/T_hard.tkl";
			break;
		case EN_STAGE_5:
			return "Assets/level/T_normal.tkl";
			break;
		case EN_STAGE_6:
			return "Assets/level/Y_normal.tkl";
			break;
		case EN_STAGE_7:
			return "Assets/level/O_hard.tkl";
			break;
		case EN_STAGE_8:
			return "Assets/level/T_easy.tkl";
			break;
		case EN_STAGE_9:
			return "Assets/level/Y_hard.tkl";
			break;
		}

		//どれでもなかったらエラー
		MessageBoxA(
			nullptr, "ステージのレベルデータが登録されていません", "エラー", MB_OK
		);

		return nullptr;
	}

	//ステージ4のスイッチのタイム
	const float SWITCH_TIME_STAGE_4 = 12.0f;

	//一番最初のステージ番号
	const int STAGE_NUM_FIRST = EN_STAGE_1;

	//一番最大のステージ番号
	const int STAGE_NUM_LAST = EN_STAGE_NUM - 1;


	/*
	* タイトルのスプライト関連
	*/

	//タイトル画面のレベル2Dのファイルパス
	const char* const LEVEL_2D_FILEPATH_TITLE = "Assets/level2D/Title.casl";

	//レベル2Dのタイトルのオブジェクトネーム
	const char* const LEVEL_2D_OBJECT_NAME_TITLE = "Title";

	//レベル2Dの「Aボタンを押してね」のオブジェクトネーム
	const char* const LEVEL_2D_OBJECT_NAME_PRESS_A_BUTTON = "Press_A_Button";

	//開始の演出の最大カウント
	const int MAX_COUNT_START_TITLE = 120;

	//「Aボタンを押してね」のアルファ値の変化スピード
	const float SPEED_CHANGE_ALPHA_PRESS_A_BUTTON = 0.02f;

	//「Aボタンを押してね」の不透明と判断するしきい値
	const float THRESHOLD_OPACITY_PRESS_A_BUTTON = 0.8f;


	/*
	* ステージセレクトのスプライト関連
	*/
	
	//ステージセレクト画面のレベル2Dのファイルパス
	const char* const LEVEL_2D_FILEPATH_STAGE_SELECTION = 
		"Assets/level2D/Stage_selection3.casl";

	//レベル2Dのステージアイコンのオブジェクトネーム
	const char* const LEVEL_2D_OBJECT_NAME_STAGE_ICON = "Stage_icon";
	//ステージアイコンのナンバーを取得するための文字
	const char* const STAGE_ICON_GET_NUM_CHAR = "n";
	//遊べないステージの乗算カラー
	const Vector4 SPRITE_MUL_COLOR_STAGE_PLAY_CANNOT = { 0.4f,0.4f,0.4f,1.0f };

	//レベル2Dのステージクリアのアイコンのオブジェクトネーム
	const char* const LEVEL_2D_OBJECT_NAME_STAGE_CLEAR = "Stage_clear";
	//ステージクリアのナンバーを取得するための文字
	const char* const STAGE_CLEAR_GET_NUM_CHAR = "r";

	//レベル2Dのカーソルのオブジェクトネーム
	const char* const LEVEL_2D_OBJECT_NAME_CURSOR = "cursor";

	//レベル2Dのステージセレクトのオブジェクトネーム
	const char* const LEVEL_2D_OBJECT_NAME_STAGE_SELECTION = "Stage_selection";

	//レベル2Dのステージ選択背景のオブジェクトネーム
	const char* const LEVEL_2D_OBJECT_NAME_STAGE_SELECTION_BASE =
		"Stage_selection_base";
	//ステージ選択背景のスプライトの乗算カラー
	const Vector4 SPRITE_MUL_COLOR_STAGE_SELECTION_BASE = { 1.0f,1.0f,1.0f,0.8f };


	/*
	* サウンド関連の定数
	*/

	//タイトルのBGMのサウンドのファイルパス
	const wchar_t* const SOUND_FILEPATH_BGM_TITLE = L"Assets/sound/Title.wav";
	//タイトルのBGMのサウンドの音量
	const float SOUND_VOLUME_BGM_TITLE = 0.5f;

	//AボタンのSEのサウンドのファイルパス
	const wchar_t* const SOUND_FILEPATH_SE_BUTTON_A = L"Assets/sound/buttonA.wav";
	//AボタンのSEのサウンドの音量
	const float SOUND_VOLUME_SE_BUTTON_A = 0.5f;

	//BボタンのSEのサウンドのファイルパス
	const wchar_t* const SOUND_FILEPATH_SE_BUTTON_B = L"Assets/sound/buttonB.wav";
	//BボタンのSEのサウンドの音量
	const float SOUND_VOLUME_SE_BUTTON_B = 0.5f;

	//カーソル移動のSEのサウンドのファイルパス
	const wchar_t* const SOUND_FILEPATH_SE_SELECT = L"Assets/sound/select.wav";
	//カーソル移動のSEのサウンドの音量
	const float SOUND_VOLUME_SE_SELECT = 0.5f;



	//左スティックの入力があると判断するしきい値
	const float THRESHOLD_STICK_L_VALUE = 0.5f;

}