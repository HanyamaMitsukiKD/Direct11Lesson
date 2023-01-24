#include "nomalBullet.h"
#include"Application/Game/KdGameSystem.h"
#include"Application/main.h"
#include"Application/Game/GameObject/EffectObject/SmokeEffect/SmokeEffect.h"
#include"System/Manager/KdAudioManager/KdGameAudioManager.h"
#include"Application/Game/GameObject/EffectObject/Particle/ParticleFire/ParticleFire.h"


void nomalBullet::Update()
{
	// �e�̏��
	NormalUpdate();

	// �n�ʔ���
	UpdateGroundCollision();
}

void nomalBullet::NormalUpdate()
{
	Math::Matrix enemyTrans = Math::Matrix::Identity;
	Math::Matrix rotation = Math::Matrix::Identity;
	std::shared_ptr<EnemyBase> spEnemy = SetEnemy();

	if (!spEnemy) { return; }

	Math::Vector3 enemypos = spEnemy->GetPos();

	enemypos.y += 8;

	switch (eObjectState)
	{
	case KdGameObject::eMove:

		// �w�����甭��
		if (bulletYPosSpeed < (0.5 * 10))
		{
			bulletYPosSpeed += 0.3f;
		}
		else
		{
			eObjectState = KdGameObject::eAttack;
		}

		m_Alive = true;

		//���X�ɏ�ɏオ���Ă���
		enemypos.y += bulletYPosSpeed;
		enemyTrans = Math::Matrix::CreateTranslation(enemypos);

		// X��180�x�Ȃ���
		rotation = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(-90));

		m_mWorld = rotation * enemyTrans;

		break;

	case KdGameObject::eAttack:

		//����Ɍ�������
		UpdateMove();

		m_Alive = true;

		m_count++;

		if (m_count > 50)
		{
			// ���G�t�F�N�g
			SmokeUpdate();
		}

		if (m_count > 300)				//���˂����5�b�o���������
		{
			NeedDeleteTrue();
		}

		break;
	}
}

void nomalBullet::Init()
{
	// �C�[�W���O�֐��p�ϐ�������
	m_moveSpeed = 0.0f;

	eObjectState = KdGameObject::eMove;
}

void nomalBullet::DrawLighting()
{
	if (m_Alive)
	{
		D3D.WorkShaderManager().m_standardShader.DrawModel(*m_spModel, m_mWorld);
	}
}


void nomalBullet::UpdateMove()
{
	std::shared_ptr<const Player> spPlayer = m_wpPlayer.lock();

	if (spPlayer->GeteState() == KdGameObject::eJustAvoidance)		//�W���X�g��𒆂�
	{
		return;
	}


	if (m_moveSpeed <= 1.0f)
	{
		m_moveSpeed += 0.01f;
	}

	Math::Vector3 vMove = m_mWorld.Backward();
	vMove.Normalize();

	Math::Matrix trans = Math::Matrix::CreateTranslation(vMove * (0.4f * m_Objectspeed));

	//�s��̍���
	m_mWorld *= trans;

	//�G�̈ʒu�擾
	if (m_moveSpeed <= 0.7f)
	{
		UpdateRotate();
	}
}

void nomalBullet::UpdateRotate()
{
	//�@���M�̌����Ă�������x�N�g��
	Math::Vector3 nowDir = m_mWorld.Backward();

	//�A�^�[�Q�b�g�Ɍ����������x�N�g��
	Math::Vector3 targetPos = Application::Instance().GetGameSystem()->GetPlayer()->GetPos();
	Math::Vector3 targetDir = targetPos - GetPos();

	nowDir.Normalize();
	targetDir.Normalize();

	//��]�p�x�����߂�
	//�@�ƇA�̓���
	float dot = nowDir.Dot(targetDir);
	dot = std::clamp(dot, -1.0f, 1.0f);

	//���ς���t�\���̒藝�ŊԂ̊p�x�����߂�B�x�N�g�����m�̊p�x
	float betweenAng = acos(dot);
	betweenAng = DirectX::XMConvertToDegrees(betweenAng);

	// -4.0f����4.0f�̊p�x�܂�
	float rotateAng = std::clamp(betweenAng, -4.0f, 4.0f);

	//3������̉�]�������߂�
	Math::Vector3 rotAxis;

	//2�̃x�N�g������u�����v�ɐL�т�x�N�g��
	nowDir.Cross(targetDir, rotAxis);	//nowDir��targetDir����݂ĂX�O�x�ɂ���rotAxis(�x�N�g��)
	if (!rotAxis.LengthSquared()) { return; }

	//�U����]���邽�߂̍s��
	Math::Matrix rotation;
	rotation = Math::Matrix::CreateFromAxisAngle(rotAxis, DirectX::XMConvertToRadians(rotateAng));

	//���̃��[���h���W���ꎞ�I�ɕۑ�
	Math::Vector3 pos = GetPos();

	//���[���h���W������W���𔲂��i��]�����̍s��ɂ���j
	m_mWorld.Translation(Math::Vector3(0, 0, 0));

	//��]�s��̍���
	m_mWorld *= rotation;
	m_mWorld.Translation(pos);
}


void nomalBullet::UpdateGroundCollision()
{
	//�n�ʔ���
	//---------------------------------------------
	//�@�����蔻��p�̏��쐬
	Math::Vector3 rayPos, rayDir;
	float rayRange = 2.0f;				//2m�B�i����o���
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
		EffectFire(GetPos());
		KdGameAudioManager::GetInstance().BomSE();
		NeedDeleteTrue();
	}
}

void nomalBullet::SmokeUpdate()
{
	//�@�������
	std::shared_ptr<ParticleSmoke> newParticleSmoke =
		std::make_shared<ParticleSmoke>();

	newParticleSmoke->SetAsset("Asset/Texture/smoke3.png");
	newParticleSmoke->SetDrawFrameMax(10);

	Math::Vector3 pos = m_mWorld.Translation();
	Math::Vector3 dir = m_mWorld.Backward();

	// ����
	dir.y += (rand() % 200 - 100) * 0.0005f;
	dir.Normalize();

	newParticleSmoke->SetMatrix(pos, -dir);

	Application::Instance().
		WorkGameSystem()->AddObject(newParticleSmoke);
}

void nomalBullet::EffectFire(Math::Vector3& pos)
{
	//����
	static const int fireNum = 400;
	ParticleFire fire[fireNum];//���̉�

	std::shared_ptr<KdTexture> particleTex = std::make_shared<KdTexture>();
	particleTex->Load("Asset/Texture/smoke.png");

	for (int i = 0; i < fireNum; i++)
	{
		std::shared_ptr<ParticleFire> spParticleFire = std::make_shared<ParticleFire>();
		spParticleFire->SetTexture(particleTex);
		spParticleFire->Init();

		spParticleFire->Emit(
			Math::Vector3(pos),			//���W
			Math::Vector3(fire[i].Rnd() + 2 - 1, fire[i].Rnd() * 3, 0),			//�ړ���
			fire[i].Rnd() * 6 - 3,										//�T�C�Y
			Math::Color(fire[i].Rnd(), 0.2, 0.2, 0.8),					//�F
			(float)fire[i].Rnd() * 90 + 10,									//�L������
			false);

		Application::Instance().
			WorkGameSystem()->AddObject(spParticleFire);
	}
}