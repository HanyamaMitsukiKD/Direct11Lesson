#include "CounterBullet.h"

void CounterBullet::Update()
{
	std::shared_ptr<Player> spPlayer = WorkPlayer();
	if (!spPlayer) { return; }

	//敵の位置取得
	for (const std::shared_ptr<KdGameObject>& spObj :
		Application::Instance().GetGameSystem()->GetGameObjects())
	{

		if (spObj->GetClassID() != KdGameObject::eEnemy)continue;

		m_moveDir = spObj->GetPos();
		m_moveDir.y += 4.0f;

		//指定された座標へのベクトル
		Math::Vector3 TargetDir = m_moveDir - GetPos();
		TargetDir.Normalize();

		//弾の位置移動
		m_mWorld *= Math::Matrix::CreateTranslation(TargetDir * 3.0f);
	}
}

void CounterBullet::DrawLighting()
{
	if (m_Alive)
	{
		D3D.WorkShaderManager().m_standardShader.DrawModel(*m_spModel, m_mWorld);
	}
}

void CounterBullet::Init()
{
	eObjectState = KdGameObject::eStandBy;
}
