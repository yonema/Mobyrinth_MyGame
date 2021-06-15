#pragma once

//�J���̖��O
const char* const NAME_EMPTY = "";

/**
 * @brief ���O���o�^����Ă���Q�[���I�u�W�F�N�g�̃^�C�v
*/
enum EN_GAME_OBJECT_TYPE
{
	EN_GO_TYPE_GAME,		//CGame�N���X
	//�Q�[���S�̂�ʂ��Ďg����CDirectionLight�N���X
	EN_GO_TYPE_GAME_DIRECTION_LIGHT,
	EN_GO_TYPE_TITLE,		//CTitle�N���X
	EN_GO_TYPE_START_DIRECTING,	//CStartDirecting�N���X
	EN_GO_TYPE_PAUSE,		//CPause�N���X
	EN_GO_TYPE_GAME_CAMERA,	//CGameCamera�N���X
	EN_GO_TYPE_MOBIUS,		//CMobius�N���X
	EN_GO_TYPE_PLAYER,		//CPlayer�N���X
	EN_GO_TYPE_UFO,			//CUFO�N���X
};


/**
 * @brief �Q�[���I�u�W�F�N�g�̖��O���擾
 * @param [in] gameObjectType �Q�[���I�u�W�F�N�g�̃^�C�v
 * @return �Q�[���I�u�W�F�N�g�̖��O
*/
constexpr const char* const GetGameObjectName(const int gameObjectType)
{
	//�^�C�v�ɑΉ�����Q�[���I�u�W�F�N�g�̖��O��߂�
	switch (gameObjectType)
	{
	case EN_GO_TYPE_GAME:
		return "Game";
		break;
	case EN_GO_TYPE_GAME_DIRECTION_LIGHT:
		return "GameDirectionLight";
		break;
	case EN_GO_TYPE_TITLE:
		return "Title";
		break;
	case EN_GO_TYPE_START_DIRECTING:
		return "StartDirecting";
		break;
	case EN_GO_TYPE_PAUSE:
		return "Pause";
		break;
	case EN_GO_TYPE_GAME_CAMERA:
		return "GameCamera";
		break;
	case EN_GO_TYPE_MOBIUS:
		return "Mobius";
		break;
	case EN_GO_TYPE_PLAYER:
		return "Player";
		break;
	case EN_GO_TYPE_UFO:
		return "UFO";
		break;
	}

	//�ǂ�ł��Ȃ�������G���[
	MessageBoxA(
		nullptr, "�Q�[���I�u�W�F�N�g�̖��O���o�^����Ă��܂���", "�G���[", MB_OK
	);

	return nullptr;
}
