#pragma once


/**
 * @brief ���O���o�^����Ă���Q�[���I�u�W�F�N�g�̃^�C�v
*/
enum EN_GAME_OBJECT_TYPE
{
	EN_GO_TYPE_GAME_CAMERA,	//CGameCamera�N���X
	EN_GO_TYPE_MOBIUS,		//CMobius�N���X
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
	case EN_GO_TYPE_GAME_CAMERA:
		return "GameCamera";
		break;
	case EN_GO_TYPE_MOBIUS:
		return "Mobius";
		break;
	}

	//�ǂ�ł��Ȃ�������G���[
	MessageBoxA(
		nullptr, "�Q�[���I�u�W�F�N�g�̖��O���o�^����Ă��܂���", "�G���[", MB_OK
	);

	return nullptr;
}
