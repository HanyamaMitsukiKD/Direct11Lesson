#include "KdGameAudioManager.h"
#include"Application/main.h"
#include"Application/Game/KdGameSystem.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/Player.h"
#include"Application/Game/GameObject/ModelObject/Character/Enemy/normalEnemy/normalEnemy.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/CounterState/CounterState.h"

void KdGameAudioManager::Update()
{
	const std::shared_ptr<Player>& spPlayer =
		Application::Instance().GetGameSystem()->GetPlayer();

	const std::shared_ptr<normalEnemy>& spEnemy =
		Application::Instance().GetGameSystem()->GetEnemy();

	if (!spPlayer) { return; }

	// Player
	if (spPlayer)
	{
		// 移動音
		if (spPlayer->GetMoveFlg() && spPlayer->GeteState() == KdGameObject::eMove)
		{
			if (!m_aWalk->IsPlaying())
			{
				m_aWalk->Play(true);
				m_aWalk->SetVolume(2.0f);
			}
		}
		else
		{
			if (m_aWalk->IsPlaying())
			{
				m_aWalk->Stop();
			}
		}

		// カウンターカウント音
		if (spPlayer->GetCounterCountFlg())
		{
			if (!m_counterCountSEFlg)
			{
				if (!m_aCounterCount->IsPlaying())
				{
					m_aCounterCount->Play(false);
					m_counterCountSEFlg = true;
					spPlayer->SetCounterCountFlg(false);
				}
			}
		}
		else
		{
			m_counterCountSEFlg = false;
		}

		// ウルト突入音
		if (CounterState::GetInstance().GetCounterFlg())
		{
			if (!m_UltSEFlg)
			{
				if (!m_aTakkuru->IsPlaying())
				{
					m_aTakkuru->Play(false);
					m_UltSEFlg = true;
				}
			}
		}
		else
		{
			m_UltSEFlg = false;
		}

		// エネミーとプレイヤーの激突音
		if (spPlayer->GeteState() == KdGameObject::eDamage)
		{
			if (!m_TakkuruSEFlg)
			{
				if (!m_aTakkuru->IsPlaying())
				{
					m_aTakkuru->Play(false);
					m_TakkuruSEFlg = true;
				}
			}
		}
		else
		{
			m_TakkuruSEFlg = false;
		}
	}

	// エネミー
	if (spEnemy)
	{
		//ミサイル音、構え音
		if (spEnemy->GetEnemyBulletCreateFlg())
		{
			if (!m_MisailSEFlg)
			{
				if (!m_aMisail->IsPlaying())
				{
					m_aMisail->Play(false);

					m_MisailStanbyCount++;
					if (m_MisailStanbyCount % 2 == 1)
					{
						m_aMisailStanby->Play(false);
					}

					spEnemy->SetEnemyBulletCreateFlg(false);
					m_MisailSEFlg = true;

				}
			}
		}
		else
		{
			m_MisailSEFlg = false;
		}


		//剣が激突した音
		if (spEnemy->GetDamageFlg())
		{
			if (!m_SwordSEFlg)
			{
				if (!m_aSword->IsPlaying())
				{
					m_aSword->Play(false);
					m_SwordSEFlg = true;
				}
			}
		}
		else
		{
			m_SwordSEFlg = false;
		}
	}
}

void KdGameAudioManager::Init()
{
	m_audioManager.Init();

	// Player
	m_audioManager.AudioAsset("Asset/SE/walk.wav", m_aWalk);
	m_audioManager.AudioAsset("Asset/SE/counterCount.wav", m_aCounterCount);
	m_audioManager.AudioAsset("Asset/SE/sword.wav", m_aSword);
	m_audioManager.AudioAsset("Asset/SE/takkuru.wav", m_aTakkuru);
	m_audioManager.AudioAsset("Asset/SE/throw.wav", m_aThrowMotion);

	// Enemy
	m_audioManager.AudioAsset("Asset/SE/misail.wav", m_aMisail);
	m_audioManager.AudioAsset("Asset/SE/misailStanby.wav", m_aMisailStanby);

	m_counterCountSEFlg = false;
	m_UltSEFlg = false;
	m_TakkuruSEFlg = false;
}

void KdGameAudioManager::SoundStop()
{
	m_audioManager.StopAllSound();
}

void KdGameAudioManager::PlayTitleBGM()
{
	m_audioManager.Play("Asset/BGM/title.wav", true);
}

void KdGameAudioManager::PlayGameBGM()
{
	m_audioManager.Play("Asset/BGM/gameBGM1.wav", true);
}

void KdGameAudioManager::PlayGameBGM2()
{
	m_audioManager.StopAllSound();
	m_audioManager.Play("Asset/BGM/gameBGM2.wav", true);
}

void KdGameAudioManager::BomSE()
{
	// 爆発音
	m_audioManager.Play("Asset/SE/bom.wav", false);
}

void KdGameAudioManager::mailSE()
{
	// 通知音
	m_audioManager.Play("Asset/SE/mail.wav", false);
}

void KdGameAudioManager::messageSE()
{
	// メッセージを開く音
	m_audioManager.Play("Asset/SE/message.wav", false);
}

void KdGameAudioManager::avoidanceSE()
{
	// 回避音
	m_audioManager.Play("Asset/SE/jump.wav", false);
}

void KdGameAudioManager::takkuruMoveSE()
{
	// エネミーがタックルするときの移動音
	m_audioManager.Play("Asset/SE/takkuruMove.wav", false);
}

void KdGameAudioManager::takkuruStandSE()
{
	// エネミーがタックルを構えているときの準備音
	m_audioManager.Play("Asset/SE/takkuruStandBY.wav", false);
}

void KdGameAudioManager::sowrdSlasheSE()
{
	m_audioManager.Play("Asset/SE/SowrdSlash.wav", false);
}

void KdGameAudioManager::sceneChangeSE()
{
	// シーンチェンジするときの準備音
	m_audioManager.Play("Asset/SE/sceneChange.wav", false);
}

void KdGameAudioManager::counterAttackSE()
{
	//カウンターするときの音
	m_audioManager.Play("Asset/SE/counterAttack.wav", false);
}

void KdGameAudioManager::attackStartSE()
{
	m_audioManager.Play("Asset/SE/attackStart.wav", false);
}
