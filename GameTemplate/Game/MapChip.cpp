#include "stdafx.h"
#include "MapChip.h"
#include "Level.h"

/**
 * @brief ���r�����X
*/
namespace nsMobyrinth
{
	/**
	 * @brief tkLevel
	*/
	namespace nsTkLevel
	{

		CMapChip::CMapChip(const LevelObjectData& objData)
		{
			//wchar��char�ɕϊ�
			char objName[256];
			wcstombs(objName, objData.name, 256);

			//�t�@�C���p�X���쐬�B
			char filePath[256];
			sprintf(filePath, "Assets/modelData/%s.tkm", objName);

			//���f�������_���[��������
			m_modelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::PRIORITY_FIRST);
			m_modelRender->Init(filePath);
			m_modelRender->SetPosition(objData.position);
			m_modelRender->SetRotation(objData.rotation);
			m_modelRender->SetScale(objData.scale);

			//�ÓI�����I�u�W�F�N�g�̏�����
			m_physicsStaticObject.CreateFromModel
			(m_modelRender->GetModel(), m_modelRender->GetModel().GetWorldMatrix());
		}

		CMapChip::~CMapChip()
		{
			if (m_modelRender)
				DeleteGO(m_modelRender);
		}
	}
}