#pragma once
#include "OBB.h"
#include "OBBWorld.h"

class CMyCharacterController
{
public:		//ここのメンバ関数を主に使う
	void Init(const SInitOBBData& initOBBData);
	const Vector3& Execute(const Vector3& moveSpeed, const float deltaTime);
	void SetPosition(const Vector3& pos)
	{
		m_obb.SetPosition(pos);
	}
	const Vector3& GetPosition()
	{
		return m_obb.GetPosition();
	}
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

private:	//データメンバ
	COBB m_obb;
};

