#include "stdafx.h"

#include "ROmizu_kori.h"

#include "TransparentSwitch.h"

bool OOTransparentSwitch::StartSub()
{
	//OBBWorld�Ɏ��g��OBB�̓o�^������������
	COBBWorld::GetInstance()->RemoveOBB(&GetOBB());

	return true;
}

void OOTransparentSwitch::UpdateSub()
{
	//���Z�b�g�^�C�}�[���O�ɂȂ����Ƃ�
	//�����I�u�W�F�N�g�����ׂĎ����グ���Ȃ��悤�ɂ���B
	//�����I�u�W�F�N�g�������Ă���ꍇ�A���̃I�u�W�F�N�g�������Ă��Ȃ���Ԃɂ���B
	//�����I�u�W�F�N�g�������ʒu�ɖ߂��B
	if (m_flagSwitchOn == true) {
		--m_resetTimer;

		if (m_resetTimer <= 0) {
			m_flagSwitchOn = false;

			//�����ɓ����I�u�W�F�N�g�̏�����ǉ�����B
		




		}
	}



	//���Z�b�g�^�C�}�[���O�̂Ƃ��ɉ��̕��̏������쓮������B
	if (m_flagSwitchOn == false) {
		//�X�C�b�`�������ꂽ�Ƃ�
		//�����I�u�W�F�N�g�����ׂĎ����グ����悤�ɂ���B
		//�X�C�b�`�̃I�u�W�F�N�g�͈͓̔���A�{�^���������ꂽ�Ƃ�
		if (IsHitPlayer() && g_pad[0]->IsTrigger(enButtonA)) {
			m_flagSwitchOn = true;
			//���Z�b�g�^�C�}�[�ɊJ�n����l����
			m_resetTimer = m_resetTimerStartValue;

			//�����ɓ����I�u�W�F�N�g�̏�����ǉ�����B





		}
	}
}