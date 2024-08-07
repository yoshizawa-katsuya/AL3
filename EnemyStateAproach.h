#pragma once
#include "BaseEnemyState.h"
#include "Vector3.h"

//接近フェーズ
class EnemyStateAproach : public BaseEnemyState{
public:

	//コンストラクタ
	EnemyStateAproach(Enemy* enemy);
	//更新
	void Update() override;

private:

	Vector3 ApproachVelocity_ = {0.0f, 0.0f, -0.2f};

};
