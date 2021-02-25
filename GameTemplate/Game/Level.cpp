#include "stdafx.h"
#include "Level.h"


void CLevel::Init(
	/*const wchar_t* filePath*/const char* filePath,
	std::function<bool(LevelObjectData& objData)> hookFunc
)
{
	//�X�P���g�������[�h����B
	Skeleton skeleton;
	//skeleton.Load(filePath);
	//skeleton.Init(filePath);
	skeleton.InitLevel(filePath);

	//�\�z�\�z�B
	//0�Ԗڂ̓��[�g�I�u�W�F�N�g�Ȃ̂Ŕ�΂��B
	for (auto i = 1; i < skeleton.GetNumBones(); i++) {
		//�����擾�B
		auto bone = skeleton.GetBone(i);
		if (bone->GetParentBoneNo() == 0) {	//�e�����[�g�̏ꍇ�����}�b�v�`�b�v�𐶐�����B
			LevelObjectData objData;
			Vector3 scale;
			bone->CalcWorldTRS(objData.position, objData.rotation, objData.scale);
			//3dsMax�Ƃ͎����Ⴄ�̂ŁA�␳������B
			auto t = objData.position.y;
			objData.position.y = objData.position.z;
			objData.position.z = -t;

			t = objData.rotation.y;
			objData.rotation.y = objData.rotation.z;
			objData.rotation.z = -t;
			objData.name = bone->GetName();

			std::swap(objData.scale.y, objData.scale.z);

			auto isHook = false;
			if (hookFunc != nullptr) {
				//hook�֐����w�肳��Ă���̂�hook�֐����Ăяo���B
				isHook = hookFunc(objData);
			}
			if (isHook == false) {
				//�}�b�v�`�b�v�����_���[���쐬����B
				//CreateMapChipRenderOrAddRenderObject(objData);
				m_mapChipPtrs.push_back(std::make_unique<CMapChip>(objData));
			}
		}
	}
	//�}�b�v�`�b�v�����_���[������������B
	//for (auto& mapChipRender : m_mapChipRenderPtrs) {
	//	mapChipRender.second->InitAfterAddAllRenderObjects();
	//	mapChipRender.second->QueryRenderObjDatas([&](const LevelObjectData& objData) {
	//		//�t�b�N����Ȃ������̂ŁA�}�b�v�`�b�v���쐬����B
	//		auto mapChip = std::make_unique<CMapChip>(objData, mapChipRender.second);
	//		m_mapChipPtrs.push_back(std::move(mapChip));
	//		});
	//}



}