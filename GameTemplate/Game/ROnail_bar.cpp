#include "stdafx.h"
#include "ROnail_bar.h"

//�X�^�[�g�֐�
bool ROnail_bar::StartSub()
{
	//�������p�֐�
	Init("Assets/modelData/nail.tkm", enNail,
		"Assets/modelData/hammer.tkm", enBar);



	return true;
}

/// <summary>
/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
/// </summary>
void ROnail_bar::QuerySub()
{
	//���g���u�B�v�̎�
	if (GetObjectType() == enNail)
	{

	}
	//���g���u�o�[���v�̎�
	else if (GetObjectType() == enBar)
	{
		//�u���ĂȂ��싞���v�Ɓu���v���ƁA�u���ĂȂ��싞���v�̕����D��I�ɔj�����邩��
		//�u���ĂȂ��싞���v���ɃN�G������
		//��Q�I�u�W�F�N�g�́u���ĂȂ��싞���v���N�G��
		QueryLOs<OOpadlock>(enNotHavePadlock, [&](OOpadlock* padlock) -> bool
			{
				//���g�Ɓu���ĂȂ��싞���v���Փ˂�����
				if (IsHitObject(*this, *padlock))
				{
					//�s���ł�����
					if (m_actionFlag)
					{
						//�u���ĂȂ��싞���v��j��
						DeleteGO(padlock);
						//���g�̃I�u�W�F�N�g��j��
						DeleteGO(this);
						//�s���ł��Ȃ�����
						m_actionFlag = false;
					}
				}
				return true;
			}
		);
		//��Q�I�u�W�F�N�g�́u���v���N�G��
		QueryLOs<OObox>(enBox, [&](OObox* box) -> bool
			{
				//���g�Ɓu���v���Փ˂�����
				if (IsHitObject(*this, *box))
				{
					//�s���ł�����
					if (m_actionFlag)
					{
						//�u���v��j��
						DeleteGO(box);
						//���g�̃I�u�W�F�N�g��j��
						DeleteGO(this);
						//�s���ł��Ȃ��悤�ɂ���
						m_actionFlag = false;
					}
				}
				return true;
			}
		);

	}
}