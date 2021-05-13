#include "stdafx.h"

#include "ROmizu_kori.h"

#include "TransparentSwitch.h"

bool OOTransparentSwitch::StartSub()
{
	Init("Assets/modelData/transparentSwitch.tkm", enTransparentSwitch);

	//OBBWorld�Ɏ��g��OBB�̓o�^������������
	COBBWorld::GetInstance()->RemoveOBB(&GetOBB());

	//�����I�u�W�F�N�g�𔼓����ɂ���B
	ChangeTransparent();

	return true;
}

void OOTransparentSwitch::UpdateSub()
{
	//���Z�b�g�^�C�}�[���O�ɂȂ����Ƃ�
	//�����I�u�W�F�N�g�����ׂĎ����グ���Ȃ��悤�ɂ���B
	//�����I�u�W�F�N�g�������Ă���ꍇ�A���̃I�u�W�F�N�g�������Ă��Ȃ���Ԃɂ���B
	//�����I�u�W�F�N�g�������ʒu�ɖ߂��B
	if (m_flagSwitchOn == true) {
		m_resetTimer -= GameTime().GetFrameDeltaTime();
		///--m_resetTimer;

		if (m_resetTimer <= 0.0f) {
			m_flagSwitchOn = false;

			//�����ɓ����I�u�W�F�N�g�̏�����ǉ�����B
			//�z�u���Ă��邷�ׂẴ��x���I�u�W�F�N�g�̎Q�Ƃ̃x�N�^�[
			//std::vector<ILevelObjectBase*> levelObjects =
			//	CLevelObjectManager::GetInstance()->GetLevelObjects();

			////���x���I�u�W�F�N�g������������o��
			//for (auto lo : levelObjects)
			//{
			//	//�������g�̎��̓X�L�b�v
			//	if (lo == this)
			//		continue;

			//	//�����I�u�W�F�N�g�̏ꍇ
			//	if (GetFlagTransparentObject() == true)
			//	{
			//		//�����ɃI�u�W�F�N�g�ɑ΂��鏈��
			//		lo->TransparentSwitchOff();

			//		//�I�u�W�F�N�g�������Ă���ꍇ
			//		lo->SetFlagHeldPlayer(false);
			//	}
			//}

			//�����I�u�W�F�N�g�𔼓����ɂ���B
			ChangeTransparent();


		}
	}
	//���Z�b�g�^�C�}�[���O�̂Ƃ��ɉ��̕��̏������쓮������B
	else if (m_flagSwitchOn == false) {
		//�X�C�b�`�������ꂽ�Ƃ�
		//�����I�u�W�F�N�g�����ׂĎ����グ����悤�ɂ���B
		//�X�C�b�`�̃I�u�W�F�N�g�͈͓̔���A�{�^���������ꂽ�Ƃ�
		if (IsHitPlayer() && g_pad[0]->IsTrigger(enButtonA)) {
			m_flagSwitchOn = true;
			//���Z�b�g�^�C�}�[�ɊJ�n����l����
			m_resetTimer = m_resetTimerStartValue;

			//�����ɓ����I�u�W�F�N�g�̏�����ǉ�����B
			//�z�u���Ă��邷�ׂẴ��x���I�u�W�F�N�g�̎Q�Ƃ̃x�N�^�[
			//std::vector<ILevelObjectBase*> levelObjects =
				//CLevelObjectManager::GetInstance()->GetLevelObjects();

			//���x���I�u�W�F�N�g������������o��
			//for (auto lo : levelObjects)
			//{
				//�������g�̎��̓X�L�b�v
				//if (lo == this)
					//continue;

				//�����I�u�W�F�N�g�̏ꍇ
				//if (GetFlagTransparentObject() == true)
				//{
				//	//�����ɃI�u�W�F�N�g�ɑ΂��鏈��
				//	lo->TransparentSwitchOn();

				//	//�I�u�W�F�N�g�������グ����悤�ɂ���B
				//	//lo->SetFlagHeld(true);
				//	//�I�u�W�F�N�g�̏Փ˔�����s���悤�ɂ���B
				//	//lo->SetFlagIsHit(true);
				//}
			//}

			//�����I�u�W�F�N�g�����̂ɂ���B
			ChangeEntity();

		}
	}
}