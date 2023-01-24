#pragma once

class KdDebug
{
public:


	static KdDebug& GetInstance()
	{
		static KdDebug instance;
		return instance;
	}

	void Update();
	void Init();
	void Draw();

	void PlayerDebugDraw();
	void EnemyDebugDraw();

	const bool GetDebugActiveFlg() const
	{
		return m_isActive;
	}

	bool IsActive() const { return m_isActive; }

	void SetDebugCameraToShader()const
	{
		m_camera.SetToShader();
	}

private:
	KdDebug() {}
	~KdDebug() {}

	//デバック関連
	bool m_isActive = false;
	bool m_pushedTab = false;

	KdCamera m_camera;	//シェアしないので普通の変数

	Math::Vector3 move;

	std::vector<KdEffectShader::Vertex> m_lines;

	POINT m_prevMousePos;
	//　カメラ回転用
	Math::Vector3 m_DegAng = Math::Vector3::Zero;

	Math::Matrix m_mWorld;
	Math::Matrix m_Rotation;
	Math::Matrix m_LocalPos;

	Math::Vector3				m_worldRot = Math::Vector3(0, 0, 0);
};