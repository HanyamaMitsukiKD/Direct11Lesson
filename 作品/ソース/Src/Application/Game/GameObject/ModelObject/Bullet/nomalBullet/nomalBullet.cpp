#include "nomalBullet.h"
#include"Application/Game/KdGameSystem.h"
#include"Application/main.h"
#include"Application/Game/GameObject/EffectObject/SmokeEffect/SmokeEffect.h"
#include"System/Manager/KdAudioManager/KdGameAudioManager.h"
#include"Application/Game/GameObject/EffectObject/Particle/ParticleFire/ParticleFire.h"


void nomalBullet::Update()
{
	// 弾の状態
	NormalUpdate();

	// 地面判定
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

		// 背中から発射
		if (bulletYPosSpeed < (0.5 * 10))
		{
			bulletYPosSpeed += 0.3f;
		}
		else
		{
			eObjectState = KdGameObject::eAttack;
		}

		m_Alive = true;

		//除々に上に上がっていく
		enemypos.y += bulletYPosSpeed;
		enemyTrans = Math::Matrix::CreateTranslation(enemypos);

		// X軸180度曲げる
		rotation = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(-90));

		m_mWorld = rotation * enemyTrans;

		break;

	case KdGameObject::eAttack:

		//相手に向く処理
		UpdateMove();

		m_Alive = true;

		m_count++;

		if (m_count > 50)
		{
			// 煙エフェクト
			SmokeUpdate();
		}

		if (m_count > 300)				//発射されて5秒経ったら消す
		{
			NeedDeleteTrue();
		}

		break;
	}
}

void nomalBullet::Init()
{
	// イージング関数用変数初期化
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

	if (spPlayer->GeteState() == KdGameObject::eJustAvoidance)		//ジャスト回避中は
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

	//行列の合成
	m_mWorld *= trans;

	//敵の位置取得
	if (m_moveSpeed <= 0.7f)
	{
		UpdateRotate();
	}
}

void nomalBullet::UpdateRotate()
{
	//①自信の向いている方向ベクトル
	Math::Vector3 nowDir = m_mWorld.Backward();

	//②ターゲットに向かう方向ベクトル
	Math::Vector3 targetPos = Application::Instance().GetGameSystem()->GetPlayer()->GetPos();
	Math::Vector3 targetDir = targetPos - GetPos();

	nowDir.Normalize();
	targetDir.Normalize();

	//回転角度を求める
	//①と②の内積
	float dot = nowDir.Dot(targetDir);
	dot = std::clamp(dot, -1.0f, 1.0f);

	//内積から逆予言の定理で間の角度を求める。ベクトル同士の角度
	float betweenAng = acos(dot);
	betweenAng = DirectX::XMConvertToDegrees(betweenAng);

	// -4.0fから4.0fの角度まで
	float rotateAng = std::clamp(betweenAng, -4.0f, 4.0f);

	//3次元上の回転軸を求める
	Math::Vector3 rotAxis;

	//2つのベクトルから「垂直」に伸びるベクトル
	nowDir.Cross(targetDir, rotAxis);	//nowDirとtargetDirからみて９０度にあるrotAxis(ベクトル)
	if (!rotAxis.LengthSquared()) { return; }

	//誘導回転するための行列
	Math::Matrix rotation;
	rotation = Math::Matrix::CreateFromAxisAngle(rotAxis, DirectX::XMConvertToRadians(rotateAng));

	//今のワールド座標を一時的に保存
	Math::Vector3 pos = GetPos();

	//ワールド座標から座標情報を抜く（回転だけの行列にする）
	m_mWorld.Translation(Math::Vector3(0, 0, 0));

	//回転行列の合成
	m_mWorld *= rotation;
	m_mWorld.Translation(pos);
}


void nomalBullet::UpdateGroundCollision()
{
	//地面判定
	//---------------------------------------------
	//①当たり判定用の情報作成
	Math::Vector3 rayPos, rayDir;
	float rayRange = 2.0f;				//2m。段差を登る量
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
		EffectFire(GetPos());
		KdGameAudioManager::GetInstance().BomSE();
		NeedDeleteTrue();
	}
}

void nomalBullet::SmokeUpdate()
{
	//　煙を作る
	std::shared_ptr<ParticleSmoke> newParticleSmoke =
		std::make_shared<ParticleSmoke>();

	newParticleSmoke->SetAsset("Asset/Texture/smoke3.png");
	newParticleSmoke->SetDrawFrameMax(10);

	Math::Vector3 pos = m_mWorld.Translation();
	Math::Vector3 dir = m_mWorld.Backward();

	// 方向
	dir.y += (rand() % 200 - 100) * 0.0005f;
	dir.Normalize();

	newParticleSmoke->SetMatrix(pos, -dir);

	Application::Instance().
		WorkGameSystem()->AddObject(newParticleSmoke);
}

void nomalBullet::EffectFire(Math::Vector3& pos)
{
	//爆煙
	static const int fireNum = 400;
	ParticleFire fire[fireNum];//実体化

	std::shared_ptr<KdTexture> particleTex = std::make_shared<KdTexture>();
	particleTex->Load("Asset/Texture/smoke.png");

	for (int i = 0; i < fireNum; i++)
	{
		std::shared_ptr<ParticleFire> spParticleFire = std::make_shared<ParticleFire>();
		spParticleFire->SetTexture(particleTex);
		spParticleFire->Init();

		spParticleFire->Emit(
			Math::Vector3(pos),			//座標
			Math::Vector3(fire[i].Rnd() + 2 - 1, fire[i].Rnd() * 3, 0),			//移動量
			fire[i].Rnd() * 6 - 3,										//サイズ
			Math::Color(fire[i].Rnd(), 0.2, 0.2, 0.8),					//色
			(float)fire[i].Rnd() * 90 + 10,									//有効期間
			false);

		Application::Instance().
			WorkGameSystem()->AddObject(spParticleFire);
	}
}