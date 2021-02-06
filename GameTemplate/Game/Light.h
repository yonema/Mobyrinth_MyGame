#pragma once

 


class Light
{
private:
	static Light* m_instance;
	Light();
	~Light();
	void Init();
	void Release();
public:
	static void CreateInstance()
	{
		m_instance = new Light;
	}
	static Light* GetInstance()
	{
		return m_instance;
	}
	static void DeleteInstance()
	{
		delete m_instance;
	}

	//ディレクションライトの構造体
	struct SDirectionLight
	{
		//ディレクションライト
		Vector3 dirLigDirection;
		float pad;
		Vector3 dirLigColor;
		float pad2;
		Vector3 eyePos;			//カメラの位置。
		float specPow;				//スペキュラの絞り。
		Vector3 ambinetLight;	//環境光。	
	};
	SDirectionLight m_directionLight;
	//ポイントライトの構造体
	struct SPointLight
	{
		Vector3 ptPosition;		//位置。
		float ptRange;			//影響範囲。
		Vector3 ptColor;		//カラー。
	};
	SPointLight m_pointLight;
};

