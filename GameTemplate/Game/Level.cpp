#include "stdafx.h"
#include "Level.h"

/**
 * @brief メビリンス
*/
namespace nsMobyrinth
{
	/**
	 * @brief tkLevel
	*/
	namespace nsTkLevel
	{

		void CLevel::Init(
			const char* filePath,
			std::function<bool(LevelObjectData& objData)> hookFunc
		)
		{
			//スケルトンをロードする。
			Skeleton skeleton;
			skeleton.InitLevel(filePath);

			//構築構築。
			//0番目はルートオブジェクトなので飛ばす。
			for (auto i = 1; i < skeleton.GetNumBones(); i++) {
				//骨を取得。
				auto bone = skeleton.GetBone(i);
				if (bone->GetParentBoneNo() == 0) {	//親がルートの場合だけマップチップを生成する。
					LevelObjectData objData;
					Vector3 scale;
					bone->CalcWorldTRS(objData.position, objData.rotation, objData.scale);
					//3dsMaxとは軸が違うので、補正を入れる。
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
						//hook関数が指定されているのでhook関数を呼び出す。
						isHook = hookFunc(objData);
					}
					if (isHook == false) {
						m_mapChipPtrs.push_back(std::make_unique<CMapChip>(objData));
					}
				}
			}

		}
	}
}