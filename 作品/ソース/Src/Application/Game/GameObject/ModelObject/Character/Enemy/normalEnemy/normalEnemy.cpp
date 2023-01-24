#include "normalEnemy.h"
#include"Application/main.h"
#include"Application/Game/KdGameSystem.h"
#include"Application/Game/GameObject/ModelObject/Bullet/nomalBullet/nomalBullet.h"

#include"Application/Game/GameObject/EffectObject/AttackEffect/AttackEffect.h"
#include"Application/Game/GameObject/EffectObject/ThunderEffect/ThunderEffect.h"
#include"System/Manager/KdAssetManager/KdAssetManager.h"
#include"Application/Game/GameObject/EffectObject/SlashingEffect/SlashingEffect.h"

#include"System/Manager/KdAudioManager/KdGameAudioManager.h"
#include"System/Manager/KdThrowManager/KdThrowManager.h"
#include"System/Easing.h"


void normalEnemy::Init()
{
	m_count = 0;
	enemyHP = 728;
	m_damegeNum = 0;
	m_damageFlg = false;
	m_Alive = true;

	m_animator.SetAnimation(m_spModel->GetAnimation("StandBY"), true);

	eObjectState = KdGameObject::eStandBy;

	Math::Matrix trans;			//エネミー初期座標
	trans.Translation({ 0,10,0 });

	m_mWorld =
		Math::Matrix::CreateRotationY
		(DirectX::XMConvertToRadians(180))
		* trans;

	m_moveSpd = 0.0f;

	m_noAliveFlg = false;

	m_nMoveFlg = false;

	m_progress = 0.0f;
}

void normalEnemy::Update()
{
	if (!m_nMoveFlg)
	{
		if (m_count++ > 300)
		{
			m_nMoveFlg = true;
		}
	}

	std::shared_ptr<Player> sp_player = m_wpPlayer.lock();
	Math::Matrix playerMat = sp_player->GetMatrix();
	Math::Vector3 vMove = sp_player->GetPos() - GetPos();
	Math::Matrix trans = Math::Matrix::Identity;
	Math::Matrix rotation = Math::Matrix::Identity;
	float progress = 0.0f;
	float speed = 1.0f / 60.0f;

	if (sp_player->GetCounterCountFlg()) {

		return;
	}

	EnemyBase::Update();

	Math::Vector3 nowDir = m_mWorld.Backward();
	Math::Vector3 targetDir = playerMat.Translation() - GetPos();

	// HPが0になったらダウン
	if (enemyHP <= 0)
	{
		m_Alive = false;

		if (!m_noAliveFlg)
		{
			m_animator.SetAnimation(m_spModel->GetAnimation("Noalive"), false);
			//シーンの切り替えリクエスト
			Application::Instance().WorkGameSystem()->RequestChangeScene("Result", false);
			m_noAliveFlg = true;

		}
		m_damageFlg = false;
	}
	else
	{
		switch (eObjectState)
		{
		case KdGameObject::eStandBy:

			if (m_nMoveFlg)
			{
				// 待機状態。向きをプレイヤーに

				m_count++;

				// 回転
				UpdateRotate(nowDir, targetDir);

				trans = Math::Matrix::CreateTranslation(GetPos());

				rotation = Math::Matrix::
					CreateRotationY(DirectX::XMConvertToRadians(m_worldRot.y));


				m_mWorld = rotation * trans;


				//2秒待つ
				if (m_count > 120)
				{
					// 攻撃モードへ移行 
					m_count = 0;

					if (m_circleCollisionFlg)
					{
						// プレイヤーが近くにいる
						// ランダムで２パターンのモーション
						float randam = rand() / (float)RAND_MAX;	//0～1
						if (randam > 0.5)
						{
							// 近距離攻撃
							eObjectState = eAttack;
							CreateAttackStartEffect();
							// アニメーションセット
							m_animator.SetAnimation(m_spModel->GetAnimation("Attack_Near2"), false);
							KdGameAudioManager::GetInstance().attackStartSE();
						}
						else
						{

							eObjectState = eMove;
							CreateAttackStartEffect();
							// タックル攻撃
							// アニメーションセット
							m_animator.SetAnimation(m_spModel->GetAnimation("Attack_Near"), false);
							KdGameAudioManager::GetInstance().takkuruStandSE();
							KdGameAudioManager::GetInstance().attackStartSE();
						}


					}
					else
					{
						//プレイヤーが遠くにいる

						// ミサイル発射
						eObjectState = eBullet;
						m_animator.SetAnimation(m_spModel->GetAnimation("Attack_Bullet"), false);
						EnemyBulletCreate();	//弾生成
						m_enemyBulletCreateFlg = true;
					}
				}
			}

			break;

		case KdGameObject::eMove:

			// タックル攻撃
			m_count++;

			// アニメーションが終わったら
			if (m_animator.IsAnimationEnd())
			{
				m_count = 0;
				eObjectState = eStandBy;
				m_animator.SetAnimation(m_spModel->GetAnimation("StandBY"), true);
				m_moveSpd = 0.0f;
				takkuruMoveSEFlg = false;
			}



			// プレイヤー方向に進む
			if (m_count > 120)
			{
				// 斬撃エフェクト
				UpdateSlashingMoveEffect();

				if (playerMove > 1.0f)
				{
					playerMove = 1.0f;
				}
				else
				{
					playerMove += 0.016f;
					easeInCubic(playerMove, m_moveSpd);
				}

				vMove = m_mWorld.Backward();
				if (!takkuruMoveSEFlg)
				{
					KdGameAudioManager::GetInstance().takkuruMoveSE();
					takkuruMoveSEFlg = true;
				}
			}
			else
			{
				vMove = sp_player->GetPos() - GetPos();
			}


			vMove.Normalize();
			vMove *= (float)(0.2 * m_moveSpd);

			trans = Math::Matrix::CreateTranslation(vMove);

			//行列の合成
			m_mWorld *= trans;

			// Playerがジャストカウンターに成功したら
			if (KdThrowManager::GetInstance().GetThrowFlg())
			{
				float spd = KdThrowManager::GetInstance().GetSpeed();
				if (spd >= 1.0f)
				{
					PlayerJustCounter();
				}
			}



			break;



		case KdGameObject::eAttack:

			// アニメーションカウント
			m_count++;

			// アニメーションが終わったら
			if (m_animator.IsAnimationEnd())
			{
				m_count = 0;
				eObjectState = eStandBy;
				m_animator.SetAnimation(m_spModel->GetAnimation("StandBY"), true);
			}

			// Playerがジャストカウンターに成功したら
			if (KdThrowManager::GetInstance().GetThrowFlg())
			{
				float spd = KdThrowManager::GetInstance().GetSpeed();
				if (spd >= 1.0f)
				{
					PlayerJustCounter();
				}
			}

			// 斬撃エフェクト
			UpdateSlashingAttackEffect();

			break;

		case KdGameObject::eBullet:

			//弾が飛んでいく
			m_count++;

			if (m_count >= (m_bulletCreate * 2))			//2発打ち終わったら
			{
				eObjectState = eStandBy;
				m_animator.SetAnimation(m_spModel->GetAnimation("StandBY"), true);
				m_count = 0;
				break;
			}

			if (m_count % m_bulletCreate == 0)				//3秒おきに弾を生成
			{
				EnemyBulletCreate();
				m_enemyBulletCreateFlg = true;
			}

			break;

		case KdGameObject::eDamage:

			// ダメージリアクション
			// もちろんイージング
			vMove = m_mWorld.Backward();
			m_progress += speed;
			progress = std::clamp(m_progress, 0.0f, 1.0f);


			vMove *= - (0.2f* EaseInOutSine(progress));					//反転とスピード
			trans = Math::Matrix::CreateTranslation(vMove);

			//行列の合成
			m_mWorld *= trans;

			// アニメーションが終わったら
			if (m_animator.IsAnimationEnd())
			{
				m_count = 0;
				eObjectState = eStandBy;
				m_animator.SetAnimation(m_spModel->GetAnimation("StandBY"), true);
			}

			break;

		default:
			break;
		}
	}


	//円判定
	UpdateCircleCollision();
	UpdateGroundCollision();

	//アニメーションの時間を進める
	m_animator.AdvanceTime(m_spModel->WorkNodes(), m_Objectspeed);
	m_spModel->CalcNodeMatrices();

	UpdateEffect();

}

void normalEnemy::DrawLighting()
{
	if (!m_spModel) { return; }
	D3D.WorkShaderManager().m_standardShader.DrawModel(*m_spModel, m_mWorld);
}


void normalEnemy::UpdateEffect()
{
	for (std::shared_ptr<EffectBase>& spEffect : m_spAttacks)
	{
		spEffect->Update(animationPos);
	}

	for (auto iter = m_spAttacks.begin();
		iter != m_spAttacks.end();)
	{
		if ((*iter)->NeedDelete())
		{
			iter = m_spAttacks.erase(iter);

			continue;
		}

		++iter;
	}
}

void normalEnemy::UpdateAttackEffect(Math::Vector3& damagePos)
{
	m_attackEffecCount++;

	if (m_attackEffecCount > 30)
	{
		m_attackEffectFlg = true;
		m_attackEffecCount = 0;
	}
	else
	{
		m_attackEffectFlg = false;
	}

	if (m_attackEffectFlg)
	{
		for (int i = 0; i < 10; i++)
		{
			std::shared_ptr<Attack> newAttack =
				std::make_shared<Attack>();

			newAttack->SetAsset("Asset/Texture/Bullet.png");

			int ra = rand() % 10 + 1;
			newAttack->SetDrawFrameMax(ra);	//5～15

			Math::Vector3 pos = damagePos;
			Math::Vector3 dir;

			// 方向
			dir.x += (rand() % 200 - 100) * 0.0005f;
			dir.y += (rand() % 200 - 100) * 0.0005f;
			dir.z += (rand() % 200 - 100) * 0.0005f;
			dir.Normalize();

			newAttack->SetMatrix(pos, dir);

			Application::Instance().
				WorkGameSystem()->AddObject(newAttack);
		}

		m_attackEffectFlg = false;
	}
}

void normalEnemy::UpdateSlashingMoveEffect()
{
	// タックル
	std::shared_ptr<SlashingEffect> newSlashingEffect =
		std::make_shared<SlashingEffect>();

	// 画像セット
	newSlashingEffect->SetAsset2D(KdAssetManager::GetInstance().m_EffectAsset.GetThunderTex());

	// サイズ
	int ra = rand() % 20 + 5;	//5～15
	newSlashingEffect->SetDrawFrameMax(ra);	//デカさ
	newSlashingEffect->lifeSpan(60);			//寿命

	if (m_spModel)
	{
		const KdModelWork::Node* pLHead
			= m_spModel->FindNode("sLight");

		if (pLHead)
		{
			Math::Matrix mShot = pLHead->m_worldTransform * m_mWorld;
			Math::Vector3 playerPos = Math::Vector3::Zero;
			animationPos = mShot.Translation();

			// 方向
			playerPos.x += (rand() % ra - (ra / 2)) * 0.05f;
			playerPos.y += (rand() % ra - (ra / 2)) * 0.5f;
			playerPos.z += (rand() % ra - (ra / 2)) * 0.05f;

			newSlashingEffect->SetMatrixS(playerPos);

			Application::Instance().
				WorkGameSystem()->AddObject(newSlashingEffect);

			m_spAttacks.push_back(newSlashingEffect);
		}
	}
}

void normalEnemy::CreateUltDamageEffect()
{
	std::shared_ptr<ThunderEffect> newThunder =
		std::make_shared<ThunderEffect>();

	newThunder->SetAsset2D(KdAssetManager::GetInstance().m_EffectAsset.GetThunderTex());


	int randNum = rand() % 2 + 5;
	newThunder->SetDrawFrameMax(randNum);
	newThunder->Speed(0.5f);
	int effectPos = 6;

	Math::Matrix mShot = m_mWorld;
	Math::Vector3 pos = mShot.Translation();
	pos.y += 4.0f;
	Math::Vector3 dir;

	// playerの周りでバチバチ
	pos.x += (rand() % effectPos - (effectPos / 2)) * 0.5f;
	pos.y += (rand() % effectPos - (effectPos / 2)) * 0.5f;
	pos.z += (rand() % effectPos - (effectPos / 2)) * 0.5f;

	// 方向
	dir.x += (rand() % 200 - 100) * 0.0005f;
	dir.z += (rand() % 200 - 100) * 0.0005f;
	dir.Normalize();

	dir = Math::Vector3::TransformNormal(dir, mShot);

	newThunder->SetMatrix(pos, dir);

	Application::Instance().
		WorkGameSystem()->AddObject(newThunder);
}

void normalEnemy::CreateAttackStartEffect()
{
	for (int i = 0; i < 20; i++)
	{
		{
			// 光
			std::shared_ptr<Attack> newAttack =
				std::make_shared<Attack>();

			newAttack->SetAsset2D(KdAssetManager::GetInstance().m_EffectAsset.GetThunderTex());

			int ra = rand() % 10 + 35;
			newAttack->SetDrawFrameMax(ra);	//5～15
			newAttack->Speed(2.0f);
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
}

void normalEnemy::UpdateSlashingAttackEffect()
{
	// 単純な斬撃
	std::shared_ptr<SlashingEffect> newSlashingEffect =
		std::make_shared<SlashingEffect>();

	// 画像セット
	newSlashingEffect->SetAsset2D(KdAssetManager::GetInstance().m_EffectAsset.GetBulletTex());

	// サイズ
	int ra = rand() % 20 + 5;	//5～15
	newSlashingEffect->SetDrawFrameMax(ra);	//デカさ
	newSlashingEffect->lifeSpan(ra);			//寿命

	if (m_spModel)
	{
		const KdModelWork::Node* pLHead
			= m_spModel->FindNode("sLight");

		if (pLHead)
		{
			Math::Matrix pos = pLHead->m_worldTransform;
			Math::Vector3 wpos = pos.Translation();
			wpos.x *= -1;
			wpos.z *= -1;
			pos.Translation(wpos);

			Math::Matrix mShot = pos * m_mWorld;
			Math::Vector3 playerPos = Math::Vector3::Zero;
			animationPos = mShot.Translation();

			newSlashingEffect->SetMatrixS(playerPos);

			Application::Instance().
				WorkGameSystem()->AddObject(newSlashingEffect);

			m_spAttacks.push_back(newSlashingEffect);
		}
	}
}

void normalEnemy::UpdateGroundCollision()
{
	//地面判定
	//---------------------------------------------
	//①当たり判定用の情報作成
	Math::Vector3 rayPos, rayDir;
	float rayRange = 1.0f;				//2m。段差を登る量
	rayDir = Math::Vector3::Down;
	rayPos = m_mWorld.Translation();
	rayPos.y += rayRange;

	CollisionMeshResult groudResult;

	//②全てのゲームオブジェクトに総当たり
	for (const std::shared_ptr<KdGameObject>& spObj :
		Application::Instance().GetGameSystem()->GetGameObjects())
	{
		if (spObj->GetClassID() == KdGameObject::eStage)
		{
			spObj->CollisionGround(rayPos, rayDir, rayRange, groudResult);
		}
	}

	//③結果を使って座標を補完する
	if (groudResult.m_hit)
	{
		SetPos(groudResult.m_hitPos);

		m_verticalMovement = 0;
	}
}

void normalEnemy::UpdateCircleCollision()
{
	//円判定の当たり判定
	for (const std::shared_ptr<KdGameObject>& spObj :
		Application::Instance().GetGameSystem()->GetGameObjects())
	{
		//プレイヤーとエネミーの距離チェック
		if (spObj->GetClassID() == KdGameObject::ePlayer)
		{
			Math::Vector3 offsetPos = { 0.0f,0.5f,0.0f };
			SphereInfo info(GetPos() + offsetPos, 20.0f);
			BumpResult result;

			spObj->CheckCollisionBump(info, result);	//円

			if (result.m_isHit)
			{
				if (spObj->GetAlive())
				{
					m_circleCollisionFlg = true;
				}
			}
			else
			{
				m_circleCollisionFlg = false;
			}
		}

		//プレイヤー剣とエネミー
		if (spObj->GetClassID() == KdGameObject::ePlayerSowd)
		{
			if (spObj->GeteState() == KdGameObject::eAttack)
			{
				Math::Vector3 offsetPos = { 0.0f,0.5f,0.0f };
				SphereInfo info(GetPos() + offsetPos, 3.5f);
				BumpResult result;

				spObj->CheckCollisionBump(info, result);	//円

				if (result.m_isHit)							//剣と当たっていたら
				{
					if (Application::Instance().GetGameSystem()->GetPlayer()->GetCounterCountFlg())
					{
						enemyHP -= 5.0f;
					}
					else
					{
						enemyHP -= 1.0f;
					}

					m_damegeNum = 1;
					m_damageFlg = true;

					UpdateAttackEffect(spObj->GetPos());
				}
			}
		}
	}
}

void normalEnemy::UpdateCollision()
{
	// ②全てのゲームオブジェクトに総当たり
	for (const std::shared_ptr<KdGameObject>& spObject :
		Application::Instance().GetGameSystem()->GetGameObjects())
	{
		if (spObject.get() == this) { continue; }

		// オブジェクト毎に押し返し処理を発生させる
		// ①当たり判定用の情報作成
		DirectX::BoundingSphere sphere;
		sphere.Center = m_mWorld.Translation();
		sphere.Center.y += 0.6f;
		sphere.Radius = 0.5f;

		CollisionResult bumpResult;

		spObject->CollisionBump(sphere, bumpResult);

		if (!bumpResult.m_hit) { continue; }

		// ③ 結果を使って座標を補完する
		m_mWorld *= Math::Matrix::CreateTranslation(bumpResult.m_hitDir * bumpResult.m_overlapDistance);
	}
}

void normalEnemy::PlayerJustCounter()
{
	m_count = 0;
	eObjectState = eDamage;
	m_animator.SetAnimation(m_spModel->GetAnimation("Damage"), false);
}



void normalEnemy::EnemyBulletCreate()
{
	std::shared_ptr<nomalBullet> m_spEnemyNormalBullet = std::make_shared<nomalBullet>();
	m_spEnemyNormalBullet->SetAsset("Asset/Model/Missile/missile.gltf");
	m_spEnemyNormalBullet->SetPlayer(Application::Instance().GetGameSystem()->GetPlayer());
	m_spEnemyNormalBullet->Init();

	Application::Instance().WorkGameSystem()->AddObject(m_spEnemyNormalBullet);
}

void normalEnemy::easeInCubic(float& move, float& pos)
{
	pos = move * move * move;
}
