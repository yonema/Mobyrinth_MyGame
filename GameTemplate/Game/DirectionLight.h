#pragma once
#include "LightBase.h"
#include "LightData.h"
class CDirectionLight : public CLightBase
{
private:
	void Init();
public:
	CDirectionLight();
	~CDirectionLight();
	//抽象クラスの純粋仮想関数のオーバーライド
	bool StartSub() override final;
	//void Update() override final;

	/// <summary>
	/// ディレクションライトの方向を設定
	/// </summary>
	/// <param name="direction">ライトの方向</param>
	void SetDirection(const Vector3& direction);
	/// <summary>
	/// ディレクションライトの方向を取得
	/// </summary>
	/// <returns>ライトの方向</returns>
	const Vector3& GetDirection() const
	{
		return m_light.dirLigDirection;
	}
	/// <summary>
	/// ディレクションライトの色の設定
	/// </summary>
	/// <param name="color">ライトの色</param>
	void SetColor(const Vector4& color);
	/// <summary>
	/// 生データを取得
	/// </summary>
	/// <returns>生データの参照</returns>
	SDirectionLight& GetRawData()
	{
		return m_light;
	};

private:
	SDirectionLight m_light;	//ディレクションライトの構造体
};

