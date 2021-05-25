#include "stdafx.h"
#include "OObigFire.h"

//スタート関数
bool OObigFire::StartSub()
{
	//モデルの初期化とタイプの設定
	Init("Assets/modelData/flame.tkm", enBigFire);

	//OBBのサイズを設定
	Vector3 obbSize;
	obbSize = { 300.0f,300.0f,400.0f };
	//OBBの方向ベクトルの長さを設定
	GetOBB().SetDirectionLength(obbSize);

	//ポイントライトを生成
	m_pointLight = NewGO<CPointLight>(0);
	Vector3 ptLigPos = m_position;
	Vector3 up = g_vec3Up;
	m_rotation.Apply(up);
	up.Scale(300.0f);
	ptLigPos += up;
	m_pointLight->SetPosition(ptLigPos);
	m_pointLight->SetColor({ 500.0f,1.0f,1.0f,1.0f });
	m_pointLight->SetRange(500.0f);

	//自己発光色を設定
	SetEmissionColor({ 1.0f,0.0f,0.0f,1.0f });

	GetOBB().SetTag(COBB::enBigFire);

	//flameSEのサウンドキューを生成する
	m_flameSE = NewGO<CSoundCue>(0);
	//flameSEのサウンドキューを、waveファイルを指定して初期化する。
	m_flameSE->Init(L"Assets/sound/flame.wav");

	//m_flame_waterエフェクトの作成
	m_flame_water = NewGO<Effect>(0);
	m_flame_water->Init(u"Assets/effect/flame_water.efk");
	float scale = 200.0f;								//小さいので大きくしておく
	m_flame_water->SetScale({ scale ,scale ,scale });
	Vector3 upVec = g_vec3Up;
	m_rotation.Apply(upVec);
	//この値を変更して高さを調節する
	const float upVecLne = 100.0f;
	upVec.Scale(upVecLne);
	m_flame_water->SetPosition(m_position + upVec);		//座標を渡す
	m_flame_water->SetRotation(m_rotation);

	//m_flame_waterSEのサウンドキューを生成する
	m_flame_waterSE = NewGO<CSoundCue>(0);
	//m_flame_waterSEのサウンドキューを、waveファイルを指定して初期化する。
	m_flame_waterSE->Init(L"Assets/sound/flame_water.wav");
	//音量調節
	m_flame_waterSE->SetVolume(1.0f);

	return true;
}

//デストラクタ
OObigFire::~OObigFire()
{
	DeleteGO(m_pointLight);


	if (m_flameSE->IsPlaying()) {
		DeleteGO(m_flameSE);
	}

	if (m_flame_water->IsPlay()) {
		DeleteGO(m_flame_water);
	}

}

//アップデート関数
void OObigFire::UpdateSub()
{
	Burn();

	if (GetFlagIsHit())
		m_pointLight->SetColor({ 500.0f,1.0f,1.0f,1.0f });
	else
		m_pointLight->SetColor({ 0.0f,0.0f,0.0f,1.0f });
}

//ダメージを受ける
void OObigFire::Damage()
{
	if (--m_hp <= 0)
	{
		//HPが0以下だったら、このオブジェクトを消去
		DeleteGO(this);
	}
	else
	{
		//OBBのサイズをHPに応じて小さくする
		Vector3 obbSize;
		obbSize = { 300.0f,300.0f,100.0f };
		//でかい火の大きさ補正
		//大きさを1/3にすると、小さすぎてゴールできちゃうから、
		//大きさを5/5→4/5→3/5→0と変化させる
		const int sizeComplement = 3;

		//m_hpとm_maxHpはint型。
		//int型同士の割り算だから小数が切り捨てられて
		//評価した値が0になってしまう。
		//だから、m_hpをfloatにキャストする
		obbSize.Scale(static_cast<float>(m_hp + sizeComplement) / (m_maxHp + sizeComplement));
		GetOBB().SetDirectionLength(obbSize);


		//モデルのサイズをHPに応じて小さくする
			//これが上のみたいに切り捨てにならない理由は
			//m_hp / m_maxHpより先にg_vec3One * m_hpが呼ばれ
			//Vector3の演算子のオーバーライドの引数がfloat型だから
			//m_hpが渡されるときに強制型変換が起きてfloat型になるから
			//これがSetScale(g_vec3One * (m_hp / m_maxHp))
			//だったらうまくいかない。
		SetScale(g_vec3One * (m_hp + sizeComplement) / (m_maxHp + sizeComplement));
	}
	m_flame_water->Play();								//再生
	m_flame_waterSE->Play(false);						//再生
}	

//プレイヤーが炎に近づくと燃えてる音を出す
void OObigFire::Burn()
{
	Vector3 distance = m_position - m_pPlayer->GetPosition();
	const float MaxDist = 700;
	const float DistLen = distance.Length();

	if (DistLen < MaxDist) {
		float Le = MaxDist - DistLen;
		float SubLe = Le / MaxDist;
		float Vo = 2.0f * SubLe;
		
		//flameSEをループ再生をオンで再生する。
		m_flameSE->Play(true);
		
		//音量調節
		m_flameSE->SetVolume(Vo);
		
	}
	else {
		if (m_flameSE->IsPlaying()) {
			m_flameSE->Stop();
		}
	}
}