#include "stdafx.h"
#include "ROkey_padlock.h"

//�X�^�[�g�֐�
bool ROkey_padlock::StartSub()
{
	//�������p�֐�
	Init("Assets/modelData/key.tkm", enKey,
		"Assets/modelData/padlock.tkm", enPadlock);

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
void ROkey_padlock::QuerySub()
{
	//���g���u���̋��^�v�̎�
	if (GetObjectType() == enKey)
	{
		//��Q�I�u�W�F�N�g�́u���ĂȂ��싞���v���N�G��
		QueryLOs<OOpadlock>(enNotHavePadlock, [&](OOpadlock* padlock) -> bool
			{
				//���g�Ɓu���ĂȂ��싞���v���Փ˂�����
				if (IsHitObject(*this, *padlock))
				{
					//�u���ĂȂ��싞���v��j��
					DeleteGO(padlock);
					//���g�̃I�u�W�F�N�g��j��
					DeleteGO(this);
				}
				return true;
			}
		);
	}
	//else if (GetObjectType() == enROPadlock)
	//{
	//	QueryLOs<OObigFire>(enBigFire, [&](OObigFire* bigFire) -> bool
	//		{
	//			if (IsHitObject(*this, *bigFire, hitDot))
	//			{
	//				bigFire->Damage();
	//				DeleteGO(this);
	//			}
	//			return true;
	//		}
	//	);
	//}
}