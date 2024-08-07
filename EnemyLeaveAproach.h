#pragma once
#include "BaseEnemyState.h"
#include "Vector3.h"

class EnemyLeaveAproach : public BaseEnemyState {
public:
	// コンストラクタ
	EnemyLeaveAproach(Enemy* enemy);
	// 更新
	void Update() override;

private:

	Vector3 LeaveVelocity_ = {0.2f, -0.2f, 0.0f};


};
