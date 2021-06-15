#pragma once

//カラの名前
const char* const NAME_EMPTY = "";

/**
 * @brief 名前が登録されているゲームオブジェクトのタイプ
*/
enum EN_GAME_OBJECT_TYPE
{
	EN_GO_TYPE_GAME,		//CGameクラス
	//ゲーム全体を通して使われるCDirectionLightクラス
	EN_GO_TYPE_GAME_DIRECTION_LIGHT,
	EN_GO_TYPE_TITLE,		//CTitleクラス
	EN_GO_TYPE_START_DIRECTING,	//CStartDirectingクラス
	EN_GO_TYPE_PAUSE,		//CPauseクラス
	EN_GO_TYPE_GAME_CAMERA,	//CGameCameraクラス
	EN_GO_TYPE_MOBIUS,		//CMobiusクラス
	EN_GO_TYPE_PLAYER,		//CPlayerクラス
	EN_GO_TYPE_UFO,			//CUFOクラス
};


/**
 * @brief ゲームオブジェクトの名前を取得
 * @param [in] gameObjectType ゲームオブジェクトのタイプ
 * @return ゲームオブジェクトの名前
*/
constexpr const char* const GetGameObjectName(const int gameObjectType)
{
	//タイプに対応するゲームオブジェクトの名前を戻す
	switch (gameObjectType)
	{
	case EN_GO_TYPE_GAME:
		return "Game";
		break;
	case EN_GO_TYPE_GAME_DIRECTION_LIGHT:
		return "GameDirectionLight";
		break;
	case EN_GO_TYPE_TITLE:
		return "Title";
		break;
	case EN_GO_TYPE_START_DIRECTING:
		return "StartDirecting";
		break;
	case EN_GO_TYPE_PAUSE:
		return "Pause";
		break;
	case EN_GO_TYPE_GAME_CAMERA:
		return "GameCamera";
		break;
	case EN_GO_TYPE_MOBIUS:
		return "Mobius";
		break;
	case EN_GO_TYPE_PLAYER:
		return "Player";
		break;
	case EN_GO_TYPE_UFO:
		return "UFO";
		break;
	}

	//どれでもなかったらエラー
	MessageBoxA(
		nullptr, "ゲームオブジェクトの名前が登録されていません", "エラー", MB_OK
	);

	return nullptr;
}
