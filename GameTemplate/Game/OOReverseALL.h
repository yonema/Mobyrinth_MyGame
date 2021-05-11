#pragma once
#include "ObstacleObject.h"
#include "ReversibleObject.h"
#include "FontRender.h"

/// <summary>
/// ��Q�I�u�W�F�N�g
/// �S���]�I�u�W�F�N�g
/// </summary>
class OOReverseALL : public CObstacleObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool StartSub() override final;		//�X�^�[�g�֐�
	~OOReverseALL();					//�f�X�g���N�^
	void UpdateSub() override final;	//�A�b�v�f�[�g�֐�

private:	//private�ȃ����o�֐�

	void BeforeHitPlayer();	//�v���C���[�ƏՓˑO�̏���

	void HitPlayer();		//�v���C���[�ƏՓˎ��̏���

	void AfterHitPlayer();	//�v���C���[�ƏՓˌ�̏���


private:	//�񋓌^

	//�A�b�v�f�[�g�X�e�[�g
	enum EnUpdateState
	{
		enBeforHitPlayer,	//�v���C���[�ƏՓˑO�̏��
		enHitPlayer,		//�v���C���[�ƏՓˎ��̏��
		enAfterHitPlayer,	//�v���C���[�ƏՓˌ�̏��
	};

private:	//�f�[�^�����o

	int m_updateState = enBeforHitPlayer;			//�A�b�v�f�[�g�X�e�[�g
	Vector3 m_playerBeforePosition = g_vec3Zero;	//�ՓˑO�̃v���C���[�̍��W
	Vector3 m_playerHitPosition = g_vec3Zero;		//�Փˎ��̃v���C���[�̍��W
	Vector3 m_playerAfterPosition = g_vec3Zero;		//�Փˌ�̃v���C���[�̍��W
	CSoundCue* m_changeSE = nullptr; //m_changeSE�̃T�E���h�L���[

private:	//�f�o�b�N�p

#ifdef MY_DEBUG
//��ŏ���
	static const int m_vertNum = 8;		//���_�̐�
	CModelRender* m_dbgVertPosMR[m_vertNum] = { nullptr };	//���_�����邽�߂̃��f��
#endif
};

