#pragma once
class CShadowModel
{
public:
	CShadowModel();
	~CShadowModel();
	void Init();
	void InitModel(ModelInitData& initData);
	void UpdateModel(const Vector3& pos, const Quaternion& rot, const Vector3& scale);
	const bool IsValid() const
	{
		return m_isValid;
	}
private:
	std::unique_ptr<Model> m_model = nullptr;
	bool m_isValid = false;
};

