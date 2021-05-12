#include "stdafx.h"
#include "ROaxe_pickaxe.h"

//�X�^�[�g�֐�
bool ROaxe_pickaxe::StartSub()
{
	//�������p�֐�
	Init("Assets/modelData/axe.tkm", enAxe,
		"Assets/modelData/pickaxe.tkm", enPickaxe);

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
void ROaxe_pickaxe::QuerySub()
{
	//���g���u���v�̎�
	if (GetObjectType() == enAxe)
	{
		//��Q�I�u�W�F�N�g�́u���v���N�G��
		QueryLOs<OObox>(enBox, [&](OObox* box) -> bool
			{
				//���g�Ɓu���v���Փ˂�����
				if (IsHitObject(*this, *box))
				{
					//�u���v��j������
					DeleteGO(box);
					//���g��j������
					DeleteGO(this);
				}
				return true;
			}
		);
	}
	//���g���u��͂��v�̎�
	else if (GetObjectType() == enPickaxe)
	{
		//��Q�I�u�W�F�N�g�́u���v���N�G��
		QueryLOs<OObox>(enBox, [&](OObox* box) -> bool
			{
				//���g�Ɓu���v���Փ˂�����
				if (IsHitObject(*this, *box))
				{
					//�u���v��j������
					DeleteGO(box);
					//���g��j������
					DeleteGO(this);
				}
				return true;
			}
		);
	}
}