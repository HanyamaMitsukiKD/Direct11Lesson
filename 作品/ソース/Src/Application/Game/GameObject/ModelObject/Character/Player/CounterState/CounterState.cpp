#include "CounterState.h"
#include"Application/main.h"
#include"Application/Game/KdGameSystem.h"
#include"System/Manager/KdThrowManager/KdThrowManager.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/Player.h"
#include"Application/Game/GameObject/ModelObject/Character/Enemy/normalEnemy/normalEnemy.h"

void CounterState::Update()
{
	std::shared_ptr<Player> spPlayer = Application::Instance().GetGameSystem()->GetPlayer();
	std::shared_ptr<normalEnemy> spNormalEnemy = Application::Instance().GetGameSystem()->GetEnemy();

	m_count++;

	KdThrowManager::GetInstance().Init();

	if (m_count > 300)
	{
		m_count = 0;
		counterFlg = false;
		spPlayer->ResetCounterCount();
	}
	else
	{
		spNormalEnemy->CreateUltDamageEffect();
		spNormalEnemy->SetObjectSpeed(0.2f);
		spPlayer->SetObjectSpeed(1.5f);
	}
}