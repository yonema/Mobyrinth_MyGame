#include "stdafx.h"
#include "ROwater_fire.h"

//ReversibleObject�̃��f���̃t�@�C���p�X�̒萔�f�[�^���g�p�\�ɂ���
using namespace ROsFilepathConstdata;

//�X�^�[�g�֐�
bool ROwater_fire::StartSub()
{
	//�������p�֐�
	Init(MODEL_FILEPATH_WATER, EN_RO_TYPE_WATER,
		MODEL_FILEPATH_FIRE, EN_RO_TYPE_FIRE);


	return true;
}

/// <summary>
/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
/// </summary>
void ROwater_fire::QuerySub()
{
	//���g���u���v�̎�
	if (GetObjectType() == EN_RO_TYPE_WATER)
	{
		//��Q�I�u�W�F�N�g�́u���v���N�G��
		QueryLOs<OOflame>(EN_OO_TYPE_FLAME, [&](OOflame* flame) -> bool
			{
				//���g���u���v�ŁA�u���v�ƏՓ˂������̏���
				WaterHitFlame(flame);

				//true��߂�
				return true;
			}
		);
		//���]�I�u�W�F�N�g�́u�΁v���N�G��
		QueryLOs<ROwater_fire>(EN_RO_TYPE_FIRE, [&](ROwater_fire* fire) -> bool
			{
				//���g���u���v�ŁA�u�΁v�ƏՓ˂������̏���
				WaterHitFire(fire);

				//true��߂�
				return true;
			}
		);
	}
	//���g���u�΁v�̎�
	else if (GetObjectType() == EN_RO_TYPE_FIRE)
	{
		//���]�I�u�W�F�N�g�́u���v���N�G��
		QueryLOs<ROwater_fire>(EN_RO_TYPE_WATER, [&](ROwater_fire* water) -> bool
			{
				//���g���u�΁v�ŁA�u���v�ƏՓ˂������̏���
				FireHitWater(water);

				//true��߂�
				return true;
			}
		);
	}

	return;
}

/**
 * @brief ���g���u���v�ŁA�u���v�ƏՓ˂������̏���
 * @param flame [in] �u���v�̃|�C���^
*/
void ROwater_fire::WaterHitFlame(OOflame* flame)
{
	//�s���\���H
	if (m_actionFlag)
	{
		//���g�Ɓu���v���Փ˂�����
		if (IsHitLevelObject(*this, *flame))
		{
			//�u���v�Ƀ_���[�W��^����
			flame->Damage();
			//���g�͔j������
			DeleteGO(this);
			//�s���ł��Ȃ�����
			m_actionFlag = false;
		}
	}

	return;
}

/**
 * @brief ���g���u���v�ŁA�u�΁v�ƏՓ˂������̏���
 * @param fire [in] �u�΁v�̃|�C���^
*/
void ROwater_fire::WaterHitFire(ROwater_fire* fire)
{
	//�s���ł��邩�H
	if (m_actionFlag)
	{
		//���g�Ɓu�΁v���Փ˂�����
		if (IsHitLevelObject(*this, *fire))
		{
			//�����j������B
			DeleteGO(fire);
			//���g�͔j������
			DeleteGO(this);
			//�s���ł��Ȃ�����
			m_actionFlag = false;
		}
	}

	return;
}

/**
 * @brief ���g���u�΁v�ŁA�u���v�ƏՓ˂������̏���
 * @param water [in] �u���v�̃|�C���^
*/
void ROwater_fire::FireHitWater(ROwater_fire* water)
{
	//�s���ł��邩�H
	if (m_actionFlag) 
	{
		//���g�Ɓu���v���Փ˂�����
		if (IsHitLevelObject(*this, *water))
		{
			//�����j������B
			DeleteGO(water);
			//���g�͔j������
			DeleteGO(this);
			//�s���ł��Ȃ�����
			m_actionFlag = false;
		}
	}

	return;
}