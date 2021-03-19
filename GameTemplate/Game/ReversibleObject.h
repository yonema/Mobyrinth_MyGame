#pragma once
#include "LevelObjectBase.h"

/// <summary>
///	リバーシブルオブジェクト（反転オブジェクト）
/// 反転オブジェクトを作るときはこのクラスを継承してね！
/// </summary>
class CReversibleObject : public ILevelObjectBase
{
public:		//自動で呼ばれるメンバ関数
	bool PureVirtualStart()override final;	//スタート関数
	virtual ~CReversibleObject();			//デストラクタ
	void PureVirtualUpdate()override final;			//アップデート関数


public:		//publicなオーバーライドしてほしいメンバ関数
	virtual bool StartSub() { return true; };	//スタート関数
	virtual void UpdateSub() {};				//アップデート関数
	/// <summary>
	/// 表と裏をセットするサブ関数
	/// これを呼ぶときは、CReversibleObjectの継承したクラスが
	/// 新たな表裏をセットする必要があるときに使う。
	/// 例はRObird_fishを参照
	/// </summary>
	virtual void SetFrontOrBackSub() {};


private:	//privateなオーバーライドしてほしいメンバ関数
	/// <summary>
	/// クエリしてほしいタイミングで呼ばれる関数。
	/// ここでQueryLOsを呼んでオブジェクト同士の反応を書く。
	/// 例はROnail_barを参照
	/// </summary>
	virtual void QuerySub() {};


protected:	//ここのメンバ関数を主に使う
	/// <summary>
	/// 初期化関数
	/// 最初に読んでね。trueを戻してね。
	/// 表のモデルとそのタイプ、裏のモデルとそのタイプ
	/// を設定する。
	/// タイプ一覧はLevelObjectBase.hを参照
	/// </summary>
	/// <param name="filePath_front">表のモデルのtkmファイルパス</param>
	/// <param name="type_front">表のタイプ</param>
	/// <param name="filePath_back">裏のモデルのtkmファイルパス</param>
	/// <param name="type_back">裏のタイプ</param>
	/// <returns>true戻してね</returns>
	bool Init
	(const char* filePath_front, const int type_front,
		const char* filePath_back, const int type_back);

private:
	virtual void QuerySub() {};

public:
	bool PureVirtualStart()override final;
	virtual bool StartSub() { return true; };
	void Update()override final;
	virtual void UpdateSub() {};
	virtual ~CReversibleObject();

public:		//ここのメンバ関数を主に使う	
	/// <summary>
	/// 反転させる
	/// </summary>
	void Reverse();

	/// <summary>
	/// 表か裏かを設定するする
	/// </summary>
	/// <param name="frontOrBack">enFrontかenBackを入れる</param>
	void SetFrontOrBack(const bool frontOrBack);


	/// <summary>
	///	表か裏かを取得する
	/// </summary>
	/// <returns>enFrontかenBack</returns>
	const bool GetFrontOrBack()
	{
		return m_frontOrBack;
	}

	/// <summary>
	/// 表と裏の両方のモデルの有効化フラグを設定する。
	/// trueを入れた場合、現在の表か裏どちらかを有効化し、他方を無効化する。
	/// falseを入れた場合、両方を無効化する
	/// </summary>
	/// <param name="activeFlag">有効化フラグ</param>
	void SetBothModelActiveFlag(const bool activeFlag);

private:	//privateなメンバ関数

	/// <summary>
	/// プレイヤーに持たれるかどうか調べる関数
	/// 持たれたらHeldPlayerへステート（状態）移行
	/// </summary>
	void CheckPlayer();

	/// <summary>
	/// プレイヤーに持たれている状態の関数。
	/// プレイヤーに持たれている場所に移動する処理。
	/// enCancelか、enThrownDownか、enThrownSideへ
	/// ステート（状態）を移行
	/// </summary>
	void HeldPlayer();

	/// <summary>
	/// 持っているオブジェクトを下に投げる関数
	/// enQueryへステート（状態）を移行
	/// </summary>
	void ThrownDown();

	/// <summary>
	/// 持っているオブジェクトをその場に置く関数
	/// enQueryへステート（状態）を移行
	/// </summary>
	void Cancel();

	/// <summary>
	/// 持っているオブジェクトを横に投げる関数
	/// enQueryへステート（状態）を移行
	/// </summary>
	void ThrownSide();

	/// <summary>
	/// クエリしてほしいタイミングで呼ばれる関数
	/// オーバーライドしてほしい関数、QuerySub()はここで呼ばれる。
	/// enCheckPlayerへステート（状態）を移行
	/// </summary>
	void Query();

	/// <summary>
	/// オブジェクトを横に投げる際の、投げる先を計算する関数
	/// </summary>
	void CalcTargetPos();

public:		//publicなデータメンバ

	/// <summary>
	/// 表か裏かを表す列挙体
	/// </summary>
	enum EnFrontAndBack
	{
		enFront,			//表状態
		enBack,				//裏状態
		enFrontAndBackNum,	//表裏の数
	};

private:	//データメンバ
	bool m_frontOrBack = enFront;				//表か裏か？
	bool m_bothModelactiveFlag = true;			//表裏両方の有効化フラグ
	CModelRender* m_modelRender[enFrontAndBackNum] = { nullptr };	//モデルレンダラー
	int m_reversibleType[enFrontAndBackNum] = { enEnpty };	//表と裏のオブジェクトのタイプ

	int m_throwCounter = 0;						//投げている時のカウンター
	Quaternion m_throwRot = g_quatIdentity;		//下に投げるときの回転

	/// <summary>
	/// オブジェクトの現在のステート（状態）
	/// これでアップデート中の処理を割り振る
	/// </summary>
	enum EnObjectState
	{
		enCheckPlayer,	//プレイヤーに持たれるかどうか調べる
		enHeldPlayer,	//プレイヤーに持たれている状態
		enCancel,		//持っているオブジェクトをその場に置く
		enThrownSide,	//持っているオブジェクトを横に投げる関数
		enThrownDown,	//持っているオブジェクトを下に投げる関数
		enQuery,		//クエリしてほしいタイミング
	};
	int m_objectState = enCheckPlayer;			//現在のオブジェクトのステート（状態）

	Quaternion m_throwRot = g_quatIdentity;
	
	//キャラクターの左右の向きを調べるのに使用
	enum EnPlayerLeftOrRight
	{
		enLeft,		//左
		enRight,	//右
	};
	int m_playerLeftOrRight = enRight;	//キャラクターの左右の向き
};

