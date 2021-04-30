#include "stdafx.h"
#include "TransparentObject.h"

void CTransparentObject::UpdateSub()
{
	//ここでリセットタイマーを減少する。
	if (m_resetTimer > 0) {
		--m_resetTimer;

		//タイマーが０になったら、オブジェクトを持ち上げられなくして、初期位置に戻す。
		if (m_resetTimer <= 0) {
			//オブジェクトを持ち上げられなくする。
			SetFlagHeld(false);
			//初期位置に戻す。
			m_position = m_startPosition;
			//回転情報も初期状態に戻す。
			m_rotation = m_startRotation;
		}
	}


}