#include "stdafx.h"
#include "OOgoal.h"

//ObstacleObjectのモデルのファイルパスとOBBのサイズの定数データを使用可能にする
using namespace OOsFilepathAndObbSizeConstData;
//ゴールの定数データを使用可能にする
using namespace goalConstData;

//スタート関数
bool OOgoal::StartSub()
{
	//初期化用関数
	Init(MODEL_FILEPATH_GOAL, EN_OO_TYPE_GOAL);

	//OBBのサイズを設定
	SetOBBDirectionLength(SIZE_OBB_GOAL);

	//エフェクトの初期化処理
	InitEffect();

	//スプライトの初期化処理
	InitSprite();

	//サウンドの初期化処理
	InitSound();

	//障害オブジェクトは、プレイヤーが素通り
	//OBBWorldに自身のOBBの登録を消去させる
	COBBWorld::GetInstance()->RemoveOBB(GetOBB());

	return true;
}

/**
 * @brief エフェクトの初期化処理
*/
void OOgoal::InitEffect()
{
	//ゴール時のエフェクトを生成
	m_goalEF = NewGO<Effect>(PRIORITY_FIRST);
	//初期化
	m_goalEF->Init(EFFECT_FILEPAHT_GOAL);
	//座標と回転と拡大率を設定
	m_goalEF->SetPosition(m_position);
	m_goalEF->SetRotation(m_rotation);
	m_goalEF->SetScale(EFFECT_SCALE_GOAL);

	return;
}

/**
 * @brief スプライトの初期化処理
*/
void OOgoal::InitSprite()
{
	//ゴールのスプライトの生成
	m_goalSR = NewGO<CSpriteRender>(PRIORITY_SECOND);
	//初期化
	m_goalSR->Init(
		SPRITE_FILEPATH_GOAL,
		SPRITE_WIDHT_GOAL,
		SPRITE_HEIGHT_GOAL,
		spriteRenderConstData::SPRITE_PIVOT_DEFALUT,
		AlphaBlendMode_Trans
	);
	//座標設定
	m_goalSR->SetPosition(SPRITE_POSITION_GOAL);
	m_goalSR->SetPostRenderFlag(true);

	//無効化する
	m_goalSR->Deactivate();

	return;
}

/**
 * @brief サウンドの初期化処理
*/
void OOgoal::InitSound()
{
	//ゴール時のサウンドを生成する
	m_goalSC = NewGO<CSoundCue>(PRIORITY_FIRST);
	//ゴール時のサウンドを、waveファイルを指定して初期化する。
	m_goalSC->Init(SOUND_FILEPATH_SE_GOAL);
	//音量調節
	m_goalSC->SetVolume(SOUND_VOLUME_SE_GOAL);

	return;
}

//デストラクタ
OOgoal::~OOgoal()
{
	//リソースの破棄
	DeleteGO(m_goalEF);
	DeleteGO(m_goalSR);
	DeleteGO(m_goalSC);

	return;
}

//アップデート関数
void OOgoal::UpdateSub()
{
	//落下中か？
	if (m_player->GetFallFalg())
		//落下中はゴール判定を取らない
		return;

	//ゴール状態ではない時、かつ
	//自身とプレイヤーが衝突したら
	if (m_isGoal == false && IsHitPlayer())
	{
		//ゴール時の処理
		Goal();
	}

	return;
}


/**
 * @brief ゴール時の処理
*/
void OOgoal::Goal()
{
	//ゴール状態にする
	m_isGoal = true;
	//プレイヤーをゴール状態にする
	m_player->SetGoal(true);

	//モデルを見えなくする
	GetModelRender()->Deactivate();

	//エフェクトの座標と回転をプレイヤーのものに設定する
	m_goalEF->SetPosition(m_player->GetPosition());
	m_goalEF->SetRotation(m_player->GetFinalWPRot());
	//ゴール時のエフェクトを再生する
	m_goalEF->Play();

	//ゴール時のスプライトを表示する
	m_goalSR->Activate();

	//ゴール時のサウンドをワンショット再生で再生する
	m_goalSC->Play(false);

	//透明オブジェクトか？
	if (GetFlagTransparentObject())
	{
		//タイマーのフォントを見えなくする
		SetTimerFontAitiveFlag(false);
	}

	return;
}
