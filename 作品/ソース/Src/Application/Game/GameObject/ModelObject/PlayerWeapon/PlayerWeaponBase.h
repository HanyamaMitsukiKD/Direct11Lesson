#pragma once

#include"Application/Game/GameObject/ModelObject/KdModelObject.h"
#include"Application/main.h"
#include"Application/Game/KdGameSystem.h"

class PlayerWeaponBase :public KdModelObject
{
public:
	PlayerWeaponBase() {}
	virtual ~PlayerWeaponBase() override {}

	const std::shared_ptr<Player> GetPlayer() const
	{
		return  Application::Instance().GetGameSystem()->GetPlayer();
	}

	std::shared_ptr<Player>& WorkPlayer()
	{
		return  Application::Instance().GetGameSystem()->WorkPlayer();
	}

protected:

};