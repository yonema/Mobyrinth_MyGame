#pragma once
#include "LightData.h"
#include "LightManager.h"

/// <summary>
/// ディレクションライトクラス
/// </summary>
class CDirectionLight : public IGameObject
{
public:		//自動で呼ばれるメンバ関数
	CDirectionLight();		//コンストラクタ
	~CDirectionLight();		//デストラクタ

public:		//メンバ関数

	/// <summary>
	/// ディレクションライトの方向を設定
	/// </summary>
	/// <param name="direction">ライトの方向</param>
	void SetDirection(const Vector3& direction);

	/// <summary>
	/// ディレクションライトの方向を取得
	/// </summary>
	/// <returns>ライトの方向</returns>
	//const Vector3& GetDirection() const
	//{
	//	return m_light.dirLigDirection;
	//}

	/// <summary>
	/// ディレクションライトの色の設定
	/// </summary>
	/// <param name="color">ライトの色</param>
	void SetColor(const Vector4& color);

	/// <summary>
	/// 生データを取得
	/// </summary>
	/// <returns>生データの参照</returns>
	SDirectionLight* GetRawData()
	{
		return m_light;
	};

	/// <summary>
	/// 生データを設定
	/// </summary>
	/// <param name="lig">生データ</param>
	void SetRawData(SDirectionLight* lig)
	{
		m_light = lig;
	}

	/// <summary>
	/// 制御ナンバーを設定
	/// </summary>
	/// <param name="num">制御ナンバー</param>
	void SetControlNumber(const int num)
	{
		m_controlNumber = num;
	}

	/// <summary>
	/// 制御ナンバーを取得
	/// </summary>
	/// <returns>制御ナンバー</returns>
	const int GetControlNumver()const 
	{
		return m_controlNumber;
	}

private:
	SDirectionLight* m_light = nullptr;	//ディレクションライトの構造体
	int m_controlNumber;				//制御ナンバー
	bool m_result;						//ライトマネージャーに登録してあるか？

	
};

