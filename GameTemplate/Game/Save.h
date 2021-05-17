#pragma once


/// <summary>
/// セーブデータの構造体
/// </summary>
struct SSaveData
{
	int highestClearStageNum;	//一番高いクリアしたステージの番号
};



/// <summary>
/// セーブクラス
/// コンストラクタでロードが入る
/// </summary>
class CSave
{
public:		//自動で呼ばれるメンバ関数
	CSave();	//コンストラクタ

public:		//メンバ関数

	/// <summary>
	/// データメンバのセーブデータをセーブする
	/// </summary>
	void SaveData();

	/// <summary>
	/// セーブデータをデータメンバにロードする
	/// </summary>
	/// <returns>ロードできたか？</returns>
	const bool LoadData();

	/// <summary>
	/// データメンバに入っているセーブデータを得る
	/// </summary>
	/// <returns>セーブデータ</returns>
	const SSaveData& GetSaveData()const
	{
		return m_saveData;
	}

	/// <summary>
	/// セーブデータをデータメンバにセットする
	/// </summary>
	/// <param name="saveData">セーブデータを</param>
	void SetSaveData(const SSaveData& saveData)
	{
		m_saveData = saveData;
	}
	
private:	//データメンバ

	SSaveData m_saveData;	//セーブデータ
};

