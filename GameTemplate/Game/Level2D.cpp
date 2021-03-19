#include "stdafx.h"
#include "Level2D.h"


/// <summary>
/// ���x�����������B
/// </summary>
/// <param name="filePath">casl�t�@�C���̃t�@�C���p�X�B</param>
/// <param name="hookFunc">�I�u�W�F�N�g���쐬���鎞�̏������t�b�N���邽�߂̊֐��I�u�W�F�N�g�B</param>
void CLevel2D::Init(
	const char* filePath,
	std::function<bool(Level2DObjectData& objData)> hookFunc
)
{
	//Casl�t�@�C�������[�h����B
	CaslFile casl;
	casl.Load(filePath);

	//�\�z�\�z�B
	for (int i = 0; i < casl.GetNumCaslData(); i++)
	{
		auto caslData = casl.GetCaslData(i);
		Level2DObjectData objData;
		objData.scale.x = caslData->scale.x;
		objData.scale.y = caslData->scale.y;
		objData.position.x = caslData->position.x;
		objData.position.y = caslData->position.y;
		objData.width = caslData->width;
		objData.height = caslData->height;
		objData.name = caslData->name.get();
		objData.ddsFilePath = caslData->ddsFilePath.get();
		auto isHook = false;
		if (hookFunc != nullptr) {
			//hook�֐����w�肳��Ă���̂�hook�֐����Ăяo���B
			isHook = hookFunc(objData);
			if (!isHook)
			{
				AddMapChip2D(caslData);
			}
		}
		else
		{
			AddMapChip2D(caslData);
		}
	}

}


/// <summary>
/// �}�b�v�`�b�v2D��ǉ�����B
/// </summary>
/// <param name="caslData">Casl�f�[�^</param>
void CLevel2D::AddMapChip2D(CaslData* caslData)
{
	auto mapChip2DPtr = std::make_unique<MapChip2D>();
	mapChip2DPtr.get()->Init(caslData);
	//std::unique_ptr���ړ������鎞��std::move()���g���B
	m_mapChip2DPtrList.push_back(std::move(mapChip2DPtr));
}



