#include "stdafx.h"
#include "SoundCue.h"


/// <summary>
/// �������p�֐�
/// wav�t�@�C���̃p�X���w�肵�ď���������
/// </summary>
/// <param name="filePath">wav�t�@�C���p�X</param>
/// <param name="is3DSound">3D�Đ����邩�H�i���Ԃ�ł��Ȃ��j</param>
void CSoundCue::Init(const wchar_t* filePath, const bool is3DSound)
{
	//�����œn���ꂽ const wchat_t* ���A
	//�f�[�^�����o�� wchar �̔z��ɓ����
	swprintf(m_filePath, filePath);

	//��x�A����Ĕj�����邱�ƂŁA
	//Flyweight�p�^�[�����g�p���Ă���WaveFileBank��
	//�o�^������B
	CSoundSource* ss = NewGO<CSoundSource>(0);
	ss->Init(m_filePath, m_is3DSound);
	DeleteGO(ss);
}


/// <summary>
/// �T�E���h���Đ�
/// </summary>
/// <param name="isLoop">���[�v�Đ����邩�H</param>
void CSoundCue::Play(const bool isLoop)
{
	//���[�v�Đ��������V���b�g�Đ����ŏ�����U�蕪����

	if (isLoop)
	{
		//���[�v�Đ�
		PlayLoop();
	}
	else
	{
		//�����V���b�g�Đ�
		PlayOneShot();
	}
}


//���[�v�Đ��̏���
void CSoundCue::PlayLoop()
{
	//���[�v�Đ��p�̃T�E���h�\�[�X��
	//���łɍ���Ă�����
	if (m_loopSoundSource)
	{
		//�ꎞ��~�������̂��ĊJ����
		m_loopSoundSource->Play(true);
		//�t���O���ꎞ��~�������������
		m_isPaused = false;
	}
	else
	{
		//���[�v�Đ��p�̃T�E���h�\�[�X��
		//�܂����g�p�̏ꍇ��
		//�V��������čĐ�����B
		m_loopSoundSource = NewGO<CSoundSource>(0);
		m_loopSoundSource->Init(m_filePath, m_is3DSound);
		m_loopSoundSource->Play(true);
	}
}

//�����V���b�g�Đ��̏���
void CSoundCue::PlayOneShot()
{
	//����A�T�E���h�\�[�X������čĐ�������
	//Flyweight�p�^�[�����g�p���Ă��邩��
	//�d���Ȃ�Ȃ����I
	CSoundSource* ss = NewGO<CSoundSource>(0);
	ss->Init(m_filePath, m_is3DSound);
	ss->SetVolume(m_volume);	//OS�̏ꍇ�͂����Ń{�����[����ݒ肷��
	ss->Play(false);
}


/// <summary>
/// �{�����[����ݒ�
/// 1.0f���ʏ�B
/// ���̒l������Ə���ɐ�Βl�ɂȂ邩�璍��
/// </summary>
/// <param name="volume">�{�����[��</param>
void CSoundCue::SetVolume(const float volume)
{
	//�{�����[����ێ�
	m_volume = volume;

	//���[�v�Đ��p�̃T�E���h�\�[�X���g���Ă�����
	if (!m_loopSoundSource)
		return;

	//���[�v�Đ��p�̃T�E���h�\�[�X�̃{�����[����ݒ肷��
	m_loopSoundSource->SetVolume(m_volume);
}

/// <summary>
/// �Đ����~����B
/// ����Œ�~����ƁA���ɍĐ��������A
/// �ŏ�����̍Đ��ɂȂ�B
/// ���̎d�l���ƁA���[�v�Đ��͒�~�ł��邪�A
/// �����V���b�g�Đ��̒�~�͂ł��Ȃ��B
/// </summary>
void CSoundCue::Stop()
{
	//���[�v�Đ��p�̃T�E���h�\�[�X���g���Ă�����
	if (!m_loopSoundSource)
		return;

	//�T�E���h���~����
	m_loopSoundSource->Stop();
}

/// <summary>
/// �Đ����ꎞ��~����B
/// ����ňꎞ��~����ƁA���ɍĐ��������A
/// �r������̍Đ��ɂȂ�B
/// ���̎d�l���ƁA���[�v�Đ��͈ꎞ��~�ł��邪�A
/// �����V���b�g�Đ��̈ꎞ��~�͂ł��Ȃ��B
/// </summary>
void CSoundCue::Pause()
{
	//���[�v�Đ��p�̃T�E���h�\�[�X���g���Ă�����
	if (!m_loopSoundSource)
		return;

	//�T�E���h���ꎞ��~����
	m_loopSoundSource->Pause();
	//�t���O���ꎞ��~���ɂ���
	m_isPaused = true;
}

/// <summary>
/// ���Đ������ǂ����H
/// ���̎d�l���ƁA���[�v�Đ��͌��m�ł��邪�A
/// �����V���b�g�Đ��̌��m�͂ł��Ȃ��B
/// </summary>
/// <returns>�Đ������H</returns>
const bool CSoundCue::IsPlaying()const
{
	//���[�v�Đ��p�̃T�E���h�\�[�X���g���Ă�����
	if (!m_loopSoundSource)
		return false;

	//�Đ������ǂ������ׂ�
	return m_loopSoundSource->IsPlaying();
}