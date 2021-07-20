#pragma once
#include "ModelRender.h"

namespace mobiusConstData
{
	//メビウスの輪のモデルのファイルパス
	constexpr const char* const MODEL_FILEPATH_MOBIUS = "Assets/modelData/Mobius.tkm";

	//メビウスの輪のシェーダーのファイルパス
	constexpr const char* const SHADER_FILEPATH_MOBIUS = "Assets/shader/PBR.fx";
}

/// <summary>
/// メビウスの輪クラス
/// </summary>
class CMobius : public IGameObject
{
public:		//自動で呼ばれるメンバ関数
	bool Start()override final;		//スタート関数
	~CMobius();						//デストラクタ

public:		//メンバ関数

	/// <summary>
	/// 場所の設定
	/// </summary>
	/// <param name="pos">場所</param>
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	/// <summary>
	/// 回転を設定
	/// </summary>
	/// <param name="rot">回転</param>
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

	/**
	 * @brief メビウスの輪とレイの交差点を求める
	 * @param [in] startPos レイの始点
	 * @param [in] endPos レイの終点
	 * @param [out] IntersectPos 交差点
	 * @return 交差しているか？
	*/
	const bool GetIntersectPosWithMobius(
		const Vector3& startPos, const Vector3& endPos, Vector3* const IntersectPos = nullptr
	);

private:	//データメンバ
	CModelRender* m_modelRender = nullptr;	//モデルレンダラー
	Vector3 m_position = g_VEC3_ZERO;		//場所
	Quaternion m_rotation = g_QUAT_IDENTITY;	//回転
};

