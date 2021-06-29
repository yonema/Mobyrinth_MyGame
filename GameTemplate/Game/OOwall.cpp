#include "stdafx.h"
#include "OOwall.h"
#include "ROrunning_stop.h"

//コンストラクタ
OOwall::OOwall()
{
	//スタート関数では間に合わない。
	//オブジェクトが生成された瞬間に呼ばれてほしい処理

	//ウェイポイントからの上の距離を設定
	SetYPosLen(50.0f);
}

//スタート関数
bool OOwall::StartSub()
{
	//初期化用関数
	Init("Assets/modelData/wall.tkm", EN_OO_TYPE_WALL);

	//OBBのサイズを設定
	Vector3 obbSize;
	obbSize = { 400.0f,1000.0f,400.0f };
	SetOBBDirectionLength(obbSize);

	//移動前の初期位置の設定
	m_startPosition = m_position;

	//稼働する片道分の時間
	const int moveTime = 3.0f;
	//移動する距離の補正
	const float moveLen = 200.0f;
	//アップベクトル
	Vector3 upVec = g_VEC3_UP;
	//現在の自身の回転で、アップベクトルを回す
	m_rotation.Apply(upVec);
	//アップベクトル
	upVec.Scale(moveLen * moveTime);
	//移動先の終端位置の設定
	m_endPosition = m_startPosition + upVec;

	SetOBBTag(COBB::EN_OO_TYPE_WALL);

	//最初に入れていおく稼働、停止オブジェクトを
	//停止で生成する
	m_pRun_stop = NewGO<ROrunning_stop>(0);
	m_pRun_stop->SetPosition(m_position);
	m_pRun_stop->SetFrontOrBack(EN_BACK);	
	//全反転しないようにと、Tipsを表示しないようにする
	m_pRun_stop->SetLock(true);

	//m_wallmoveSEのサウンドキューを生成する
	m_wallmoveSE = NewGO<CSoundCue>(0);
	//m_wallmoveSEのサウンドキューを、waveファイルを指定して初期化する。
	m_wallmoveSE->Init(L"Assets/sound/wallmove.wav");
	//音量調節
	m_wallmoveSE->SetVolume(0.1f);

	SetYPosLen(100.0f);

	return true;
}

OOwall::~OOwall()
{
	DeleteGO(m_wallmoveSE);
}

//アップデート関数
void OOwall::UpdateSub()
{
	if (m_firstUpdateFlag)
		FirstUpdate();

	//稼働中か？
	if (m_moveFlag)
	{
		//稼働する片道分の時間
		const float moveTime = 3.0f;

		//初期位置から終端位値へのベクトル
		Vector3 movePos = m_endPosition - m_startPosition;	//終端位置へのベクトル
		
		//現在の時間 / 片道分の時間 で倍率を出す
		float moveScale = static_cast<float>(m_moveTimer) / moveTime;	//移動速度の倍率

		//カウンターが片道分の時間を越していたら
		if (m_moveTimer >= moveTime)
		{
			//終端位置への残りのカウンターを出して、倍率を出す
			moveScale = static_cast<float>(moveTime * 2 - m_moveTimer) / moveTime;

			//カウンターが往復分の時間を越していたら
			if (m_moveTimer >= moveTime * 2)
			{
				//カウンターを0にする
				m_moveTimer = 0;
			}
		}

		//移動先へのベクトルに、倍率を掛ける
		movePos.Scale(moveScale);

		//現在の場所を、初期位置から移動先へのベクトルを加算した場所にする
		m_position = m_startPosition + movePos;

		//カウンターを進める
		m_moveTimer += GameTime().GetFrameDeltaTime();


		MoveSE();
	}

	return;
}

//プレイヤーがUFOに近づくと音を鳴らす
void OOwall::MoveSE()
{
	Vector3 distance = m_position - m_player->GetPosition();
	const float MaxDist = 1500;
	const float DistLen = distance.Length();

	if (DistLen < MaxDist) {
		float Le = MaxDist - DistLen;
		float SubLe = Le / MaxDist;
		float Vo = 2.0f * SubLe;

		//UFOmoveSEをループ再生をオンで再生する。
		m_wallmoveSE->Play(true);

		//音量調節
		m_wallmoveSE->SetVolume(Vo);

	}
	else {
		if (m_wallmoveSE->IsPlaying()) {
			m_wallmoveSE->Stop();
		}
	}
}

/// <summary>
/// 一回目のアップデートでだけ呼ばれる関数
/// </summary>
void OOwall::FirstUpdate()
{
	//稼働、停止オブジェクトがクエリすると、
	//m_pRun_stopの中身のオブジェクトを消してしまうから、
	//いったんローカル変数のポインタに避難させておいて
	//m_pRun_stopにはnullptrを入れておく
	ROrunning_stop* pRun_stop = m_pRun_stop;
	m_pRun_stop = nullptr;
	//クエリをして壁を探す
	pRun_stop->QuerySub();

	//一回目のアップデートの終了
	m_firstUpdateFlag = false;
}