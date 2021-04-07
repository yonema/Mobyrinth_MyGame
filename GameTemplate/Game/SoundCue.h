#pragma once
#include "SoundSource.h"

//3D�T�E���h�͎����ĂȂ�
//���Ԃ�ł��Ȃ�


/// <summary>
/// �T�E���h�L���[�B
/// �T�E���h���Đ�����Ƃ��͂��̃N���X���g���ĂˁB
/// �ʂ�CSoundSource���g���Ă��������ǁA�ł���΂������g���Ăق����ȁI
/// </summary>
class CSoundCue : public IGameObject
{
public:		//�����̃����o�֐�����Ɏg��

	/// <summary>
	/// �������p�֐�
	/// wav�t�@�C���̃p�X���w�肵�ď���������
	/// </summary>
	/// <param name="filePath">wav�t�@�C���p�X</param>
	/// <param name="is3DSound">3D�Đ����邩�H�i���Ԃ�ł��Ȃ��j</param>
	void Init(const wchar_t* filePath, const bool is3DSound = false);

	/// <summary>
	/// �T�E���h���Đ�
	/// </summary>
	/// <param name="isLoop">���[�v�Đ����邩�H</param>
	void Play(const bool isLoop);

	/// <summary>
	/// �{�����[����ݒ�
	/// 1.0f���ʏ�B
	/// ���̒l������Ə���ɐ�Βl�ɂȂ邩�璍��
	/// </summary>
	/// <param name="volume">�{�����[��</param>
	void SetVolume(const float volume);

	/// <summary>
	/// �{�����[�����擾
	/// </summary>
	/// <returns>�{�����[��</returns>
	const float GetVolume()const
	{
		return m_volume;
	}

	/// <summary>
	/// �Đ����~����B
	/// ����Œ�~����ƁA���ɍĐ��������A
	/// �ŏ�����̍Đ��ɂȂ�B
	/// ���̎d�l���ƁA���[�v�Đ��͒�~�ł��邪�A
	/// �����V���b�g�Đ��̒�~�͂ł��Ȃ��B
	/// </summary>
	void Stop();

	/// <summary>
	/// �Đ����ꎞ��~����B
	/// ����ňꎞ��~����ƁA���ɍĐ��������A
	/// �r������̍Đ��ɂȂ�B
	/// ���̎d�l���ƁA���[�v�Đ��͈ꎞ��~�ł��邪�A
	/// �����V���b�g�Đ��̈ꎞ��~�͂ł��Ȃ��B
	/// </summary>
	void Pause();

	/// <summary>
	/// ���Đ������ǂ����H
	/// �ꎞ��~�̌��m��IsPaused()���g�����A
	/// ��~�̌��m�ɂ͂���IsPlaying()���g���B
	/// ���̎d�l���ƁA���[�v�Đ��͌��m�ł��邪�A
	/// �����V���b�g�Đ��̌��m�͂ł��Ȃ��B
	/// </summary>
	/// <returns>�Đ������H</returns>
	const bool IsPlaying()const;

	/// <summary>
	/// �ꎞ��~�����ǂ����H
	/// ��~�̌��m��IsPlaying()���g�����A
	/// �ꎞ��~�̌��m�͂���IsPaused()���g���B
	/// ���̎d�l���ƁA���[�v�Đ��͌��m�ł��邪�A
	/// �����V���b�g�Đ��̌��m�͂ł��Ȃ��B
	/// </summary>
	/// <returns>��~�����H</returns>
	const bool IsPaused()const
	{
		return m_isPaused;
	}


private:	//private�ȃ����o�֐�
	void PlayLoop();	//���[�v�Đ��̏���
	void PlayOneShot();	//�����V���b�g�Đ��̏���

private:	//�f�[�^�����o
	wchar_t m_filePath[256];					//wav�t�@�C���p�X
	bool m_is3DSound = false;					//3D�T�E���h�ōĐ����邩�H
	float m_volume = 1.0f;						//�{�����[��
	bool m_isPaused = false;					//�ꎞ��~�����H
	CSoundSource* m_loopSoundSource = nullptr;	//���[�v�Đ��p�̃T�E���h�\�[�X
};

