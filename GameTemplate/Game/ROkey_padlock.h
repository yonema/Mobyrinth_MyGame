#pragma once
#include "ReversibleObject.h"
#include "OOpadlock.h"

/**
 * @brief �u���v�u�싞���v�̒萔�f�[�^
*/
namespace key_padlockConstData
{
	//�싞�����J���Ƃ��̃T�E���h�̃t�@�C���p�X
	constexpr const wchar_t* const SOUND_FILEPATH_BREAK_PADLOCK = L"Assets/sound/padlockbreak.wav";
	//�싞�����J���Ƃ��̃T�E���h�̉���
	constexpr const float SOUND_VOLUME_BREAK_PADLOCK = 1.0f;
}

/// <summary>
/// ���]�I�u�W�F�N�g
/// ���A�싞���I�u�W�F�N�g
/// </summary>
class ROkey_padlock : public CReversibleObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool StartSub() override final;	//�X�^�[�g�֐�

	/// <summary>
	/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
	/// </summary>
	void QuerySub()override final;

private:	//private�ȃ����o�֐�

	/**
	 * @brief �T�E���h�̏���������
	*/
	void InitSound();

	/**
	 * @brief ���g���u���v�ŁA�u�傫�ȓ싞���v�ƏՓ˂������̏���
	 * @param [in] bigPadlock �u�傫�ȓ싞���v�̃|�C���^
	*/
	void KeyHitBigPadlock(OOpadlock* bigPadlock);

private:	//�f�[�^�����o
	CSoundCue* m_padlockbreakSE = nullptr;	//�싞�����J���Ƃ��̃T�E���h
	bool m_actionFlag = true;				//�s���ł��邩�H
};

