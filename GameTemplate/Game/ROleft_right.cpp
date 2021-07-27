#include "stdafx.h"
#include "ROleft_right.h"

//ReversibleObject�̃��f���̃t�@�C���p�X�̒萔�f�[�^���g�p�\�ɂ���
using namespace ROsFilepathConstdata;

//�X�^�[�g�֐�
bool ROleft_right::StartSub()
{
	//�������p�֐�
	Init(MODEL_FILEPATH_LEFT, EN_RO_TYPE_LEFT,
		MODEL_FILEPATH_RIGHT, EN_RO_TYPE_RIGHT);


	return true;
}

/// <summary>
/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
/// </summary>
void ROleft_right::QuerySub()
{

	//��Q�I�u�W�F�N�g�́u����ʍs�v���N�G��
	QueryLOs<OOoneway>(EN_OO_TYPE_ONEWAY, [&](OOoneway* oneway) -> bool
		{
			//�u����ʍs�v�ƏՓ˂������̏���
			HitOneway(oneway);

			//true��߂�
			return true;
		}
	);

	//�s���ł��邩�H
	if (m_actionFlag)
		//���������Ƃ��I�u�W�F�N�g�����E���]������
		ObjectReverse();

	return;
}

/**
 * @brief �u����ʍs�v�ƏՓ˂������̏���
 * @param [in] oneway �u����ʍs�v�̃|�C���^
*/
void ROleft_right::HitOneway(OOoneway* oneway)
{
	//�s���ł��邩�H
	if (m_actionFlag)
	{
		//���g�Ɓu����ʍs�v���Փ˂�����
		if (IsHitLevelObject(*this, *oneway))
		{
			//�E�������H
			//�f�t�H�͍�
			EN_LEFT_OR_RIGHT leftOrRight = EN_LEFT;

			//���g���u�E�v�̎�
			if (GetObjectType() == EN_RO_TYPE_RIGHT)
				//�E�ɂ���
				leftOrRight = EN_RIGHT;

			//�u����ʍs�v�̌������������ɂ���
			oneway->SetLeftOrRight(leftOrRight);
			//�s���ł��Ȃ�����
			m_actionFlag = false;
			//���g�͔j������
			DeleteGO(this);
		}
	}

	return;
}

/// <summary>
/// ���������I�u�W�F�N�g�����E���]������
/// </summary>
void ROleft_right::ObjectReverse()
{
	//�z�u���Ă��邷�ׂẴ��x���I�u�W�F�N�g�̎Q�Ƃ̃x�N�^�[
	std::vector<ILevelObjectBase*> levelObjects =
		CLevelObjectManager::GetInstance()->GetLevelObjects();

	//���x���I�u�W�F�N�g������������o��
	for (auto lo : levelObjects)
	{

		//�������g�̎��̓X�L�b�v
		if (lo == this)
			continue;

		//���g�ƃI�u�W�F�N�g���Փ˂��Ă��邩�H
		if (IsHitLevelObject(*this, *lo))
		{
			//�Փ˂��Ă�����



				//���E���]�������
			Vector3 scale = lo->GetScale();
			if (GetObjectType() == EN_RO_TYPE_RIGHT)
				//�E�����Ȃ�A���E���]������
				scale.x *= -1.0f;
			//�Փ˂����I�u�W�F�N�g�̃X�P�[����ݒ肷��
			lo->SetScale(scale);
			//���g�͔j������
			DeleteGO(this);
			//�s���ł��Ȃ�����
			m_actionFlag = false;

			//����ȏ�s�����Ȃ��ėǂ��̂ŁAreturn����
			return;

		}
	}

	return;
}