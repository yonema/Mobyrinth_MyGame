#include "stdafx.h"
#include "OObigFire.h"


bool OObigFire::StartSub()
{
	//モデルの初期化とタイプの設定
	Init("Assets/modelData/bigFire.tkm", enBigFire);

	//OBBのサイズを設定
	Vector3 obbSize;
	obbSize = { 300.0f,300.0f,100.0f };
	GetOBB().SetDirectionLength(obbSize);

	//ポイントライトを生成
	m_pointLight = NewGO<CPointLight>(0);
	Vector3 ptLigPos = m_position;
	Vector3 up = g_vec3Up;
	m_rotation.Apply(up);
	up.Scale(150.0f);
	ptLigPos += up;
	m_pointLight->SetPosition(m_position);
	m_pointLight->SetColor({ 1000.0f,1.0f,1.0f,1.0f });
	m_pointLight->SetRange(1000.0f);

	//自己発光色を設定
	SetEmissionColor({ 1.0f,0.0f,0.0f,1.0f });

	return true;
}

OObigFire::~OObigFire()
{
	DeleteGO(m_pointLight);
}


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
			//m_hpとm_maxHpはint型。
			//int型同士の割り算だから小数が切り捨てられて
			//評価した値が0になってしまう。
			//だから、m_hpをfloatにキャストする
		obbSize.Scale(static_cast<float>(m_hp) / m_maxHp);
		GetOBB().SetDirectionLength(obbSize);

		//モデルのサイズをHPに応じて小さくする
			//これが上のみたいに切り捨てにならない理由は
			//m_hp / m_maxHpより先にg_vec3One * m_hpが呼ばれ
			//Vector3の演算子のオーバーライドの引数がfloat型だから
			//m_hpが渡されるときに強制型変換が起きてfloat型になるから
			//これがSetScale(g_vec3One * (m_hp / m_maxHp))
			//だったらうまくいかない。
		SetScale(g_vec3One * m_hp / m_maxHp);
	}
}