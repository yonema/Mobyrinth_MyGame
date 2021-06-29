#include "stdafx.h"
#include "LevelObjectManager.h"
#include "LevelObjectBase.h"
#include "ReversibleObject.h"
#include "Player.h"


//インスタンスの初期化
CLevelObjectManager* CLevelObjectManager::m_instance = nullptr;

//コンストラクタ
CLevelObjectManager::CLevelObjectManager()
{
	if (m_instance != nullptr) {
		//インスタンスがすでに作られている。
		std::abort();
	}
	return;
}

//デストラクタ
CLevelObjectManager::~CLevelObjectManager()
{
	//nullptrを入れておく
	m_instance = nullptr;

	return;
}

/// <summary>
/// ウェイポイントの「場所」を初期化
/// </summary>
/// <param name="vecSize">ウェイポイントのサイズ</param>
/// <param name="posMap">場所のマップ</param>
void CLevelObjectManager::InitWayPointPos(std::map<int, Vector3>& posMap)
{
	//vectorのサイズの確保
	m_wayPointPos.resize(posMap.size());
	//m_wayPointPosにウェイポイントの「場所」を格納する
	std::map<int, Vector3>::iterator it = posMap.begin();
	for (int index = 0; it != posMap.end(); index++, it++)
	{
		m_wayPointPos[index] = it->second;
	}

	//表側と裏側のしきい値の初期化
	InitFrontOrBackSideThreshold();

	return;
}


/// <summary>
/// ウェイポイントの「回転」を初期化
/// </summary>
/// <param name="vecSize">ウェイポイントのサイズ</param>
/// <param name="rotMap">回転のマップ</param>
void CLevelObjectManager::InitWayPointRot(std::map<int, Quaternion>& rotMap)
{
	//vectorのサイズの確保
	m_wayPointRot.resize(rotMap.size());
	//m_wayPointRotにウェイポイントの「回転」を格納する
	std::map<int, Quaternion>::iterator it = rotMap.begin();
	for (int index = 0; it != rotMap.end(); index++, it++)
	{
		m_wayPointRot[index] = it->second;
	}

	return;
}


/**
 * @brief 表側と裏側のしきい値の初期化
*/
void CLevelObjectManager::InitFrontOrBackSideThreshold()
{
	/*
	* ウェイポイントの番号の最初4分の1と最後4分の1が表側、
	* 4分の2～4分の3は裏側になる。
	* 
	* ウェイポイントの番号は左周りで増えていき、表側か裏側かは
	* 自身の左側のウェイポイントを使って計算するから、小さいほうの
	* しきい値が1加算する。
	*/

	m_frontOrBackSideThresholdSmall = m_wayPointPos.size() / 4 + 1;
	m_frontOrBackSideThresholdBig = (m_wayPointPos.size() / 4) * 3;

	return;
}


/**
 * @brief ウェイポイント上での移動先を計算する関数
 * @param [in] rpIndex 現在の右側のウェイポイントのインデックス
 * @param [in] pos 現在の座標
 * @param [in] dist 移動する距離
 * @param [in] leftOrRight 右側に移動するか左側い移動するか。0:左,1:右
 * @param [out] pNextIndex 移動先のウェイポイントのインデックス
 * @return 移動先の座標
*/
const Vector3 CLevelObjectManager::CalcWayPointNextPos
(const int rpIndex, const Vector3& pos, const float dist, const bool leftOrRight, int* pNextIndex)
{
	//左側のウェイポイントは右側のウェイポイントの1上
	int lpIndex = rpIndex + 1;
	//ウェイポイントの最大数はサイズの1下
	const int maxWayPointIndex = m_wayPointPos.size() - 1;
	//左側のウェイポイントが最大値より大きかったら
	if (lpIndex > maxWayPointIndex)
	{
		//一周して0になる
		lpIndex = 0;
	}

	//移動するために加えるベクトル
	Vector3 addVec = g_VEC3_ZERO;
	//移動する先にあるウェイポイントのインデックス
	int nextIndex = 0;
	//左右どちらに飛ばすか？
	if (leftOrRight == EN_RIGHT)
	{
		//右に飛ばす
		nextIndex = rpIndex;
	}
	else
	{
		//左に飛ばす
		nextIndex = lpIndex;
	}
	//移動前の座標
	Vector3 originPos = pos;
	//元の場所から移動先の方向のウェイポイントへのベクトル
	addVec = m_wayPointPos[nextIndex] - originPos;
	//移動先への長さ
	float addLen = addVec.Length();
	//指定された移動する距離
	float rDist = dist;
	//指定された移動距離が、計算した移動距離より大きかったら
	if (addLen <= rDist)
	{
		//指定された移動距離を計算した移動距離分減算する
		rDist -= addLen;
		//変更前の移動先のウェイポイントのインデックを変更する
		int otherIndex = nextIndex;
		if (leftOrRight == EN_RIGHT)
		{
			//右に飛ばす
			nextIndex--;
			if (nextIndex < 0)
				nextIndex = maxWayPointIndex;
		}
		else
		{
			//左に飛ばす
			nextIndex++;
			if (nextIndex > maxWayPointIndex)
				nextIndex = 0;
		}
		//移動前の座標
		originPos = m_wayPointPos[otherIndex];
		//移動前から移動後へのベクトル
		addVec = m_wayPointPos[nextIndex] - originPos;
		//addLen = addVec.Length();
	}
	//移動する方向を正規化する
	addVec.Normalize();
	//指定された移動量だけ方向を伸ばす
	addVec.Scale(rDist);
	//次のウェイポイントが引数で渡されていたら
	if (pNextIndex)
		//次のウェイポイントに値を変更する
		*pNextIndex = nextIndex;

	//移動前の座標に移動するベクトルを加えて戻す
	return originPos + addVec;
}



/// <summary>
/// 自身（LevelObjectManager）にオブジェクトを追加する
/// </summary>
/// <param name="object">追加するオブジェクト</param>
void CLevelObjectManager::AddObject(ILevelObjectBase* object)
{
	m_levelObjects.push_back(object);
}

/// <summary>
/// 自身（LevelObjectManager）からオブジェクトを破棄する
/// </summary>
/// <param name="object">破棄するオブジェクト</param>
void CLevelObjectManager::RemoveObject(ILevelObjectBase* object)
{
	//一度に一個しか消せないよ
	std::vector<ILevelObjectBase*>::iterator itr;
	for (itr = m_levelObjects.begin(); itr != m_levelObjects.end(); itr++)
	{
		if ((*itr)->GetIsDead())
		{
			m_levelObjects.erase(itr);
			break;
		}
	}
}

/// <summary>
/// レベルオブジェクトをすべて消去する
/// </summary>
void CLevelObjectManager::AllDeleteLOs()
{
	for (int i = 0; i < m_levelObjects.size(); i++)
	{
		DeleteGO(m_levelObjects[i]);
	}
}

/**
 * @brief 全てのレベルオブジェクトを調べて、衝突していないか調べる
 * @param [in] thisObject 自身のインスタンス
 * @return 衝突しているか？
*/
bool CLevelObjectManager::IsHitAllLevelObjects(const ILevelObjectBase& thisObject)
{
	for (auto lo : m_levelObjects) {
		//自分自身じゃないとき
		if (lo != &thisObject) {
			//重なっているオブジェクトがある。
			if (IsHitLevelObject(thisObject, *lo)) {
				return true;
			}
		}
	}
	return false;
}

/// <summary>
/// プレイヤーに一番近いオブジェクトのオブジェクトタイプを戻す
/// </summary>
/// <returns>オブジェクトタイプ</returns>
const int CLevelObjectManager::GetNearestObjectType(const float nearDist)
{
	//プレイヤーが何か持っていたら、
	if (m_player->GetHoldObject())
	{
		//プレイヤーが持っているオブジェクトのタイプを戻す
		if (m_player->GetReversibleObject())
			return m_player->GetReversibleObject()->GetObjectType();
	}
	//プレイヤーがUFOに捕まっているか？
	else if (m_player->GetCapturedUFOFlag())
	{
		//捕まっていたら

		//UFOに捕まっている状態を戻す
		return EN_OO_TYPE_UFO_CAPTURE;
	}


	//ウェイポイントの最大値
	const int maxWayPoint = m_wayPointPos.size() - 1;
	//プレイヤーのいるウェイポイント
	int wayPoint = m_player->GetLeftPointIndex();
	//ウェイポイント左のウェイポイント
	int leftWayPoint = wayPoint + 1;
	//最大値より大きかったら
	if (leftWayPoint > maxWayPoint)
		//一周させて0にする
		leftWayPoint = 0;
	//ウェイポイント左のウェイポイント
	int rightWayPoint = wayPoint - 1;
	//0より小さかったら
	if (rightWayPoint < 0)
		//一周させて最大値にする
		rightWayPoint = maxWayPoint;

	//オブジェクトとの距離のしきい値
	const float threshold = nearDist;

	//オブジェクトとの距離
	//しきい値以下の一番近い距離を探す
	float dist = threshold;

	//戻すオブジェクトタイプ
	//近くにオブジェクトがなかったらenEmptyを戻す
	int objectType = EN_OBJECT_TYPE_EMPTY;

	//レベルオブジェクトを全部調べる
	for (int i = 0; i < m_levelObjects.size(); i++)
	{
		//ロック中ならスキップ
		if (m_levelObjects[i]->GetLock())
			continue;

		//オブジェクトがいるウェイポイントを調べる
		int objectsWayPoint = m_levelObjects[i]->GetLeftWayPointIndex();
		//オブジェクトの位置を調べる
		if (objectsWayPoint == leftWayPoint || objectsWayPoint == wayPoint ||
			objectsWayPoint == rightWayPoint)
		{
			//プレイヤーと同じウェイポイント、または、その左右のどちらかにいたら

			//プレイヤーからオブジェクトへのベクトル
			Vector3 playerToObject = m_levelObjects[i]->GetPosition() - m_player->GetPosition();
			//ベクトルの長さ
			float length = playerToObject.Length();

			//長さが、しきい値以下で
			//今までの長さより近かったら
			if (length <= dist)
			{
				//戻すオブジェクトタイプを更新
				objectType = m_levelObjects[i]->GetObjectType();
				//距離も更新
				dist = length;
			}
		}
		else
		{
			//ウェイポイントの場所が違ったらスキップ
			continue;
		}
	}
	
	//オブジェクトタイプを戻す
	return objectType;

}


/**
 * @brief 表側にいるのか裏側にいるのかを調べる
 * @param [in] lpIndex 自身の左側のウェイポイントの番号
 * @return 表側か裏側か
*/
const int CLevelObjectManager::CheckFrontOrBackSide(const int lpIndex)
{
	//表側か裏側か
	int frontOrBackSide = EB_NONE_SIDE;

	//左側のウェイポイントをしきい値と比べて、表側か裏側か調べる
	if (m_frontOrBackSideThresholdSmall <= lpIndex && lpIndex <= m_frontOrBackSideThresholdBig)
	{
		//裏側
		frontOrBackSide = EN_BACK_SIDE;
	}
	else
	{
		//表側
		frontOrBackSide = EN_FRONT_SIDE;
	}

	//結果を戻す
	return frontOrBackSide;

}