#include "stdafx.h"
#include "StageBase.h"
#include "Title.h"

bool IStageBase::Start()
{
	//�f�B���N�V�������C�g�̍쐬
	m_stageDirectionLight = NewGO<CDirectionLight>(0);
	m_stageDirectionLight->SetDirection({ 1.0f,1.0f,-1.0f });
	m_stageDirectionLight->SetColor({ 0.1f,0.1f,0.1f,1.0f });

	//�X�e�[�W�J�n���̉��o�̍쐬
	m_startDirecting = NewGO<StartDirecting>(0, "StartDirecting");

	//�Q�[���J�����̍쐬
	NewGO<GameCamera>(0, "GameCamera");

	//�|�[�Y��ʗp�N���X�̍쐬
	m_pause = NewGO<CPause>(0, "Pause");

	return StartSub();

}

void IStageBase::LoadLevel(const char* tklFilePath)
{

	//�E�F�C�|�C���g�́u�ꏊ�v���i�[����}�b�v
	std::map<int, Vector3> posMap;
	//�E�F�C�|�C���g�́u��]�v���i�[����}�b�v
	std::map<int, Quaternion> rotMap;
	//�E�F�C�|�C���g�̐�
	std::size_t vecSize = 0;

	//�v���C���[�̃|�C���^
	Player* pPlayer;

	//���x�������[�h����
	m_level.Init(tklFilePath, [&](LevelObjectData& objData)
		{//���[�h���郌�x�����ɃN�G�����s��

			///
			///
			/// �u��{�I�u�W�F�N�g�v
			
			//�I�u�W�F�N�g�l�[����"player_kari"�Ɠ�����������
			if (objData.EqualObjectName(L"player_kari") == true)
			{
				//�v���C���[���쐬����
				pPlayer = NewGO<Player>(0, "Player");
				//�v���C���[�̃|�W�V���������[�h�����I�u�W�F�N�g�Ɠ����|�W�V�����ɂ���
				pPlayer->SetPosition(objData.position);
				//�t�b�N�������߁Atrue��Ԃ��B
				return true;
			}
			else if (objData.EqualObjectName(L"Mobius") == true)
			{
				Mobius* mobius;
				mobius = NewGO<Mobius>(0, "Mobius");
				mobius->SetPosition(objData.position);
				mobius->SetRotation(objData.rotation);
				return true;
			}
		

			///
			///
			/// �u���]�I�u�W�F�N�g�vReversibleObject
			 
			////////////////////////////////////////////////////////////
			// ���]�I�u�W�F�N�g���쐬���A�����ɒǉ�
			////////////////////////////////////////////////////////////

			//mizu_kori
			else if (objData.EqualObjectName(L"mizu") == true)
			{
				ROmizu_kori* RObject;
				RObject = NewGO<ROmizu_kori>(0, "mizu_kori");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enFront);
				return true;
			}
			else if (objData.EqualObjectName(L"koori") == true)
			{
				ROmizu_kori* RObject;
				RObject = NewGO<ROmizu_kori>(0, "mizu_kori");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enBack);
				return true;
			}
			//bird_fish
			else if (objData.EqualObjectName(L"bird") == true)
			{
				RObird_fish* RObject;
				RObject = NewGO<RObird_fish>(0, "bird_fish");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enFront);
				return true;
			}
			else if (objData.EqualObjectName(L"fish") == true)
			{
				RObird_fish* RObject;
				RObject = NewGO<RObird_fish>(0, "bird_fish");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enBack);
				return true;
			}
			//running_stop
			else if (objData.EqualObjectName(L"kadou") == true)
			{
				ROrunning_stop* RObject;
				RObject = NewGO<ROrunning_stop>(0, "running_stop");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enFront);
				return true;
			}
			else if (objData.EqualObjectName(L"teishi") == true)
			{
				ROrunning_stop* RObject;
				RObject = NewGO<ROrunning_stop>(0, "running_stop");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enBack);
				return true;
			}
			//wire_string
			else if (objData.EqualObjectName(L"wire") == true)
			{
				ROwire_string* RObject;
				RObject = NewGO<ROwire_string>(0, "wire_string");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enFront);
				return true;
			}
			else if (objData.EqualObjectName(L"string") == true)
			{
				ROwire_string* RObject;
				RObject = NewGO<ROwire_string>(0, "wire_string");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enBack);
				return true;
			}
			//nail_bar
			else if (objData.EqualObjectName(L"nail") == true)
			{
				ROnail_bar* RObject;
				RObject = NewGO<ROnail_bar>(0, "nail_bar");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enFront);
				return true;
			}
			else if (objData.EqualObjectName(L"bar") == true)
			{
				ROnail_bar* RObject;
				RObject = NewGO<ROnail_bar>(0, "nail_bar");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enBack);
				return true;
			}
			//axe_pickaxe
			else if (objData.EqualObjectName(L"axe") == true)
			{
				ROaxe_pickaxe* RObject;
				RObject = NewGO<ROaxe_pickaxe>(0, "axe_pickaxe");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enFront);
				return true;
			}
			else if (objData.EqualObjectName(L"pickaxe") == true)
			{
				ROaxe_pickaxe* RObject;
				RObject = NewGO<ROaxe_pickaxe>(0, "axe_pickaxe");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enBack);
				return true;
			}
			//keymold_empty
			else if (objData.EqualObjectName(L"key mold") == true)
			{
				ROkeymold_empty* RObject;
				RObject = NewGO<ROkeymold_empty>(0, "keymold_empty");
				RObject->SetPosition(objData.position);
				RObject->SetFrontOrBack(CReversibleObject::enFront);
				return true;
			}

			///
			///
			/// �u��Q�I�u�W�F�N�g�vObstacleObject
			
			////////////////////////////////////////////////////////////
			// ��Q�I�u�W�F�N�g���쐬���A�����ɒǉ�
			////////////////////////////////////////////////////////////

			//goal
			else if (objData.EqualObjectName(L"goal") == true)
			{
				//�S�[���̓����o�ϐ��ŕێ����Ă����B
				m_goal = NewGO<OOgoal>(0, "goal");
				m_goal->SetPosition(objData.position);
				return true;
			}
			//bigFire
			else if (objData.EqualObjectName(L"bigFire") == true)
			{
				OObigFire* OObject;
				OObject = NewGO<OObigFire>(0, "bigFire");
				OObject->SetPosition(objData.position);
				return true;
			}
			//wall
			else if (objData.EqualObjectName(L"wall") == true)
			{
				OOwall* OObject;
				OObject = NewGO<OOwall>(0, "wall");
				OObject->SetPosition(objData.position);
				return true;
			}
			//bigPadlock
			else if (objData.EqualObjectName(L"padlock") == true)
			{
				OOpadlock* OObject;
				OObject = NewGO<OOpadlock>(0, "bigPadlock");
				OObject->SetPosition(objData.position);
				return true;
			}
			//box
			else if (objData.EqualObjectName(L"box") == true)
			{
				OObox* OObject;
				OObject = NewGO<OObox>(0, "box");
				OObject->SetPosition(objData.position);
				return true;
			}


			///
			///
			/// �u�E�F�C�|�C���g�v

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


				//�f�o�b�N�p
				//��ŏ���
				CModelRender* dbgModel;
				dbgModel = NewGO<CModelRender>(0, "waypoint");
				dbgModel->Init("Assets/modelData/yuka.tkm");
				dbgModel->SetPosition(objData.position);
				dbgModel->SetRotation(objData.rotation);


				return true;
			}
			return false;
		});


	//�v���C���[���}�l�[�W���[�ɓo�^
	CLevelObjectManager::GetInstance()->SetPlayer(pPlayer);
	//���[�h�������x���ɂ������E�F�C�|�C���g���}�l�[�W���[�ɓo�^����
	CLevelObjectManager::GetInstance()->InitWayPointPos(vecSize, posMap);
	CLevelObjectManager::GetInstance()->InitWayPointRot(vecSize, rotMap);

	//�E�F�C�|�C���g���v���C���[�ɐݒ肷��
	pPlayer->SetWayPointPos
	(vecSize, CLevelObjectManager::GetInstance()->GetWayPointPos());
	pPlayer->SetWayPointRot
	(vecSize, CLevelObjectManager::GetInstance()->GetWayPointRot());


	//�X�e�[�W�J�n���̉��o���̃J�����̒����_�̍��W��ݒ肷��
	m_startDirecting->SetPosition(pPlayer->GetPosition());
	//�E�F�C�|�C���g���X�e�[�W�J�n���̉��o�ɐݒ肷��
	m_startDirecting->SetWayPointPos
	(vecSize, CLevelObjectManager::GetInstance()->GetWayPointPos());
	m_startDirecting->SetWayPointRot
	(vecSize, CLevelObjectManager::GetInstance()->GetWayPointRot());


	return;
}

IStageBase::~IStageBase()
{
	//�P�̂̃I�u�W�F�N�g������
	DeleteGO(m_stageDirectionLight);
	DeleteGO(FindGO<GameCamera>("GameCamera"));
	DeleteGO(m_pause);

	//���x���Ń��[�h�����I�u�W�F�N�g������

	////////////////////////////////////////////////////////////
	// �V�����I�u�W�F�N�g���쐬���A������DeleteGO���邱��
	////////////////////////////////////////////////////////////

	//�u��{�I�u�W�F�N�g�v
	QueryGOs<Player>("Player", [&](Player* player)->bool
		{
			DeleteGO(player);
			return true;
		}
	);
	QueryGOs<Mobius>("Mobius", [&](Mobius* mobius)->bool
		{
			DeleteGO(mobius);
			return true;
		}
	);
	QueryGOs<StartDirecting>("StartDirecting", [&](StartDirecting* startDirecting)-> bool
		{
			DeleteGO(startDirecting);
			return true;
		}
	);
	//�u���]�I�u�W�F�N�g�vReversibleObject
	QueryGOs<ROmizu_kori>("mizu_kori", [&](ROmizu_kori* RObject)->bool
		{
			DeleteGO(RObject);
			return true;
		}
	);
	QueryGOs<RObird_fish>("bird_fish", [&](RObird_fish* RObject)->bool
		{
			DeleteGO(RObject);
			return true;
		}
	);
	QueryGOs<ROrunning_stop>("running_stop", [&](ROrunning_stop* RObject)->bool
		{
			DeleteGO(RObject);
			return true;
		}
	);
	QueryGOs<ROwire_string>("wire_string", [&](ROwire_string* RObject)->bool
		{
			DeleteGO(RObject);
			return true;
		}
	);
	QueryGOs<ROnail_bar>("nail_bar", [&](ROnail_bar* RObject)->bool
		{
			DeleteGO(RObject);
			return true;
		}
	);
	QueryGOs<ROaxe_pickaxe>("axe_pickaxe", [&](ROaxe_pickaxe* RObject)->bool
		{
			DeleteGO(RObject);
			return true;
		}
	);
	QueryGOs<ROkeymold_empty>("keymold_empty", [&](ROkeymold_empty* RObject)->bool
		{
			DeleteGO(RObject);
			return true;
		}
	);
	QueryGOs<ROkey_padlock>("key_padlock", [&](ROkey_padlock* RObject)->bool
		{
			DeleteGO(RObject);
			return true;
		}
	);
	//�u��Q�I�u�W�F�N�g�vObstacleObject
	QueryGOs<OOgoal>("goal", [&](OOgoal* OObject)->bool
		{
			DeleteGO(OObject);
			return true;
		}
	);
	QueryGOs<OObigFire>("bigFire", [&](OObigFire* OObject)->bool
		{
			DeleteGO(OObject);
			return true;
		}
	);
	QueryGOs<OOwall>("wall", [&](OOwall* OObject)->bool
		{
			DeleteGO(OObject);
			return true;
		}
	);
	QueryGOs<OOpadlock>("bigPadlock", [&](OOpadlock* OObject)->bool
		{
			DeleteGO(OObject);
			return true;
		}
	);
	QueryGOs<OObox>("box", [&](OObox* OObject)->bool
		{
			DeleteGO(OObject);
			return true;
		}
	);



	//�f�o�b�N�p
	//��ŏ���
	QueryGOs<CModelRender>("waypoint", [&](CModelRender* waypoint)->bool
		{
			DeleteGO(waypoint);
			return true;
		}
	);
}


void IStageBase::Update()
{
	if (m_goal)
	{
		if (m_goal->GetIsGoal())
		{
			Goal();

			m_pause->SetCanPause(false);
		}
	}

	return;
}
void IStageBase::UpdateOnlyPaused()
{
	if (m_pause->GetRetryFlag())
	{
		Retry();
	}
	else if (m_pause->GetQuitFlag())
	{
		Quit();
	}
}

void IStageBase::Clear()
{
	NewGO<Title>(0, "Title");
	Release();
}

void IStageBase::Retry()
{
	RetryStage();
	Release();
}

void IStageBase::Quit()
{
	NewGO<Title>(0, "Title");
	Release();
}

void IStageBase::Goal()
{
	m_goalCounter++;

	if (m_goalCounter >= 180)
	{
		Clear();
	}
}