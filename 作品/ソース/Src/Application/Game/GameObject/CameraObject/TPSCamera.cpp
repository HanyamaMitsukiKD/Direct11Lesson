#include "TPSCamera.h"

void TPSCamera::Init()
{
	CameraBase::Init();

	m_FixMousePos.x = 640;
	m_FixMousePos.y = 360;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);
}

void TPSCamera::UpdateRotateByMouse()
{
	// マウスでカメラを回転させる処理
	POINT nowPos;
	GetCursorPos(&nowPos);

	POINT mouseMove;
	mouseMove.x = nowPos.x - m_FixMousePos.x;
	mouseMove.y = nowPos.y - m_FixMousePos.y;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	// カメラを回転させる処理
	m_DegAng.x += mouseMove.y * 0.25f;
	m_DegAng.y += mouseMove.x * 0.25f;

	// 回転制御
	m_DegAng.x = std::clamp(m_DegAng.x, -FLT_MAX, FLT_MAX);
}

const Math::Matrix TPSCamera::GetRotationMatrix() const
{
	return	Math::Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(m_DegAng.y),
		DirectX::XMConvertToRadians(m_DegAng.x),
		DirectX::XMConvertToRadians(m_DegAng.z));
}

void TPSCamera::Update()
{
	// 以下の改造をして下さい
	//①カメラがキャラクターについてくるようにして下さい。
	Math::Matrix localGazePos = Math::Matrix::Identity;
	std::shared_ptr<KdGameObject> spTarget = m_wpTarget.lock();
	if (spTarget)
	{
		Math::Vector3 playerPos = spTarget->GetPos();
		playerPos.y += 2.0f;

		localGazePos = Math::Matrix::
			CreateTranslation(playerPos);
	}

	//②任意位置(キャラクター本体でも可)を中心にカメラが回転する
	// キャラクターから回転したい位置までのローカル座標
	m_LocalPos = Math::Matrix::CreateTranslation(spTarget->GetCameraPos());

	// カメラの回転
	UpdateRotateByMouse();
	//m_Rotation = GetRotationMatrix();
	m_Rotation = GetRotationMatrix();

	// 最終的な行列を合成する
	m_mWorld = m_LocalPos * m_Rotation * localGazePos;

	// 親クラスの更新を呼び出す
	CameraBase::Update();
}