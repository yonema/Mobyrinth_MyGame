#include "stdafx.h"
#include "TransparentObject.h"

void CTransparentObject::UpdateSub()
{
	//�����Ń��Z�b�g�^�C�}�[����������B
	if (m_resetTimer > 0) {
		--m_resetTimer;

		//�^�C�}�[���O�ɂȂ�����A�I�u�W�F�N�g�������グ���Ȃ����āA�����ʒu�ɖ߂��B
		if (m_resetTimer <= 0) {
			//�I�u�W�F�N�g�������グ���Ȃ�����B
			SetFlagHeld(false);
			//�����ʒu�ɖ߂��B
			m_position = m_startPosition;
			//��]����������Ԃɖ߂��B
			m_rotation = m_startRotation;
		}
	}


}