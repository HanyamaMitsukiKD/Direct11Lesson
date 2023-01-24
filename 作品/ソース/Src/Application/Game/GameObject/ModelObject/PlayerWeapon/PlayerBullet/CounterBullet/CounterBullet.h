#pragma once


#include"Application/Game/GameObject/ModelObject/PlayerWeapon/PlayerWeaponBase.h"
#include"Application/Game/GameObject/ModelObject/PlayerWeapon/PlayerBullet/PlayerBullet.h"


class CounterBullet :public PlayerWeaponBase
{
public:
	CounterBullet() {}
	virtual ~CounterBullet() override {}

	void Update()override;
	void DrawLighting()override;

	void Init()override;

	eClassID GetClassID() const override { return ePlayerDownBullet; }

protected:


private:
	
};
