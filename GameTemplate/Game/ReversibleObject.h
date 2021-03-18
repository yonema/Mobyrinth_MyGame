#pragma once
#include "LevelObjectBase.h"



class CReversibleObject : public ILevelObjectBase
{
protected:
	bool Init
	(const char* filePath_front, const int type_front,
		const char* filePath_back, const int type_back);
private:
	/// <summary>
	/// �I�u�W�F�N�g���E������
	/// </summary>
	void CheckPlayer();
	/// <summary>
	/// �I�u�W�F�N�g���������Ă���Ƃ��̃{�^�����m
	/// </summary>
	void HeldPlayer();
	/// <summary>
	/// �I�u�W�F�N�g�������AA�{�^�����������Ƃ��̏���
	/// �I�u�W�F�N�g�𗠑��ɓ����āA�I�u�W�F�N�g�̐����𔽓]������B
	/// </summary>
	void ThrownDown();
	/// <summary>
	/// �I�u�W�F�N�g�������AB�{�^�����������Ƃ��̏���
	/// �I�u�W�F�N�g���v���C���[�̑����ɒu���B
	/// </summary>
	void Cancel();
	/// <summary>
	/// �I�u�W�F�N�g�������AC�{�^�����������Ƃ��̏���
	/// �I�u�W�F�N�g�����݌����Ă�������Ɍ������ē�����B
	/// </summary>
	void ThrownSide();
	/// <summary>
	/// ���݂̍����ɂ���E�F�C�|�C���g�����m���鏈��
	/// </summary>
	void CalcTargetPos();
	/// <summary>
	/// �I�u�W�F�N�g����������Ƃ���
	/// �I�u�W�F�N�g���擾���鏈�����s����悤�ɂ��鏈��
	/// </summary>
	void Query();
	virtual void QuerySub() {};

public:
	bool PureVirtualStart()override final;
	virtual bool StartSub() { return true; };
	void Update()override final;
	virtual void UpdateSub() {};
	virtual ~CReversibleObject();
	void Reverse();
	void SetFrontOrBack(const bool frontOrBack);
	virtual void SetFrontOrBackSub() {};
	const bool GetFrontOrBack()
	{
		return m_frontOrBack;
	}
	void SetBothModelActiveFlag(const bool activeFlag);

public:
	enum EnFrontAndBack
	{
		enFront,
		enBack,
		enFrontAndBackNum,
	};
private:
	bool m_frontOrBack = enFront;
	bool m_bothModelactiveFlag = true;
	CModelRender* m_modelRender[enFrontAndBackNum] = { nullptr };
	int m_reversibleType[enFrontAndBackNum] = { enEnpty };
	enum EnObjectState
	{
		enCheckPlayer,
		enHeldPlayer,
		enCancel,
		enThrownSide,
		enThrownDown,
		enQuery,
	};
	int m_objectState = enCheckPlayer;
	int m_throwCounter = 0;

	Quaternion m_throwRot = g_quatIdentity;
	
	//�L�����N�^�[�̍��E�̌����𒲂ׂ�̂Ɏg�p
	enum EnPlayerLeftOrRight
	{
		enLeft,		//��
		enRight,	//�E
	};
	int m_playerLeftOrRight = enRight;	//�L�����N�^�[�̍��E�̌���
};

