#pragma once


/// <summary>
/// オブジェクトの番号
/// オブジェクトを増やしたらここで番号を増やすこと
/// </summary>
enum EnObjectType
{
	enEmpty,			//「空っぽ」

	//反転オブジェクト
	enWater,			//「水」
	enFire,				//「火」
	enBird,				//「鳥」
	enFish,				//「魚」
	enGrilledChicken,	//「焼き鳥」
	enGrilledFish,		//「焼き魚」
	enRunning,			//「稼働」
	enStop,				//「停止」
	enWire,				//「針金」
	enString,			//「紐」
	enNail,				//「釘」
	enBar,				//「バール」
	enAxe,				//「斧」
	enPickaxe,			//「つるはし」
	enKeymold,			//「鍵の金型」
	enKey,				//「鍵」
	enPadlock,			//「南京錠」
	enLeftType,			//「左」	//enLeftは他でもよく使っているためTypeを加える
	enRightType,		//「右」	//enRightは他でもよく使っているためTypeを加える

	//障害オブジェクト

	enBigFire,			//「炎」
	enWall,				//「壁」
	enGoal,				//「ゴール」
	enNotHavePadlock,	//「持てない南京錠」
	enBox,				//「箱」
	enOneway,			//「一方通行」
	enTransparentSwitch,//「スイッチ」
	enUFOCapture,		//「UFOに捕まった」

	enObjectTypeNum		//オブジェクトタイプの総数
};