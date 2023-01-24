#include "KdThrowManager.h"
#include"Application/main.h"
#include"Application/Game/KdGameSystem.h"
#include"System/GameObject/KdGameObject.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/Player.h"
#include"System/Manager/KdAudioManager/KdGameAudioManager.h"

void KdThrowManager::Update()
{
	std::shared_ptr<Player> spPlayer = Application::Instance().GetGameSystem()->GetPlayer();

	auto& workCbCamera = D3D.WorkShaderManager().
		m_cb7_Camera.Work();

	// �W���X�g��𒆂Ȃ�X���[���[�V����
	if (spPlayer->GeteState() == KdGameObject::eJustAvoidance)
	{
		throwFlg = true;
	}
	else
	{
		throwFlg = false;
	}


	// �W���X�g��������Ă��Ȃ��Ȃ�I�u�W�F�N�g�X�s�[�h�͂P
	if (!throwFlg)
	{
		workCbCamera.GrayScaleValue = 0;

		throwSpeed = 1.0f;
		animationthrowSpeed = 0.0f;
		for (const std::shared_ptr<KdGameObject>& spObj :
			Application::Instance().GetGameSystem()->GetGameObjects())
		{
			spObj->SetObjectSpeed(throwSpeed);
		}
		return;
	}

	//��ԍŏ��ɃQ�[���X�s�[�h��0�ɂ���
	if (animationthrowSpeed == 0.0f)
	{
		if (KdGameAudioManager::GetInstance().GetTakkuruSE()->IsPlaying())
		{
			KdGameAudioManager::GetInstance().GetTakkuruSE()->Play(false);
		}
		throwSpeed = 0.0f;
	}

	//�X���[���[�V����
	if (throwSpeed <= 1.0f)
	{
		animationthrowSpeed += 0.01f;
	}

	if (throwSpeed > 1.0f)
	{
		//�X���[���[�V�����I��

		return;
	}

	//�ړ��ʂ𒲐�
	EaseInOutQuint(animationthrowSpeed);

	//���f���ɃX�s�[�h���Z�b�g
	for (const std::shared_ptr<KdGameObject>& spObj :
		Application::Instance().GetGameSystem()->GetGameObjects())
	{
		spObj->SetObjectSpeed(throwSpeed);

		if (spObj->GetClassID() == KdGameObject::ePlayer)
		{
			// �J�E���^�[�t���O��true�Ȃ�
			if (spPlayer->GetCounterCountFlg())
			{
				float counterSpeed = 0.7f;
				spPlayer->SetObjectSpeed(counterSpeed);
			}
		}
	}

	workCbCamera.GrayScaleValue = 1.0f - throwSpeed;
}

void KdThrowManager::Init()
{
	throwSpeed = 1.0f;
	animationthrowSpeed = 0.0f;

}

void KdThrowManager::Draw()
{
}

void KdThrowManager::EaseInOutQuint(float& x)
{
	float a = x * x * x;
	throwSpeed += a;
}
