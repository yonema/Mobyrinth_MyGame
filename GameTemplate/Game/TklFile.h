#pragma once
class TklFile
{
public:
	/// <summary>
	/// ボーン。
	/// </summary>
	struct SBone {
		//tklファイルのデータ
		std::unique_ptr<char[]> name;	//骨の名前。
		int parentNo = -1;				//親の番号。
		float bindPose[4][3];			//バインドポーズ。
		float invBindPose[4][3];		//バインドポーズの逆数。
		int no;							//ボーンの番号。

	};
	/// <summary>
	/// TKlファイルをロードする。
	/// </summary>
	/// <param name="filePath"></param>
	void Load(const char* filePath);

	/// <summary>
/// ボーンに対してクエリを行う。
/// </summary>
/// <param name="query">クエリ関数</param>
	void QueryBone(std::function<void(SBone& bone)> query)
	{
		for (auto& bone : m_bones) {
			query(bone);
		}
	}
private:
	int m_numBone = 0;			//骨の数。
	std::vector<SBone> m_bones;	//骨のリスト。
	using BonePtr = std::unique_ptr<SBone>;


};

