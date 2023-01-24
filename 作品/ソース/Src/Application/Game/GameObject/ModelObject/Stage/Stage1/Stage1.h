#pragma once

#include"Application/Game/GameObject/ModelObject/KdModelObject.h"

class Stage1 :public KdModelObject
{
public:
	Stage1() {}
	virtual ~Stage1() override {}

	void Update()override {};

	eClassID GetClassID() const override { return eStage; }

	

protected:

	
};