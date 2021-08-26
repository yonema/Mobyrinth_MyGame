#pragma once
#include "SoundSource.h"

//3D�T�E���h�͎����ĂȂ�
//���Ԃ�ł��Ȃ�

/**
 * @brief ���r�����X
*/
namespace nsMobyrinth
{
	/**
	 * @brief �T�E���h
	*/
	namespace nsSound
	{

		/// <summary>
		/// �T�E���h�L���[�B
		/// �T�E���h���Đ�����Ƃ��͂��̃N���X���g���ĂˁB
		/// �ʂ�CSoundSource���g���Ă��������ǁA�ł���΂������g���Ăق����ȁI
		/// </summary>
		class CSoundCue : public IGameObject
		{
		public:		//�����ŌĂ΂��֐�
			~CSoundCue();	//�f�X�g���N�^
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

			/// <summary>
			/// ���[�v����Ȃ��r���̂Ƃ���ɔ�ԏꍇ�A
			/// ���̊֐��̕Ԃ�l���g�p���Ĕ��肷��B
			/// </summary>
			/// <returns>�P��ȏ�Ȃ̏I���ɂ�����</returns>
			const bool IsLoop()const
			{
				return m_loopSoundSource->GetLoopStart();
			}

			/// <summary>
			/// �T�E���h�^�C�v��ݒ肷��
			/// CSoundCue::enBGM��CSoundCue::enSE�������Ɏw�肵�ĂˁB
			/// </summary>
			/// <param name="soundType">�T�E���h�^�C�v</param>
			void SetSoundType(const int soundType)
			{
				m_soundType = soundType;
			}



		public:
			//�T�E���h�^�C�v
			enum EnSountType
			{
				enBGM,	//BGM
				enSE,	//SE
			};
		private:	//private�ȃ����o�֐�
			void PlayLoop();		//���[�v�Đ��̏���
			void PlayOneShot();		//�����V���b�g�Đ��̏���
			float GetTypeVolume();	//�^�C�v�ʂ̃{�����[�����擾

		private:	//�f�[�^�����o
			wchar_t m_filePath[256];					//wav�t�@�C���p�X
			bool m_is3DSound = false;					//3D�T�E���h�ōĐ����邩�H
			float m_volume = 1.0f;						//�S�̂̃{�����[��
			int m_soundType = enSE;						//�T�E���h�̃^�C�v
			bool m_isPaused = false;					//�ꎞ��~�����H
			CSoundSource* m_loopSoundSource = nullptr;	//���[�v�Đ��p�̃T�E���h�\�[�X

		public:		//static�ȃ����o�֐�

			/// <summary>
			/// BGM�̃{�����[����ݒ肷��
			/// </summary>
			/// <param name="BGMVolume">BGM�̃{�����[��</param>
			static void SetBGMVolume(const float BGMVolume)
			{
				m_BGMVolume = BGMVolume;
			}

			/// <summary>
			/// SE�̃{�����[����ݒ肷��
			/// </summary>
			/// <param name="SEVolume">SE�̃{�����[��</param>
			static void SetSEVolume(const float SEVolume)
			{
				m_SEVolume = SEVolume;
			}

		private:	//static�ȃf�[�^�����o
			static float m_BGMVolume;					//BGM�̃{�����[��
			static float m_SEVolume;					//SE�̃{�����[��
		};

	}
}