#pragma once


/// <summary>
/// オブジェクトの番号
/// オブジェクトを増やしたらここで番号を増やすこと
/// </summary>
enum EN_OBJECT_TYPE
{
	EN_OBJECT_TYPE_EMPTY,			//「空っぽ」

	/*
	* 反転オブジェクト
	* RO(ReversibleObject)
	*/

	EN_RO_TYPE_WATER,	//「水」
	EN_RO_TYPE_FIRE,	//「火」
	EN_RO_TYPE_RUNNING,	//「稼働」
	EN_RO_TYPE_STOP,	//「停止」
	EN_RO_TYPE_NAIL,	//「釘」
	EN_RO_TYPE_BAR,		//「バール」
	EN_RO_TYPE_AXE,		//「斧」
	EN_RO_TYPE_HAMMER,	//「つるはし」
	EN_RO_TYPE_KEY,		//「鍵」
	EN_RO_TYPE_PADLOCK,	//「南京錠」
	EN_RO_TYPE_LEFT,	//「左」
	EN_RO_TYPE_RIGHT,	//「右」

	/*
	* 障害オブジェクト
	* OO(ObstacleObject)
	*/

	EN_OO_TYPE_FLAME,				//「炎」
	EN_OO_TYPE_WALL,				//「壁」
	EN_OO_TYPE_GOAL,				//「ゴール」
	EN_OO_TYPE_BIG_PADLOCK,			//「大きな南京錠」
	EN_OO_TYPE_BOX,					//「箱」
	EN_OO_TYPE_ONEWAY,				//「一方通行」
	EN_OO_TYPE_TRANSPARENT_SWITCH,	//「スイッチ」
	EN_OO_TYPE_REVERSE_ALL,			//「全反転」
	EN_OO_TYPE_UFO_CAPTURE,			//「UFOに捕まった」
	EN_OO_TYPE_UFO_LANDING_POINT,	//「UFOの運び先」

	EN_OBJECT_TYPE_NUM				//オブジェクトタイプの総数
};