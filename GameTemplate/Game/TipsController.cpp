#include "stdafx.h"
#include "TipsController.h"

//�X�^�[�g�֐�
bool CTipsController::Start()
{
	return true;
}

//�A�b�v�f�[�g�֐�
void CTipsController::Update()
{
	//�v���C���[�Ɉ�ԋ߂��I�u�W�F�N�g�̃^�C�v���擾
	int objectType = CLevelObjectManager::GetInstance()->GetNearestObjectType(1000.0f);

	//Tips�ɃZ�b�g����
	m_tips.SetText(objectType);
	
}