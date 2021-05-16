#include "stdafx.h"
#include "ROmizu_kori.h"

//�X�^�[�g�֐�
bool ROmizu_kori::StartSub()
{
	//�������p�֐�
	Init("Assets/modelData/water.tkm", enWater,
		"Assets/modelData/fire.tkm", enFire);


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
		//���]�I�u�W�F�N�g�́u�΁v���N�G��
		QueryLOs<ROmizu_kori>(enFire, [&](ROmizu_kori* Fire) -> bool
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
	else if (GetObjectType() == enFire)
	{
		//���]�I�u�W�F�N�g�́u���v���N�G��
		QueryLOs<ROmizu_kori>(enWater, [&](ROmizu_kori* Water) -> bool
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