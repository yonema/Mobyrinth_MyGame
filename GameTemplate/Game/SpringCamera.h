#pragma once
#include "GameTime.h"

/// <summary>
/// ばねカメラ
/// </summary>
class CSpringCamera
{
public:		//自動で呼ばれるメンバ関数
	CSpringCamera();		//コンストラクタ
	~CSpringCamera();		//デストラクタ

public:		//メンバ関数

	/// <summary>
	///	初期化
	/// コリジョン処理は現在はただの飾り。実装していない。
	/// </summary>
	/// <param name="camera">カメラ</param>
	/// <param name="maxMoveSpeed">カメラの最大移動速度</param>
	/// <param name="isEnableCollisionSolver">コリジョン処理を有効にする？</param>
	/// <param name="sphereCollisionRadius">球体コリジョンの半径</param>
	void Init(
		Camera& camera,
		float maxMoveSpeed,
		bool isEnableCollisionSolver,
		float sphereCollisionRadius
	);

	/// <summary>
	/// 目標となる注視点を設定。
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const Vector3& target)
	{
		m_target = target;
	}

	/// <summary>
	/// 目標となる視点を設定。
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(const Vector3 position)
	{
		m_position = position;
	}

	/// <summary>
	/// 遠平面を設定。
	/// </summary>
	/// <param name="_far"></param>
	void SetFar(float _far)
	{
		if (m_camera == nullptr) {
			return;
		}
		m_camera->SetFar(_far);
	}

	/// <summary>
	/// 近平面を設定。
	/// </summary>
	/// <param name="_near"></param>
	void SetNear(float _near)
	{
		if (m_camera == nullptr) {
			return;
		}
		m_camera->SetNear(_near);
	}

	/// <summary>
	/// カメラの上方向を設定
	/// </summary>
	/// <param name="up"></param>
	void SetUp(const Vector3 up)
	{
		if (m_camera == nullptr)
		{
			return;
		}
		m_camera->SetUp(up);

	}

	/// <summary>
	/// カメラの回転行列を取得。
	/// </summary>
	/// <returns></returns>
	const Matrix& GetCameraRotation() const
	{
		if (m_camera == nullptr) {
			return Matrix::Identity;
		}
		return m_camera->GetCameraRotation();
	}

	/// <summary>
	/// 目標となる注視点を取得。
	/// </summary>
	/// <returns></returns>
	const Vector3& GetTarget() const
	{
		return m_target;
	}

	/// <summary>
	/// 目標となる視点を取得。
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/// <summary>
	/// 実際の注視点を取得。
	/// </summary>
	/// <returns></returns>
	const Vector3& GetRealTarget() const
	{
		if (m_camera == nullptr) {
			return Vector3::Zero;
		}
		return m_camera->GetTarget();

	}

	/// <summary>
	/// 実際の視点を取得。
	/// </summary>
	/// <returns></returns>
	const Vector3& GetRealPosition() const
	{
		if (m_camera == nullptr) {
			return Vector3::Zero;
		}
		return m_camera->GetPosition();
	}

	/// <summary>
	/// 更新。
	/// </summary>
	void Update();

	/// <summary>
	/// カメラを取得。
	/// </summary>
	/// <returns></returns>
	const Camera* GetCamera() const
	{
		return m_camera;
	}

	/// <summary>
	/// バネの減衰率を設定。
	/// 値が大きいほどカメラが遅れて付いてきます
	/// </summary>
	/// <param name="dampingRate"></param>
	void SetDampingRate(float dampingRate)
	{
		m_targetDampingRate = dampingRate;
	}

	/// <summary>
	/// カメラを取得。
	/// </summary>
	/// <returns></returns>
	Camera* GetCamera()
	{
		return m_camera;
	}
	void SetViewAngle(float angle)
	{
		if (m_camera == nullptr) {
			return;
		}
		m_camera->SetViewAngle(angle);
	}
	float GetViewAngle() const
	{
		if (m_camera == nullptr) {
			return 0.0f;
		}
		return m_camera->GetViewAngle();
	}

	/// <summary>
	/// リフレッシュ
	/// この関数を呼び出すと、バネの移動がリフレッシュされて、Update関数を呼び出すとカメラの位置が
	/// 即座に目標視点と目標注視点に移動します。シーンが切り替わってカメラを一気に切り替えたいときなどに
	/// 使用してください。
	/// </summary>
	void Refresh()
	{
		m_targetMoveSpeed = Vector3::Zero;
		m_positionMoveSpeed = Vector3::Zero;
		m_isRefresh = true;
	}

	/// <summary>
	/// バネカメラの更新処理。
	/// </summary>
	void UpdateSpringCamera();

	/// <summary>
	/// カメラの更新。
	/// </summary>
	void UpdateCamera()
	{
		if (m_camera) {
			m_camera->Update();
		}
	}

private:	//データメンバ
	Camera* m_camera = nullptr;							//!<カメラ。
	Vector3	m_target = Vector3::Zero;					//!<目標となる注視点。
	Vector3	m_position = Vector3::Zero;					//!<目標となる視点。
	Vector3	m_targetMoveSpeed = Vector3::Zero;			//!<注視点の移動速度。
	Vector3	m_positionMoveSpeed = Vector3::Zero;		//!<視点の移動速度。
	float		m_maxMoveSpeed = 0.0f;					//!<最高移動速度。
	float		m_targetDampingRate = 1.0f;				//!<減衰率。値が大きいほどカメラが遅れ付いてくる。
	float		m_dampingRate = 1.0f;					//!<減衰率。
	float		m_dampingRateVel = 0.0f;
	bool		m_isEnableCollisionSolver = false;		//!<コリジョン処理が有効？
	bool		m_isRefresh = true;						//!<リフレッシュが必要？
	//CCameraCollisionSolver m_cameraCollisionSolver;
};

