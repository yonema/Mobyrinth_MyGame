#pragma once
#include "ReversibleObject.h"

class CTransparentObject : public CReversibleObject
{
public:		//publicなオーバーライドしてほしいメンバ関数
	virtual bool StartSub() { return true; };	//スタート関数
	void UpdateSub()override final;		//アップデート関数

private:	//privateなオーバーライドしてほしいメンバ関数
	/// <summary>
	/// クエリしてほしいタイミングで呼ばれる関数。
	/// ここでQueryLOsを呼んでオブジェクト同士の反応を書く。
	/// 例はROnail_barを参照
	/// </summary>
	virtual void QuerySub() {};

public: //Set関数
	/// <summary>
	/// リセットタイマーの時間をセットし、タイマーをスタートする。
	/// </summary>
	/// <param name="i">リセットタイマーの設定値</param>
	void SetANDStartResetTimer(int i)
	{
		m_resetTimer = i;
		//オブジェクトを持ち上げられるようにする。
		m_flagHeld = true;
	}

protected: //メンバ変数
	//bool m_flagTransparentObject = true; //一定時間だけ持ち上げられるオブジェクトかどうかのフラグ
	bool m_flagHeld = false; //オブジェクトが現在持ち上げられるかのフラグ
	int m_resetTimer = 0; //オブジェクトの状態をリセットするまでのカウントに使用するリセットタイマー
	Vector3 m_startPosition = { 0.0f,0.0f,0.0f }; //オブジェクトの初期位置を保存する位置情報変数
	Quaternion m_startRotation = g_quatIdentity;


};

