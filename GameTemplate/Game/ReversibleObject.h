#pragma once
#include "LevelObjectBase.h"
#include "SoundCue.h"

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

public:
	/// <summary>
	/// 表か裏かのモデルの参照を得る
	/// </summary>
	/// <param name="frontOrBack">表か裏か？</param>
	/// <returns>モデルの参照</returns>
	CModelRender* GetModelRender(const int frontOrBack = 0) override final
	{
		return m_modelRender[frontOrBack];
	}

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
	virtual void QuerySub()
	{
		//重なっているかを判定しない場合、この関数の処理をしない
		if (GetFlagIsHit() == false) {
			return;
		}
	};


protected:	//ここのメンバ関数を主に使う
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
	void SetEmissionColor(const Vector4& color)
	{
		m_modelRender[enFront]->SetEmissionColor(color);
		m_modelRender[enBack]->SetEmissionColor(color);
	}

public:
	//void Update()override final;

public:		//ここのメンバ関数を主に使う	
	/// <summary>
	/// 反転させる
	/// </summary>
	void Reverse();

	/// <summary>
	/// 特定の場所を通過した際にすべてのオブジェクトを反転させる処理に使用する関数
	/// </summary>
	void AllReverse();

	/// <summary>
	/// 表か裏かを設定するする
	/// </summary>
	/// <param name="frontOrBack">enFrontかenBackを入れる</param>
	void SetFrontOrBack(const bool frontOrBack);

	/// <summary>
	/// オブジェクトが重なっているかを確認する処理を動かすか確認するフラグの値を変更
	/// </summary>
	/// <param name="b">trueかfalseを入れる</param>
	void SetFlagOverlap(const bool b)
	{
		m_flagOverlap = b;
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
		m_objectState = enCancel;
	}

	/// <summary>
	/// 持ち上げた時の、上に持ち上げるベクトルの長さを設定
	/// </summary>
	/// <param name="heldUpLen"></param>
	void SetHeldUpLen(const float heldUpLen)
	{
		m_heldUpLen = heldUpLen;
	}

	/// <summary>
	/// オブジェクトの状態をenCheckPlayerにセットする。
	/// </summary>
	void SetObjectStateEnCheckPlayer()
	{
		m_objectState = enCheckPlayer;
	}

	int GetObjectState()
	{
		return m_objectState;
	}

	int GetEnCheckPlayer()
	{
		return enCheckPlayer;
	}


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
	//void ThrownSide();

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
	/// オブジェクトを横に投げる際の、投げる先を計算する関数
	/// </summary>
	//void CalcTargetPos();

	/// <summary>
	/// 反転オブジェクトが障害オブジェクトの中に埋まっているかの確認
	/// </summary>
	void CheckObjectsOverlap();

	/// <summary>
	/// オブジェクトが戻ってくる処理（縦）
	/// </summary>
	void OverlapThrownDown();

	/// <summary>
	/// オブジェクトが戻ってくる処理（横）
	/// </summary>
	//void OverlapThrownSide();

	/// <summary>
	/// 表側か裏側のキャパシティがオーバーしているか？
	/// を調べる関数
	/// 調整値には、自身はまだ対象の側にいないが、行ったとしたら
	/// キャパシティはオーバーするのか？を調べるときに、1を入れる。
	/// </summary>
	/// <param name="frontOrBackSide">表側か裏側か？</param>
	/// <param name="adjust">調整値</param>
	/// <returns>オーバーしているか？</returns>
	const bool IsCapacityOver(const int frontOrBackSide, const int adjust = 0);

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
	int m_reversibleType[enFrontAndBackNum] = { enEmpty };	//表と裏のオブジェクトのタイプ

	float m_throwCounter = 0.0f;						//投げている時のカウンター
	Quaternion m_throwRot = g_quatIdentity;		//下に投げるときの回転

	float m_heldUpLen = 100.0f;					//持ち上げた時の、上に持ち上げるベクトルの長さ
	float m_timer = 0.0f;						//タイマー
	Vector3 m_addPosition = g_vec3Zero;			//ポジションに加えるベクトル
	bool m_virticalRepelledFlag = false;		//盾に弾かれるか？
	/// <summary>
	/// オブジェクトの現在のステート（状態）
	/// これでアップデート中の処理を割り振る
	/// </summary>
	enum EnObjectState
	{
		enCheckPlayer,	//プレイヤーに持たれるかどうか調べる
		enHeldPlayer,	//プレイヤーに持たれている状態
		enThrownDown,	//持っているオブジェクトを下に投げる関数
		enCancel,		//持っているオブジェクトをその場に置く
		//enThrownSide,	//持っているオブジェクトを横に投げる関数
		enRepelled,		//横に弾かれる
		enQuery,		//クエリしてほしいタイミング

		enOverlap,		//障害オブジェクトに重ねっているかの確認
		enOverlapThrownDown, //オブジェクトが戻ってくる処理（縦）
		//enOverlapThrownSide //オブジェクトが戻ってくる処理（横）
	};
	int m_objectState = enCheckPlayer;			//現在のオブジェクトのステート（状態）
	int m_objectAction = enCheckPlayer;			//オブジェクトに行ったアクションを保存（戻ってくる処理に使用）

	
	//キャラクターの左右の向きを調べるのに使用
	enum EnPlayerLeftOrRight
	{
		enLeft,		//左
		enRight,	//右
	};
	//int m_playerLeftOrRight = enRight;	//キャラクターの左右の向き
	int m_leftOrRight = enLeft;
	bool m_checkOverlap = false; //このオブジェクトを戻らせるかのフラグ

	Vector3 test = { 0.0f,0.0f,0.0f };

	CSoundCue* m_changeSE = nullptr; //m_changeSEのサウンドキュー

	bool m_changeObject = true;

protected:
	bool m_flagOverlap = true; //このオブジェクトが重なっているかを判定する処理を動かすかどうか

};

