#include "stdafx.h"
#include "ROwire_string.h"

//�X�^�[�g�֐�
bool ROwire_string::StartSub()
{
	//�������p�֐�
	Init("Assets/modelData/wire.tkm", enWire,
		"Assets/modelData/string.tkm", enString);

	//OBB�̃T�C�Y��ݒ�
	Vector3 obbSize;
	obbSize = { 300.0f,150.0f,100.0f };
	//OBB�̕����x�N�g���̒�����ݒ�
	GetOBB().SetDirectionLength(obbSize);

	return true;
}

/// <summary>
/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
/// </summary>
void ROwire_string::QuerySub()
{
	//���g���u�j���v�̎�
	if (GetObjectType() == enWire)
	{
		//���]�I�u�W�F�N�g�́u���̋��^�v���N�G��
		QueryLOs<ROkeymold_empty>(enKeymold, [&](ROkeymold_empty* keymold) -> bool
			{
				//���g�Ɓu���̋��^�v���Փ˂�����
				if (IsHitObject(*this, *keymold))
				{
					//�u���v�𐶐����Ď��g�̃I�u�W�F�N�g��j������
					ChangeToKey();
				}
				return true;
			}
		);
	}
	//���g���u�R�v�̎�
	else if (GetObjectType() == enString)
	{
		//QueryLOs<OObigFire>(enBigFire, [&](OObigFire* bigFire) -> bool
		//	{
		//		if (IsHitObject(*this, *bigFire, hitDot))
		//		{
		//			bigFire->Damage();
		//			DeleteGO(this);
		//		}
		//		return true;
		//	}
		//);
	}
}

/// <summary>
/// �u���v�𐶐����Ď��g�̃I�u�W�F�N�g��j������
/// </summary>
void ROwire_string::ChangeToKey()
{
	//�u���A�싞���v�𐶐�
	ROkey_padlock* RObjrct;
	RObjrct = NewGO<ROkey_padlock>(0, "key_padlock");
	//�ꏊ��ݒ�
	RObjrct->SetPosition(m_position);
	//�u�\�v��ԁi�u���v��ԁj�ɐݒ�
	RObjrct->SetFrontOrBack(ROkey_padlock::enFront);

	//���g�̃I�u�W�F�N�g��j������
	DeleteGO(this);
}