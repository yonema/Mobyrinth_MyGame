#pragma once
#include <time.h>
#include "HUDConstData.h"
#include "SpriteRenderConstData.h"

/// <summary>
/// 場面転換クラス
/// </summary>
class CHUD
{
public:		//自動で呼ばれるメンバ関数
	CHUD();			//コンストラクタ
	~CHUD() {};		//デストラクタ

public:		//グラフィックエンジンで呼ばれるメンバ関数
	void Init();					//初期化関数
	void UpdateParam();				//パラメータのアップデート関数
	void Draw(RenderContext& rc);	//描画関数

public:		//メンバ関数

	/// <summary>
	/// ワイプインを開始する
	/// </summary>
	void WipeIn()
	{
		//ワイプインに設定して
		SetInOrOut(EN_IN);
		//ワイプ開始
		WipeStart();
	}

	/// <summary>
	/// ワイプアウトを開始する
	/// </summary>
	void WipeOut()
	{
		//ワイプアウトに設定して
		SetInOrOut(EN_OUT);
		//ワイプ開始
		WipeStart();
	}

	/// <summary>
	/// ワイプの種類をランダムにして、ワイプアウト開始
	/// </summary>
	void RandomWipeOut();


	/// <summary>
	/// ワイプ終了
	/// </summary>
	void WipeEnd()
	{
		//ワイプを開始位置に戻す
		m_wipeParam.wipeSize = 0.0f;
		//ワイプをワイプアウトにしておく
		//こうしないでワイプを開始位置に戻すと、たぶん真っ暗になる
		SetInOrOut(EN_OUT);
		//ワイプフラグを折る
		m_wipeFlag = false;
	}

	/// <summary>
	/// ワイプする方向を設定する
	/// leftOrLightと矛盾するように設定しないでね。
	/// </summary>
	/// <param name="dir">ワイプする方向</param>
	void SetWipeDirection(const Vector2& dir);

	/// <summary>
	/// 左側からワイプするか、右側からワイプするか設定する
	/// WipeDirectionと矛盾するように設定しないでね。
	/// </summary>
	/// <param name="leftOrLight">右側か左側</param>
	void SetWipeLeftOrLight(const bool leftOrLight)
	{
		m_wipeParam.leftOtLight = leftOrLight;
	}

	/// <summary>
	/// 上側からワイプするか、下側からワイプするか設定する
	/// </summary>
	/// <param name="topOrDown">上側か下側</param>
	void SetWipeTopOrDown(const bool topOrDown)
	{
		m_wipeParam.topOrDown = topOrDown;
	}

	/// <summary>
	/// ワイプの種類を設定する
	/// </summary>
	/// <param name="wipeType"></param>
	void SetWipeType(const int wipeType)
	{
		m_wipeParam.wipeType = wipeType;
	}


	/// <summary>
	/// ワイプする速度を設定する
	/// </summary>
	/// <param name="wipeSpeed">ワイプする速度</param>
	void SetWipeSpeed(const float wipeSpeed)
	{
		m_wipeSpeed = wipeSpeed;
	}

	/// <summary>
	/// どれくらいワイプしているかを得る
	/// </summary>
	/// <returns>どれくらいワイプしているか</returns>
	const float GetWipeSize() const
	{
		return m_wipeParam.wipeSize;
	}

	/// <summary>
	/// 各種類のワイプの最大サイズを得る
	/// </summary>
	/// <param name="wipeType">ワイプの種類</param>
	/// <returns>各種類のワイプの最大サイズ</returns>
	const float GetWipeSizeMax(const int wipeType) const;

	/// <summary>
	/// 現在のワイプの種類を得る
	/// </summary>
	/// <returns>現在のワイプの種類</returns>
	const int GetWipeType() const
	{
		return m_wipeParam.wipeType;
	}

	/// <summary>
	/// ワイプが最後までワイプしたかを得る
	/// </summary>
	/// <returns>最後までワイプしたか？</returns>
	const bool IsWipeFinished() const;

	/**
	 * @brief フェードのスプライトのアルファ値を設定する
	 * @param [in] alphaValue アルファ値
	*/
	void SetFadeSpriteAlphaValue(const float alphaValue)
	{
		m_fadeSprite.SetAlphaValue(alphaValue);
	}


private:	//privateなメンバ関数

	/// <summary>
	/// レンダリングターゲットの初期化
	/// </summary>
	void InitRenderTarget();

	/// <summary>
	/// スプライトの初期化
	/// </summary>
	void InitSprite();

	/// <summary>
	/// フェードの初期化
	/// </summary>
	void InitFade();

	/// <summary>
	/// ワイプ開始
	/// </summary>
	void WipeStart()
	{
		//ワイプを開始位置にする
		m_wipeParam.wipeSize = 0.0f;
		//ワイプフラグを立てる
		m_wipeFlag = true;
	};

	/// <summary>
	/// ワイプインかワイプアウトを設定
	/// </summary>
	/// <param name="inOrOut"></param>
	void SetInOrOut(const bool inOrOut)
	{
		m_wipeParam.inOrOut = inOrOut;
	};


private:	//データメンバ
	RenderTarget m_beforeWipingRenderTarget;	//ワイプ前のレンダリングターゲット
	Sprite m_mainRenderTargetSprite;			//メインレンダリングターゲットのスプライト
	Sprite m_afterWipingSprite;					//ワイプした後のスプライト
	Sprite m_backSprite;						//ワイプの後ろに映すスプライト
	Sprite m_fadeSprite;						//フェードイン、アウトで使うスプライトレンダラー
	HUDConstData::SWipeParam m_wipeParam;		//ワイプのパラメータ
	bool m_wipeFlag = false;					//ワイプするか？
	float m_wipeSpeed = 0.0f;					//ワイプのスピード
	float m_deltaTime = 0.0f;					//デルタタイム

};