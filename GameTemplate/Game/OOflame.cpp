#include "stdafx.h"
#include "OOflame.h"

//ObstacleObjectのモデルのファイルパスとOBBのサイズの定数データを使用可能にする
using namespace OOsFilepathAndObbSizeConstData;
//炎の定数データを使用可能にする
using namespace flameConstData;

//スタート関数
bool OOflame::StartSub()
{
	//モデルの初期化とタイプの設定
	Init(MODEL_FILEPATH_FLAME, EN_OO_TYPE_FLAME);

	//OBBの方向ベクトルの長さを設定
	SetOBBDirectionLength(SIZE_OBB_FLAME);

	//自己発光色を設定
	SetModelEmissionColor(g_VEC4_RED);

	//OBBのタグを設定する
	SetOBBTag(COBB::EN_OO_TYPE_FLAME);

	
	//ポイントライトの初期化処理
	InitPointLight();

	//サウンドの初期化処理
	InitSound();

	//エフェクトの初期化処理
	InitEffect();

	return true;
}

/**
 * @brief ポイントライトの初期化処理
*/
void OOflame::InitPointLight()
{
	//ポイントライトを生成
	m_pointLight = NewGO<CPointLight>(PRIORITY_FIRST);
	Vector3 ptLigPos = m_position;
	Vector3 up = g_VEC3_UP;
	m_rotation.Apply(up);
	up.Scale(LENGTH_UP_POSITION_POINT_LIGHT);
	//ポイントライトを自身の座標より上に置く
	ptLigPos += up;
	//ポイントライトの座標を設定
	m_pointLight->SetPosition(ptLigPos);

	//ポイントライトのカラーを設定
	m_pointLight->SetColor(COLOR_POINT_LIGHT);
	//ポイントライトの影響範囲を設定
	m_pointLight->SetRange(RANGE_POINT_LIGHT);

	return;
}

/**
 * @brief サウンドの初期化処理
*/
void OOflame::InitSound()
{
	//炎の常時鳴っているサウンドを生成する
	m_flameSE = NewGO<CSoundCue>(PRIORITY_FIRST);
	//炎の常時鳴っているサウンドを、waveファイルを指定して初期化する。
	m_flameSE->Init(SOUND_FILEPATH_FLAME);
	//音量調節
	m_flameSE->SetVolume(SOUND_VOLUME_FLAME);


	//炎と水の反応サウンドを生成する
	m_flame_waterSE = NewGO<CSoundCue>(PRIORITY_FIRST);
	//炎と水の反応サウンドを、waveファイルを指定して初期化する。
	m_flame_waterSE->Init(SOUND_FILEPATH_FLAME_WATAER);
	//音量調節
	m_flame_waterSE->SetVolume(SOUND_VOLUME_FLAME_WATER);

	return;
}

/**
 * @brief エフェクトの初期化処理
*/
void OOflame::InitEffect()
{
	//炎と水の反応エフェクト作成
	m_flame_waterEF = NewGO<Effect>(PRIORITY_FIRST);
	m_flame_waterEF->Init(EFFECT_FILEPATH_FLAME);
	m_flame_waterEF->SetScale(EFFECT_SCALE_FLAME);
	Vector3 upVec = g_VEC3_UP;
	m_rotation.Apply(upVec);
	//この値を変更して高さを調節する
	upVec.Scale(LENGTH_UP_POSITION_EFFECT);
	m_flame_waterEF->SetPosition(m_position + upVec);		//座標を渡す
	m_flame_waterEF->SetRotation(m_rotation);

	return;
}

//デストラクタ
OOflame::~OOflame()
{
	DeleteGO(m_pointLight);

	DeleteGO(m_flameSE);

	DeleteGO(m_flame_waterSE);

	DeleteGO(m_flame_waterEF);

	return;
}

//アップデート関数
void OOflame::UpdateSub()
{
	//プレイヤーが炎に近づくと燃えてる音を出す
	Burn();

	//このオブジェクトのヒットフラグが立っているか？
	if (GetIsHitFlag())
	{
		//ライトをつける
		m_pointLight->SetColor(COLOR_POINT_LIGHT);
	}
	//オブジェクトが透明の時にヒットフラグが折れている
	else
	{
		//ライトを消す
		m_pointLight->SetColor(g_VEC4_BLACK);
	}

	return;
}

//ダメージを受ける
void OOflame::Damage()
{
	//HPを減らして、0以下か？
	if (--m_hp <= 0)
	{
		//HPが0以下だったら、このオブジェクトを消去
		DeleteGO(this);
	}
	else
	{
		//OBBのサイズをHPに応じて小さくする
		Vector3 obbSize;
		obbSize = SIZE_OBB_FLAME;

		//m_hpとm_maxHpはint型。
		//int型同士の割り算だから小数が切り捨てられて
		//評価した値が0になってしまう。
		//だから、m_hpをfloatにキャストする
		obbSize.Scale(static_cast<float>(m_hp + SIZE_COMPLEMENT_FLAME) / (HP_MAX + SIZE_COMPLEMENT_FLAME));
		SetOBBDirectionLength(obbSize);


		//モデルのサイズをHPに応じて小さくする
			//これが上のみたいに切り捨てにならない理由は
			//m_hp / m_maxHpより先にg_vec3One * m_hpが呼ばれ
			//Vector3の演算子のオーバーライドの引数がfloat型だから
			//m_hpが渡されるときに強制型変換が起きてfloat型になるから
			//これがSetScale(g_VEC3_ONE * (m_hp / m_maxHp))
			//だったらうまくいかない。
		SetScale(g_VEC3_ONE * (m_hp + SIZE_COMPLEMENT_FLAME) / (HP_MAX + SIZE_COMPLEMENT_FLAME));
	}

	//エフェクト再生
	m_flame_waterEF->Play();
	//サウンド再生
	m_flame_waterSE->Play(false);

	return;
}	

//プレイヤーが炎に近づくと燃えてる音を出す
void OOflame::Burn()
{
	//プレイヤーから自身へのベクトル
	Vector3 fromPlayerVec = m_position - m_player->GetPosition();
	//プレイヤーからの距離
	const float distFromPlayer = fromPlayerVec.Length();

	//距離が最大距離より小さいか？
	if (distFromPlayer < DISTANCE_MAX_FLAME_SOUND)
	{
		//音量の倍率
		//distFromPlayerが小さくなるほど、倍率が～1.0fに近づく
		const float volumeRate = (DISTANCE_MAX_FLAME_SOUND - distFromPlayer) / DISTANCE_MAX_FLAME_SOUND;
		//音量計算
		const float volume = SOUND_VOLUME_FLAME * volumeRate;
		
		//flameSEをループ再生をオンで再生する。
		m_flameSE->Play(true);
		
		//音量調節
		m_flameSE->SetVolume(volume);
		
	}
	else 
	{
		//距離が最大距離より大きい

		//再生中か？
		if (m_flameSE->IsPlaying())
		{
			//再生中なら停止する
			m_flameSE->Stop();
		}
	}

	return;
}