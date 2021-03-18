#pragma once
#include "LevelObjectBase.h"



class CReversibleObject : public ILevelObjectBase
{
protected:
	bool Init
	(const char* filePath_front, const int type_front,
		const char* filePath_back, const int type_back);
private:
	/// <summary>
	/// オブジェクトを拾う処理
	/// </summary>
	void CheckPlayer();
	/// <summary>
	/// オブジェクトを所持しているときのボタン検知
	/// </summary>
	void HeldPlayer();
	/// <summary>
	/// オブジェクト所持時、Aボタンを押したときの処理
	/// オブジェクトを裏側に投げて、オブジェクトの性質を反転させる。
	/// </summary>
	void ThrownDown();
	/// <summary>
	/// オブジェクト所持時、Bボタンを押したときの処理
	/// オブジェクトをプレイヤーの足元に置く。
	/// </summary>
	void Cancel();
	/// <summary>
	/// オブジェクト所持時、Cボタンを押したときの処理
	/// オブジェクトを現在向いている方向に向かって投げる。
	/// </summary>
	void ThrownSide();
	/// <summary>
	/// 現在の左側にあるウェイポイントを検知する処理
	/// </summary>
	void CalcTargetPos();
	/// <summary>
	/// オブジェクトを手放したときに
	/// オブジェクトを取得する処理を行えるようにする処理
	/// </summary>
	void Query();
	virtual void QuerySub() {};

public:
	bool PureVirtualStart()override final;
	virtual bool StartSub() { return true; };
	void Update()override final;
	virtual void UpdateSub() {};
	virtual ~CReversibleObject();
	void Reverse();
	void SetFrontOrBack(const bool frontOrBack);
	virtual void SetFrontOrBackSub() {};
	const bool GetFrontOrBack()
	{
		return m_frontOrBack;
	}
	void SetBothModelActiveFlag(const bool activeFlag);

public:
	enum EnFrontAndBack
	{
		enFront,
		enBack,
		enFrontAndBackNum,
	};
private:
	bool m_frontOrBack = enFront;
	bool m_bothModelactiveFlag = true;
	CModelRender* m_modelRender[enFrontAndBackNum] = { nullptr };
	int m_reversibleType[enFrontAndBackNum] = { enEnpty };
	enum EnObjectState
	{
		enCheckPlayer,
		enHeldPlayer,
		enCancel,
		enThrownSide,
		enThrownDown,
		enQuery,
	};
	int m_objectState = enCheckPlayer;
	int m_throwCounter = 0;

	Quaternion m_throwRot = g_quatIdentity;
	
	//キャラクターの左右の向きを調べるのに使用
	enum EnPlayerLeftOrRight
	{
		enLeft,		//左
		enRight,	//右
	};
	int m_playerLeftOrRight = enRight;	//キャラクターの左右の向き
};

