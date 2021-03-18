#include "stdafx.h"
#include "LevelObjectBase.h"
//デバック用
//後で消す
int ILevelObjectBase::objectNumber = 0;

//スタート関数
bool ILevelObjectBase::Start()
{
	//近くのウェイポイントを探して、イイ感じに回転する
	CheckWayPoint();
	//プレイヤーの参照を保持する
	m_pPlayer = CLevelObjectManager::GetInstance()->GetPlayer();

	//LevelObjectManagerにオブジェクトが追加されたことを報告
	CLevelObjectManager::GetInstance()->AddObject(this);

	//OBBを初期化する
	InitOBB();

	//デバック用
	//後で消す。
	m_objectNumber = objectNumber++;

	Vector3* vertPos = m_obb.GetBoxVertex();
	for (int i = 0; i < m_dbgOBBNum; i++)
	{
		m_dbgOBBVert[i] = NewGO<CModelRender>(0);
		m_dbgOBBVert[i]->Init("Assets/modelData/dbgBox.tkm");
		m_dbgOBBVert[i]->SetPosition(vertPos[i]);
	}

	//デバック用ここまで

	//オーバーライドしてほしい関数PureVirtualStart()
	return PureVirtualStart();

}

ILevelObjectBase::~ILevelObjectBase()
{
	//デバック用
	//後で消す
	for (int i = 0; i < m_dbgOBBNum; i++)
	{
		DeleteGO(m_dbgOBBVert[i]);
	}
	CLevelObjectManager::GetInstance()->RemoveObject(this);
}

void ILevelObjectBase::InitOBB()
{
	//OBBの初期化データ
	SInitOBBData initData;
	initData.position = m_position;
	initData.width = 100.0f;
	initData.height = 100.0f;
	initData.length = 100.0f;
	initData.pivot = { 0.5f,0.0f,0.5f };
	initData.rotation = m_rotation;

	//OBBの初期化
	m_obb.Init(initData);
}


void ILevelObjectBase::CheckWayPoint()
{
	std::vector<Vector3>* wayPointPosVec
		= CLevelObjectManager::GetInstance()->GetWayPointPos();
	std::vector<Quaternion>* wayPointRotVec
		= CLevelObjectManager::GetInstance()->GetWayPointRot();
	float dist = FLT_MAX;
	int index = 0;
	int vecSize = (*wayPointPosVec).size();

	//一番近いウェイポイントを調べる
	for (int i = 0; i < vecSize; i++)
	{
		Vector3 lenVec = (*wayPointPosVec)[i] - m_position;
		float distTmp = lenVec.Length();

		if (distTmp < dist)
		{
			index = i;
			dist = distTmp;
		}
	}

	int lpIndex;
	int rpIndex;
	//一番近いウェイポイントの左右のインデックスを調べる
	{
		int right = index - 1;
		int left = index + 1;

		if (right < 0)
		{
			right = vecSize;
			right -= 1;
		}
		if (left >= vecSize)
		{
			left = 0;
		}

		Vector3 leftLen = (*wayPointPosVec)[left] - m_position;
		Vector3 rightLen = (*wayPointPosVec)[right] - m_position;


		if (leftLen.Length() <= rightLen.Length())
		{
			lpIndex = left;
			rpIndex = index;
		}
		else
		{
			rpIndex = right;
			lpIndex = index;
		}
	}

	//左のウェイポイントから右のウェイポイントへのベクトル
	Vector3 lpToRpLen = (*wayPointPosVec)[rpIndex] - (*wayPointPosVec)[lpIndex];

	//左のウェイポイントからプレイヤーへのベクトル
	Vector3 lpToPosLen = m_position - (*wayPointPosVec)[lpIndex];

	//補完率
	float ComplementRate = lpToPosLen.Length() / lpToRpLen.Length();

	//球面線形補完
	m_rotation.Slerp(ComplementRate, (*wayPointRotVec)[lpIndex], (*wayPointRotVec)[rpIndex]);


}


bool ILevelObjectBase::IsHitPlayer()
{
	//プレイヤーが見つかっていなかったら何もせずにreturn
	if (!m_pPlayer)
		return false;

	//OBB同士の当たり判定をして、
	//その結果を戻す
	return CollisionOBBs(m_pPlayer->GetOBB(), m_obb);


}
bool ILevelObjectBase::IsHitObject
(ILevelObjectBase& lhs, ILevelObjectBase& rhs)
{
	//OBB同士の当たり判定をして、
	//その結果を戻す
	return CollisionOBBs(lhs.GetOBB(), rhs.GetOBB());
}

void ILevelObjectBase::Update()
{
	PureVirtualUpdate();

	m_obb.SetPosition(m_position);
	m_obb.SetRotation(m_rotation);

	//デバック用
	//後で消す
	Vector3* vertPos = m_obb.GetBoxVertex();
	for (int i = 0; i < m_dbgOBBNum; i++)
	{
		m_dbgOBBVert[i]->SetPosition(vertPos[i]);
		m_dbgOBBVert[i]->SetRotation(m_rotation);
	}
	//デバック用ここまで
}