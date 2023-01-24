#include "CameraBase.h"

void CameraBase::Init()
{
	m_spCamera = std::make_shared<KdCamera>();

	m_spCamera->SetProjectionMatrix(60.0f);
}

void CameraBase::Update()
{
	if (!m_spCamera) { return; }

	m_spCamera->SetCameraMatrix(m_mWorld);
}

void CameraBase::SetProjectionMatrix(float fov, float maxRange,
	float minRange,
	float aspectRatio)
{
	if (!m_spCamera) { return; }

	m_spCamera->SetProjectionMatrix(fov, maxRange, minRange, aspectRatio);
}

void CameraBase::SetTarget(const std::shared_ptr<KdModelObject>& target)
{
	if (!target) { return; }

	m_wpTarget = target;
}
