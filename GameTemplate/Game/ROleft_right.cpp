#include "stdafx.h"
#include "ROleft_right.h"

//�X�^�[�g�֐�
bool ROleft_right::StartSub()
{
	//�������p�֐�
	Init("Assets/modelData/left.tkm", enLeftType,
		"Assets/modelData/right.tkm", enRightType);

	//OBB�̃T�C�Y��ݒ�
	Vector3 obbSize;
	obbSize = { 200.0f,200.0f,400.0f };
	//OBB�̕����x�N�g���̒�����ݒ�
	GetOBB().SetDirectionLength(obbSize);

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

void ROleft_right::ObjectReverse()
{
	std::vector<ILevelObjectBase*> levelObjects =
		CLevelObjectManager::GetInstance()->GetLevelObjects();

	for (auto lo : levelObjects)
	{
		if (lo == this)
			continue;

		if (IsHitObject(*this, *lo))
		{
			if (m_actionFlag)
			{
				float xScale = 1.0f;
				if (GetObjectType() == enRightType)
					xScale *= -1.0f;
				lo->SetScale({ xScale,1.0f,1.0f });
				//���g�͔j������
				DeleteGO(this);
				//�s���ł��Ȃ�����
				m_actionFlag = false;
			}
		}
	}
}