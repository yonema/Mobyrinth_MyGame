#include "stdafx.h"

#include "ROmizu_kori.h"

#include "TransparentSwitch.h"

bool OOTransparentSwitch::StartSub()
{
	//OBBWorldに自身のOBBの登録を消去させる
	COBBWorld::GetInstance()->RemoveOBB(&GetOBB());

	return true;
}

void OOTransparentSwitch::UpdateSub()
{
	//リセットタイマーが０になったとき
	//透明オブジェクトをすべて持ち上げられないようにする。
	//透明オブジェクトを持っている場合、そのオブジェクトを持っていない状態にする。
	//透明オブジェクトを初期位置に戻す。
	if (m_flagSwitchOn == true) {
		--m_resetTimer;

		if (m_resetTimer <= 0) {
			m_flagSwitchOn = false;

			//ここに透明オブジェクトの処理を追加する。
		




		}
	}



	//リセットタイマーが０のときに下の文の処理を作動させる。
	if (m_flagSwitchOn == false) {
		//スイッチが押されたとき
		//透明オブジェクトをすべて持ち上げられるようにする。
		//スイッチのオブジェクトの範囲内でAボタンが押されたとき
		if (IsHitPlayer() && g_pad[0]->IsTrigger(enButtonA)) {
			m_flagSwitchOn = true;
			//リセットタイマーに開始する値を代入
			m_resetTimer = m_resetTimerStartValue;

			//ここに透明オブジェクトの処理を追加する。





		}
	}
}