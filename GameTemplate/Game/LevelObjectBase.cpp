#include "stdafx.h"
#include "LevelObjectBase.h"
//デバック用
#ifdef MY_DEBUG
//後で消す
int ILevelObjectBase::objectNumber = 0;
#endif


//スタート関数
bool ILevelObjectBase::Start()
{
	//近くのウェイポイントを探して、イイ感じに回転する
	CheckWayPoint();
	//プレイヤーの参照を保持する
	m_player = CLevelObjectManager::GetInstance()->GetPlayer();

	//LevelObjectManagerにオブジェクトが追加されたことを報告
	CLevelObjectManager::GetInstance()->AddObject(this);

	//OBBを初期化する
	InitOBB();


#ifdef MY_DEBUG
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
#endif
	//オーバーライドしてほしい関数PureVirtualStart()
	return PureVirtualStart();

}

//デストラクタ
ILevelObjectBase::~ILevelObjectBase()
{
	//m_swichon削除
	DeleteGO(m_swichonEffect);

	//フォントのタイマーの破棄
	if (m_timerFR)
		DeleteGO(m_timerFR);

#ifdef MY_DEBUG
	//デバック用
	//後で消す
	for (int i = 0; i < m_dbgOBBNum; i++)
	{
		DeleteGO(m_dbgOBBVert[i]);
	}
	//デバック用ここまで
#endif

	//LevelObjectManagerにオブジェクトが破棄されたことを伝える
	CLevelObjectManager::GetInstance()->RemoveObject(this);
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
#ifdef MY_DEBUG
	//デバック用
	//後で消す
	Vector3* vertPos = m_obb.GetBoxVertex();
	for (int i = 0; i < m_dbgOBBNum; i++)
	{
		m_dbgOBBVert[i]->SetPosition(vertPos[i]);
		m_dbgOBBVert[i]->SetRotation(m_rotation);
	}
	//デバック用ここまで
#endif
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
void ILevelObjectBase::CheckWayPoint(const bool checkRotaton, const bool checkPosition)
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


	m_lpIndex = lpIndex;
	m_rpIndex = rpIndex;
	
	if (checkRotaton)
		CheckRotation();
	if (checkPosition)
		CheckPosition();
}

void ILevelObjectBase::CheckRotation()
{
	//LevelObjectManagerからウェイポイントの情報を持ってくる
	//ウェイポイントの「場所」を持ってくる
	std::vector<Vector3>* wayPointPosVec
		= CLevelObjectManager::GetInstance()->GetWayPointPos();
	//ウェイポイントの「回転」を持ってくる
	std::vector<Quaternion>* wayPointRotVec

		= CLevelObjectManager::GetInstance()->GetWayPointRot();
	//左のウェイポイントから右のウェイポイントへのベクトル
	Vector3 lpToRpLen = (*wayPointPosVec)[m_rpIndex] - (*wayPointPosVec)[m_lpIndex];

	//左のウェイポイントから自身へのベクトル
	Vector3 lpToPosLen = m_position - (*wayPointPosVec)[m_lpIndex];

	//自身が左右のウェイポイントの間のどれくらいの位置にいるかで
	//補完率を計算する
	float ComplementRate = lpToPosLen.Length() / lpToRpLen.Length();

	//球面線形補完
	//を使って回転させる
	m_rotation.Slerp(ComplementRate, (*wayPointRotVec)[m_lpIndex], (*wayPointRotVec)[m_rpIndex]);

}

void ILevelObjectBase::CheckPosition()
{
	Vector3 onWayPosition;
	//LevelObjectManagerからウェイポイントの情報を持ってくる
	//ウェイポイントの「場所」を持ってくる
	const std::vector<Vector3>* wayPointPosVec
		= CLevelObjectManager::GetInstance()->GetWayPointPos();
	//左のウェイポイントから右のウェイポイントへのベクトル
	Vector3 lpToRpVec = (*wayPointPosVec)[m_rpIndex] - (*wayPointPosVec)[m_lpIndex];
	lpToRpVec.Normalize();
	//左のウェイポイントから自身へのベクトル
	const Vector3 lpToPosVec = m_position - (*wayPointPosVec)[m_lpIndex];

	const float projectionLen = Dot(lpToRpVec, lpToPosVec);

	Vector3 correctionPos = lpToRpVec;
	correctionPos.Scale(projectionLen);
	correctionPos += (*wayPointPosVec)[m_lpIndex];

	Vector3 zVec = g_VEC3_BACK;
	m_rotation.Apply(zVec);
	zVec.Scale(m_zPosLen);
	correctionPos += zVec;

	m_position = correctionPos;

}

/// <summary>
/// 自身が表側にあるか裏側にあるかを調べる関数
/// </summary>
/// <param name="reversibleObject">反転オブジェクトか？</param>
void ILevelObjectBase::CheckFrontOrBackSide(const bool reversibleObject)
{
	//表側か裏側か
	int nextFrontOrBackSide = CLevelObjectManager::GetInstance()->CheckFrontOrBackSide(m_lpIndex);


	if (reversibleObject)
	{
		//初期値ではなかったら
		if (m_frontOrBackSide != EB_NONE_SIDE)
		{
			//前の場所の反転オブジェクトの数を減算する
			CLevelObjectManager::GetInstance()->RemoveReversibleObjectNum(m_frontOrBackSide);
		}

		//次の場所の反転オブジェクトの数を加算する
		CLevelObjectManager::GetInstance()->AddReversibleObjectNum(nextFrontOrBackSide);
	}
		//現在の表側か裏側かを更新する
	m_frontOrBackSide = nextFrontOrBackSide;
}

/// <summary>
/// 自身とプレイヤーの当たり判定
/// </summary>
/// <returns>trueが戻ってきたら当たっている</returns>
bool ILevelObjectBase::IsHitPlayer()
{
	//プレイヤーが見つかっていなかったら何もせずにreturn
	if (!m_player)
		return false;

	//OBB同士の当たり判定をして、
	//その結果を戻す
	return CollisionOBBs(m_player->GetOBB(), m_obb);
}


/// <summary>
/// 透明オブジェクトに使用するデータを初期化する。
/// </summary>
void ILevelObjectBase::SetTransparentObject()
{
	//透明オブジェクト判定に使用するフラグをtrueにする。
	m_transparentObjectFlag = true;
	//オブジェクトの重なっている判定を行わないようにする。
	m_isHitFlag = false;



	//タイマーのフォントレンダラーの生成と初期化
	m_timerFR = NewGO<CFontRender>(0);
	m_timerFR->Init(L"10", { 0.0f,0.0f });
	m_timerFR->SetPostRenderFlag(true);
	//非表示にする
	m_timerFR->Deactivate();

	m_swichonEffect = NewGO<Effect>(0);
	m_swichonEffect->Init(u"Assets/effect2/activation.efk");
	float scale = 70.0f;								//小さいので大きくしておく
	m_swichonEffect->SetScale({ scale ,scale ,scale });


	m_swichoffEffect = NewGO<Effect>(0);
	m_swichoffEffect->Init(u"Assets/effect2/activation.efk");
	float scale2 = 70.0f;								//小さいので大きくしておく
	m_swichoffEffect->SetScale({ scale2 ,scale2 ,scale2 });
}


/// <summary>
/// 透明スイッチが押されたときに使用される関数
/// </summary>
void ILevelObjectBase::TransparentSwitchOn()
{
	m_swichonEffect->SetPosition(m_position);
	m_swichonEffect->SetRotation(m_rotation);
	m_swichonEffect->Play();

	//オブジェクトの衝突判定を行うようにする。
	m_isHitFlag = true;

	//オブジェクトの当たり判定を有効にする。
	m_obb.SetExceptionFlag(false);


	//タイマーのフォントを表示する
	m_timerFR->Activate();

	m_swichonEffect->SetPosition(m_position);
	m_swichonEffect->SetRotation(m_rotation);
	m_swichonEffect->Play();

	ReversibleSwitchOn();
}


/// <summary>
/// 透明スイッチの効果が消えたときに使用される関数
/// </summary>
void ILevelObjectBase::TransparentSwitchOff()
{
	//スイッチが有効か？
	if (m_switchValid)
	{
		//有効

		//エフェクトを再生する
		m_swichoffEffect->SetPosition(m_position);
		m_swichoffEffect->SetRotation(m_rotation);
		m_swichoffEffect->Play();
	}
	else
	{
		//有効じゃない

		//有効にする
		m_switchValid = true;
	}

	//オブジェクトの衝突判定を行わないようにする。
	m_isHitFlag = false;

	CheckWayPoint();
	CheckFrontOrBackSide();
	//オブジェクトの当たり判定を無効にする。
	m_obb.SetExceptionFlag(true);

	//タイマーのフォントを非表示にする
	m_timerFR->Deactivate();


	ReversibleSwitchOff();
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
	//オブジェクトの衝突判定を行わない状態の場合
	if (lhs.GetIsHitFlag() == false || rhs.GetIsHitFlag() == false) {
		//衝突していない判定する。
		return false;
	}
	if (lhs.GetFrontOrBackSide() != rhs.GetFrontOrBackSide())
		return false;

	//OBB同士の当たり判定をして、
	//その結果を戻す
	return CollisionOBBs(lhs.GetOBB(), rhs.GetOBB());
}