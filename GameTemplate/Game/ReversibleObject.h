#pragma once
#include "ReversibleObjectConstData.h"
#include "ROsFilepathConstData.h"
#include "LevelObjectBase.h"
#include "SoundCue.h"
#include "effect/Effect.h"

/**
 * @brief リバーシブルオブジェクト（反転オブジェクト）
 * @note 反転オブジェクトを作るときはこのクラスを継承してね！
*/
class CReversibleObject : public ILevelObjectBase
{
public:		//自動で呼ばれるメンバ関数
	CReversibleObject();					//コンストラクタ
	bool PureVirtualStart()override final;	//スタート関数
	virtual ~CReversibleObject();			//デストラクタ
	void PureVirtualUpdate()override final;	//アップデート関数


public:		//publicなオーバーライドしてほしいメンバ関数
	virtual bool StartSub() { return true; };	//スタート関数
	virtual void UpdateSub() {};				//アップデート関数

	/// <summary>
	/// 表と裏をセットするサブ関数
	/// これを呼ぶときは、CReversibleObjectの継承したクラスが
	/// 新たな表裏をセットする必要があるときに使う。
	/// </summary>
	virtual void SetFrontOrBackSub() {};


private:	//privateなオーバーライドしてほしいメンバ関数
	/// <summary>
	/// クエリしてほしいタイミングで呼ばれる関数。
	/// ここでQueryLOsを呼んでオブジェクト同士の反応を書く。
	/// 例はROnail_barを参照
	/// </summary>
	virtual void QuerySub()
	{
		//重なっているかを判定しない場合、この関数の処理をしない
		if (GetIsHitFlag() == false) {
			return;
		}
	};

public:		//オーバーライドしたメンバ関数
	/// <summary>
	/// 表か裏かのモデルの参照を得る
	/// </summary>
	/// <param name="frontOrBack">表か裏か？</param>
	/// <returns>モデルの参照</returns>
	CModelRender* GetModelRender(const int frontOrBack = 0) override final
	{
		return m_modelRender[frontOrBack];
	}

	/**
	 * @brief 反転オブジェクトのスイッチをONにするときの処理
	*/
	void ReversibleSwitchOn() override final
	{
		//オブジェクトを持ち上げられるようにする。
		m_heldFlag = true;
	}

	/**
	 * @brief 反転オブジェクトのスイッチをOFFにするときの処理
	*/
	void ReversibleSwitchOff()override final;

	/**
	 * @brief 特定の場所を通過した際にすべてのオブジェクトを反転させる処理に使用する関数
	*/
	void AllReverse()override final;


public:		//メンバ関数	
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
	/// オブジェクトが重なっているかを確認する処理を動かすか確認するフラグの値を変更
	/// </summary>
	/// <param name="b">trueかfalseを入れる</param>
	void SetOverlapFlag(const bool b)
	{
		m_OverlapFlag = b;
	}


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

	/// <summary>
	/// ステートをキャンセル状態にする
	/// </summary>
	void StateToCancel()
	{
		m_objectState = reversibleObjectConstData::EN_CANCEL;
	}

	/// <summary>
	/// オブジェクトの状態をenCheckPlayerにセットする。
	/// </summary>
	void SetObjectStateEnCheckPlayer()
	{
		m_objectState = reversibleObjectConstData::EN_CHECK_HELD_PLAYER;
	}


protected:	//protectedなメンバ関数

	/// <summary>
	/// 初期化関数
	/// 最初に読んでね。trueを戻してね。
	/// 表のモデルとそのタイプ、裏のモデルとそのタイプ
	/// を設定する。
	/// タイプ一覧はObjectType.hを参照
	/// </summary>
	/// <param name="filePath_front">表のモデルのtkmファイルパス</param>
	/// <param name="type_front">表のタイプ</param>
	/// <param name="filePath_back">裏のモデルのtkmファイルパス</param>
	/// <param name="type_back">裏のタイプ</param>
	/// <returns>true戻してね</returns>
	bool Init
	(const char* filePath_front, const int type_front,
		const char* filePath_back, const int type_back);

	/// <summary>
	/// 自己発光色を設定する
	/// </summary>
	/// <param name="color">自己発光色</param>
	void SetModelEmissionColor(const Vector4& color)
	{
		m_modelRender[EN_FRONT]->SetModelEmissionColor(color);
		m_modelRender[EN_BACK]->SetModelEmissionColor(color);
	}

private:	//privateなメンバ関数

	/**
	 * @brief サウンドの初期化
	*/
	void InitSound();

	/**
	 * @brief エフェクトの初期化
	*/
	void InitEffect();

	/**
	 * @brief OBBの初期化
	*/
	void InitOBB();

	/**
	 * @brief 透明オブジェクト用のパラメーターの初期化
	*/
	void InitTransparentObjectParam();

	/// <summary>
	/// プレイヤーに持たれるかどうか調べる関数
	/// 持たれたらLiftedPlayerへステート（状態）移行
	/// </summary>
	void CheckPlayer();

	/// <summary>
	/// プレイヤーに持ち上げられ中の状態の関数
	/// 持ち上げ終わったらHeldPlayerへステート（状態）移行
	/// </summary>
	void LiftedPlayer();

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
	/// 横に弾かれる
	/// </summary>
	void Repelled();

	/// <summary>
	/// クエリしてほしいタイミングで呼ばれる関数
	/// オーバーライドしてほしい関数、QuerySub()はここで呼ばれる。
	/// enOverlapへステート（状態）を移行
	/// </summary>
	void Query();

	/// <summary>
	/// 反転オブジェクトが障害オブジェクトの中に埋まっているかの確認
	/// </summary>
	void CheckObjectsOverlap();


	/// <summary>
	/// 表側か裏側のキャパシティがオーバーしているか？
	/// を調べる関数
	/// 調整値には、自身はまだ対象の側にいないが、行ったとしたら
	/// キャパシティはオーバーするのか？を調べるときに、1を入れる。
	/// </summary>
	/// <param name="frontOrBackSide">表側か裏側か？</param>
	/// <returns>オーバーしているか？</returns>
	const bool IsCapacityOver(const int frontOrBackSide);


private:	//データメンバ

	/*
	* モデル関連
	*/
	CModelRender* m_modelRender[EN_FRONT_AND_BACK_NUM] = { nullptr };	//表と裏のモデルレンダラー

	/*
	* フラグ関連
	*/
	bool m_bothModelActiveFlag = true;		//表裏両方が有効か？
	bool m_changeObject = true;				//反転するか？（二重反転防止）
	bool m_OverlapFlag = true;				//オブジェクトが重なっている時の処理を行うか？
	bool m_frontOrBack = EN_FRONT;			//表か裏か？
	bool m_heldFlag = true;					//オブジェクトが現在持ち上げられるか？

	/*
	* オブジェクトタイプ関連
	*/
	//表と裏のオブジェクトのタイプ
	int m_reversibleType[EN_FRONT_AND_BACK_NUM] = { EN_OBJECT_TYPE_EMPTY, EN_OBJECT_TYPE_EMPTY };

	/*
	* 状態関連
	*/
	//現在のオブジェクトのステート（状態）
	int m_objectState = reversibleObjectConstData::EN_CHECK_HELD_PLAYER;
	//投げた後の行動
	int m_overlapAction = reversibleObjectConstData::EN_CHECK_HELD_PLAYER;

	/*
	* 投げられている時関連
	*/
	float m_thrownDownTimer = 0.0f;					//投げている時のタイマー
	Quaternion m_throwRot = g_QUAT_IDENTITY;		//下に投げるときの回転
	int m_trownDownOrUp = reversibleObjectConstData::EN_START_THROWN;	//下に投げられるか上に投げられるか？

	/*
	* 弾かれている時関連
	*/
	Vector3 m_addPosition = g_VEC3_ZERO;	//ポジションに加えるベクトル
	int m_leftOrRight = EN_LEFT;			//右に弾かれるか、左に弾かれるか

	/*
	* 全体で使うタイマー
	*/
	float m_timer = 0.0f;	//タイマー


	/*
	* スイッチ関連
	*/
	bool m_startFrontOrBack = EN_FRONT;				//表か裏かの初期値
	Vector3 m_startPosition = g_VEC3_ZERO;			//座標の初期値
	Quaternion m_startRotation = g_QUAT_IDENTITY;	//回転の初期値

	/*
	* エフェクト関連
	*/
	Effect* m_reverseAllEffect = nullptr;	//全反転のエフェクト

	/*
	* サウンド関連
	*/
	CSoundCue* m_changeSE = nullptr;		//性質反転するときのSE
	CSoundCue* m_throwSE = nullptr;			//投げるときのSE
	CSoundCue* m_capacityOverSE = nullptr;	//キャパシティがオーバーした時のSE
};

