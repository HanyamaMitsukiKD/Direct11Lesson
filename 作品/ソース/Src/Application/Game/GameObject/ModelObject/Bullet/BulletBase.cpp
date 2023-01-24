#include "BulletBase.h"
#include"Application/main.h"
#include"Application/Game/KdGameSystem.h"

const std::shared_ptr<normalEnemy> BulletBase::SetEnemy()
{
	return Application::Instance().GetGameSystem()->GetEnemy();
}
