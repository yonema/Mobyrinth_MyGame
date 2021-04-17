#pragma once
#include <time.h>

/// <summary>
/// 場面転換クラス
/// </summary>
class CSceneChange
{
public:		//メンバ関数

	void Init();
	void UpdateParam();
	void Draw(RenderContext& rc);

	void WipeIn()
	{
		SetInOrOut(enIn);
		WipeStart();
	}
	void WipeOut()
	{
		SetInOrOut(enOut);
		WipeStart();
	}
	void RandomWipeStart();
	void WipeStart()
	{
		m_wipeParam.wipeSize = 0.0f;
		m_wipeFlag = true;
	};
	void WipeEnd()
	{
		m_wipeParam.wipeSize = 0.0f;
		SetInOrOut(enOut);
		m_wipeFlag = false;
	}
	void SetWipeDirection(const Vector2& dir);

	void SetWipeLeftOrLight(const bool leftOrLight)
	{
		m_wipeParam.leftOtLight = leftOrLight;
	}
	void SetWipeTopOrDown(const bool topOrDown)
	{
		m_wipeParam.topOrDown = topOrDown;
	}
	void SetWipeType(const int wipeType)
	{
		m_wipeParam.wipeType = wipeType;
	}
	void SetInOrOut(const bool inOrOut)
	{
		m_wipeParam.inOrOut = inOrOut;
	}
	void SetWipeSpeed(const float wipeSpeed)
	{
		m_wipeSpeed = wipeSpeed;
	}

	const float GetWipeSize() const
	{
		return m_wipeParam.wipeSize;
	}

	const float GetWipeSizeMax(const int wipeType) const;

	const int GetWipeType() const
	{
		return m_wipeParam.wipeType;
	}

	const bool IsWipeFinished() const;

private:
	void InitRenderTarget();
	void InitSprite();

public:
	enum EnLeftOrLight
	{
		enLeft,
		enRight,
	};
	enum EnTopOrDown
	{
		enTop,
		enDown,
	};
	enum EnWipeType
	{
		enWipe,
		enCircleWipe,
		enVerticalStripeWipe,
		enHorizontalStripeWipe,
		enCheckerboardWipe,
		enMonochrome,
		enSepia,
		enNega,
		enNois,
	};
	enum EnInOrOut
	{
		enIn,
		enOut,
	};
private:
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
	const float m_wipeMaxX = 1280.0f;
	const float m_wipeMaxY = 720.f;
	const float m_circleWipeMax = 1280.0f / 1.5;
	const float m_verticalStripeWipeMax = 64.0f;
	const float m_horizontalStripeWipeMax = 64.0f;
	const float m_checkerboardWipeMax = 128.0f;

private:	//データメンバ
	RenderTarget m_renderTarget;
	Sprite m_sprite;
	Sprite m_finalSprite;
	Sprite m_backSprite;
	WipeParam m_wipeParam;
	bool m_wipeFlag = false;
	float m_wipeSpeed = 3.0f;
};