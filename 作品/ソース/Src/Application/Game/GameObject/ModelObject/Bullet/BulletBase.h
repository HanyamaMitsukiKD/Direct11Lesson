#pragma once

#include"Application/Game/GameObject/ModelObject/KdModelObject.h"
#include"Application/Game/GameObject/ModelObject/Character/Enemy/EnemyBase.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/Player.h"
#include"Application/Game/GameObject/ModelObject/Character/Enemy/normalEnemy/normalEnemy.h"

class BulletBase :public KdModelObject
{
public:
	BulletBase() {}
	virtual ~BulletBase() override {}

	void Update()override{};

	const std::shared_ptr<normalEnemy> SetEnemy();

	void SetPlayer(const std::shared_ptr<Player> player)
	{
		m_wpPlayer = player;	
	}

	eClassID GetClassID() const override { return eEnemyBullet; }


protected:
	std::weak_ptr<Player>		m_wpPlayer;								//ÉvÉåÉCÉÑÅ[èÓïÒ

	
};