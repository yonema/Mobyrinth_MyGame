#pragma once
#include <time.h>
//#include "GameTime.h"

/// <summary>
/// 場面転換クラス
/// </summary>
class CSceneChange
{
public:
	~CSceneChange();				//デストラクタ

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
		SetInOrOut(enIn);
		//ワイプ開始
		WipeStart();
	}

	/// <summary>
	/// ワイプアウトを開始する
	/// </summary>
	void WipeOut()
	{
		//ワイプアウトに設定して
		SetInOrOut(enOut);
		//ワイプ開始
		WipeStart();
	}

	/// <summary>
	/// ワイプの種類をランダムにして、ワイプアウト開始
	/// </summary>
	void RandomWipeOut();

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
	/// ワイプ終了
	/// </summary>
	void WipeEnd()
	{
		//ワイプを開始位置に戻す
		m_wipeParam.wipeSize = 0.0f;
		//ワイプをワイプアウトにしておく
		//こうしないでワイプを開始位置に戻すと、たぶん真っ暗になる
		SetInOrOut(enOut);
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
	/// CSceneChange::enLeftかCSceneChange::enRightを引数にする
	/// </summary>
	/// <param name="leftOrLight">右側か左側</param>
	void SetWipeLeftOrLight(const bool leftOrLight)
	{
		m_wipeParam.leftOtLight = leftOrLight;
	}

	/// <summary>
	/// 上側からワイプするか、下側からワイプするか設定する
	/// CSceneChange::enTopかCSceneChange::enDownを引数にする
	/// </summary>
	/// <param name="topOrDown">上側か下側</param>
	void SetWipeTopOrDown(const bool topOrDown)
	{
		m_wipeParam.topOrDown = topOrDown;
	}

	/// <summary>
	/// ワイプの種類を設定する
	/// ワイプの種類はSceneChange.hを参照
	/// </summary>
	/// <param name="wipeType"></param>
	void SetWipeType(const int wipeType)
	{
		m_wipeParam.wipeType = wipeType;
	}

	/// <summary>
	/// ワイプインかワイプアウトを設定
	/// CSceneChange::enInかCSceneChange::enOutを引数にする
	/// </summary>
	/// <param name="inOrOut"></param>
	void SetInOrOut(const bool inOrOut)
	{
		m_wipeParam.inOrOut = inOrOut;
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
	/// ワイプの種類はSceneChange.hを参照
	/// </summary>
	/// <param name="wipeType">ワイプの種類</param>
	/// <returns>各種類のワイプの最大サイズ</returns>
	const float GetWipeSizeMax(const int wipeType) const;

	/// <summary>
	/// 現在のワイプの種類を得る
	/// ワイプの種類はSceneChange.hを参照
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

public:		//列挙型

	/// <summary>
	/// 左か右か
	/// </summary>
	enum EnLeftOrLight
	{
		enLeft,		//左
		enRight,	//右
	};

	/// <summary>
	/// 上か下か
	/// </summary>
	enum EnTopOrDown
	{
		enTop,		//上
		enDown,		//下
	};

	/// <summary>
	/// ワイプインかワイプアウトか
	/// </summary>
	enum EnInOrOut
	{
		enIn,		//ワイプイン
		enOut,		//ワイプアウト
	};

	/// <summary>
	/// ワイプの種類
	/// </summary>
	enum EnWipeType
	{
		enWipe,					//普通のワイプ
		enCircleWipe,			//円形ワイプ
		enVerticalStripeWipe,	//縦縞ワイプ
		enHorizontalStripeWipe,	//横縞ワイプ
		enCheckerboardWipe,		//チェッカーボードワイプ	//ここまでしか実装していない
		enMonochrome,			//モノクロ加工
		enSepia,				//セピア調加工
		enNega,					//ネガポジ反転
		enNois,					//ノイズ加工
	};

private:	//構造体

	/// <summary>
	/// ワイプのパラメータ構造体
	/// ここを変更したら、SceneChange.fxの定数バッファも変更すること
	/// </summary>
	struct WipeParam
	{
		Vector2 wipeDir = { -1.0f,1.0f };
		float wipeSize = 0.0f;
		int leftOtLight = enLeft;
		int topOrDown = enTop;
		int wipeType = enWipe;
		int inOrOut = enOut;
	};
private:	//定数
	//各ワイプの最大ワイプサイズ
	//ここを変更したら、SceneChange.fxの定数も変更すること
	const float m_wipeMaxX = 1280.0f;				//普通のワイプのX軸の最大サイズ
	const float m_wipeMaxY = 720.f;					//普通のワイプのY軸の最大サイズ
	const float m_circleWipeMax = 1280.0f / 1.5;	//円形ワイプの最大サイズ
	const float m_verticalStripeWipeMax = 64.0f;	//縦縞ワイプの最大サイズ
	const float m_horizontalStripeWipeMax = 64.0f;	//横縞ワイプの最大サイズ
	const float m_checkerboardWipeMax = 128.0f;		//チェッカーボードワイプの最大サイズ

private:	//データメンバ
	RenderTarget m_renderTarget;	//レンダリングターゲット
	Sprite m_sprite;				//スプライト
	Sprite m_finalSprite;			//最終的なスプライト
	Sprite m_backSprite;			//後ろに映すスプライト
	WipeParam m_wipeParam;			//ワイプのパラメータ
	bool m_wipeFlag = false;		//ワイプするか？
	float m_wipeSpeed = 120.0f;		//ワイプのスピード
	float m_deltaTime = 0.0f;		//デルタタイム
	Sprite m_fadeSprite;			//フェードイン、アウトで使うスプライトレンダラー
};