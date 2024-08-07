#include "EnemyLeaveAproach.h"
#include "Enemy.h"
#include "Vector.h"

EnemyLeaveAproach::EnemyLeaveAproach(Enemy* enemy)
: BaseEnemyState("State Leave", enemy)
{ 
	DebugLog();
}

void EnemyLeaveAproach::Update() {

	// 移動
	enemy_->SetTranslation(Add(enemy_->GetTranslation(), LeaveVelocity_));

}
