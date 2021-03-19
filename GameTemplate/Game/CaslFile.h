#pragma once

/// <summary>
/// Caslファイルのデータの構造体
/// </summary>
struct CaslData
{
	std::unique_ptr<char[]> name;			//名前
	std::unique_ptr<char[]> fileName;	//ファイルパス
	std::unique_ptr<char[]> ddsFileName;		//ddsファイル
	std::unique_ptr<char[]> ddsFilePath;	//ddsファイルパス
	Vector2 position = g_vec2Zero;			//座標
	int width = 0;							//横幅
	int height = 0;							//縦幅
	int numberLayer = 0;					//レイヤー優先度
	Vector2 scale = { 1.0f,1.0f };			//大きさ
};




/// <summary>
/// caslファイルを扱うクラス
/// </summary>
class CaslFile
{
private:
	//CaslDataのユニークポインタ
	using CaslDataPtr = std::unique_ptr<CaslData>;
public:		//ここのメンバ関数を主に使う

	/// <summary>
	/// caslファイルを読み込む
	/// </summary>
	/// <param name="filePath">caslファイルパス</param>
	void Load(const char* filePath);

	/// <summary>
	/// Caslデータの数を取得
	/// </summary>
	/// <returns></returns>
	const int GetNumCaslData() const
	{
		return m_caslDataList.size();
	}

	/// <summary>
	/// Caslデータを取得
	/// </summary>
	/// <param name="number">ナンバー</param>
	/// <returns>Caslデータのポインタ</returns>
	CaslData* GetCaslData(int number) const
	{
		return m_caslDataList[number].get();
	}

private:	//privateなメンバ関数

	/// <summary>
	/// 1文字だけ読み込む。「 , 」や「 \n 」を読み込むのに使う
	/// </summary>
	/// <param name="file">ファイル</param>
	void ReadOnlyOneCharacter(FILE* file)const;

	/// <summary>
	/// int型を読み込む
	/// </summary>
	/// <param name="file">ファイル</param>
	/// <returns>int型の値</returns>
	int ReadInteger(FILE* file)const;

	/// <summary>
	/// float型を読み込む
	/// </summary>
	/// <param name="file">ファイル</param>
	/// <returns>float型の値</returns>
	float ReadDecimal(FILE* file)const;

private:	//データメンバ
	std::vector<CaslDataPtr> m_caslDataList;	//CaslDataのコンテナ
};

