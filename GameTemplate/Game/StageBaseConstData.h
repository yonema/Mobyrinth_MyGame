#pragma once
#include "ObjectType.h"

namespace stageBaseConstData
{
	//プレイヤーのオブジェクトネーム
	const wchar_t* const LEVEL_OBJECT_NAME_PLAYER = L"player_kari";
	//メビウスの輪のオブジェクトネーム
	const wchar_t* const LEVEL_OBJECT_NAME_MOBIUS = L"Mobius";

	//一方通行（左）のオブジェクトタイプ
	const int OO_TYPE_ONEWAY_LEFT = 100;
	//一方通行（右）のオブジェクトタイプ
	const int OO_TYPE_ONEWAY_RIGHT = 101;

	//透明のオブジェクトタイプ
	const int TYPE_TRANSPARENT_OBJECT = 200;

	//火（透明）のオブジェクトタイプ
	const int RO_TYPE_FIRE_TRANSPARENT = EN_RO_TYPE_FIRE + TYPE_TRANSPARENT_OBJECT;
	//釘（透明）のオブジェクトタイプ
	const int RO_TYPE_NAIL_TRANSPARENT = EN_RO_TYPE_NAIL + TYPE_TRANSPARENT_OBJECT;
	//金槌（透明）のオブジェクトタイプ
	const int RO_TYPE_HAMMER_TRANSPARENT = EN_RO_TYPE_HAMMER + TYPE_TRANSPARENT_OBJECT;
	//鍵（透明）のオブジェクトタイプ
	const int RO_TYPE_KEY_TRANSPARENT = EN_RO_TYPE_KEY + TYPE_TRANSPARENT_OBJECT;
	//南京錠（透明）のオブジェクトタイプ
	const int RO_TYPE_PADLOCK_TRANSPARENT = EN_RO_TYPE_PADLOCK + TYPE_TRANSPARENT_OBJECT;
	//ゴール（透明）のオブジェクトタイプ
	const int OO_TYPE_GOAL_TRANSPARENT = EN_OO_TYPE_GOAL + TYPE_TRANSPARENT_OBJECT;
	//炎（透明）のオブジェクトタイプ
	const int OO_TYPE_FLAME_TRANSPARENT = EN_OO_TYPE_FLAME + TYPE_TRANSPARENT_OBJECT;
	//箱（透明）のオブジェクトタイプ
	const int OO_TYPE_BOX_TRANSPARENT = EN_OO_TYPE_BOX + TYPE_TRANSPARENT_OBJECT;


	/**
	 * @brief レベルのオブジェクトネームを取得
	 * @param [in] objectType オブジェクトタイプ
	 * @return オブジェクトネーム
	*/
	constexpr const wchar_t* const GetLevelObjectName(const int objectType)
	{
		//オブジェクトタイプに対応するオブジェクトネームを戻す

		switch (objectType)
		{
			/*
			* 反転オブジェクト
			*/
		case EN_RO_TYPE_WATER:
			return L"mizu";
			break;
		case EN_RO_TYPE_FIRE:
			return L"fire";
			break;
		case EN_RO_TYPE_RUNNING:
			return L"kadou";
			break;
		case EN_RO_TYPE_STOP:
			return L"teishi";
			break;
		case EN_RO_TYPE_NAIL:
			return L"nail";
			break;
		case EN_RO_TYPE_HAMMER:
			return L"bar";
			break;
		case EN_RO_TYPE_KEY:
			return L"key";
			break;
		case EN_RO_TYPE_PADLOCK:
			return L"padlock";
			break;
		case EN_RO_TYPE_LEFT:
			return L"left";
			break;
		case EN_RO_TYPE_RIGHT:
			return L"right";
			break;

			/*
			* 障害オブジェクト
			*/
		case EN_OO_TYPE_GOAL:
			return L"goal";
			break;
		case EN_OO_TYPE_FLAME:
			return L"bigFire";
			break;
		case EN_OO_TYPE_WALL:
			return L"wall";
			break;
		case EN_OO_TYPE_BIG_PADLOCK:
			return L"notHavePadlock";
			break;
		case EN_OO_TYPE_BOX:
			return L"box";
			break;
		case OO_TYPE_ONEWAY_LEFT:
			return L"oneway_left";
			break;
		case OO_TYPE_ONEWAY_RIGHT:
			return L"oneway_right";
			break;
		case EN_OO_TYPE_REVERSE_ALL:
			return L"reverseAll";
			break;
		case EN_GO_TYPE_UFO:
			return L"UFO";
			break;
		case EN_OO_TYPE_TRANSPARENT_SWITCH:
			return L"switch";
			break;


			/*
			* 透明反転オブジェクト
			*/
			
		case RO_TYPE_FIRE_TRANSPARENT:
			return L"fire_switch";
			break;
		case RO_TYPE_NAIL_TRANSPARENT:
			return L"nail_switch";
			break;
		case RO_TYPE_HAMMER_TRANSPARENT:
			return L"bar_switch";
			break;
		case RO_TYPE_KEY_TRANSPARENT:
			return L"key_switch";
			break;
		case RO_TYPE_PADLOCK_TRANSPARENT:
			return L"padlock_switch";
			break;

			/*
			* 透明障害オブジェクト
			*/

		case OO_TYPE_GOAL_TRANSPARENT:
			return L"goal_switch";
			break;
		case OO_TYPE_FLAME_TRANSPARENT:
			return L"bigFire_switch";
			break;
		case OO_TYPE_BOX_TRANSPARENT:
			return L"box_switch";
			break;
		}

		//どれでもなかったらエラー
		MessageBoxA(
			nullptr, "レベルオブジェクトの名前が登録されていません", "エラー", MB_OK
		);

		return nullptr;
	}

	//ウェイポイントのオブジェクトネーム
	const wchar_t* const OBJECT_NAME_WAYPOINT = L"waypoint";

	const wchar_t* const CHAR_GET_WAYPOINT_NUM = L"0";


	const bool MODE_TRANS = true;		//透明モード
	const bool MODE_OPACITY = false;	//不透明モード
}