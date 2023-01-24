#include "Player.h"
#include"Application/Game/KdGameSystem.h"
#include"Application/main.h"
#include"Application/Game/GameObject/CameraObject/CameraBase.h"
#include"Application/Game/GameObject/ModelObject/Character/Enemy/normalEnemy/normalEnemy.h"
#include"Application/Game/GameObject/ModelObject/PlayerWeapon/Sword/Sword.h"
#include"Application/Game/GameObject/EffectObject/CounterEffect/CounterEffect.h"
#include"System/Manager/KdAssetManager/KdAssetManager.h"
#include"Application/Game/GameObject/EffectObject/ThunderEffect/ThunderEffect.h"
#include"Application/Game/GameObject/EffectObject/UltEffect/UltEffect.h"
#include"Application/Game/GameObject/EffectObject/UltEffect/UltParticle/UltParticle.h"
#include"Application/Game/GameObject/EffectObject/Particle/ParticleEffect/ParticleEffect.h"
#include"Application/Game/GameObject/EffectObject/AttackEffect/AttackEffect.h"
#include"System/Manager/KdAudioManager/KdGameAudioManager.h"
#include"System/Manager/KdThrowManager/KdThrowManager.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/PlayerState/Move/PlayerMove.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/PlayerState/Ult/PlayerUlt.h"
#include"System/Easing.h"


void Player::Init()
{
	eObjectState = eMove;

	playerMove = 0.0f;
	m_playerMoveSpd = 0.0f;

	// 徐々にはやくなるスピード
	m_MoveTransitionSpd = 1.0f / 60.0f;

	//剣
	m_spSword = std::make_shared<Sword>();
	m_spSword->SetModelDate(KdAssetManager::GetInstance().m_ModelAsset.GetModel_Sword());
	m_spSword->Init();
	Application::Instance().WorkGameSystem()->AddObject(m_spSword);

	m_animator.SetAnimation(m_spModel->GetAnimation("StandBY"), true);

	m_attackMotionFlg = false;

	Set2DAsset(m_spUlTTex, "Asset/Texture/PlayerUI/ult.png");
	m_ultRec = 0;
	m_uilTexPos = 0;

	UIpos = { 0,0 };

	m_paAtaackState = eNoAnimationAttack;
	m_attackAnimotionContinueFlg = false;

	m_cameraPos = { 0, 1, -10 };
	m_outLineValue = 0.0f;

	m_nowActionState = std::make_shared <PlayerMove>();
}

void Player::Set2DAsset(std::shared_ptr<KdTexture>& sp_Tex, std::string_view fileName)
{
	if (!sp_Tex)
	{
		sp_Tex = std::make_shared<KdTexture>();
	}

	sp_Tex->Load(fileName.data());
}

void Player::DrawLighting()
{
	if (m_Alive)
	{
		D3D.WorkShaderManager().m_standardShader.DrawModel(*m_spModel, m_mWorld);
		if (m_CounterCountFlg)
		{
			D3D.WorkShaderManager().m_standardShader.
				DrawModelOutline(*m_spModel, m_mWorld,
					m_outLineValue);
		}
	}
}


void Player::DrawSprite()
{
	if (m_CounterCountFlg)
	{
		Math::Rectangle src = { m_uilTexPos,0,1280,720 };

		D3D.WorkShaderManager().m_spriteShader.DrawTex
		(m_spUlTTex.get(), (int)UIpos.x, (int)UIpos.y, 1280, 720, &src);
	}
}

void Player::Update()
{
	CharacterBase::Update();

	// 体力があるかどうか
	if (m_playerHP <= 0)
	{
		m_playerHP = 0;
		if (eObjectState != eState::eNoAlive)
		{
			eObjectState = eState::eNoAlive;
			m_animator.SetAnimation(m_spModel->GetAnimation("NoAlive"), false);

			//シーンの切り替えリクエスト
			Application::Instance().WorkGameSystem()->RequestChangeScene("Game", true);
		}
	}
	else
	{
		/////////////////////////////////////////////////
		//// カウンター処理
		if (m_CounterCount == 3)
		{
			if (!m_CounterCountFlg)
			{
				// ウルト突入
				m_CounterCountFlg = true;
				ChangeActionState(std::make_shared<PlayerUlt>());
				m_count = 0;
				UltInit();
			}
		}
		else
		{
			m_CounterCountFlg = false;
		}

		if (m_CounterCountFlg)
		{
			m_ultRec++;
			if (m_ultRec >= 4)
			{
				m_ultRec = 0;
			}

			m_uilTexPos = 1280 * m_ultRec;	//画像を表示させる座標

			if (eObjectState == eState::eUlt)
			{
				m_outLineValue += 0.3f / (40.0f * 2.0f);
			}
			else
			{
				m_outLineValue -= 0.3f / (60.0f * 3.0f);
			}

			m_outLineValue = std::clamp(m_outLineValue, 0.0f, 1.0f);
		}
		else
		{
			m_ultRec = 0;
			m_ultCount = 0;
			ultOption1Flg = false;
			ultOption2Flg = false;
			m_cameraPos = { 0, 1, -10 };
			m_outLineValue = 0.0f;
		}

		if (m_nowActionState)
		{
			m_nowActionState->Update(*this);
		}

		
		UpdateCircleCollision();		//円判定

		UpdateCollision();				// 押し返し処理
	}

	//当たり判定
	UpdateGroundCollision();		//地面判定
	
	//アニメーションの時間を進める
	m_animator.AdvanceTime(m_spModel->WorkNodes(), m_Objectspeed);
	m_spModel->CalcNodeMatrices();

	JustRightCheck();
}


void Player::UpdateCounterEffect()
{
	std::shared_ptr<Counter> newCounter =
		std::make_shared<Counter>();

	newCounter->SetAsset2D(KdAssetManager::GetInstance().m_EffectAsset.GetThunderTex());

	newCounter->lifeSpan(60);
	newCounter->SetDrawFrameMax(10);
	newCounter->SetUltFlg(false);

	Math::Matrix mShot = m_mWorld;
	Math::Vector3 pos = mShot.Translation();
	Math::Vector3 dir;

	// 方向
	dir = m_mWorld.Backward();
	newCounter->SetMatrix(pos, dir);


	Application::Instance().
		WorkGameSystem()->AddObject(newCounter);
}

void Player::UpdateUltAttackEffect()
{
	if (!ultOption1Flg)
	{
		std::shared_ptr<Counter> newCounter =
			std::make_shared<Counter>();

		newCounter->SetAsset2D(KdAssetManager::GetInstance().m_EffectAsset.GetThunderTex());
		newCounter->lifeSpan(120);
		newCounter->Speed(2.0f);
		newCounter->SetUltFlg(true);

		int ra = rand() % 10 + 5;

		Math::Matrix mShot = m_mWorld;
		Math::Vector3 pos = mShot.Translation();
		pos.y += 1.0f;
		Math::Vector3 dir;

		// 方向
		dir = m_mWorld.Backward();

		newCounter->SetMatrix(pos, dir);

		Application::Instance().
			WorkGameSystem()->AddObject(newCounter);

		m_ultCount = 0;
		ultOption1Flg = true;
		ultOption2Flg = true;
	}

	if (ultOption2Flg)
	{
		m_ultCount++;
		m_cameraPos.z += 0.3f;
		m_cameraPos.y -= 0.03f;
		if (m_ultCount > 20)		// アニメーションに合わせる
		{
			UpdateUltSecondOption();
			ultOption2Flg = false;
			ultOption3Flg = true;
		}
	}

	if (ultOption3Flg)
	{
		m_ultCount++;
		m_cameraPos.z -= 0.4f;
		m_cameraPos.y += 0.05f;

		std::shared_ptr<UltEffect> newUlt =
			std::make_shared<UltEffect>();

		newUlt->SetAsset2D(KdAssetManager::GetInstance().m_EffectAsset.GetUltTex());

		float size = 0.2f;
		newUlt->SetDrawFrameMax(20);	//5～15

		m_ultZpos += 1;

		Math::Matrix mShot = m_mWorld;
		Math::Vector3 pos = mShot.Translation();
		pos.y += 1.0f;
		Math::Vector3 dir;

		// playerの周りでバチバチ
		pos.x += (rand() % m_ultZpos - (m_ultZpos / 2)) * 0.5f;	//-1 ～ 1
		pos.y += (rand() % m_ultZpos - (m_ultZpos / 2)) * 0.5f;	//-2 ～ 2
		pos.z += (rand() % m_ultZpos - (m_ultZpos / 2)) * 0.5f;	//-2 ～ 2

		// 方向
		dir.x += (rand() % 200 - 100) * 0.0005f;
		dir.y += (rand() % 200 - 100) * 0.0005f;
		dir.z += (rand() % 200 - 100) * 0.0005f;
		dir.Normalize();

		dir = Math::Vector3::TransformNormal(dir, mShot);

		newUlt->SetMatrix(size, -5, pos, dir);

		Application::Instance().
			WorkGameSystem()->AddObject(newUlt);

		if (m_ultCount > 60)
		{
			ultOption3Flg = false;
			m_ultCount = 0;
			m_ultZpos = 4;
		}
	}

	if (m_cameraPos.z <= -10)
	{
		m_cameraPos.z = -10;
	}
	if (m_cameraPos.y >= 1)
	{
		m_cameraPos.y = 1;
	}
}

void Player::UltInit()
{
	ultOption1Flg = false;
	ultOption2Flg = false;
	ultOption3Flg = false;
	m_cameraPos = { 0, 1, -10 };
	m_ultZpos = 4;
	m_ultCount = 0;
}

void Player::UpdateUltSecondOption()
{
	for (int i = 0; i < 20; i++)
	{
		{
			// 光
			std::shared_ptr<Attack> newAttack =
				std::make_shared<Attack>();

			newAttack->SetAsset2D(KdAssetManager::GetInstance().m_EffectAsset.GetUltThunderTex());

			int ra = rand() % 10 + 35;
			newAttack->SetDrawFrameMax(ra);	//5～15
			//newAttack->Speed(1.0f);
			newAttack->lifeSpan(60);


			Math::Matrix mShot = m_mWorld;
			Math::Vector3 pos = mShot.Translation();
			pos.y += 1.0f;
			Math::Vector3 dir;

			// 方向
			dir.x += (rand() % 200 - 100) * 0.0005f;
			dir.y += (rand() % 200 - 100) * 0.0005f;
			dir.z += (rand() % 200 - 100) * 0.0005f;
			dir.Normalize();

			dir = Math::Vector3::TransformNormal(dir, mShot);

			newAttack->SetMatrix(pos, dir);

			Application::Instance().
				WorkGameSystem()->AddObject(newAttack);
		}
	}

	for (int i = 0; i < 30; i++)
	{
		{
			// パーティクル
			std::shared_ptr<ParticleEffect> newParticle =
				std::make_shared<ParticleEffect>();

			// 画像セット
			newParticle->SetAsset2D(KdAssetManager::GetInstance().m_EffectAsset.GetUltTex());

			// サイズ
			float size = 0.1f;
			newParticle->SetDrawFrameMax(10);	//デカさ
			newParticle->Speed(0.01f);			//速さ
			newParticle->lifeSpan(240);			//寿命

			int pPos = 3;

			Math::Matrix mShot = m_mWorld;
			Math::Vector3 pos = mShot.Translation();
			pos.y += 1.0f;
			Math::Vector3 dir;

			// playerの周りでバチバチ
			pos.x += (rand() % pPos - (pPos / 2)) * 0.5f;	//-1 ～ 1
			pos.y += (rand() % pPos - (pPos / 2)) * 0.5f;	//-2 ～ 2
			pos.z += (rand() % pPos - (pPos / 2)) * 0.5f;	//-2 ～ 2

			// 方向
			dir.x += (rand() % 200 - 100) * 0.0005f;
			dir.y += (rand() % 200 - 100) * 0.0005f;
			dir.z += (rand() % 200 - 100) * 0.0005f;
			dir.Normalize();

			dir = Math::Vector3::TransformNormal(dir, mShot);

			newParticle->SetMatrix(size, pos, dir);

			Application::Instance().
				WorkGameSystem()->AddObject(newParticle);
		}
	}
}

void Player::JustRightCheck()
{
	bool rightFlg = false;

	for (const std::shared_ptr<KdGameObject>& spObj :
		Application::Instance().GetGameSystem()->GetGameObjects())
	{
		if (spObj->GetClassID() == eEnemy)
		{
			if (
				spObj->GeteState() == KdGameObject::eMove ||
				spObj->GeteState() == KdGameObject::eAttack)
			{
				rightFlg = true;
			}
		}

		if (spObj->GetClassID() == KdGameObject::eEnemyBullet)
		{
			rightFlg = true;
		}
	}

	// ウルトなら
	if (m_CounterCountFlg) { rightFlg = false; }

	if (rightFlg)
	{
		float speed = 1.0f / 30.0f;

		m_spotRightRadius += speed;

		float progress = std::clamp(m_spotRightRadius, 0.0f, 1.0f);

		// ジャスト回避が始まったら消える
		float ObjSpeed = 1 - KdThrowManager::GetInstance().GetSpeed();


		//　ポイントライト
		Math::Vector3 pos = m_mWorld.Translation();
		pos.y += 1.5;
		D3D.WorkShaderManager().SetPointLight(pos, { 1, 1, 0 }, 5.5f * EaseInOutSine(progress - ObjSpeed));

		// GPUに送る
		D3D.WorkShaderManager().m_cb8_Light.Write();
	}
	else
	{
		m_spotRightRadius = 0;

		//　ポイントライト
		Math::Vector3 pos = m_mWorld.Translation();
		pos.y += 1.5;
		D3D.WorkShaderManager().SetPointLight(pos, { 0, 0, 0 }, 5.5f);

		// GPUに送る
		D3D.WorkShaderManager().m_cb8_Light.Write();
	}
}

void Player::SetPlayerAttackAnimation(std::string_view fileName, const bool loopFlg)
{
	m_animator.SetAnimation(m_spModel->GetAnimation(fileName.data()), loopFlg);
	m_count = 0;
}

void Player::UpdateMove(Math::Vector3& dstMove, Math::Vector3& nowPos)
{
	// 軸情報を取得
	Math::Vector3 inputAxis = Math::Vector3::Zero;

	if (GetAsyncKeyState('D')) { inputAxis.x = 1.0f; }
	if (GetAsyncKeyState('A')) { inputAxis.x = -1.0f; }
	if (GetAsyncKeyState('W')) { inputAxis.y = 1.0f; }
	if (GetAsyncKeyState('S')) { inputAxis.y = -1.0f; }

	//入力されているなら
	if (inputAxis.LengthSquared())
	{
		// 動いていないなら
		if (!m_moveFlg)
		{
			m_moveFlg = true;
			m_animator.SetAnimation(m_spModel->GetAnimation("MoveTransition"), false);
		}
		else
		{
			// 移動に切り替えたら
			if (m_animator.IsAnimationEnd())
			{
				m_animator.SetAnimation(m_spModel->GetAnimation("Move"), true);
			}
		}


		// イージング
		if (playerMove > 1.0f)
		{
			playerMove = 1.0f;
		}
		else
		{
			// 加減速
			float spd = m_MoveTransitionSpd;
			playerMove += spd;
		}
	}
	else
	{
		// 入力されていないのなら１度だけセット
		if (m_moveFlg)
		{
			m_animator.SetAnimation(m_spModel->GetAnimation("StandBY"), true);
		}

		m_moveFlg = false;

		playerMove = 0.0f;
		m_playerMoveSpd = 0.2f;
	}


	Math::Vector3 moveVec = Math::Vector3::Zero;
	moveVec.x = inputAxis.x;
	moveVec.z = inputAxis.y;

	moveVec.Normalize();

	moveVec *= (m_playerMoveSpd * playerMove);


	std::shared_ptr<CameraBase> spCamera = m_wpCamera.lock();
	if (spCamera)
	{
		moveVec = moveVec.TransformNormal
		(moveVec, spCamera->GetRotationYMatrix());
	}

	UpdateEnemyFutureCircleCollision(dstMove, nowPos, moveVec);
}

void Player::UpdateDamageMove(Math::Vector3& dstMove, Math::Vector3& nowPos)
{
	Math::Vector3 moveVec = m_damageDir;					//後ろに飛ぶ

	moveVec.Normalize();
	moveVec *= m_playerMoveSpd;

	nowPos.x += moveVec.x;										//現在位置から動いてもらう
	nowPos.z += moveVec.z;
	dstMove = moveVec;
}

void Player::UpdateAvoidance(Math::Vector3& dstMove, Math::Vector3& nowPos)
{
	//回避アクション
	Math::Vector3 moveVec = m_AvoidanceDir;						//正面方向のベクトル。移動してほしい方向

	float avoidanceSpeed = 0.2f;

	moveVec.Normalize();
	moveVec *= avoidanceSpeed;

	// 未来当たり判定
	UpdateEnemyFutureCircleCollision(dstMove, nowPos, moveVec);

	m_Rotation *= Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(20.0f));
}

void Player::UpdateJustAvoidance(Math::Vector3& dstMove, Math::Vector3& nowPos)
{
	//回避アクション
	Math::Vector3 moveVec;
	float rotation = 0;

	rotation = 20.0f;
	moveVec = m_AvoidanceDir;
	

	moveVec.Normalize();
	moveVec *= (0.2f * m_Objectspeed);

	UpdateEnemyFutureCircleCollision(dstMove, nowPos, moveVec);

	m_Rotation *= Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(rotation * m_Objectspeed));
}

void Player::UpdateMatrix(Math::Vector3& nowPos)
{
	// キャラクターのワールド行列を創る処理
	Math::Matrix trans = Math::Matrix::Identity;
	Math::Matrix rotation = Math::Matrix::Identity;

	trans = Math::Matrix::CreateTranslation(nowPos);
	rotation = Math::Matrix::
		CreateRotationY(DirectX::XMConvertToRadians(m_worldRot.y));

	switch (eObjectState)
	{
	case KdGameObject::eAvoidance:

		m_mWorld = trans;

		break;

	case KdGameObject::eJustAvoidance:

		if (!justCounterFlg)
		{
			m_mWorld = trans;
		}

		break;

	case KdGameObject::eMove:

		m_mWorld = rotation * trans;
		break;

	case KdGameObject::eDamage:

		m_mWorld = rotation * trans;

		break;

	default:
		break;
	}
}

void Player::SetAvoidanceDir(Math::Vector3& dir)
{
	m_AvoidanceDir = dir;
}

void Player::UpdateGroundCollision()
{
	Math::Vector3 rayPos, rayDir;
	float rayRange = 2.0f;
	rayDir = Math::Vector3::Down;
	rayPos = m_mWorld.Translation();
	rayPos.y += rayRange;

	CollisionMeshResult groudResult;

	for (const std::shared_ptr<KdGameObject>& spObj :
		Application::Instance().GetGameSystem()->GetGameObjects())
	{
		if (spObj->GetClassID() == KdGameObject::eStage)
		{
			spObj->CollisionGround(rayPos, rayDir, rayRange, groudResult);
		}
	}

	if (groudResult.m_hit)
	{
		SetPos(groudResult.m_hitPos);
		m_verticalMovement = 0;
	}
}

void Player::UpdateEnemyFutureCircleCollision(Math::Vector3& dstMove, Math::Vector3& nowPos, Math::Vector3& moveVec)
{
	for (const std::shared_ptr<KdGameObject>& spObj :
		Application::Instance().GetGameSystem()->GetGameObjects())
	{
		if (spObj->GetClassID() != KdGameObject::eEnemy) { continue; }

		Math::Vector3 futurePos;							//未来座標
		futurePos = nowPos + moveVec;

		Math::Vector3 offsetPos = { 0.0f,0.5f,0.0f };
		SphereInfo info(futurePos + offsetPos, 1.5f);		//未来座標で判定
		BumpResult result;

		spObj->CheckCollisionBump(info, result);		//円

		// 敵と当たっていないときだけ動く
		if (!result.m_isHit)
		{
			// Playerを移動
			nowPos.x += moveVec.x;
			nowPos.z += moveVec.z;
			dstMove = moveVec;
		}
		else
		{
			dstMove = m_mWorld.Backward();
		}
	}
}

void Player::UpdateCircleCollision()
{
	for (const std::shared_ptr<KdGameObject>& spObj :
		Application::Instance().GetGameSystem()->GetGameObjects())
	{
		if (!spObj->GetAlive()) { continue; }

		if (eObjectState == KdGameObject::eJustAvoidance) { continue; }

		if (spObj->GetClassID() == KdGameObject::eEnemyBullet)
		{
			Math::Vector3 offsetPos = { 0.0f,0.5f,0.0f };
			SphereInfo info(GetPos() + offsetPos, 1.0f);
			BumpResult result;

			spObj->CheckCollisionBump(info, result);		//円

			if (result.m_isHit)								//弾と当たっていたら
			{
				spObj->NeedDeleteTrue();
				m_playerHP -= 30;					//HP減少
				eObjectState = eState::eDamage;
				m_animator.SetAnimation(m_spModel->GetAnimation("Damage"), false);
				m_damageDir = GetPos() - spObj->GetPos();
			}
		}

		Math::Vector3 offsetPos = { 0.0f,0.5f,0.0f };
		SphereInfo info(GetPos() + offsetPos, 3.0f);
		BumpResult result;

		if (spObj->GetClassID() == KdGameObject::eEnemy)
		{
			// ウルト状態の時の急ごしらえダメージ判定
			if (m_CounterCountFlg)
			{
				if (eObjectState == KdGameObject::eAttack)
				{
					spObj->CheckCollisionBump(info, result);		//円
					Application::Instance().GetGameSystem()->GetEnemy()->CounterDamage(m_spSword->GetPos());
					Application::Instance().GetGameSystem()->GetEnemy()->SetDamageFlgTrue();
				}
			}
			else
			{
				if (spObj->GeteState() == KdGameObject::eMove)
				{
					if (spObj->GetCount() >= 120)
					{
						spObj->CheckCollisionBump(info, result);		//円

						if (result.m_isHit)								//弾と当たっていたら
						{
							m_playerHP -= 2;					//HP減少
							eObjectState = eState::eDamage;
							m_animator.SetAnimation(m_spModel->GetAnimation("Damage"), false);
							m_damageDir = GetPos() - spObj->GetPos();
						}
					}
				}

				if (spObj->GeteState() == KdGameObject::eAttack)
				{
					if (spObj->GetCount() >= 85)
					{
						spObj->CheckCollisionBump(info, result);		//円

						if (result.m_isHit)								//弾と当たっていたら
						{
							m_playerHP -= 2;					//HP減少
							eObjectState = eState::eDamage;
							m_animator.SetAnimation(m_spModel->GetAnimation("Damage"), false);
							m_damageDir = GetPos() - spObj->GetPos();
						}
					}
				}
			}			
		}
	}
}

void Player::UpdateCollision()
{
	for (const std::shared_ptr<KdGameObject>& spObject :
		Application::Instance().GetGameSystem()->GetGameObjects())
	{
		if (spObject.get() == this) { continue; }

		if (spObject->GetClassID() == KdGameObject::eStage) { return; }

		DirectX::BoundingSphere sphere;
		sphere.Center = m_mWorld.Translation();
		sphere.Center.y += 0.6f;
		sphere.Radius = 0.5f;

		CollisionResult bumpResult;

		spObject->CollisionBump(sphere, bumpResult);

		if (!bumpResult.m_hit) { continue; }

		m_mWorld *= Math::Matrix::CreateTranslation(bumpResult.m_hitDir * bumpResult.m_overlapDistance);
	}
}

void Player::SetPlayerDamageFlg(bool flg)
{
	m_playerDamageFlg = flg;
	m_playerHPDecreaseValue = 0;
}

void Player::QTECheck()
{
	//ジャスト回避中
	// QTE
	if (GetAsyncKeyState('R'))
	{
		if (!m_qtePointFlg)
		{
			//QET(二度目のジャスト判定)
			if (m_Objectspeed > 0.03f && m_Objectspeed < 0.16f ||
				m_Objectspeed > 0.03f && m_Objectspeed < 0.16f)
			{
				m_CounterCountFlg = true;
				m_CounterCount++;
				UpdateCounterEffect();
			}

			m_qtePointFlg = true;
		}
	}
	else
	{
		m_qtePointFlg = false;
	}

}

bool Player::JustAvoidanceFlg()
{
	bool justFlg = false;

	for (const std::shared_ptr<KdGameObject>& spObj :
		Application::Instance().GetGameSystem()->GetGameObjects())
	{
		if (spObj->GetClassID() != KdGameObject::eEnemyBullet)continue;

		if (!spObj->GetAlive()) { continue; }

		Math::Vector3 offsetPos = { 0.0f,0.5f,0.0f };
		SphereInfo info(GetPos() + offsetPos, 5.5f);	//エネミー弾の円判定よりっも少し大きめに
		BumpResult result;

		spObj->CheckCollisionBump(info, result);

		//ジャスト回避判定内なら
		if (result.m_isHit)
		{
			justFlg = true;
		}
	}

	return justFlg;
}

bool Player::JustCounterFlg()
{
	const std::shared_ptr<KdGameObject>& spEnemy = Application::Instance().GetGameSystem()->GetEnemy();
	if (!spEnemy) { return false; }

	// ウルトなら
	if (m_CounterCountFlg) { return false; }

	bool justFlg = false;

	Math::Vector3 offsetPos = { 0.0f,0.5f,0.0f };
	SphereInfo info(GetPos() + offsetPos, 5.5f);	//エネミー弾の円判定よりっも少し大きめに
	BumpResult result;

	spEnemy->CheckCollisionBump(info, result);

	//ジャスト回避判定内なら
	if (result.m_isHit)
	{

		if (spEnemy->GeteState() == KdGameObject::eAttack ||
			spEnemy->GeteState() == KdGameObject::eMove
			)
		{
			justFlg = true;
		}
		else
		{
			justFlg = false;
		}
	}

	return justFlg;
}

void Player::SetNextAnimation(std::string_view fileName, const bool flg)
{
	m_animator.SetAnimation(m_spModel->GetAnimation(fileName.data()), flg);
}

const bool Player::GetAnimationEndFlg() const
{
	bool animationEndFlg = m_animator.IsAnimationEnd();

	return animationEndFlg;
}

void Player::ChangeActionState(std::shared_ptr<StateBase> nextState)
{
	m_nowActionState = nextState;
	m_nowActionState->SetAnimation(*this);	// アニメーションセット
	m_nowActionState->Exit(*this);			// Playerの状態を初期化
	m_nowActionState->Enter(*this);
}

void Player::QTEExit()
{
	m_qtePointFlg = false;				//QTEポイント初期化
}
