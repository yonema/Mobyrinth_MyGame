#pragma once
#include "CommonData.h"

//レベルオブジェクトの抽象クラス
class ILevelObjectBase;

//プレイヤークラス
class Player;

//ステージのメビウスの輪クラス
class CMobius;


/// <summary>
/// レベルオブジェクトのマネージャー
/// </summary>
class CLevelObjectManager
{
private:	//自動で呼ばれるメンバ関数
	CLevelObjectManager();		//コンストラクタをprivateに隠す
	~CLevelObjectManager();		//デストラクタをprivateに隠す

private:	//staticなデータメンバ
	static CLevelObjectManager* m_instance;	//自身の唯一のインスタンスを持つ変数

public:		//staticなメンバ関数

	/// <summary>
	/// シングルトンパターン
	/// 唯一のインスタンスを作る関数
	/// 最初に呼んでね！
	/// </summary>
	static void CreateInstance()
	{
		//唯一のインスタンスを生成する
		m_instance = new CLevelObjectManager;
	}

	/// <summary>
	/// インスタンスを消す関数
	/// </summary>
	static void DeleteInstance()
	{
		//唯一のインスタンスを破棄する
		delete m_instance;
	}

	/// <summary>
	/// 唯一インスタンスの参照を戻す関数
	/// </summary>
	/// <returns>唯一インスタンスの参照</returns>
	static CLevelObjectManager* GetInstance()
	{
		return m_instance;
	}

public:		//メンバ関数

	/// <summary>
	/// プレイヤーの参照を得る
	/// </summary>
	/// <returns>プレイヤーの参照</returns>
	Player* GetPlayer()const
	{
		return m_player;
	}

	/// <summary>
	/// プレイヤーの参照を設定する
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	void SetPlayer(Player* player)
	{
		m_player = player;
	}


	/// <summary>
	/// ウェイポイントの「場所」を初期化
	/// </summary>
	/// <param name="vecSize">ウェイポイントのサイズ</param>
	/// <param name="posMap">場所のマップ</param>
	void InitWayPointPos(std::map<int, Vector3>& posMap);

	/// <summary>
	/// ウェイポイントの「回転」を初期化
	/// </summary>
	/// <param name="vecSize">ウェイポイントのサイズ</param>
	/// <param name="rotMap">回転のマップ</param>
	void InitWayPointRot(std::map<int, Quaternion>& rotMap);

	/// <summary>
	/// ウェイポイントの「場所」の参照を得る
	/// </summary>
	/// <returns>ウェイポイントの「場所」の参照</returns>
	std::vector<Vector3>* GetWayPointPos()
	{
		return &m_wayPointPos;
	}

	/// <summary>
	/// ウェイポイントの「回転」の参照を得る
	/// </summary>
	/// <returns>ウェイポイントの「回転」の参照</returns>
	std::vector<Quaternion>* GetWayPointRot()
	{
		return &m_wayPointRot;
	}

	/// <summary>
	/// ウェイポイントのベクターのサイズを得る
	/// </summary>
	/// <returns>ウェイポイントのベクターのサイズ</returns>
	int GetVecSize()const
	{
		return m_vecSize;
	}

	/// <summary>
	/// ウェイポイント上での移動先を計算する関数
	/// </summary>
	/// <param name="rpIndex">現在の右側のウェイポイントのインデックス</param>
	/// <param name="pos">現在の座標</param>
	/// <param name="dist">移動する距離</param>
	/// <param name="leftOrRight">右側に移動するか左側い移動するか。0:左,1:右</param>
	/// <returns>移動先の座標</returns>
	const Vector3 CalcWayPointNextPos
	(const int rpIndex, const Vector3& pos, const float dist, const bool leftOrRight, int* pNextIndex = nullptr);

	/// <summary>
	/// 自身（LevelObjectManager）にオブジェクトを追加する
	/// </summary>
	/// <param name="object">追加するオブジェクト</param>
	void AddObject(ILevelObjectBase* object);

	/// <summary>
	/// 自身（LevelObjectManager）からオブジェクトを破棄する
	/// </summary>
	/// <param name="object">破棄するオブジェクト</param>
	void RemoveObject(ILevelObjectBase* object);

	/// <summary>
	/// 自身（LevelObjectManager）に登録してあるオブジェクトの中から
	/// クエリ（問い合わせ）をする。
	/// タイプ一覧はObjectType.hを参照
	/// </summary>
	/// <typeparam name="T">クエリするオブジェクトの型</typeparam>
	/// <param name="objectType">クエリするオブジェクトのタイプ</param>
	/// <param name="func">クエリして実行したい関数</param>
	template<class T>
	void QueryLevelObjects(const int objectType, std::function<bool(T* lo)> func)
	{
		//自身（LevelObjectManager）に登録してあるオブジェクトの中から
		//一個ずつ取り出してクエリしていく
		for (auto lo : m_levelObjects) {
			//指定したタイプと一致するオブジェクトを調べる
				if (lo->GetObjectType() == objectType) {
					//見つけた。
					T* p = dynamic_cast<T*>(lo);
					//関数を実行する
					if (func(p) == false) {
						//クエリ中断。
						return;
					}
				}
		}
	}

	/// <summary>
	/// オブジェクトをすべて検索して
	/// 自分自身以外で重なっているものを探す。
	/// </summary>
	/// <param name="thisObject">自分自身の情報</param>
	/// <param name="objectType">自分自身の名前</param>
	/// <returns></returns>
	bool QueryLevelAllObjects(ILevelObjectBase& thisObject, const int objectType);


	/// <summary>
	/// レベルオブジェクトをすべて消去する
	/// </summary>
	void AllDeleteLOs();

	/// <summary>
	/// 表側か裏側かの、反転オブジェクトの数を加算する
	/// </summary>
	/// <param name="frontOrBackSide">表側か裏側か</param>
	void AddReversibleObjectNum(const int frontOrBackSide)
	{
		m_reversibleObjectNum[frontOrBackSide]++;
	}

	/// <summary>
	/// 表側か裏側かの、反転オブジェクトの数を減算する
	/// </summary>
	/// <param name="frontOrBackSide">表側か裏側か</param>
	void RemoveReversibleObjectNum(const int frontOrBackSide)
	{
		m_reversibleObjectNum[frontOrBackSide]--;
	}

	/// <summary>
	/// 反転オブジェクトの、表側と裏側のそれぞれの数を戻す。
	/// 配列の先頭アドレスを戻す
	/// </summary>
	/// <returns>数の配列の先頭アドレス</returns>
	const int* GetReversibleObjectNum() const
	{
		return m_reversibleObjectNum;
	}

	/// <summary>
	/// 反転オブジェクトの、表側と裏側のそれぞれの最大数を設定する
	/// </summary>
	/// <param name="frontOfBackSide">表側か裏側か？</param>
	/// <param name="maxNum">最大数</param>
	void SetReversibleObjectMaxNum(const int frontOfBackSide, const int maxNum)
	{
		m_reversibleObjectMaxNum[frontOfBackSide] = maxNum;
	}

	/// <summary>
	/// 反転オブジェクトの、表側と裏側のそれぞれの最大数を戻す。
	/// 配列の先頭アドレスを戻す
	/// </summary>
	/// <returns>数の配列の先頭アドレス</returns>
	const int* GetReversibleObjectMaxNum()
	{
		return m_reversibleObjectMaxNum;
	}
		

	/// <summary>
	/// プレイヤーに一番近いオブジェクトのオブジェクトタイプを戻す
	/// </summary>
	/// <returns>オブジェクトタイプ</returns>
	const int GetNearestObjectType(const float nearDist = 500.0f);


	/// <summary>
	/// 配置していあるレベルオブジェクトたちの参照のベクター
	/// </summary>
	/// <returns>レベルオブジェクトたちの参照のベクター</returns>
	std::vector<ILevelObjectBase*> GetLevelObjects()
	{
		return m_levelObjects;
	}

	/// <summary>
	/// ステージのメビウスの輪の参照を得る
	/// </summary>
	/// <returns>メビウスの輪の参照</returns>
	CMobius* GetMobius()const
	{
		return m_mobius;
	}

	/// <summary>
	/// ステージのメビウスの輪の参照を設定する
	/// </summary>
	/// <param name="mobius">メビウスの輪の参照</param>
	void SetMobius(CMobius* mobius)
	{
		m_mobius = mobius;
	}

	/**
	 * @brief 表側にいるのか裏側にいるのかを調べる
	 * @param [in] lpIndex 自身の左側のウェイポイントの番号
	 * @return 表側か裏側か
	*/
	const int CheckFrontOrBackSide(const int lpIndex);

private:	//privateなメンバ関数

	/**
	 * @brief 表側と裏側のしきい値の初期化
	*/
	void InitFrontOrBackSideThreshold();

public:	//列挙体
	/// <summary>
	/// 表側か裏側か
	/// </summary>
	enum EnFrontOrBackSide
	{
		enFrontSide,			//表側
		enBackSide,				//裏側
		enFrontOrBackSideNum,	//表と裏の数
		enNone = -1,			//無し
	};
private:	//データメンバ
	Player* m_player = nullptr;				//プレイヤーのポインタ
	std::vector<Vector3> m_wayPointPos;		//ウェイポイントの「場所」のコンテナ
	std::vector<Quaternion> m_wayPointRot;	//ウェイポイントの「回転」のコンテナ
	int m_vecSize = 0;						//ウェイポイントステートの最大の値
	int m_frontOrBackSideThresholdSmall = 0;	//表側と裏側のしきい値の小さい方
	int m_frontOrBackSideThresholdBig = 0;		//表側と裏側のしきい値の大きい方

	std::vector<ILevelObjectBase*> m_levelObjects;	//インスタンスしたレベルオブジェクトの配列
	//反転オブジェクトの、表側と裏側のそれぞれの数
	int m_reversibleObjectNum[enFrontOrBackSideNum] = { 0,0 };
	//反転オブジェクトの、表側と裏側のそれぞれの最大数
	int m_reversibleObjectMaxNum[enFrontOrBackSideNum] = { 0,0 };
	CMobius* m_mobius = nullptr;				//ステージのメビウスの輪のポインタ
};


/// <summary>
/// LevelObjectManagerに登録してあるオブジェクトの中から
/// クエリ（問い合わせ）をする。
/// タイプ一覧はObjectType.hを参照
/// </summary>
/// <typeparam name="T">クエリするオブジェクトの型</typeparam>
/// <param name="objectType">クエリするオブジェクトのタイプ</param>
/// <param name="func">クエリして実行したい関数</param>
template<class T>
static inline void QueryLOs(const int objectType, std::function<bool(T* lo)> func)
{
	//LevelObjectManagerの参照を持ってきて、
	//クエリのメンバ関数を引っ張ってくる。
	return CLevelObjectManager::GetInstance()->QueryLevelObjects(objectType, func);
}

static inline bool QueryAllLOs(ILevelObjectBase& thisObject, const int objectType)
{
	return CLevelObjectManager::GetInstance()->QueryLevelAllObjects(thisObject, objectType);
}