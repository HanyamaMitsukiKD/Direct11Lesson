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

	Math::Matrix trans;			//�G�l�~�[�������W
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

	// HP��0�ɂȂ�����_�E��
	if (enemyHP <= 0)
	{
		m_Alive = false;

		if (!m_noAliveFlg)
		{
			m_animator.SetAnimation(m_spModel->GetAnimation("Noalive"), false);
			//�V�[���̐؂�ւ����N�G�X�g
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
				// �ҋ@��ԁB�������v���C���[��

				m_count++;

				// ��]
				UpdateRotate(nowDir, targetDir);

				trans = Math::Matrix::CreateTranslation(GetPos());

				rotation = Math::Matrix::
					CreateRotationY(DirectX::XMConvertToRadians(m_worldRot.y));


				m_mWorld = rotation * trans;


				//2�b�҂�
				if (m_count > 120)
				{
					// �U�����[�h�ֈڍs 
					m_count = 0;

					if (m_circleCollisionFlg)
					{
						// �v���C���[���߂��ɂ���
						// �����_���łQ�p�^�[���̃��[�V����
						float randam = rand() / (float)RAND_MAX;	//0�`1
						if (randam > 0.5)
						{
							// �ߋ����U��
							eObjectState = eAttack;
							CreateAttackStartEffect();
							// �A�j���[�V�����Z�b�g
							m_animator.SetAnimation(m_spModel->GetAnimation("Attack_Near2"), false);
							KdGameAudioManager::GetInstance().attackStartSE();
						}
						else
						{

							eObjectState = eMove;
							CreateAttackStartEffect();
							// �^�b�N���U��
							// �A�j���[�V�����Z�b�g
							m_animator.SetAnimation(m_spModel->GetAnimation("Attack_Near"), false);
							KdGameAudioManager::GetInstance().takkuruStandSE();
							KdGameAudioManager::GetInstance().attackStartSE();
						}


					}
					else
					{
						//�v���C���[�������ɂ���

						// �~�T�C������
						eObjectState = eBullet;
						m_animator.SetAnimation(m_spModel->GetAnimation("Attack_Bullet"), false);
						EnemyBulletCreate();	//�e����
						m_enemyBulletCreateFlg = true;
					}
				}
			}

			break;

		case KdGameObject::eMove:

			// �^�b�N���U��
			m_count++;

			// �A�j���[�V�������I�������
			if (m_animator.IsAnimationEnd())
			{
				m_count = 0;
				eObjectState = eStandBy;
				m_animator.SetAnimation(m_spModel->GetAnimation("StandBY"), true);
				m_moveSpd = 0.0f;
				takkuruMoveSEFlg = false;
			}



			// �v���C���[�����ɐi��
			if (m_count > 120)
			{
				// �a���G�t�F�N�g
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

			//�s��̍���
			m_mWorld *= trans;

			// Player���W���X�g�J�E���^�[�ɐ���������
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

			// �A�j���[�V�����J�E���g
			m_count++;

			// �A�j���[�V�������I�������
			if (m_animator.IsAnimationEnd())
			{
				m_count = 0;
				eObjectState = eStandBy;
				m_animator.SetAnimation(m_spModel->GetAnimation("StandBY"), true);
			}

			// Player���W���X�g�J�E���^�[�ɐ���������
			if (KdThrowManager::GetInstance().GetThrowFlg())
			{
				float spd = KdThrowManager::GetInstance().GetSpeed();
				if (spd >= 1.0f)
				{
					PlayerJustCounter();
				}
			}

			// �a���G�t�F�N�g
			UpdateSlashingAttackEffect();

			break;

		case KdGameObject::eBullet:

			//�e�����ł���
			m_count++;

			if (m_count >= (m_bulletCreate * 2))			//2���ł��I�������
			{
				eObjectState = eStandBy;
				m_animator.SetAnimation(m_spModel->GetAnimation("StandBY"), true);
				m_count = 0;
				break;
			}

			if (m_count % m_bulletCreate == 0)				//3�b�����ɒe�𐶐�
			{
				EnemyBulletCreate();
				m_enemyBulletCreateFlg = true;
			}

			break;

		case KdGameObject::eDamage:

			// �_���[�W���A�N�V����
			// �������C�[�W���O
			vMove = m_mWorld.Backward();
			m_progress += speed;
			progress = std::clamp(m_progress, 0.0f, 1.0f);


			vMove *= - (0.2f* EaseInOutSine(progress));					//���]�ƃX�s�[�h
			trans = Math::Matrix::CreateTranslation(vMove);

			//�s��̍���
			m_mWorld *= trans;

			// �A�j���[�V�������I�������
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


	//�~����
	UpdateCircleCollision();
	UpdateGroundCollision();

	//�A�j���[�V�����̎��Ԃ�i�߂�
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
			newAttack->SetDrawFrameMax(ra);	//5�`15

			Math::Vector3 pos = damagePos;
			Math::Vector3 dir;

			// ����
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
	// �^�b�N��
	std::shared_ptr<SlashingEffect> newSlashingEffect =
		std::make_shared<SlashingEffect>();

	// �摜�Z�b�g
	newSlashingEffect->SetAsset2D(KdAssetManager::GetInstance().m_EffectAsset.GetThunderTex());

	// �T�C�Y
	int ra = rand() % 20 + 5;	//5�`15
	newSlashingEffect->SetDrawFrameMax(ra);	//�f�J��
	newSlashingEffect->lifeSpan(60);			//����

	if (m_spModel)
	{
		const KdModelWork::Node* pLHead
			= m_spModel->FindNode("sLight");

		if (pLHead)
		{
			Math::Matrix mShot = pLHead->m_worldTransform * m_mWorld;
			Math::Vector3 playerPos = Math::Vector3::Zero;
			animationPos = mShot.Translation();

			// ����
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

	// player�̎���Ńo�`�o�`
	pos.x += (rand() % effectPos - (effectPos / 2)) * 0.5f;
	pos.y += (rand() % effectPos - (effectPos / 2)) * 0.5f;
	pos.z += (rand() % effectPos - (effectPos / 2)) * 0.5f;

	// ����
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
			// ��
			std::shared_ptr<Attack> newAttack =
				std::make_shared<Attack>();

			newAttack->SetAsset2D(KdAssetManager::GetInstance().m_EffectAsset.GetThunderTex());

			int ra = rand() % 10 + 35;
			newAttack->SetDrawFrameMax(ra);	//5�`15
			newAttack->Speed(2.0f);
			newAttack->lifeSpan(60);


			Math::Matrix mShot = m_mWorld;
			Math::Vector3 pos = mShot.Translation();
			pos.y += 1.0f;
			Math::Vector3 dir;

			// ����
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
	// �P���Ȏa��
	std::shared_ptr<SlashingEffect> newSlashingEffect =
		std::make_shared<SlashingEffect>();

	// �摜�Z�b�g
	newSlashingEffect->SetAsset2D(KdAssetManager::GetInstance().m_EffectAsset.GetBulletTex());

	// �T�C�Y
	int ra = rand() % 20 + 5;	//5�`15
	newSlashingEffect->SetDrawFrameMax(ra);	//�f�J��
	newSlashingEffect->lifeSpan(ra);			//����

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
	//�n�ʔ���
	//---------------------------------------------
	//�@�����蔻��p�̏��쐬
	Math::Vector3 rayPos, rayDir;
	float rayRange = 1.0f;				//2m�B�i����o���
	rayDir = Math::Vector3::Down;
	rayPos = m_mWorld.Translation();
	rayPos.y += rayRange;

	CollisionMeshResult groudResult;

	//�A�S�ẴQ�[���I�u�W�F�N�g�ɑ�������
	for (const std::shared_ptr<KdGameObject>& spObj :
		Application::Instance().GetGameSystem()->GetGameObjects())
	{
		if (spObj->GetClassID() == KdGameObject::eStage)
		{
			spObj->CollisionGround(rayPos, rayDir, rayRange, groudResult);
		}
	}

	//�B���ʂ��g���č��W��⊮����
	if (groudResult.m_hit)
	{
		SetPos(groudResult.m_hitPos);

		m_verticalMovement = 0;
	}
}

void normalEnemy::UpdateCircleCollision()
{
	//�~����̓����蔻��
	for (const std::shared_ptr<KdGameObject>& spObj :
		Application::Instance().GetGameSystem()->GetGameObjects())
	{
		//�v���C���[�ƃG�l�~�[�̋����`�F�b�N
		if (spObj->GetClassID() == KdGameObject::ePlayer)
		{
			Math::Vector3 offsetPos = { 0.0f,0.5f,0.0f };
			SphereInfo info(GetPos() + offsetPos, 20.0f);
			BumpResult result;

			spObj->CheckCollisionBump(info, result);	//�~

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

		//�v���C���[���ƃG�l�~�[
		if (spObj->GetClassID() == KdGameObject::ePlayerSowd)
		{
			if (spObj->GeteState() == KdGameObject::eAttack)
			{
				Math::Vector3 offsetPos = { 0.0f,0.5f,0.0f };
				SphereInfo info(GetPos() + offsetPos, 3.5f);
				BumpResult result;

				spObj->CheckCollisionBump(info, result);	//�~

				if (result.m_isHit)							//���Ɠ������Ă�����
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
	// �A�S�ẴQ�[���I�u�W�F�N�g�ɑ�������
	for (const std::shared_ptr<KdGameObject>& spObject :
		Application::Instance().GetGameSystem()->GetGameObjects())
	{
		if (spObject.get() == this) { continue; }

		// �I�u�W�F�N�g���ɉ����Ԃ������𔭐�������
		// �@�����蔻��p�̏��쐬
		DirectX::BoundingSphere sphere;
		sphere.Center = m_mWorld.Translation();
		sphere.Center.y += 0.6f;
		sphere.Radius = 0.5f;

		CollisionResult bumpResult;

		spObject->CollisionBump(sphere, bumpResult);

		if (!bumpResult.m_hit) { continue; }

		// �B ���ʂ��g���č��W��⊮����
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
