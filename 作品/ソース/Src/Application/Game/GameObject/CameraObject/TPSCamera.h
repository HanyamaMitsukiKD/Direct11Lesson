#pragma once

#include"CameraBase.h"

// 1�l�̎��_�`�������W(cpp�����O�Ŏ������ĉ�����)
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