#pragma once

#include"Application/Game/GameObject/EffectObject/Particle/ParticleBase/ParticleBase.h"

class ParticleFire :public ParticleBase
{
public:
	ParticleFire() {}
	virtual ~ParticleFire() {}

	 void Init()override;
	 void Update()override;
	
private:
	
};
