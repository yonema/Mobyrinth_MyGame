#pragma once
#include "LevelObjectBase.h"
#include "ModelRender.h"
#include "SpotLight.h"
#include "UFOLandingPoint.h"

/// <summary>
/// UFO�N���X
/// </summary>
class CUFO : public ILevelObjectBase
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool PureVirtualStart() override final;		//�X�^�[�g�֐�
	~CUFO();									//�f�X�g���N�^
	void PureVirtualUpdate() override final;	//�A�b�v�f�[�g�֐�

public:		//�����o�֐�

private:	//private�ȃ����o�֐�

	void Search();		//�v���C���[��T������
	void Capture();		//�v���C���[�����������̏���
	void Transport();	//�v���C���[���^�ԏ���
	void Landing();
	void Leave();

	void Move();		//�ړ�����
	void GetOnStage();	//���r�E�X�̗ւ̃X�e�[�W�ɃC�C�����ɍ��킹�鏈��

private:	//�񋓌^

	//�A�b�v�f�[�g�̃X�e�[�g�i��ԁj
	enum EnUpdateState
	{
		enSearch,		//�v���C���[��T��
		enCapture,		//�v���C���[��������
		enTransport,	//�v���C���[���^��
		enLanding,		//���n
		enLeave,
	};

	enum EnLeftOrRight
	{
		enLeft,
		enRight,
	};
private:	//�f�[�^�����o
	CModelRender* m_modelRender = nullptr;	//���f�������_���[
	Vector3 m_onWayPosition = g_vec3Zero;	//�E�F�C�|�C���g��̍��W
	Mobius* m_mobius = nullptr;				//�X�e�[�W�̃��r�E�X�̗ւ̃|�C���^
	Vector3 m_upVec = g_vec3Up;				//�A�b�v�x�N�g��
	bool m_leftOrRight = enRight;
	float m_defaultSpeed = 1300.0f;			//�f�t�H���g�̈ړ����x
	float m_moveSpeed = m_defaultSpeed;		//�ړ����x
	int m_updateState = enSearch;			//�A�b�v�f�[�g�̃X�e�[�g�i��ԁj
	int m_searchFlag = true;				//�v���C���[��T���Ă��邩�H
	float m_timer = 0.0f;					//�^�C�}�[
	CSpotLight* m_spotLight = nullptr;		//�X�|�b�g���C�g
	CUFOLandingPoint* m_ufoLandingPoint = nullptr;	//���n�_
	bool m_getOnStageFlag = true;

	//�f�o�b�N�p
private:
	static const int m_vertNum = 8;
	CModelRender* m_dbgVertPosMR[m_vertNum];	//OBB�̒��_�����邽�߂̃��f��
	CModelRender* m_dbgRay[2] = { nullptr,nullptr };	//���C�̎n�_�ƏI�_�����邽�߂̃��f��
};

