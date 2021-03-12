#include "stdafx.h"
#include "stage_proto01.h"


bool stage_proto01::Start()
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
	m_level.Init("Assets/level/stage_proto01.tkl", [&](LevelObjectData& objData)
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
				lObject = NewGO<ROmizu_kori>(0, "mizu_koori");
				lObject->SetPosition(objData.position);
				lObject->SetFrontOrBack(ROmizu_kori::enFront);
				return true;
			}
			else if (objData.EqualObjectName(L"koori") == true)
			{
				ROmizu_kori* rObject;
				rObject = NewGO<ROmizu_kori>(0, "mizu_koori");
				rObject->SetPosition(objData.position);
				rObject->SetFrontOrBack(CReversibleObject::enBack);
				return true;
			}
			else if (objData.EqualObjectName(L"bird") == true)
			{
				RObird_fish* lObject;
				lObject = NewGO<RObird_fish>(0, "bird_fish");
				lObject->SetPosition(objData.position);
				lObject->SetFrontOrBack(CReversibleObject::enFront);
				return true;
			}
			else if (objData.EqualObjectName(L"fish") == true)
			{
				RObird_fish* lObject;
				lObject = NewGO<RObird_fish>(0, "bird_fish");
				lObject->SetPosition(objData.position);
				lObject->SetFrontOrBack(CReversibleObject::enBack);
				return true;
			}
			else if (objData.EqualObjectName(L"bigFire") == true)
			{
				OObigFire* OObject;
				OObject = NewGO<OObigFire>(0, "bigFire");
				OObject->SetPosition(objData.position);
				return true;
			}
			else if (objData.EqualObjectName(L"wall") == true)
			{
				OOwall* OObject;
				OObject = NewGO<OOwall>(0, "wall");
				OObject->SetPosition(objData.position);
				return true;
			}
			else if (objData.EqualObjectName(L"goal") == true)
			{
				OOgoal* OObject;
				OObject = NewGO<OOgoal>(0, "goal");
				OObject->SetPosition(objData.position);
				return true;
			}
			else if (objData.EqualObjectName(L"kadou") == true)
			{
				ROrunning_stop* lObject;
				lObject = NewGO<ROrunning_stop>(0, "kadou_teishi");
				lObject->SetPosition(objData.position);
				lObject->SetFrontOrBack(CReversibleObject::enFront);
				return true;
			}
			else if (objData.EqualObjectName(L"teishi") == true)
			{
				ROrunning_stop* lObject;
				lObject = NewGO<ROrunning_stop>(0, "kadou_teishi");
				lObject->SetPosition(objData.position);
				lObject->SetFrontOrBack(CReversibleObject::enBack);
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
				model = NewGO<CModelRender>(0, "waypoint");
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
stage_proto01::~stage_proto01()
{
	QueryGOs<Player_kari>("player", [&](Player_kari* player)->bool
		{
			DeleteGO(FindGO<Player_kari>("player"));
			return true;
		}
	);
	QueryGOs<Mobius>("Mobius", [&](Mobius* player)->bool
		{
			DeleteGO(FindGO<Mobius>("Mobius"));
			return true;
		}
	);
	QueryGOs<ROmizu_kori>("mizu_koori", [&](ROmizu_kori* player)->bool
		{
			DeleteGO(FindGO<ROmizu_kori>("mizu_koori"));
			return true;
		}
	);
	QueryGOs<RObird_fish>("bird_fish", [&](RObird_fish* player)->bool
		{
			DeleteGO(FindGO<RObird_fish>("bird_fish"));
			return true;
		}
	);
	QueryGOs<OObigFire>("bigFire", [&](OObigFire* player)->bool
		{
			DeleteGO(FindGO<OObigFire>("bigFire"));
			return true;
		}
	);
	QueryGOs<OOwall>("wall", [&](OOwall* player)->bool
		{
			DeleteGO(FindGO<OOwall>("wall"));
			return true;
		}
	);
	QueryGOs<OOgoal>("goal", [&](OOgoal* player)->bool
		{
			DeleteGO(FindGO<OOgoal>("goal"));
			return true;
		}
	);
	QueryGOs<ROrunning_stop>("kadou_teishi", [&](ROrunning_stop* player)->bool
		{
			DeleteGO(FindGO<ROrunning_stop>("kadou_teishi"));
			return true;
		}
	);
	QueryGOs<CModelRender>("waypoint", [&](CModelRender* player)->bool
		{
			DeleteGO(FindGO<CModelRender>("waypoint"));
			return true;
		}
	);
}

