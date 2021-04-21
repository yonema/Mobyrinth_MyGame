#include "stdafx.h"
#include "LevelObjectBase.h"
//デバック用
//後で消す
//int ILevelObjectBase::objectNumber = 0;

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
	//m_objectNumber = objectNumber++;

	//Vector3* vertPos = m_obb.GetBoxVertex();
	//for (int i = 0; i < m_dbgOBBNum; i++)
	//{
	//	m_dbgOBBVert[i] = NewGO<CModelRender>(0);
	//	m_dbgOBBVert[i]->Init("Assets/modelData/dbgBox.tkm");
	//	m_dbgOBBVert[i]->SetPosition(vertPos[i]);
	//}

	//デバック用ここまで

	//オーバーライドしてほしい関数PureVirtualStart()
	return PureVirtualStart();

}

//デストラクタ
ILevelObjectBase::~ILevelObjectBase()
{
	//デバック用
	//後で消す
	//for (int i = 0; i < m_dbgOBBNum; i++)
	//{
	//	DeleteGO(m_dbgOBBVert[i]);
	//}
	//デバック用ここまで

	//LevelObjectManagerにオブジェクトが破棄されたことを伝える
	CLevelObjectManager::GetInstance()->RemoveObject(this);
}

/// <summary>
/// OBBの初期化関数
/// </summary>
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

/// <summary>
/// 近くのウェイポイントを探して、イイ感じに回転する関数
/// </summary>
void ILevelObjectBase::CheckWayPoint()
{
	//LevelObjectManagerからウェイポイントの情報を持ってくる
	//ウェイポイントの「場所」を持ってくる
	std::vector<Vector3>* wayPointPosVec
		= CLevelObjectManager::GetInstance()->GetWayPointPos();
	//ウェイポイントの「回転」を持ってくる
	std::vector<Quaternion>* wayPointRotVec
		= CLevelObjectManager::GetInstance()->GetWayPointRot();


	//一番近いウェイポイントを調べる

	//一番近い距離を調べるようの変数
	//FLT_MAX（float型の大きい数）を最初に入れておく
	float dist = FLT_MAX;
	//一番近いウェイポイントのベクターのインデックスを保持しておく用の変数
	int index = 0;
	//ウェイポイントのベクターのサイズを保持
	int vecSize = (*wayPointPosVec).size();

	//一番近いウェイポイントを調べる
	for (int i = 0; i < vecSize; i++)
	{
		//ウェイポイントと自身の間のベクトルを計算する
		Vector3 lenVec = (*wayPointPosVec)[i] - m_position;
		//ウェイポイントと自信の間の長さを計算する
		float distTmp = lenVec.Length();

		//distより近い距離なら
		if (distTmp < dist)
		{
			//一番近いウェイポイントのベクターのインデックスに
			//現在の繰り返し回数を保持させて、
			index = i;
			//distにその距離を保持させる
			dist = distTmp;
		}
	}

	//自身の左のウェイポイントのベクターのインデックス
	int lpIndex;
	//自身の右のウェイポイントのベクターのインデックス
	int rpIndex;

	//一番近いウェイポイントの左右のインデックスを調べる
	{
		//一番近いウェイポイントの右のインデックス
		int right = index - 1;
		//一番近いウェイポイントの左のインデックス
		int left = index + 1;

		//0より小さかったら
		if (right < 0)
		{
			//一周させる
			right = vecSize;
			//インデックスの最大値はベクターのサイズより1小さいから-1する
			right -= 1;
		}
		//ベクターのサイズ以上だったら
		//インデックスの最大値はベクターのサイズより1小さいから
		//　>=　で比較する
		if (left >= vecSize)
		{
			//一周させる
			left = 0;
		}

		//一番近いウェイポイントの左のウェイポイントと、自身との間のベクトル
		Vector3 leftLen = (*wayPointPosVec)[left] - m_position;
		//一番近いウェイポイントの右のウェイポイントと、自身との間のベクトル
		Vector3 rightLen = (*wayPointPosVec)[right] - m_position;

		//右と左の距離がどちらが近いか比較する
		if (leftLen.Length() <= rightLen.Length())
		{
			//左の方が近いなら
			//左のインデックスを、一番近いウェイポイントの左のウェイポイント
			//に決定する
			lpIndex = left;
			//右のインデックスを、一番近いウェイポイント
			//に決定する
			rpIndex = index;
		}
		else
		{
			//右の方が近いなら
			//みぎのインデックスを、一番近いウェイポイントの右のウェイポイント
			//に決定する
			rpIndex = right;
			//左のウェイポイントを、一番近いウェイポイント
			//に決定する
			lpIndex = index;
		}
	}

	//左のウェイポイントから右のウェイポイントへのベクトル
	Vector3 lpToRpLen = (*wayPointPosVec)[rpIndex] - (*wayPointPosVec)[lpIndex];

	//左のウェイポイントからプレイヤーへのベクトル
	Vector3 lpToPosLen = m_position - (*wayPointPosVec)[lpIndex];

	//自身が左右のウェイポイントの間のどれくらいの位置にいるかで
	//補完率を計算する
	float ComplementRate = lpToPosLen.Length() / lpToRpLen.Length();

	//球面線形補完
	//を使って回転させる
	m_rotation.Slerp(ComplementRate, (*wayPointRotVec)[lpIndex], (*wayPointRotVec)[rpIndex]);

}



//アップデート関数
void ILevelObjectBase::Update()
{
	//オーバーライドされる関数
	//派生クラスのアップデート関数から先に呼ばれる
	PureVirtualUpdate();

	//OBBの場所と回転を設定する
	m_obb.SetPosition(m_position);
	m_obb.SetRotation(m_rotation);

	//デバック用
	//後で消す
	//Vector3* vertPos = m_obb.GetBoxVertex();
	//for (int i = 0; i < m_dbgOBBNum; i++)
	//{
	//	m_dbgOBBVert[i]->SetPosition(vertPos[i]);
	//	m_dbgOBBVert[i]->SetRotation(m_rotation);
	//}
	//デバック用ここまで
}


/// <summary>
/// 自身とプレイヤーの当たり判定
/// </summary>
/// <returns>trueが戻ってきたら当たっている</returns>
bool ILevelObjectBase::IsHitPlayer()
{
	//プレイヤーが見つかっていなかったら何もせずにreturn
	if (!m_pPlayer)
		return false;

	//OBB同士の当たり判定をして、
	//その結果を戻す
	return CollisionOBBs(m_pPlayer->GetOBB(), m_obb);


}



/// <summary>
/// OBB同士の当たり判定
/// </summary>
/// <param name="lhs">レベルオブジェクト1</param>
/// <param name="rhs">レベルオブジェクト2</param>
/// <returns>trueが戻ってきたら当たっている</returns>
bool IsHitObject
(ILevelObjectBase& lhs, ILevelObjectBase& rhs)
{
	//OBB同士の当たり判定をして、
	//その結果を戻す
	return CollisionOBBs(lhs.GetOBB(), rhs.GetOBB());
}