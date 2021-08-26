#include "stdafx.h"
#include "Tips.h"

/**
 * @brief ���r�����X
*/
namespace nsMobyrinth
{
	/**
	 * @brief �`�b�v�X
	*/
	namespace nsTips
	{

		//�R���X�g���N�^
		CTips::CTips()
		{
			//�uTips��UI�v�̃X�v���C�g�����_���[�̐����Ə�����
			m_tipsUISR = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::PRIORITY_SECOND);
			const float tipsUIX = 400.0f;
			const float tipsUIY = 260.0f;
			m_tipsUISR->Init("Assets/image/Tips_UI.DDS",
				512.0f * 1.5f, 256.0f * 0.8f, { 0.5f,0.5f }, AlphaBlendMode_Trans);
			m_tipsUISR->SetPosition({ tipsUIX, tipsUIY, 0.0f });

			//�uTips�v�̃t�H���g�����_�[�̐����Ə�����
			m_tipsFR = NewGO<nsGraphic::nsFont::CFontRender>(nsCommonData::PRIORITY_SECOND);
			const float tipsX = 200.0f;
			const float tipsY = 330.0f;
			m_tipsFR->Init(L"T ip s",
				{ tipsX,tipsY },			//���W
				//{ 1.0f,1.0f,0.0f,1.0f },	//�J���[
				{ 1.0f,1.0f,1.0f,1.0f },	//�J���[
				0.0f,						//��]
				0.8f						//�g��
			);
			m_tipsFR->SetShadowParam(true, 0.4f, { 0.0f,0.0f,0.0f,1.0f });

			//Tips�̕��͂�\������t�H���g�����_���[�̐����Ə�����
			m_sentenceFR = NewGO<nsGraphic::nsFont::CFontRender>(nsCommonData::PRIORITY_SECOND);
			const float sentenceDistX = 20.0f;
			const float sentenceDistY = -50.0f;
			m_sentenceFR->Init(L"", { tipsX + sentenceDistX,tipsY + sentenceDistY });
			m_sentenceFR->SetScale(0.7f);
			m_sentenceFR->SetShadowParam(true, 0.4f, { 0.0f,0.0f,0.0f,1.0f });

			//�e�L�X�g�̏�����
			InitText();

		}

		//�f�X�g���N�^
		CTips::~CTips()
		{
			DeleteGO(m_tipsUISR);
			DeleteGO(m_tipsFR);
			DeleteGO(m_sentenceFR);
		}


		/// <summary>
		/// �e�L�X�g�̏�����
		/// </summary>
		void CTips::InitText()
		{
			//���̊֐������
			//���x���I�u�W�F�N�g���g�p�\�ɂ���
			using namespace nsLevelObject;

			//�I�u�W�F�N�g�^�C�v�̐������J��Ԃ�
			for (int objectType = 0; objectType < EN_OBJECT_TYPE_NUM; objectType++)
			{
				//�I�u�W�F�N�g�^�C�v�ɂ���ă��[�h����e�L�X�g��U�蕪����
				switch (objectType)
				{
				case EN_OBJECT_TYPE_EMPTY:	//�����
					LoadText(objectType, "empty");
					break;
				case EN_RO_TYPE_WATER:	//��
					LoadText(objectType, "water");
					break;
				case EN_RO_TYPE_FIRE:	//��
					LoadText(objectType, "fire");
					break;
				case EN_RO_TYPE_RUNNING:	//�ғ�
					LoadText(objectType, "running");
					break;
				case EN_RO_TYPE_STOP:	//��~
					LoadText(objectType, "stop");
					break;
				case EN_RO_TYPE_NAIL:	//�B
					LoadText(objectType, "nail");
					break;
				case EN_RO_TYPE_BAR:		//�o�[��
					LoadText(objectType, "bar");
					break;
				case EN_RO_TYPE_KEY:		//��
					LoadText(objectType, "key");
					break;
				case EN_RO_TYPE_PADLOCK:	//�싞��
					LoadText(objectType, "padlock");
					break;
				case EN_RO_TYPE_LEFT://��
					LoadText(objectType, "left");
					break;
				case EN_RO_TYPE_RIGHT://�E
					LoadText(objectType, "right");
					break;
				case EN_OO_TYPE_FLAME:		//��
					LoadText(objectType, "bigFire");
					break;
				case EN_OO_TYPE_WALL:		//��
					LoadText(objectType, "wall");
					break;
				case EN_OO_TYPE_GOAL:		//�S�[��
					LoadText(objectType, "goal");
					break;
				case EN_OO_TYPE_BIG_PADLOCK:	//���ĂȂ��싞��
					LoadText(objectType, "notHavePadlock");
					break;
				case EN_OO_TYPE_BOX:			//��
					LoadText(objectType, "box");
					break;
				case EN_OO_TYPE_ONEWAY:		//����ʍs
					LoadText(objectType, "oneWay");
					break;
				case EN_OO_TYPE_TRANSPARENT_SWITCH:	//����ʍs
					LoadText(objectType, "switch");
					break;
				case EN_OO_TYPE_REVERSE_ALL:	//�S���]
					LoadText(objectType, "reverseAll");
					break;
				case EN_OO_TYPE_UFO_CAPTURE:	//UFO�ɕ߂܂���
					LoadText(objectType, "UFOCapture");
					break;
				case EN_OO_TYPE_UFO_LANDING_POINT:	//UFO�ɕ߂܂���
					LoadText(objectType, "UFOLandingPoint");
					break;
				default:		//�f�t�H���g
					LoadText(objectType, "empty");
					break;
				}
			}
		}


		/// <summary>
		/// �e�L�X�g�����[�h����
		/// </summary>
		/// <param name="objectType">�I�u�W�F�N�g�^�C�v</param>
		/// <param name="fileName">�e�L�X�g�t�@�C���̖��O</param>
		void CTips::LoadText(const int objectType, const char* fileName)
		{
			//���{����Z�b�g
			setlocale(LC_CTYPE, "ja_JP.UTF-8");
			//setlocale(LC_CTYPE, "ja_JP.SJIS");
			//std::locale("");

			//�t�@�C���p�X�̓��ɂ��镶����
			const char* headFilePath = "Assets/text/";
			//�t�@�C���p�X�̌��ɂ��镶����
			const char* tailFilePath = ".txt";
			//�t�@�C���p�X�̕�����
			char FilePath[m_maxTextSize];
			//�Ȃ��Ƃ͎v�����ǁA�ő�T�C�Y�ȏ�Ȃ�؂�̂�
			const int maxSize = m_maxTextSize - 1;
			FilePath[maxSize] = 0;

			//�t�@�C���p�X����������Ƃ��̃G���[
			if (sizeof(headFilePath) + sizeof(fileName) + sizeof(tailFilePath) > maxSize)
			{
				MessageBoxA(nullptr, "�t�@�C���p�X���������܂��B", "�G���[", MB_OK);
				return;
			}

			//�t�@�C���p�X�Ƀt�@�C���p�X�̓��ɂ��镶������R�s�[
			strncpy(FilePath, headFilePath, maxSize);
			//�t�@�C���p�X�Ƀt�@�C���̖��O��ǉ�
			strncat(FilePath, fileName, maxSize - sizeof(headFilePath));
			//�t�@�C���p�X�Ƀt�@�C���p�X�̌��ɂ��镶�����ǉ�
			strncat(FilePath, tailFilePath, maxSize - sizeof(FilePath));

			//�t�@�C����ǂݎ�胂�[�h�ŊJ��
			auto fp = fopen(FilePath, "r");

			if (fp == nullptr)
			{
				//�t�@�C���Ȃ���
				const char* cantOpen = "�̃t�@�C�����J���܂���B";
				strncat(FilePath, cantOpen, sizeof(FilePath) + sizeof(cantOpen));
				MessageBoxA(nullptr, FilePath, "�G���[", MB_OK);
				return;
			}

			//�t�@�C���̕��������Ă�������
			char c;
			//���s���镶����
			const int newLineNum = 12;
			//���s���邩�H
			bool newLineFlag = true;

			//�t�@�C�����ꕶ�����ǂݎ��
			//EOF�܂œǂݑ�����
			while ((c = fgetc(fp)) != EOF)
			{
				//���������J�E���g����֐���1�o�C�g�ڂŔ��肵�Ă邩��A
				//�܂��o�b�t�@�Ɏ��̕��������āA���s����̕������̈����
				//�������ɂȂ�����Am_text�ɉ��s�����Ă��玟�̕���������

				//���[�h���������ɉ��s�����邩�H
				if (c == '\n')
					//��������A��������ȏ���s���Ȃ��悤�ɂ���
					newLineFlag = false;

				//���s���邩�H
				if (newLineFlag)
				{
					//���s����

					//�o�b�t�@
					char buf[m_maxTextSize];
					//�o�b�t�@�ɑO�t���[���܂ł̃��[�h�����e�L�X�g���R�s�[
					strncpy(buf, m_text[objectType], maxSize);
					//�o�b�t�@�Ɏ��̕���������
					strncat(buf, &c, sizeof(char));
					//�o�b�t�@�̕��������A���s���镶�����̈�������������H
					if (u8len(buf) % (newLineNum + 1) == 0)
						//�����ꍇ
						//m_text�ɉ��s������
						strncat(m_text[objectType], "\n", sizeof(char));
				}

				//���[�h�������͂��A�f�[�^�����o�ɕێ����Ă���
				strncat(m_text[objectType], &c, sizeof(char));

			}

			//�t�@�C�������
			fclose(fp);
		}

		//u8len�֐��iUTF8�̕��������J�E���g����֐��j
		int u8len(const char* str)
		{
			int cnt = 0;
			while (*str != '\0') {
				cnt++;
				str += u8mb(*str);
			}
			return cnt;
		}

		//u8mb�֐��FUTF-8������1�o�C�g�ڂ𔻒肵�ĕ����̃o�C�g����Ԃ��֐�
		int u8mb(const char chr)
		{
			int byt = 1;
			if ((chr & 0x80) == 0x00) { //1byte�����͉������Ȃ��i[byt = 1]�̂܂܁j
			}
			else if ((chr & 0xE0) == 0xC0) { //2byte����
				byt = 2;
			}
			else if ((chr & 0xF0) == 0xE0) { //3byte����
				byt = 3;
			}
			else if ((chr & 0xF8) == 0xF0) { //4byte����
				byt = 4;
			}
			else if ((chr & 0xFC) == 0xF8) { //5byte����
				byt = 5;
			}
			else if ((chr & 0xFE) == 0xFC) { //6byte����
				byt = 6;
			}
			return byt;
		}



		/// <summary>
		/// ���[�h�ς݂̃I�u�W�F�N�g�^�C�v�̃e�L�X�g���Z�b�g����
		/// </summary>
		/// <param name="textType">�I�u�W�F�N�g�^�C�v</param>
		void CTips::SetText(const int textType)
		{
			//�t�H���g�����_���[�ɃZ�b�g����e�L�X�g��wchar_t�^���炩
			//char�^�̃f�[�^�����o��ϊ�����

			//wchar_t�^�̃e�L�X�g
			wchar_t text[m_maxTextSize];
			//char�^�̃f�[�^�����o��wchar_t�^�ɕϊ�����

			if (textType != nsLevelObject::EN_RO_TYPE_WATER)
				int a = 1;

			std::string str = m_text[textType];
			mbstowcs(text, str.c_str(), m_maxTextSize);

			//mbstowcs(text, m_text[textType], m_maxTextSize);
			//�e�L�X�g���t�H���g�����_���[�ɃZ�b�g
			m_sentenceFR->SetText(text);
		}

		/// <summary>
		/// �e�L�X�g�����ɂ��\�����Ȃ�
		/// </summary>
		void CTips::ClearText()
		{
			//����ۂ̃e�L�X�g���Z�b�g
			SetText(nsLevelObject::EN_OBJECT_TYPE_EMPTY);
		}

		void CTips::SetTitleMode()
		{
			m_tipsUISR->Deactivate();
			m_tipsFR->Deactivate();
			m_sentenceFR->Deactivate();
		}
	}
}