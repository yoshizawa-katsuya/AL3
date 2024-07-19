#include "Hammer.h"
#include "cassert"
#include "Matrix.h"
#include "CollisionTypeIdDef.h"
#include "Enemy.h"

void Hammer::Initialize(Model* model) {

	Collider::Initialize();
	Collider::SetRadius(3.0f);
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayerWeapon));

	assert(model);

	model_ = model;
	worldTransform_.Initialize();

}

void Hammer::UpdateWorldTransform() {

	worldTransform_.UpdateMatrix();

}

void Hammer::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection);

}

void Hammer::OnCollision([[maybe_unused]] Collider* other) {

	//衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	//消灯相手が敵なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) {
	
		Enemy* enemy = static_cast<Enemy*>(other);
		//敵の位置にエフェクトを発生
		enemy->Hit();
	}

}

Vector3 Hammer::GetCenterPosition() const { 

	// ローカル座標でのオフセット
	const Vector3 offset = {0.0f, 10.5f, 0.0f};
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);
	return worldPos;

}
