#include "stdafx.h"
#include "ObstacleObject.h"

//スタート関数
bool CObstacleObject::PureVirtualStart()
{
	//OBBWorldに自身のOBBを登録する
	//これによってPlayerが通れなくなる
	COBBWorld::GetInstance()->AddOBB(&GetOBB());

	//モデルの回転を、現在の場所とイイ感じに合わせる
	CheckWayPoint();

	CheckFrontOrBackSide(false);

	//オーバーライドしてほしい関数StartSub()はここで呼ばれる。
	return StartSub();
}


//デストラクタ
CObstacleObject::~CObstacleObject()
{
	//LevelObjectManagerにこのオブジェクトは消すよ！って伝える。
	Delete();

	//OBBWorldに自身のOBBの登録を消去させる
	COBBWorld::GetInstance()->RemoveOBB(&GetOBB());

	//モデルの消去
	DeleteGO(m_modelRender);
}



/// <summary>
/// 初期化関数
/// 最初に呼んでね。trueを戻してね。
/// モデルのファイルパスとオブジェクトのタイプを
/// 設定する
/// タイプ一覧はObjectType.hを参照
/// </summary>
/// <param name="filePath">モデルのtkmファイルパス</param>
/// <param name="objectType">タイプ</param>
/// <returns>trueを戻してね</returns>
bool CObstacleObject::Init(const char* filePath, int objectType)
{
	//モデルレンダラーを生成
	m_modelRender = NewGO<CModelRender>(0);
	//モデルレンダラーを初期化
	m_modelRender->Init(filePath, D3D12_CULL_MODE_NONE);
	m_modelRender->SetShadowReceiverFlag(false);
	//オブジェクトのタイプを設定する
	SetObjectType(objectType);

	return true;
}

//アップデート関数
void CObstacleObject::PureVirtualUpdate()
{
	//プレイヤーが見つかっていなかったら
	if (!m_pPlayer)
	{
		//プレイヤーを探す
		m_pPlayer = CLevelObjectManager::GetInstance()->GetPlayer();

		//それでも見つからなかったら何もずにreturn
		if (!m_pPlayer)
			return;
	}

	//モデルレンダラーの場所と回転と拡大
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
	m_modelRender->SetScale(m_scale);

	//オーバーライドしてほしい関数UpdateSub()
	UpdateSub();

	return;
}