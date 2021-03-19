#pragma once

//COBBの初期化用の構造体
struct SInitOBBData
{
	Vector3 position = g_vec3Zero;			//場所
	float width = 1.0f;						//Xの辺の長さ
	float height = 1.0f;					//Yの辺の長さ
	float length = 1.0f;					//Zの辺の長さ
	Quaternion rotation = g_quatIdentity;	//回転
	Vector3 pivot = { 0.5f, 0.5f, 0.5f };	//ピボット（基点）
};


/// <summary>
/// OBBクラス（Oriented Bounding Box）
/// 当たり判定用
/// </summary>
class COBB
{
public:		//自動で呼ばれるメンバ関数
	COBB();		//コンストラクタ

public:		//ここのメンバ関数を主に使う

	/// <summary>
	/// 初期化関数。
	/// 最初に呼んでね。
	/// </summary>
	/// <param name="initData">COBBの初期化用の構造体</param>
	void Init(SInitOBBData& initData);

	/// <summary>
	/// 場所の設定
	/// </summary>
	/// <param name="pos">場所</param>
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
		//センターポジションを計算する
		CalcCenterPosition();
	}

	/// <summary>
	/// 場所の取得
	/// </summary>
	/// <returns>場所</returns>
	const Vector3 GetPosition()const
	{
		return m_position;
	}

	/// <summary>
	/// センターポジションの取得
	/// </summary>
	/// <returns>センターポジション</returns>
	const Vector3 GetCenterPosition()const
	{
		return m_centerPosition;
	}

	/// <summary>
	/// ピボット（基点）を設定する
	/// </summary>
	/// <param name="pivot">ピボット（基点）</param>
	void SetPivot(const Vector3& pivot)
	{
		m_pivot = pivot;
		//センターポジションを計算する
		CalcCenterPosition();
	}

	/// <summary>
	/// 回転を設定する
	/// </summary>
	/// <param name="rot">回転</param>
	void SetRotation(const Quaternion& rot)
	{
		Rotating(rot);
		//センターポジションを計算する
		CalcCenterPosition();
	}

	void SetDirectionLength(const Vector3& directionLength)
	{
		m_directionLength[enLocalX] = directionLength.x / 2;
		m_directionLength[enLocalY] = directionLength.y / 2;
		m_directionLength[enLocalZ] = directionLength.z / 2;
		//センターポジションを計算する
		CalcCenterPosition();
	}

	/// <summary>
	/// ボックスの8つの頂点の座標を取得する。
	/// （注意）配列の先頭アドレスを戻しているけど、
	/// 配列の要素数は8だよ。
	/// </summary>
	/// <returns>頂点の配列の先頭アドレス</returns>
	Vector3* GetBoxVertex();


	/// <summary>
	/// 引数で指定したローカル軸の単位方向ベクトルの取得
	/// </summary>
	/// <param name="localAxis">戻したい単位方向ベクトルのローカル軸</param>
	/// <returns>単位方向ベクトル</returns>
	const Vector3 GetNormalDirection(const int localAxis)const
	{
		return m_normalDirection[localAxis];
	}
	/// <summary>
	/// 引数で指定したローカル軸の方向ベクトルの長さ
	/// </summary>
	/// <param name="localAxis">戻したい方向ベクトルの長さのローカル軸</param>
	/// <returns>方向ベクトルの長さ</returns>
	const float GetDirectionLength(const int localAxis)const
	{
		return m_directionLength[localAxis];
	}

private:	//privateなメンバ関数
	/// <summary>
	/// 単位方向ベクトルを回す関数
	/// </summary>
	/// <param name="rot">回転</param>
	void Rotating(const Quaternion& rot);
	/// <summary>
	/// ピボットとポジションをもとに、
	/// センターポジションを計算する。
	/// </summary>
	void CalcCenterPosition();
public:		//publicなデータメンバ
	//オブジェクトのローカルな軸
	enum ENLocalAxis
	{
		enLocalX,		//ローカルX軸
		enLocalY,		//ローカルY軸
		enLocalZ,		//ローカルZ軸
		enLocalAxisNum,	//ローカル軸の数
	};
private:	//データメンバ
	Vector3 m_position;							//場所
	Vector3 m_pivot;							//ピボット（基点）
	Vector3 m_centerPosition;					//センターポジション（OBBの中央の場所）
	
	Vector3 m_normalDirection[enLocalAxisNum];	//各軸の単位方向ベクトル
	float m_directionLength[enLocalAxisNum];	//各軸の方向ベクトルの長さ

};

//クラス外
//関数


/// <summary>
/// OBB同士の当たり判定
/// 当たったらtrueを戻す
/// </summary>
/// <param name="obb1">片方のOBB</param>
/// <param name="obb2">もう片方のOBB</param>
/// <returns>当たったかどうか</returns>
const bool CollisionOBBs(COBB& obb1, COBB& obb2);

/// <summary>
/// 射影線分を計算する関数
/// </summary>
/// <param name="sepAxis">射影される分離軸（正規化済みを渡すこと）</param>
/// <param name="dirVec_X">射影する方向ベクトルX（大きさ付きを渡すこと）</param>
/// <param name="dirVec_Y">射影する方向ベクトルY（大きさ付きを渡すこと）</param>
/// <param name="dirVec_Z">射影する方向ベクトルZ（大きさ付きを渡すこと）(Zはなくても良い)</param>
/// <returns>射影線分</returns>
const float CalcProjectionLen(
	const Vector3& sepAxis,
	const Vector3& dirVec_X,
	const Vector3& dirVec_Y,
	const Vector3& dirVec_Z = { 0.0f,0.0f,0.0f }
);