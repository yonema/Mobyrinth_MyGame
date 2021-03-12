#include "stdafx.h"
#include "stage_proto02.h"

bool stage_proto02::Start()
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
	m_level.Init("Assets/level/stage_proto02.tkl", [&](LevelObjectData& objData)
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
			else if (objData.EqualObjectName(L"goal") == true)
			{
				OOgoal* OObject;
				OObject = NewGO<OOgoal>(0);
				OObject->SetPosition(objData.position);
				return true;
			}

			else if (objData.EqualObjectName(L"wire") == true)
			{
				ROwire_string* lObject;
				lObject = NewGO<ROwire_string>(0);
				lObject->SetPosition(objData.position);
				lObject->SetFrontOrBack(CReversibleObject::enFront);
				return true;
			}
			else if (objData.EqualObjectName(L"string") == true)
			{
				ROwire_string* lObject;
				lObject = NewGO<ROwire_string>(0);
				lObject->SetPosition(objData.position);
				lObject->SetFrontOrBack(CReversibleObject::enBack);
				return true;
			}
			else if (objData.EqualObjectName(L"nail") == true)
			{
				ROnail_bar* lObject;
				lObject = NewGO<ROnail_bar>(0);
				lObject->SetPosition(objData.position);
				lObject->SetFrontOrBack(CReversibleObject::enFront);
				return true;
			}
			else if (objData.EqualObjectName(L"bar") == true)
			{
				ROnail_bar* lObject;
				lObject = NewGO<ROnail_bar>(0);
				lObject->SetPosition(objData.position);
				lObject->SetFrontOrBack(CReversibleObject::enBack);
				return true;
			}
			else if (objData.EqualObjectName(L"axe") == true)
			{
				ROaxe_pickaxe* lObject;
				lObject = NewGO<ROaxe_pickaxe>(0);
				lObject->SetPosition(objData.position);
				lObject->SetFrontOrBack(CReversibleObject::enFront);
				return true;
			}
			else if (objData.EqualObjectName(L"pickaxe") == true)
			{
				ROaxe_pickaxe* lObject;
				lObject = NewGO<ROaxe_pickaxe>(0);
				lObject->SetPosition(objData.position);
				lObject->SetFrontOrBack(CReversibleObject::enBack);
				return true;
			}
			else if (objData.EqualObjectName(L"padlock") == true)
			{
				OOpadlock* OObject;
				OObject = NewGO<OOpadlock>(0);
				OObject->SetPosition(objData.position);
				return true;
			}
			else if (objData.EqualObjectName(L"box") == true)
			{
				OObox* OObject;
				OObject = NewGO<OObox>(0);
				OObject->SetPosition(objData.position);
				return true;
			}
			else if (objData.EqualObjectName(L"key mold") == true)
			{
				ROkeymold_empty* lObject;
				lObject = NewGO<ROkeymold_empty>(0);
				lObject->SetPosition(objData.position);
				lObject->SetFrontOrBack(CReversibleObject::enFront);
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
stage_proto02::~stage_proto02()
{
	//DeleteGO(m_Player_kari);
	//DeleteGO(m_background_kari);
}