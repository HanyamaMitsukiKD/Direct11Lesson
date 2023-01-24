#pragma once

#include"Application/Game/GameObject/ModelObject/PlayerWeapon/PlayerWeaponBase.h"
#include"Application/Game/GameObject/ModelObject/PlayerWeapon/PlayerBullet/PlayerBullet.h"


class HomingBullet :public PlayerWeaponBase
{
public:
	HomingBullet() {}
	virtual ~HomingBullet() override {}

	void Update()override;
	void DrawLighting()override;
	
	void Init()override;
	
	eClassID GetClassID() const override { return ePlayerBullet; }

protected:
	

private:

};
