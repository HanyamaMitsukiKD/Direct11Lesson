#pragma once

#include"CameraBase.h"

// 1人称視点チャレンジ(cppを自前で実装して下さい)
class TPSCamera : public CameraBase
{
public:
	TPSCamera() {};
	virtual ~TPSCamera() override {};

	void SetCameraMatrix(const DirectX::SimpleMath::Matrix& mMat)
	{
		m_mWorld = mMat;
	}

	const Math::Matrix GetRotationMatrix() const;
	void UpdateRotateByMouse();

	void Init()		override;
	void Update()	override;

private:
	POINT			m_FixMousePos;
};