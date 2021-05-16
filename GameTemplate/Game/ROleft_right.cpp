#include "stdafx.h"
#include "ROleft_right.h"

//�X�^�[�g�֐�
bool ROleft_right::StartSub()
{
	//�������p�֐�
	Init("Assets/modelData/left.tkm", enLeftType,
		"Assets/modelData/right.tkm", enRightType);


	return true;
}

/// <summary>
/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
/// </summary>
void ROleft_right::QuerySub()
{
	//���g���u���v�̎�
	if (GetObjectType() == enLeftType)
	{

		ObjectReverse();

		//��Q�I�u�W�F�N�g�́u����ʍs�v���N�G��
		QueryLOs<OOoneway>(enOneway, [&](OOoneway* oneway) -> bool
			{
				//���g�Ɓu����ʍs�v���Փ˂�����
				if (IsHitObject(*this, *oneway))
				{
					//�u����ʍs�v�̌������������ɂ���
					oneway->SetLeftOrRight(OOoneway::enLeft);
				}
				//true��߂�
				return true;
			}
		);
	}
	//���g���u�E�v�̎�
	else if (GetObjectType() == enRightType)
	{
		ObjectReverse();

		//��Q�I�u�W�F�N�g�́u����ʍs�v���N�G��
		QueryLOs<OOoneway>(enOneway, [&](OOoneway* oneway) -> bool
			{
				//���g�Ɓu����ʍs�v���Փ˂�����
				if (IsHitObject(*this, *oneway))
				{
					//�u����ʍs�v�̌������E�����ɂ���
					oneway->SetLeftOrRight(OOoneway::enRight);
				}
				//true��߂�
				return true;
			}
		);
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
				float xScale = 1.0f;
				if (GetObjectType() == enRightType)
					//�E�����Ȃ�A���E���]������
					xScale *= -1.0f;
				//�Փ˂����I�u�W�F�N�g�̃X�P�[����ݒ肷��
				lo->SetScale({ xScale,1.0f,1.0f });
				//���g�͔j������
				DeleteGO(this);
				//�s���ł��Ȃ�����
				m_actionFlag = false;
			}
		}
	}
}