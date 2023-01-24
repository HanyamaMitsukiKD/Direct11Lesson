#include "Sword.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/Player.h"
#include"Application/Game/GameObject/ModelObject/PlayerWeapon/Sword/DoppelSowrd/DoppelSowrd.h"
#include"Application/main.h"
#include"Application/Game/KdGameSystem.h"

#include"System/Manager/KdAssetManager/KdAssetManager.h"

void Sword::Update()
{
	std::shared_ptr<Player> spPlayer = GetPlayer();
	if (!spPlayer) { return; }

	// プレイヤーが生きていたら表示
	m_Alive = spPlayer->GetAlive();

	eObjectState = spPlayer->GeteState();

	const KdModelWork::Node* pNode = spPlayer->GetModelWork()->FindNode("WeaponPos");

	// 指定ノードがみつかったら
	if (pNode)
	{
		Math::Matrix mat;

		mat = pNode->m_worldTransform * spPlayer->GetMatrix();

		SetMatrix(mat);
	}

	if (spPlayer->GeteState() == eState::eAttack)
	{
		CreateSwordEffect();
	}

	SwordEffectUpdate();
}

void Sword::SwordEffectUpdate()
{
	std::shared_ptr<Player> spPlayer = GetPlayer();
	if (!spPlayer) { return; }

	const KdModelWork::Node* pEffectStartNode = m_spModel->FindNode("EffectStart");
	const KdModelWork::Node* pEffectEndNode = m_spModel->FindNode("EffectEnd");
		

	Math::Matrix startMat = pEffectStartNode->m_worldTransform * m_mWorld;
	Math::Matrix endMat = pEffectEndNode->m_worldTransform * m_mWorld;

	for (std::shared_ptr<SwordEffect>& spEffect : m_spAttacks)
	{
		spEffect->Update(startMat.Translation(), endMat.Translation(), m_mWorld);
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

void Sword::CreateSwordEffect()
{
	// 単純な斬撃
	std::shared_ptr<SwordEffect> newSwordEffect =
		std::make_shared<SwordEffect>();

	// 画像セット
	newSwordEffect->SetAsset2D(KdAssetManager::GetInstance().m_EffectAsset.GetSwordEffectTex());

	newSwordEffect->SetMatrix();

	Application::Instance().
		WorkGameSystem()->AddObject(newSwordEffect);

	m_spAttacks.push_back(newSwordEffect);
}

void Sword::DrawLighting()
{
	std::shared_ptr<Player> spPlayer = GetPlayer();
	if (!spPlayer) { return; }

	if (m_Alive)
	{
		D3D.WorkShaderManager().m_standardShader.DrawModel(*m_spModel, m_mWorld);
		if (spPlayer->GetCounterCountFlg())
		{
			// ウルト中なら縁取り
			D3D.WorkShaderManager().m_standardShader.DrawModelOutline(*m_spModel, m_mWorld, spPlayer->GetOutLineValue());
		}
	}
}
