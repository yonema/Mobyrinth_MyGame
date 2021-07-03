#pragma once
#include "OBB.h"
#include "OBBWorld.h"

/// <summary>
/// 自分で作ったキャラクターコントローラー
/// このゲームでしか使用することを考えてない。
/// </summary>
class CMyCharacterController
{
public:		//ここのメンバ関数を主に使う

	/// <summary>
	/// 初期化用関数
	/// </summary>
	/// <param name="initOBBData">OBBの初期化用データ構造体</param>
	void Init(const SInitOBBData& initOBBData);

	/// <summary>
	/// 実行関数。
	/// 動くスピードとデルタタイムを渡すと、
	/// OBBWorldに登録してあるOBBとの衝突解決をした
	/// 移動後の座標を戻す。
	/// </summary>
	/// <param name="moveSpeed">動くスピード</param>
	/// <param name="deltaTime">デルタタイム</param>
	/// <returns>移動後の座標</returns>
	const Vector3 Execute(const Vector3& moveSpeed, const float deltaTime);

	/// <summary>
	/// 衝突解決を気にしないで、場所を移動させる関数
	/// </summary>
	/// <param name="pos">場所</param>
	void SetPosition(const Vector3& pos)
	{
		m_obb.SetPosition(pos);
	}

	/// <summary>
	/// 現在の場所を取得
	/// </summary>
	/// <returns>場所</returns>
	const Vector3& GetPosition()
	{
		return m_obb.GetPosition();
	}

	/// <summary>
	/// 回転を設定する
	/// </summary>
	/// <param name="rot">回転</param>
	void SetRotation(const Quaternion& rot)
	{
		m_obb.SetRotation(rot);
	}

	/// <summary>
	/// OBBの参照を戻す
	/// </summary>
	/// <returns>OBBの参照</returns>
	COBB& GetOBB()
	{
		return m_obb;
	}

	const int GetTag() const
	{
		return m_tag;
	}

	const COBB* GetHitOBB() const
	{
		return m_hitOBB;
	}

private:	//データメンバ
	COBB m_obb;					//OBB
	int m_tag = COBB::EB_NONE_SIDE;	//タグ
	const COBB* m_hitOBB = nullptr;
	std::list<COBB*> m_exceptionalOBB;
};

