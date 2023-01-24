#include "CharacterBase.h"
#include"Application/main.h"
#include"Application/Game/KdGameSystem.h"

void CharacterBase::Update()
{
	//d—Í
	m_verticalMovement +=Application::Instance().GetGameSystem()->GetGravity();
	
	m_mWorld._42 += m_verticalMovement;
}