#pragma once

/**
 * @brief メビリンス
*/
namespace nsMobyrinth
{
	/**
	 * @brief セーブ
	*/
	namespace nsSave
	{
		/**
		 * @brief セーブデータの構造体
		*/
		struct SSaveData
		{
			int highestClearStageNum;	//一番高いクリアしたステージの番号
		};

		/**
		 * @brief セーブクラスの定数データ
		*/
		namespace
		{
			//セーブデータのファイルパス
			const char* const SAVE_DATA_FILEPATH = "Assets/saveData/saveData.dat";
			//バイナリファイルで書き込むモード
			const char* const MODE_WRITE_BINARY = "wb";
			//バイナリばいるで読み込むモード
			const char* const MODE_READ_BINARY = "rb";
		}


		/**
		 * @brief セーブクラス
		 * @note コンストラクタでロードが入る
		*/
		class CSave
		{
		public:		//自動で呼ばれるメンバ関数
			CSave();	//コンストラクタ

		public:		//メンバ関数

			/**
			 * @brief データメンバのセーブデータをセーブする
			*/
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

	}
}