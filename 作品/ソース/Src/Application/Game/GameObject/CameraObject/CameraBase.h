#pragma once

#include"Application/Game/GameObject/ModelObject/KdModelObject.h"

class CameraBase : public KdModelObject
{
public:
	CameraBase() {}
	virtual ~CameraBase() override {}

	void Init()		override;
	void Update()	override;

	std::shared_ptr<KdCamera> WorkCamera() const { return m_spCamera; }
	void SetTarget
	(const std::shared_ptr<KdModelObject>& target);

	void SetProjectionMatrix(float fov, float maxRange = 2000,
		float minRange = 0.01f,
		float aspectRatio = D3D.GetBackBuffer()->GetAspectRatio());

	const Math::Matrix GetRotationYMatrix() const
	{
		return Math::Matrix::
			CreateRotationY(DirectX::
				XMConvertToRadians(m_DegAng.y));
	}
protected:
	std::shared_ptr<KdCamera>	m_spCamera = nullptr;
	std::weak_ptr<KdModelObject> m_wpTarget;

	Math::Matrix				m_LocalPos;
	Math::Matrix				m_Rotation;

	//　カメラ回転用角度
	Math::Vector3				m_DegAng;
};