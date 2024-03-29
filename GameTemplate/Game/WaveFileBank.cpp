#include "stdafx.h"
#include "WaveFileBank.h"

/**
 * @brief メビリンス
*/
namespace nsMobyrinth
{
	/**
	 * @brief サウンド
	*/
	namespace nsSound
	{

		CWaveFileBank::CWaveFileBank()
		{
		}
		CWaveFileBank::~CWaveFileBank()
		{
			ReleaseAll();
		}
		void CWaveFileBank::Release(int groupID)
		{
			for (auto waveFile : m_waveFileMap[groupID]) {
				waveFile.second->Release();
			}
			m_waveFileMap[groupID].clear();
		}
		void CWaveFileBank::UnregistWaveFile(int groupID, CWaveFilePtr waveFile)
		{
			//TK_ASSERT(groupID < MAX_GROUP, "groupID is invalid");
			if (!(groupID < MAX_GROUP))
				MessageBoxA(nullptr, "groupID is invalid", "エラー", MB_OK);
			m_waveFileMap->erase(waveFile->GetFilePathHash());
		}
		void CWaveFileBank::RegistWaveFile(int groupID, CWaveFilePtr waveFile)
		{
			//TK_ASSERT(groupID < MAX_GROUP, "groupID is invalid");
			if (!(groupID < MAX_GROUP))
				MessageBoxA(nullptr, "groupID is invalid", "エラー", MB_OK);
			m_waveFileMap[groupID].insert({ waveFile->GetFilePathHash(), waveFile });
		}
		CWaveFilePtr CWaveFileBank::FindWaveFile(int groupID, const wchar_t* filePath)
		{
			//TK_ASSERT(groupID < MAX_GROUP, "groupID is invalid");
			if (!(groupID < MAX_GROUP))
				MessageBoxA(nullptr, "groupID is invalid", "エラー", MB_OK);
			auto value = m_waveFileMap[groupID].find(CUtil::MakeHash(filePath));
			if (value != m_waveFileMap[groupID].end()) {
				return value->second;
			}
			return CWaveFilePtr();
		}


		//WNameKeyにはハッシュ値も保存されているから、いちいちハッシュ値を作る
		//必要がなくなるため、少し早くなる。
		//必要に感じたら作ろう
		//CWaveFilePtr CWaveFileBank::FindWaveFile(int groupID, const WNameKey& nameKey)
		//{
		//	TK_ASSERT(groupID < MAX_GROUP, "groupID is invalid");
		//	auto value = m_waveFileMap[groupID].find(nameKey.GetHashCode());
		//	if (value != m_waveFileMap[groupID].end()) {
		//		return value->second;
		//	}
		//	return CWaveFilePtr();
		//}
	}
}