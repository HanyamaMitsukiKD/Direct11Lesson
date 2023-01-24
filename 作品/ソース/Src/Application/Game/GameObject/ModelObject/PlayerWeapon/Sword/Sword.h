#pragma once

#include"Application/Game/GameObject/ModelObject/PlayerWeapon/PlayerWeaponBase.h"
#include"Application/Game/GameObject/EffectObject/SwordEffect/SwordEffect.h"

class Sword :public PlayerWeaponBase
{
public:
	Sword() {}
	virtual ~Sword() override {}

	void Update()override;
	void DrawLighting()override;
	
	eClassID GetClassID() const override { return ePlayerSowd; }

	void SwordEffectUpdate();
	void CreateSwordEffect();

private:
	std::list<std::shared_ptr<SwordEffect>>	m_spAttacks;
};