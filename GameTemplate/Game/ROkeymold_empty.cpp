#include "stdafx.h"
#include "ROkeymold_empty.h"
#include "ROwire_string.h"

//�X�^�[�g�֐�
bool ROkeymold_empty::StartSub()
{
	//�������p�֐�
	Init("Assets/modelData/key_mold.tkm", enKeymold,
		"Assets/modelData/key_mold.tkm", enKeymold);

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
void ROkeymold_empty::QuerySub()
{
	//���g���u���̋��^�v�̎�
	if (GetObjectType() == enKeymold)
	{
		//���]�I�u�W�F�N�g�́u�j���v���N�G��
		QueryLOs<ROwire_string>(enWire, [&](ROwire_string* wire) -> bool
			{
				//���g�Ɓu�j���v���Փ˂�����
				if (IsHitObject(*this, *wire))
				{
					//�j�������ɕς���
					wire->ChangeToKey();
				}
				return true;
			}
		);
	}
	//else if (GetObjectType() == enKeymold)
	//{
	//	//QueryLOs<OObigFire>(enBigFire, [&](OObigFire* bigFire) -> bool
	//	//	{
	//	//		if (IsHitObject(*this, *bigFire, hitDot))
	//	//		{
	//	//			bigFire->Damage();
	//	//			DeleteGO(this);
	//	//		}
	//	//		return true;
	//	//	}
	//	//);
	//}
}