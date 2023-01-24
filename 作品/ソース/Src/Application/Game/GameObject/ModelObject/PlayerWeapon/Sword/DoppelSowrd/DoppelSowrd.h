#pragma once

#include"Application/Game/GameObject/ModelObject/PlayerWeapon/PlayerWeaponBase.h"

class DoppelSowrd :public PlayerWeaponBase
{
public:
	DoppelSowrd() {}
	virtual ~DoppelSowrd() override {}

	void Update()override;
	void DrawLighting()override;

	eClassID GetClassID() const override { return ePlayerSowd; }
};