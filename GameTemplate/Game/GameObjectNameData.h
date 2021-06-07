#pragma once


/**
 * @brief 名前が登録されているゲームオブジェクトのタイプ
*/
enum EN_GAME_OBJECT_TYPE
{
	EN_GO_TYPE_GAME_CAMERA,	//CGameCameraクラス
	EN_GO_TYPE_MOBIUS,		//CMobiusクラス
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
	case EN_GO_TYPE_GAME_CAMERA:
		return "GameCamera";
		break;
	case EN_GO_TYPE_MOBIUS:
		return "Mobius";
		break;
	}

	//どれでもなかったらエラー
	MessageBoxA(
		nullptr, "ゲームオブジェクトの名前が登録されていません", "エラー", MB_OK
	);

	return nullptr;
}
