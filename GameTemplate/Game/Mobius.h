#pragma once
#include "ModelRender.h"

/// <summary>
/// メビウスの輪クラス
/// </summary>
class CMobius : public IGameObject
{
public:		//自動で呼ばれるメンバ関数
	bool Start()override final;		//スタート関数
	~CMobius();						//デストラクタ

public:		//ここのメンバ関数を主に使う。

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

	const bool GetIntersectPosWithMobius(
		const Vector3& startPos, const Vector3& endPos, Vector3* const IntersectPos = nullptr
	);

private:	//データメンバ
	CModelRender* m_modelRender = nullptr;	//モデルレンダラー
	Vector3 m_position = g_VEC3_ZERO;		//場所
	Quaternion m_rotation = g_QUAT_IDENTITY;	//回転
};

