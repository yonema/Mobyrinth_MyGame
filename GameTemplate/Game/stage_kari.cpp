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

	//�v���C���[�̃|�C���^
	Player_kari* pPlayer;

	//�E�F�C�|�C���g�́u�ꏊ�v���i�[����}�b�v
	std::map<int, Vector3> posMap;
	//�E�F�C�|�C���g�́u��]�v���i�[����}�b�v
	std::map<int, Quaternion> rotMap;
	//�E�F�C�|�C���g�̐�
	std::size_t vecSize = 0;



	//���x���̓ǂݍ���
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

			//�I�u�W�F�N�g�l�[����"waypoint"����������
			else if (std::wcsstr(objData.name, L"waypoint") != NULL)
			{
				//�ԍ��i"0"�j�̕����񂪂���A�h���X��Ԃ�
				std::wstring buff = std::wcsstr(objData.name, L"0");
				//wstring��int�ɕϊ�
				int num = _wtoi(buff.c_str());
				//�}�b�v�ɓ����
				posMap.insert(std::make_pair(num, objData.position));
				rotMap.insert(std::make_pair(num, objData.rotation));
				//�E�F�C�|�C���g�̐������Z
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

	//�E�F�C�|�C���g���v���C���[�ɐݒ肷��
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

