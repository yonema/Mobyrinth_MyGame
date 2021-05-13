#include "stdafx.h"
#include "OOReverseALL.h"

//スタート関数
bool OOReverseALL::StartSub()
{
	//モデルの初期化とタイプの設定
	Init("Assets/modelData/grilled fish.tkm", enReverseAll);

	m_scale = { 0.0f,0.0f,0.0f };

	//OBBWorldに自身のOBBの登録を消去させる
	COBBWorld::GetInstance()->RemoveOBB(&GetOBB());

	//自己発光色を設定
	SetEmissionColor({ 0.5f,0.0f,0.5f,1.0f });

	//OBBのパラメーターを設定する
	GetOBB().SetDirectionLength({ 10.0f,400.0f,400.0f });
	GetOBB().SetPivot({ 0.5f,0.0f,0.5f });

	//changeSEのサウンドキューを生成する
	m_changeSE = NewGO<CSoundCue>(0);
	//changeSEのサウンドキューを、waveファイルを指定して初期化する。
	m_changeSE->Init(L"Assets/sound/change.wav");
	//音量調節
	m_changeSE->SetVolume(0.5f);

	//エフェクトの作成
	m_obujectefk = NewGO<Effect>(0);
	m_obujectefk->Init(u"Assets/effect/reverseall.efk");
	float scale = 200.0f;								//小さいので大きくしておく
	m_obujectefk->SetScale({ scale ,scale ,scale });
	m_obujectefk->SetPosition(m_position);				//座標を渡す
	m_obujectefk->SetRotation(m_rotation);
	m_obujectefk->Play();								//再生

	//デバック用
#ifdef MY_DEBUG
	//後で消す
	Vector3* vertPos = GetOBB().GetBoxVertex();
	for (int i = 0; i < m_vertNum; i++)
	{
		//OBBの頂点を見るためのモデルの生成と初期化
		m_dbgVertPosMR[i] = NewGO<CModelRender>(0);
		m_dbgVertPosMR[i]->Init("Assets/modelData/dbgBox.tkm");
		m_dbgVertPosMR[i]->SetPosition(vertPos[i]);
		m_dbgVertPosMR[i]->SetRotation(m_rotation);
	}
	//デバック用ここまで
#endif
	return true;
}

//デストラクタ
OOReverseALL::~OOReverseALL()
{
	DeleteGO(m_obujectefk);
#ifdef MY_DEBUG
	//デバック用
	//後で消す
	for (int i = 0; i < m_vertNum; i++)
	{
		//OBBの頂点を見るためのモデルの破棄
		DeleteGO(m_dbgVertPosMR[i]);
	}
	//デバック用ここまで
#endif
}

//アップデート関数
void OOReverseALL::UpdateSub()
{
	//エフェクト再生までのタイマー
	i++;
	if (i == 121.0f) {				//120フレームでエフェクトが終わるので121から始める
		m_obujectefk->Play();
		i = 0;
	}
	//アップデートステートで処理を振り分ける
	//switch (m_updateState)
	//{
	//case enBeforHitPlayer:
	//	//プレイヤーと衝突前の処理
	//	BeforeHitPlayer();
	//	break;
	//case enHitPlayer:
	//	//プレイヤーと衝突時の処理
	//	HitPlayer();
	//	break;
	//case enAfterHitPlayer:
	//	//プレイヤーと衝突後の処理
	//	AfterHitPlayer();
	//	break;
	//}


	///
	///
	/// switchやelse ifにしてしまうと、
	/// 目的の処理が1フレーム遅れてしまうため
	/// 全部ifで振り分けた。

	//アップデートステートで処理を振り分ける
	if (m_updateState == enBeforHitPlayer)
		//プレイヤーと衝突前の処理
		BeforeHitPlayer();

	if (m_updateState == enHitPlayer)
		//プレイヤーと衝突時の処理
		HitPlayer();

	if (m_updateState == enAfterHitPlayer)
		//プレイヤーと衝突後の処理
		AfterHitPlayer();

}

//プレイヤーと衝突前の処理
void OOReverseALL::BeforeHitPlayer()
{
	//プレイヤーとの衝突を調べる
	if (IsHitPlayer())
	{
		//衝突している

		//衝突時のプレイヤーの座標を保治
		m_playerHitPosition = m_pPlayer->GetPosition();
		//アップデートステートをプレイヤーと衝突時の状態へ
		m_updateState = enHitPlayer;
	}
	else
	{
		//衝突していない

		//衝突前のプレイヤーの座標を保持
		m_playerBeforePosition = m_pPlayer->GetPosition();
	}
}

//プレイヤーと衝突時の処理
void OOReverseALL::HitPlayer()
{
	//プレイヤーとの衝突を調べる
	if (IsHitPlayer())
	{
		//衝突している

		//衝突時のプレイヤーの座標を保治
		m_playerHitPosition = m_pPlayer->GetPosition();
	}
	else
	{
		//衝突していない

		//衝突後のプレイヤーの座標の確保
		m_playerAfterPosition = m_pPlayer->GetPosition();
		//アップデートステートをプレイヤーと衝突後の状態へ
		m_updateState = enAfterHitPlayer;
	}
}

//プレイヤーと衝突後の処理
void OOReverseALL::AfterHitPlayer()
{
	//衝突後のプレイヤーの座標の確保
	m_playerAfterPosition = m_pPlayer->GetPosition();

	//衝突前から衝突時の座標へのベクトル
	Vector3 beforeToHit = m_playerHitPosition - m_playerBeforePosition;
	//衝突時から衝突後の座標へのベクトル
	Vector3 hitToAfter = m_playerAfterPosition - m_playerHitPosition;
	//正規化しておく
	beforeToHit.Normalize();
	hitToAfter.Normalize();
	//二つの内積を取る
	float inner = Dot(beforeToHit, hitToAfter);

	//内積で二つのベクトルの向きを調べる
	if (inner >= 0.0f)
	{
		//正、同じ向きなら

		//レベルオブジェクトを取ってくる
		std::vector<ILevelObjectBase*> levelObjects
			= CLevelObjectManager::GetInstance()->GetLevelObjects();
		//全てのレベルオブジェクトに検索
		for (int i = 0; i < levelObjects.size(); i++)
		{
			//モデルの参照を得てから、SetMulColor()を呼ぶ
			//Obstacleの場合は無駄に二回呼ばれるけど、我慢しよう。
			/*levelObjects[i]->GetModelRender(CReversibleObject::enFront)->SetMulColor({ 1.0f,1.0f,1.0f,0.5f });
			levelObjects[i]->GetModelRender(CReversibleObject::enBack)->SetMulColor({ 1.0f,1.0f,1.0f,0.5f });*/
			//CReversibleObjectなら反転させる
			CReversibleObject* revers = dynamic_cast<CReversibleObject*>(levelObjects[i]);
			if (revers)
				revers->AllReverse();
		}

	}

	//changeSEをループ再生をオフで再生する。
	m_changeSE->Play(false);

	//衝突前のプレイヤーの座標を保持
	m_playerBeforePosition = m_pPlayer->GetPosition();

	//アップデートステートをプレイヤーと衝突前の状態へ
	m_updateState = enBeforHitPlayer;
}