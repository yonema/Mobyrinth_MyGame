#include "stdafx.h"
#include "ROmizu_kori.h"

//�X�^�[�g�֐�
bool ROmizu_kori::StartSub()
{
	//�������p�֐�
	Init("Assets/modelData/water.tkm", EN_RO_TYPE_WATER,
		"Assets/modelData/fire.tkm", EN_RO_TYPE_FIRE);


	return true;
}

/// <summary>
/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
/// </summary>
void ROmizu_kori::QuerySub()
{
	//���g���u���v�̎�
	if (GetObjectType() == EN_RO_TYPE_WATER)
	{
		//��Q�I�u�W�F�N�g�́u�ł����΁v���N�G��
		QueryLOs<OObigFire>(EN_OO_TYPE_FLAME, [&](OObigFire* bigFire) -> bool
			{
				if (m_actionFlag)
				{
					//���g�Ɓu�ł����΁v���Փ˂�����
					if (IsHitObject(*this, *bigFire))
					{
						//�u�ł����΁v�Ƀ_���[�W��^����
						bigFire->Damage();
						//���g�͔j������
						DeleteGO(this);
						m_actionFlag = false;
					}
				}
				//true��߂�
				return true;
			}
		);
		//���]�I�u�W�F�N�g�́u�΁v���N�G��
		QueryLOs<ROmizu_kori>(EN_RO_TYPE_FIRE, [&](ROmizu_kori* Fire) -> bool
			{
				//���g�Ɓu�΁v���Փ˂�����
				if (IsHitObject(*this, *Fire))
				{
					if (m_actionFlag) {
						//�����j������B
						DeleteGO(Fire);
						//���g�͔j������
						DeleteGO(this);
						//�s���ł��Ȃ�����
						m_actionFlag = false;
					}
				}
				//true��߂�
				return true;
			}
		);
	}
	//���g���u�΁v�̎�
	else if (GetObjectType() == EN_RO_TYPE_FIRE)
	{
		//���]�I�u�W�F�N�g�́u���v���N�G��
		QueryLOs<ROmizu_kori>(EN_RO_TYPE_WATER, [&](ROmizu_kori* Water) -> bool
			{
				//���g�Ɓu���v���Փ˂�����
				if (IsHitObject(*this, *Water))
				{
					if (m_actionFlag) {
						//�����j������B
						DeleteGO(Water);
						//���g�͔j������
						DeleteGO(this);
						//�s���ł��Ȃ�����
						m_actionFlag = false;
					}
				}
				//true��߂�
				return true;
			}
		);
	}
}