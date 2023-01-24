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
	// �}�E�X�ŃJ��������]�����鏈��
	POINT nowPos;
	GetCursorPos(&nowPos);

	POINT mouseMove;
	mouseMove.x = nowPos.x - m_FixMousePos.x;
	mouseMove.y = nowPos.y - m_FixMousePos.y;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	// �J��������]�����鏈��
	m_DegAng.x += mouseMove.y * 0.25f;
	m_DegAng.y += mouseMove.x * 0.25f;

	// ��]����
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
	// �ȉ��̉��������ĉ�����
	//�@�J�������L�����N�^�[�ɂ��Ă���悤�ɂ��ĉ������B
	Math::Matrix localGazePos = Math::Matrix::Identity;
	std::shared_ptr<KdGameObject> spTarget = m_wpTarget.lock();
	if (spTarget)
	{
		Math::Vector3 playerPos = spTarget->GetPos();
		playerPos.y += 2.0f;

		localGazePos = Math::Matrix::
			CreateTranslation(playerPos);
	}

	//�A�C�ӈʒu(�L�����N�^�[�{�̂ł���)�𒆐S�ɃJ��������]����
	// �L�����N�^�[�����]�������ʒu�܂ł̃��[�J�����W
	m_LocalPos = Math::Matrix::CreateTranslation(spTarget->GetCameraPos());

	// �J�����̉�]
	UpdateRotateByMouse();
	//m_Rotation = GetRotationMatrix();
	m_Rotation = GetRotationMatrix();

	// �ŏI�I�ȍs�����������
	m_mWorld = m_LocalPos * m_Rotation * localGazePos;

	// �e�N���X�̍X�V���Ăяo��
	CameraBase::Update();
}