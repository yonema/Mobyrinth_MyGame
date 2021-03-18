#pragma once
#include "LevelObjectBase.h"

/// <summary>
///	���o�[�V�u���I�u�W�F�N�g�i���]�I�u�W�F�N�g�j
/// ���]�I�u�W�F�N�g�����Ƃ��͂��̃N���X���p�����ĂˁI
/// </summary>
class CReversibleObject : public ILevelObjectBase
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool PureVirtualStart()override final;	//�X�^�[�g�֐�
	virtual ~CReversibleObject();			//�f�X�g���N�^
	void PureVirtualUpdate()override final;			//�A�b�v�f�[�g�֐�


public:		//public�ȃI�[�o�[���C�h���Ăق��������o�֐�
	virtual bool StartSub() { return true; };	//�X�^�[�g�֐�
	virtual void UpdateSub() {};				//�A�b�v�f�[�g�֐�
	/// <summary>
	/// �\�Ɨ����Z�b�g����T�u�֐�
	/// ������ĂԂƂ��́ACReversibleObject�̌p�������N���X��
	/// �V���ȕ\�����Z�b�g����K�v������Ƃ��Ɏg���B
	/// ���RObird_fish���Q��
	/// </summary>
	virtual void SetFrontOrBackSub() {};


private:	//private�ȃI�[�o�[���C�h���Ăق��������o�֐�
	/// <summary>
	/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐��B
	/// ������QueryLOs���Ă�ŃI�u�W�F�N�g���m�̔����������B
	/// ���ROnail_bar���Q��
	/// </summary>
	virtual void QuerySub() {};


protected:	//�����̃����o�֐�����Ɏg��
	/// <summary>
	/// �������֐�
	/// �ŏ��ɓǂ�łˁBtrue��߂��ĂˁB
	/// �\�̃��f���Ƃ��̃^�C�v�A���̃��f���Ƃ��̃^�C�v
	/// ��ݒ肷��B
	/// �^�C�v�ꗗ��LevelObjectBase.h���Q��
	/// </summary>
	/// <param name="filePath_front">�\�̃��f����tkm�t�@�C���p�X</param>
	/// <param name="type_front">�\�̃^�C�v</param>
	/// <param name="filePath_back">���̃��f����tkm�t�@�C���p�X</param>
	/// <param name="type_back">���̃^�C�v</param>
	/// <returns>true�߂��Ă�</returns>
	bool Init
	(const char* filePath_front, const int type_front,
		const char* filePath_back, const int type_back);


public:		//�����̃����o�֐�����Ɏg��	
	/// <summary>
	/// ���]������
	/// </summary>
	void Reverse();

	/// <summary>
	/// �\��������ݒ肷�邷��
	/// </summary>
	/// <param name="frontOrBack">enFront��enBack������</param>
	void SetFrontOrBack(const bool frontOrBack);


	/// <summary>
	///	�\���������擾����
	/// </summary>
	/// <returns>enFront��enBack</returns>
	const bool GetFrontOrBack()
	{
		return m_frontOrBack;
	}

	/// <summary>
	/// �\�Ɨ��̗����̃��f���̗L�����t���O��ݒ肷��B
	/// true����ꂽ�ꍇ�A���݂̕\�����ǂ��炩��L�������A�����𖳌�������B
	/// false����ꂽ�ꍇ�A�����𖳌�������
	/// </summary>
	/// <param name="activeFlag">�L�����t���O</param>
	void SetBothModelActiveFlag(const bool activeFlag);

private:	//private�ȃ����o�֐�

	/// <summary>
	/// �v���C���[�Ɏ�����邩�ǂ������ׂ�֐�
	/// �����ꂽ��HeldPlayer�փX�e�[�g�i��ԁj�ڍs
	/// </summary>
	void CheckPlayer();

	/// <summary>
	/// �v���C���[�Ɏ�����Ă����Ԃ̊֐��B
	/// �v���C���[�Ɏ�����Ă���ꏊ�Ɉړ����鏈���B
	/// enCancel���AenThrownDown���AenThrownSide��
	/// �X�e�[�g�i��ԁj���ڍs
	/// </summary>
	void HeldPlayer();

	/// <summary>
	/// �����Ă���I�u�W�F�N�g�����̏�ɒu���֐�
	/// enQuery�փX�e�[�g�i��ԁj���ڍs
	/// </summary>
	void Cancel();

	/// <summary>
	/// �����Ă���I�u�W�F�N�g�����ɓ�����֐�
	/// enQuery�փX�e�[�g�i��ԁj���ڍs
	/// </summary>
	void ThrownSide();

	/// <summary>
	/// �����Ă���I�u�W�F�N�g�����ɓ�����֐�
	/// enQuery�փX�e�[�g�i��ԁj���ڍs
	/// </summary>
	void ThrownDown();

	/// <summary>
	/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
	/// �I�[�o�[���C�h���Ăق����֐��AQuerySub()�͂����ŌĂ΂��B
	/// enCheckPlayer�փX�e�[�g�i��ԁj���ڍs
	/// </summary>
	void Query();


	/// <summary>
	/// �I�u�W�F�N�g�����ɓ�����ۂ́A���������v�Z����֐�
	/// </summary>
	void CalcTargetPos();

public:		//public�ȃf�[�^�����o

	/// <summary>
	/// �\��������\���񋓑�
	/// </summary>
	enum EnFrontAndBack
	{
		enFront,			//�\���
		enBack,				//�����
		enFrontAndBackNum,	//�\���̐�
	};

private:	//�f�[�^�����o
	bool m_frontOrBack = enFront;				//�\�������H
	bool m_bothModelactiveFlag = true;			//�\�������̗L�����t���O
	CModelRender* m_modelRender[enFrontAndBackNum] = { nullptr };	//���f�������_���[
	int m_reversibleType[enFrontAndBackNum] = { enEnpty };	//�\�Ɨ��̃I�u�W�F�N�g�̃^�C�v

	int m_throwCounter = 0;						//�����Ă��鎞�̃J�E���^�[
	Quaternion m_throwRot = g_quatIdentity;		//���ɓ�����Ƃ��̉�]

	/// <summary>
	/// �I�u�W�F�N�g�̌��݂̃X�e�[�g�i��ԁj
	/// ����ŃA�b�v�f�[�g���̏���������U��
	/// </summary>
	enum EnObjectState
	{
		enCheckPlayer,	//�v���C���[�Ɏ�����邩�ǂ������ׂ�
		enHeldPlayer,	//�v���C���[�Ɏ�����Ă�����
		enCancel,		//�����Ă���I�u�W�F�N�g�����̏�ɒu��
		enThrownSide,	//�����Ă���I�u�W�F�N�g�����ɓ�����֐�
		enThrownDown,	//�����Ă���I�u�W�F�N�g�����ɓ�����֐�
		enQuery,		//�N�G�����Ăق����^�C�~���O
	};
	int m_objectState = enCheckPlayer;			//���݂̃I�u�W�F�N�g�̃X�e�[�g�i��ԁj

};

