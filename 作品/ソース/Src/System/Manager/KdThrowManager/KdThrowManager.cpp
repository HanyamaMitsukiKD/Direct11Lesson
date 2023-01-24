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

	// ジャスト回避中ならスローモーション
	if (spPlayer->GeteState() == KdGameObject::eJustAvoidance)
	{
		throwFlg = true;
	}
	else
	{
		throwFlg = false;
	}


	// ジャスト回避をしていないならオブジェクトスピードは１
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

	//一番最初にゲームスピードを0にする
	if (animationthrowSpeed == 0.0f)
	{
		if (KdGameAudioManager::GetInstance().GetTakkuruSE()->IsPlaying())
		{
			KdGameAudioManager::GetInstance().GetTakkuruSE()->Play(false);
		}
		throwSpeed = 0.0f;
	}

	//スローモーション
	if (throwSpeed <= 1.0f)
	{
		animationthrowSpeed += 0.01f;
	}

	if (throwSpeed > 1.0f)
	{
		//スローモーション終了

		return;
	}

	//移動量を調整
	EaseInOutQuint(animationthrowSpeed);

	//モデルにスピードをセット
	for (const std::shared_ptr<KdGameObject>& spObj :
		Application::Instance().GetGameSystem()->GetGameObjects())
	{
		spObj->SetObjectSpeed(throwSpeed);

		if (spObj->GetClassID() == KdGameObject::ePlayer)
		{
			// カウンターフラグがtrueなら
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
