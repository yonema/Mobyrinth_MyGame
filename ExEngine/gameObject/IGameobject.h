/*!
 *@brief	�Q�[���I�u�W�F�N�g�̊��N���X�B
 */

#pragma once

#include <list>
#include "../../GameTemplate/Game/Noncopyable.h"

class RenderContext;

/*!
	*@brief	�Q�[���I�u�W�F�N�g�B
	*/
class IGameObject : private Noncopyable
{
public:
	/*!
		*@brief	�f�X�g���N�^
		*/
	virtual ~IGameObject()
	{
	}
public:

	
	/*!
	*@brief	Update�̒��O�ŌĂ΂��J�n�����B
	*@details
	* 
	* �{�֐���true��Ԃ��ƃQ�[���I�u�W�F�N�g�̏��������������Ɣ��f�����</br>
	* Update�֐����Ă΂�o���܂��Btrue��Ԃ��Ĉȍ~��Start�֐��͌Ă΂�Ȃ��Ȃ�܂��B</br>
	* �Q�[���I�u�W�F�N�g�̏������ɕ����t���[��������ꍇ�Ȃǂ�false��Ԃ��āA�������X�e�b�v�Ȃǂ��g����</br>
	* �K�؂ɏ��������s���Ă��������B
	*/
	virtual bool Start() { return true; }
	/*!
		*@brief	�X�V
		*/
	virtual void Update() {}
	/*!
	 *@brief	�`��
	*/
	virtual void Render(RenderContext& renderContext)
	{
		(void)renderContext;
	}

	/**
	 * @brief �t�H���[�h�����_�����O
	 * @param renderContext �����_�[�R���e�L�X�g
	*/
	virtual void ForwardRender(RenderContext& renderContext)
	{
		(void)renderContext;
	}

	
public:
	/*!
	*@brief Start�֐������������H
	*/
	bool IsStart() const
	{
		return m_isStart;
	}
	/*!
	*@brief �A�N�e�B�u���ǂ����𔻒�B
	*/
	bool IsActive() const
	{
		return m_isActive;
	}
	/// <summary>
	/// �Q�[���I�u�W�F�N�g���A�N�e�B�u�ɂ���B
	/// </summary>
	void Activate()
	{
		m_isActive = true;
	}
	/// <summary>
	/// �Q�[���I�u�W�F�N�g���A�N�e�B�u�ɂ���B
	/// </summary>
	void Deactivate()
	{
		m_isActive = false;
	}

	/**
	 * @brief �f�B�t�@�\�h�����_�����O�ŕ`�悷�邩�H��ݒ�B
	 * @param [in] isDefferdRender �f�B�t�@�\�h�����_�����O�ŕ`�悷�邩�H
	*/
	void SetIsDefferdRender(const bool isDefferdRender)
	{
		m_isDefferdRender = isDefferdRender;
	}

	/**
	 * @brief �f�B�t�@�\�h�����_�����O�ŕ`�悷�邩�H���擾
	 * @return �f�B�t�@�\�h�����_�����O�ŕ`�悷�邩�H
	*/
	bool IsDefferdRender()const
	{
		return m_isDefferdRender;
	}
	
	/// <summary>
	/// ���S������B
	/// </summary>
	void Dead()
	{
		m_isDead = true;
	}
	/// <summary>
	/// ���S���Ă���H
	/// </summary>
	/// <returns>true���Ԃ��Ă����玀�S���Ă���</returns>
	bool IsDead() const
	{
		return m_isDead;
	}
	bool EqualName(const char* name)const
	{
		return strcmp(m_name.c_str(), name) == 0;
	}
public:

	void RenderWrapper(RenderContext& renderContext)
	{
		if (m_isActive && m_isStart && m_isDefferdRender && !m_isDead ) {
			Render(renderContext);
		}
	}

	void ForwardRenderWrapper(RenderContext& renderContext)
	{
		if (m_isActive && m_isStart && !m_isDefferdRender && !m_isDead) {
			ForwardRender(renderContext);
		}
	}
	
	void UpdateWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead ) {
			Update();
		}
	}
	void StartWrapper()
	{
		if (/*m_isActive && */!m_isStart && !m_isDead ) {
			if (Start()) {
				//���������������B
				m_isStart = true;
			}
		}
	}
	void SetName(const char* name)
	{
		if (name != nullptr) {
			m_name = name;
		}
	}
	friend class CGameObjectManager;
protected:
	std::string m_name;								//�Q�[���I�u�W�F�N�g�̖��O
	bool m_isStart = false;							//Start�̊J�n�t���O�B
	bool m_isDead = false;							//���S�t���O�B
	bool m_isRegistDeadList = false;				//���S���X�g�ɐς܂�Ă���B
	bool m_isNewFromGameObjectManager;	//GameObjectManager��new���ꂽ�B
	bool m_isRegist = false;							//GameObjectManager�ɓo�^����Ă���H
	bool m_isActive = true;							//Active�t���O�B
	bool m_isDefferdRender = true;					//�f�B�t�@�\�h�����_�����O�ŕ`�悷�邩�H

	//�ǉ�
private:
	static bool m_gamePaused;				//�Q�[�����|�[�Y���BUpdate���Ă΂�Ȃ��B
public:
	static void SetGamePaused(const bool paused)
	{
		m_gamePaused = paused;
	}
	static const bool GetGamePaused()
	{
		return m_gamePaused;
	}
	/// <summary>
	/// ��ɌĂ΂��Update�i�X�V�j
	/// </summary>
	virtual void AlwaysUpdate() {};
	/// <summary>
	/// �|�[�Y���ɂ����Ă΂��Update�i�X�V�j
	/// </summary>
	virtual void UpdateOnlyPaused() {};
	
	void AlwaysUpdateWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead) {
			AlwaysUpdate();
		}
	}
	void UpdateOnlyPausedWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead) {
			UpdateOnlyPaused();
		}
	}
};
