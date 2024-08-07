#include "EnemyStateAproach.h"
#include "EnemyLeaveAproach.h"
#include "Enemy.h"
#include "Vector.h"

EnemyStateAproach::EnemyStateAproach(Enemy* enemy) 
: BaseEnemyState("State Approache", enemy) {
	DebugLog();
}

void EnemyStateAproach::Update() {

	// 移動
	enemy_->SetTranslation(Add(enemy_->GetTranslation(), ApproachVelocity_));
	// 規定の位置に到達したら離脱
	if (enemy_->GetTranslation().z < 0.0f) {
		enemy_->ChangeState(std::make_unique<EnemyLeaveAproach>(enemy_));
	}

}
