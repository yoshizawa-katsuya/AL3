#include "Enemy.h"
#include "Matrix.h"
#include "Vector.h"

void Enemy::Initialize(const std::vector<Model*>& models, ViewProjection* viewProjection) {

	BaseCharacter::Initialize(models, viewProjection);

	worldTransformBody_.Initialize();
	worldTransformBody_.parent_ = &worldTransform_;
	
	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.translation_ = Vector3(-1.7f, 1.3f, 0.0f);
	worldTransformL_arm_.parent_ = &worldTransformBody_;

	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.translation_ = Vector3(1.7f, 1.3f, 0.0f);
	worldTransformR_arm_.parent_ = &worldTransformBody_;

}

void Enemy::Update() {

	worldTransform_.rotation_.y += 0.05f;

	//移動速度
	const float kSpeed = 0.5f;
	Vector3 velocity(0, 0, kSpeed);

	//移動方向を向きに合わせる
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	//移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity);

	BaseCharacter::Update();

	worldTransformBody_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

}

void Enemy::Draw() {

	models_[kModelIndexBody]->Draw(worldTransformBody_, *viewProjection_);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, *viewProjection_);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, *viewProjection_);
	

	
}