#include "KdUIObject.h"
#include"System/Easing.h"

void KdUIObject::Init()
{
	// Player----------------------------------------------------------------------
	SetAsset(m_spPlayerHPTex, "Asset/Texture/UI/PlayerUI/HP.png");
	SetAsset(m_spPlayerHPGaugeTex, "Asset/Texture/UI/PlayerUI/HP gauge.png");
	SetAsset(m_spPlayerUltGaugeTex, "Asset/Texture/UI/PlayerUI/Ult gauge.png");

	// QTE
	SetAsset(m_spQTETex, "Asset/Texture/UI/PlayerUI/QTE2.png");
	SetAsset(m_spQTEPointTex, "Asset/Texture/UI/PlayerUI/QTEpoint 2.png");
	m_qtePointPushFlg = false;

	SetAsset(m_spULTTex, "Asset/Texture/UI/PlayerUI/ult.png");
	// ----------------------------------------------------------------------------

	// Enemy----------------------------------------------------------------------
	SetAsset(m_spEnemyHPTex, "Asset/Texture/UI/EnemyUI/enemy HP.png");
	SetAsset(m_spEnemyHPGaugeTex, "Asset/Texture/UI/EnemyUI/enemyGauge.png");

	// シーンチェンジ用----------------------------------------------------------
	SetAsset(m_spSceneChengeTex, "Asset/Texture/SceneChange/sceneChenge.png");
	SetAsset(m_spNowLoadingTex, "Asset/Texture/SceneChange/tuusin.png");

	// 画面外で待機
	m_sceneChengePos = { 0,1440 };

	m_sceneChengePosSpd = -20.0f;	//降ってくるスピード

	// シーンチェンジのリクエスト
	m_requestChangeSceneFlg = false;
	m_readyChangeGameSceneFlg = false;

	m_nowLoadingCount = 0;

	m_ultGugeSpeed = 1.0f / (60.0f * 1);
	m_ultGugesProgress = 0;
	//----------------------------------------------------------------------------

	SetAsset(m_spKeyUITex, "Asset/Texture/UI/PlayerUI/KeyUI.png");
}

void KdUIObject::Update()
{
	//=======================================
	// シーンチェンジが押されたら
	//=======================================
	if (m_requestChangeSceneFlg)
	{
		// yの値が0になったら
		if (m_sceneChengePos.y <= 0)
		{
			// 2秒経ったら
			m_nowLoadingCount++;

			// NowLoadingのアニメーション処理
			if (m_nowLoadingCount > 10)
			{
				if (m_nowLoadingCount % 10 == 0)
				{
					m_nowLoadingRec++;
					if (m_nowLoadingRec >= 4)
					{
						m_nowLoadingRec = 0;
					}
				}
			}

			if (m_nowLoadingCount >= 120)
			{
				//今度は上方向に上がっていく
				m_sceneChengePosSpd = -10.0f;
				m_sceneChengePosSpd *= -1;

				// if文から抜けるように補正
				m_sceneChengePos.y += m_sceneChengePosSpd + 1.0f;

				m_readyChangeGameSceneFlg = false;

				m_nowLoadingRec = 0;
			}

			m_readyChangeGameSceneFlg = true;
		}
		else
		{
			m_sceneChengePos.y += m_sceneChengePosSpd;
			m_nowLoadingRec = 0;
		}

		// 元の位置に戻ったらシーンチェンジ終了
		if (m_sceneChengePos.y > 1440)
		{
			m_requestChangeSceneFlg = false;
		}
	}
}

void KdUIObject::DrawSprite()
{
	if (m_SceneName == "Title")
	{

	}
	else if (m_SceneName == "Game")
	{
		//Player関係
		DrawPlayerHP();
		DrawPlayerUltGauge();
		DrawPlayerQTE();

		//エネミー状態
		DrawEnemyHP();

		// KeyUI
		D3D.WorkShaderManager().m_spriteShader.DrawTex(m_spKeyUITex.get(), 480, -280);
	}

	// シーンチェンジ用の画像
	if (m_requestChangeSceneFlg)
	{
		D3D.WorkShaderManager().m_spriteShader.DrawTex(m_spSceneChengeTex.get(), (int)m_sceneChengePos.x, (int)m_sceneChengePos.y);

		D3D.WorkShaderManager().m_spriteShader.DrawTex(m_spNowLoadingTex.get(),
			(int)m_sceneChengePos.x + 463, (int)m_sceneChengePos.y - 230, 275, 232, &Math::Rectangle{ m_nowLoadingRec * 275 ,0,275,232 });
	}

	
}

void KdUIObject::DrawPlayerHP()
{
	std::shared_ptr<Player> sp_player = m_wpPlayer.lock();

	PlayerHPMove();

	int playerHpTexPosX;
	playerHpTexPosX =(int) (playerHPTexPos.x + (((playerHPGauge.x - playerHPGaugeTexX) / 2)));

	if (sp_player->GetPlayerHP() >= 0)
	{
		D3D.WorkShaderManager().m_spriteShader.DrawTex
		(m_spPlayerHPTex.get(), (int)playerHPTexPos.x, (int)playerHPTexPos.y);

		D3D.WorkShaderManager().m_spriteShader.DrawTex
		(m_spPlayerHPGaugeTex.get(), playerHpTexPosX + 54, (int)playerHPTexPos.y - 7, (int)playerHPGauge.x, (int)playerHPGauge.y);
	}
}

void KdUIObject::DrawPlayerUltGauge()
{
	std::shared_ptr player = m_wpPlayer.lock();

	if (player->GetCounterCount() < 1) { return; }

	Math::Vector2 UltGaugeWH = { 83,114 };
	int UltGaugeH = 114;
	Math::Vector2 UltGaugePos = { playerHPTexPos.x + (-128),playerHPTexPos.y - 2 };
	int ultGaugeRatio = 3 / player->GetCounterCount();	// ウルトゲージの割合
	int  ultGauge = 0;
	int ultGaugeraite = 0;

	switch (player->GetCounterCount())
	{
	case 0:
		ultGaugeRatio = 0;

		break;
	case 1:
		ultGaugeRatio = 3;
		ultGaugeraite = 3;
		break;

	case 2:
		ultGaugeRatio = 2;
		ultGauge = (int)(UltGaugeWH.y / 3);
		break;

	case 3:
		ultGaugeRatio = 1;
		ultGauge = (int)(UltGaugeWH.y / 2);
		break;

	}

	m_ultGugesProgress += m_ultGugeSpeed;
	float progress = std::clamp(m_ultGugesProgress, 0.0f, 1.0f);


	// イージング計算
	ultGauge *= (int)(1 - EaseOutQuad(progress));
	UltGaugeWH.y = (UltGaugeWH.y / ultGaugeRatio) * EaseOutQuad(progress) + ultGauge;
	UltGaugePos.y = UltGaugePos.y + (((UltGaugeWH.y * EaseOutQuad(progress)) - UltGaugeH) / 2) + (ultGauge / 2);


	// ウルトゲージ
	D3D.WorkShaderManager().m_spriteShader.DrawTex
	(m_spPlayerUltGaugeTex.get(), (int)UltGaugePos.x, (int)UltGaugePos.y, (int)UltGaugeWH.x, (int)UltGaugeWH.y);
}

void KdUIObject::DrawPlayerQTE()
{
	std::shared_ptr player = m_wpPlayer.lock();
	ID3D11BlendState* blend = nullptr;
	Math::Matrix InitMat = Math::Matrix::Identity;
	Math::Matrix QteMat;

	// QTEボタンが押されない間移動
	if (player->GetQtePointFlg())
	{
		m_qtePointPushFlg = true;
	}

	// ジャスト回避中
	if (player->GeteState() == eState::eJustAvoidance)
	{

		// QTEポイントが移動
		if (!m_qtePointPushFlg)
		{
			// 回転
			m_qtePointMat *= Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(10.0f));
		}
		else
		{
			// 押されたら
			QteMat = m_qtePointMat;
			m_qtePointMat *= Math::Matrix::CreateScale(1.1f);
			QteMat = Math::Matrix::CreateScale(1.1f);

			m_qteAlpha -= 0.2f;

			UltGugesProgressExit();
		}

		D3D.WorkShaderManager().m_spriteShader.SetMatrix(QteMat);
		D3D.WorkShaderManager().m_spriteShader.DrawTex
		(m_spQTETex.get(), 0, 0, nullptr, &Math::Color(1, 1, 1, m_qteAlpha));

		D3D.WorkShaderManager().m_spriteShader.SetMatrix(m_qtePointMat);
		D3D.WorkShaderManager().m_spriteShader.DrawTex
		(m_spQTEPointTex.get(), 0, 0, nullptr, &Math::Color(1, 1, 1, m_qteAlpha));

		// 行列初期化
		D3D.WorkShaderManager().m_spriteShader.SetMatrix(InitMat);
	}
	else
	{
		m_qtePointPushFlg = false;
		m_qtePointMat = Math::Matrix::Identity;
		m_qteAlpha = 1.0f;
	}
}

void KdUIObject::PlayerHPMove()
{
	std::shared_ptr<Player> sp_player = m_wpPlayer.lock();

	if (!sp_player) { return; }

	if (!sp_player->GetDamageFlg())
	{
		energySpeed = 1.0f;
		animationEnergySpeed = 0.0f;
		playerHPGauge.x =(float) sp_player->GetPlayerHP();	//体力を持ってくる
		return;
	}


	int useEnergy = sp_player->GetHPDecreaseValue();				//受けたダメージ量
	int playerEnergy = sp_player->GetPlayerHP() + useEnergy;		//プレイヤーのHP


	if (animationEnergySpeed < 1.0f)
	{
		animationEnergySpeed += 0.1f;
	}
	else
	{
		sp_player->SetPlayerDamageFlg(false);
	}


	UIEaseOutCubic(energySpeed, animationEnergySpeed);

	//移動する数
	float moveEnergyGauge = (useEnergy * energySpeed);
	playerHPGauge.x = (float)(playerEnergy - moveEnergyGauge);
}


void KdUIObject::DrawEnemyHP()
{
	if (!m_wpEnemy.lock()->GetAlive())
	{
		enemyHpGaugeTexSizeX = enemyHpGaugeTexInitSizeX;	//初期値
		return;
	}

	EnemyHPMove();
	int enemyHPTexX;
	enemyHPTexX = (int)m_enemyHpTexPos.x + ((enemyHpGaugeTexSizeX - enemyHpGaugeTexInitSizeX) / 2);


	if (m_wpEnemy.lock()->GetHP() >= 0)
	{
		//HP
		D3D.WorkShaderManager().m_spriteShader.DrawTex
		(m_spEnemyHPTex.get(), (int)m_enemyHpTexPos.x, (int)m_enemyHpTexPos.y);

		//HPゲージ
		D3D.WorkShaderManager().m_spriteShader.DrawTex
		(m_spEnemyHPGaugeTex.get(), enemyHPTexX, (int)m_enemyHpTexPos.y, (int)enemyHpGaugeTexSizeX, (int)enemyHpGaugeTexSizeY);
	}
}

void KdUIObject::SetAsset(std::shared_ptr<KdTexture>& sp_Tex, std::string_view fileName)
{
	if (!sp_Tex)
	{
		sp_Tex = std::make_shared<KdTexture>();
	}

	sp_Tex->Load(fileName.data());
}

void KdUIObject::EnemyHPMove()
{
	std::shared_ptr<normalEnemy> sp_enemy = m_wpEnemy.lock();
	bool damegeFlg = sp_enemy->GetDamageFlg();

	//エネミーがダメージを受けたか
	if (damegeFlg)
	{

		int damege = sp_enemy->GetDamegeNum();			//受けたダメージ量
		int enemyHP = sp_enemy->GetHP() + damege;			//受ける前の体力取得＋補正

		if (animationHpSpeed < 1.0f)
		{
			animationHpSpeed += 0.1f;
		}
		else
		{
			sp_enemy->SetDamageFlg(false);
		}

		UIEaseOutCubic(hpGaugeSpeed, animationHpSpeed);

		//移動する数
		int moveHPGauge = (int)(damege * hpGaugeSpeed);
		enemyHpGaugeTexSizeX = (enemyHP - moveHPGauge);
	}
	else
	{
		hpGaugeSpeed = 1.0f;
		animationHpSpeed = 0.0f;
		enemyHpGaugeTexSizeX = sp_enemy->GetHP();
	}
}

void KdUIObject::UIEaseOutCubic(float& speed, float& animetion)
{
	speed = (float)(1 - pow(1 - animetion, 3));
}

void KdUIObject::SetNowSceneName(std::string_view fileName)
{
	m_SceneName = fileName;
}

void KdUIObject::RequestChangeScene(bool sceneChangeflg)
{
	m_requestChangeSceneFlg = sceneChangeflg;
	m_nowLoadingRec = 0;
	m_nowLoadingCount = 0;
	m_sceneChengePos = { 0,1440 };
	m_sceneChengePosSpd = -20.0f;	//降ってくるスピード
	m_readyChangeGameSceneFlg = !sceneChangeflg;
}

void KdUIObject::UltGugesProgressExit()
{
	m_ultGugesProgress = 0;
}
