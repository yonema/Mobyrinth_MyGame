#include "stdafx.h"
#include "ROleft_right.h"

//�X�^�[�g�֐�
bool ROleft_right::StartSub()
{
	//�������p�֐�
	Init("Assets/modelData/left.tkm", EN_RO_TYPE_LEFT,
		"Assets/modelData/right.tkm", EN_RO_TYPE_RIGHT);


	return true;
}

/// <summary>
/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
/// </summary>
void ROleft_right::QuerySub()
{
	//���g���u���v�̎�
	if (GetObjectType() == EN_RO_TYPE_LEFT)
	{

		//���E���]�����邩�H
		bool reverseFlag = true;

		//��Q�I�u�W�F�N�g�́u����ʍs�v���N�G��
		QueryLOs<OOoneway>(EN_OO_TYPE_ONEWAY, [&](OOoneway* oneway) -> bool
			{
				//���g�Ɓu����ʍs�v���Փ˂�����
				if (IsHitObject(*this, *oneway))
				{
					//�u����ʍs�v�̌������������ɂ���
					oneway->SetLeftOrRight(OOoneway::enLeft);
					reverseFlag = false;
					//���g�͔j������
					DeleteGO(this);
					//�s���ł��Ȃ�����
					m_actionFlag = false;
				}

				//true��߂�
				return true;
			}
		);

		if (reverseFlag)
			ObjectReverse();
	}
	//���g���u�E�v�̎�
	else if (GetObjectType() == EN_RO_TYPE_RIGHT)
	{

		//���E���]�����邩�H
		bool reverseFlag = true;

		//��Q�I�u�W�F�N�g�́u����ʍs�v���N�G��
		QueryLOs<OOoneway>(EN_OO_TYPE_ONEWAY, [&](OOoneway* oneway) -> bool
			{
				//���g�Ɓu����ʍs�v���Փ˂�����
				if (IsHitObject(*this, *oneway))
				{
					//�u����ʍs�v�̌������E�����ɂ���
					oneway->SetLeftOrRight(OOoneway::enRight);
					reverseFlag = false;
					//���g�͔j������
					DeleteGO(this);
					//�s���ł��Ȃ�����
					m_actionFlag = false;
				}
				//true��߂�
				return true;
			}
		);

		if (reverseFlag)
			ObjectReverse();
	}
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
		if (IsHitObject(*this, *lo))
		{
			//�Փ˂��Ă�����

			//�s���\���H
			if (m_actionFlag)
			{
				//�\

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
			}
		}
	}
}