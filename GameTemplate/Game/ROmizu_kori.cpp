#include "stdafx.h"
#include "ROmizu_kori.h"

//�X�^�[�g�֐�
bool ROmizu_kori::StartSub()
{
	//�������p�֐�
	Init("Assets/modelData/water.tkm", enWater,
		"Assets/modelData/fire.tkm", enFire);

	//OBB�̃T�C�Y��ݒ�
	Vector3 obbSize;
	obbSize = { 200.0f,200.0f,100.0f };
	//OBB�̕����x�N�g���̒�����ݒ�
	GetOBB().SetDirectionLength(obbSize);

	return true;
}

/// <summary>
/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
/// </summary>
void ROmizu_kori::QuerySub()
{
	//���g���u���v�̎�
	if (GetObjectType() == enWater)
	{
		//��Q�I�u�W�F�N�g�́u�ł����΁v���N�G��
		QueryLOs<OObigFire>(enBigFire, [&](OObigFire* bigFire) -> bool
			{
				//���g�Ɓu�ł����΁v���Փ˂�����
				if (IsHitObject(*this, *bigFire))
				{
					//�u�ł����΁v�Ƀ_���[�W��^����
					bigFire->Damage();
					//���g�͔j������
					DeleteGO(this);
				}
				//true��߂�
				return true;
			}
		);
	}
	//���g���u�΁v�̎�
	else if (GetObjectType() == enFire)
	{

	}
}