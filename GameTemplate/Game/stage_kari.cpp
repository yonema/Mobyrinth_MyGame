#include "stdafx.h"
#include "stage_kari.h"
#include "Player_kari.h"
#include "background_kari.h"
#include "DirectionLight.h"
class GameCamera;

bool stage_kari::Start()
{
	CDirectionLight* d1 = NewGO<CDirectionLight>(0, "directionLight1");
	d1->SetDirection({ 1.0f,1.0f,-1.0f });
	d1->SetColor({ 0.1f,0.1f,0.1f,1.0f });

	//プレイヤーのポインタ
	Player_kari* pPlayer;

	//ウェイポイントの「場所」を格納するマップ
	std::map<int, Vector3> posMap;
	//ウェイポイントの「回転」を格納するマップ
	std::map<int, Quaternion> rotMap;
	//ウェイポイントの数
	std::size_t vecSize = 0;



	//レベルの読み込み
	m_level.Init("Assets/level/stage_kari02.tkl", [&](LevelObjectData& objData)
		{
			if (objData.EqualObjectName(L"player_kari") == true)
			{
				pPlayer = NewGO<Player_kari>(0, "player");
				pPlayer->SetPosition
				({ objData.position.x, objData.position.y, objData.position.z });
				return true;
			}
			else if (objData.EqualObjectName(L"Mobius") == true)
			{
				Mobius* mobius;
				mobius = NewGO<Mobius>(0, "Mobius");
				Vector3 pos = objData.position;
				//pos.y += 1500.0f;
				mobius->SetPosition(pos);
				mobius->SetRotation(objData.rotation);
				return true;
			}
			else if (objData.EqualObjectName(L"mizu") == true)
			{
				ROmizu_kori* lObject;
				lObject = NewGO<ROmizu_kori>(0);
				lObject->SetPosition(objData.position);
				lObject->SetFrontOrBack(ROmizu_kori::enFront);
				return true;
			}
			else if (objData.EqualObjectName(L"koori") == true)
			{
				ROmizu_kori* rObject;
				rObject = NewGO<ROmizu_kori>(0);
				rObject->SetPosition(objData.position);
				rObject->SetFrontOrBack(CReversibleObject::enBack);
				return true;
			}
			else if (objData.EqualObjectName(L"bird") == true)
			{
				RObird_fish* lObject;
				lObject = NewGO<RObird_fish>(0);
				lObject->SetPosition(objData.position);
				lObject->SetFrontOrBack(CReversibleObject::enFront);
				return true;
			}
			else if (objData.EqualObjectName(L"fish") == true)
			{
				RObird_fish* lObject;
				lObject = NewGO<RObird_fish>(0);
				lObject->SetPosition(objData.position);
				lObject->SetFrontOrBack(CReversibleObject::enBack);
				return true;
			}
			else if (objData.EqualObjectName(L"bigFire") == true)
			{
				OObigFire* OObject;
				OObject = NewGO<OObigFire>(0);
				OObject->SetPosition(objData.position);
				return true;
			}
			else if (objData.EqualObjectName(L"wall") == true)
			{
				OOwall* OObject;
				OObject = NewGO<OOwall>(0);
				OObject->SetPosition(objData.position);
				return true;
			}
			else if (objData.EqualObjectName(L"goal") == true)
			{
				OOgoal* OObject;
				OObject = NewGO<OOgoal>(0);
				OObject->SetPosition(objData.position);
				return true;
			}
			else if (objData.EqualObjectName(L"kadou") == true)
			{
				ROrunning_stop* lObject;
				lObject = NewGO<ROrunning_stop>(0);
				lObject->SetPosition(objData.position);
				lObject->SetFrontOrBack(CReversibleObject::enFront);
				return true;
			}
			else if (objData.EqualObjectName(L"teishi") == true)
			{
				ROrunning_stop* lObject;
				lObject = NewGO<ROrunning_stop>(0);
				lObject->SetPosition(objData.position);
				lObject->SetFrontOrBack(CReversibleObject::enBack);
				return true;
			}

			//オブジェクトネームに"waypoint"があったら
			else if (std::wcsstr(objData.name, L"waypoint") != NULL)
			{
				//番号（"0"）の文字列があるアドレスを返す
				std::wstring buff = std::wcsstr(objData.name, L"0");
				//wstringをintに変換
				int num = _wtoi(buff.c_str());
				//マップに入れる
				posMap.insert(std::make_pair(num, objData.position));
				rotMap.insert(std::make_pair(num, objData.rotation));
				//ウェイポイントの数を加算
				vecSize++;

				CModelRender* model;
				model = NewGO<CModelRender>(0);
				model->Init("Assets/modelData/yuka.tkm");
				model->SetPosition(objData.position);
				model->SetRotation(objData.rotation);
				return true;
			}

			return false;
		
		});

	CLevelObjectManager::GetInstance()->InitWayPointPos(vecSize, posMap);
	CLevelObjectManager::GetInstance()->InitWayPointRot(vecSize, rotMap);

	//ウェイポイントをプレイヤーに設定する
	pPlayer->SetWayPointPos
	(vecSize, CLevelObjectManager::GetInstance()->GetWayPointPos());
	pPlayer->SetWayPointRot
	(vecSize, CLevelObjectManager::GetInstance()->GetWayPointRot());

	NewGO<GameCamera>(0);


	return true;
}
stage_kari::~stage_kari()
{
	//DeleteGO(m_Player_kari);
	//DeleteGO(m_background_kari);
}
void stage_kari::Update()
{
}
void stage_kari::Render(RenderContext& rc)
{
	//m_level.Draw(rc);
	////フォント
	//Font font;
	//wchar_t text[256];
	//wsprintf(text, L"バンドリ！%d", m_numText);
	//font.Begin(rc);

	//font.Draw(text,				//表示したいテキスト
	//	{ 0.0f,0.0f },		//ポジション
	//	{ 1.0f,1.0f,1.0f,1.0f },	//カラー
	//	0.0f,						//回転
	//	1.0f,						//拡大
	//	{ 0.0f,0.0f }				//ピボット
	//);

	//font.End(rc);

	m_numText++;
	if (m_numText >= 100)
	{
		m_numText = 0;
	}
}

void stage_kari::PostRender(RenderContext& rc)
{
	//フォント
	Font font;
	wchar_t text[256];
	wsprintf(text, L"バンドリ！%02d", m_numText);
	font.Begin(rc);

	font.Draw(text,				//表示したいテキスト
		{ -640.0f,350.0f },		//ポジション
		{ 1.0f,1.0f,1.0f,1.0f },	//カラー
		0.0f,						//回転
		1.0f,						//拡大
		{ 0.0f,0.0f }				//ピボット
	);

	font.End(rc);
}