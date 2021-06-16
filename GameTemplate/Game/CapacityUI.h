#pragma once
#include "SpriteRender.h"
#include "FontRender.h"
#include "LevelObjectManager.h"
#include "SoundCue.h"
#include "Player.h"

/// <summary>
/// �L���p�V�e�B��UI�\���N���X
/// </summary>
class CCapacityUI : public IGameObject
{
public:		//�����ŌĂ΂�郁���o�֐�

	bool Start() override final;	//�X�^�[�g�֐�
	~CCapacityUI();					//�f�X�g���N�^
	void Update() override final;	//�A�b�v�f�[�g�֐�

public:		//�����o�֐�S

	/// <summary>
	/// �^�C�g�����H��ݒ肷��
	/// </summary>
	/// <param name="titleFlag">�^�C�g�����H</param>
	void SetTitleFlag(const bool titleFlag)
	{
		m_titleFlag = titleFlag;
	}

private:	//private�ȃ����o�֐�

	//�X�v���C�g�̏�����
	void InitSprite();

	//�t�H���g�̏�����
	void InitFont();

	/// <summary>
	/// �ǂ̉��o���N�������`�F�b�N����
	/// </summary>
	/// <param name="num">�\���Ɨ����̃A�C�e���̐�</param>
	/// <param name="maxNum">�\���Ɨ����̃A�C�e���̍ő吔</param>
	void CheckDirecting(const int* num);

	//���o�̊���U��
	void Directing();
	
	/// <summary>
	/// ���������o
	/// </summary>
	/// <param name="frontOrBackSide">�\�����������H</param>
	void Increased(const int frontOrBackSide);

	/// <summary>
	/// ���������o
	/// </summary>
	/// <param name="frontOrBackSide">�\�����������H</param>
	void Decreased(const int frontOrBackSide);

	/// <summary>
	/// �I�[�o�[�������o
	/// </summary>
	/// <param name="frontOrBackSide">�\�����������H</param>
	void Over(const int frontOrBackSide);

	/// <summary>
	/// �ʏ�̒l�ɖ߂�
	/// </summary>
	void DefaultParam(const int frontOrBackSide);

	void CheckActiveFontColor();


private:	//�񋓌^
	
	//���o�̃X�e�[�g�̗񋓌^
	enum EnDirectingState
	{
		enNormal,		//�ʏ�
		enIncreased,	//������
		enDecreased,	//������
		enOver,			//�I�[�o�[����
	};

	//�\���Ɨ����̗񋓌^
	enum EnFrontOrBackSide
	{
		EN_FRONT_SIDE,			//�\��
		EN_BACK_SIDE,				//����
		enFrontAndBackSideNum,	//�\���Ɨ����̐�
	};

private:	//�f�[�^�����o

	CSpriteRender* m_capacityDisplaySR = nullptr;	//�L���p�V�e�B��\������X�v���C�g
	CFontRender* m_RONumFR[enFrontAndBackSideNum];	//�\���Ɨ����ɂ���A�C�e���̐��̃t�H���g
	CFontRender* m_capacityFR[enFrontAndBackSideNum];	//�L���p�V�e�B�̃t�H���g
	Vector2 m_capacityPos[enFrontAndBackSideNum];	//�L���p�V�e�B�̃t�H���g�̍��W
	float m_diffCapacityToNum = 0.0f;				//�L���p�V�e�B����A�C�e���̐��̃t�H���g�ւ̍�
	Vector4 m_defaultFontColor[enFrontAndBackSideNum];	//�t�H���g�̃J���[
	Vector4 m_activeFontColor;						//�v���C���[�����鑤�̃t�H���g�̃J���[
	Vector4 m_inactiveFontColor;					//�v���C���[�����Ȃ����̃t�H���g�̃J���[
	int m_oldRONum[enFrontAndBackSideNum];			//�O��̕\���Ɨ����ɂ���A�C�e���̐�
	bool m_firstFrame = true;						//�ŏ��̃t���[�����H
	bool m_titleFlag = false;						//�^�C�g�����H
	int m_directingState[enFrontAndBackSideNum] = { enNormal };	//���o�̃X�e�[�g
	float m_timer[enFrontAndBackSideNum] = { 0.0f };	//�^�C�}�[
	int m_maxCapacityNum[enFrontAndBackSideNum] = { 5,5 };	//�L���p�V�e�B�̍ő�l
	float m_defaultScale = 1.0f;					//�ʏ�̊g��
	CSoundCue* m_capacity_over = nullptr;			//�L���p�I�[�o�[�̉�
	Player* m_player = nullptr;						//�v���C���[�̃|�C���^
	//�v���C���[���\���ɂ��邩�����ɂ��邩�H
	int m_playerFrontOrBackSide = EN_FRONT_SIDE;
};

