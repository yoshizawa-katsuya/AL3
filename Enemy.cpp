#include "Enemy.h"
#include "cassert"
#include "Lerp.h"

void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {

	// NULLポインタチェック
	assert(model);

	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float>  * 1.5f;

	viewProjection_ = viewProjection;

	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
}

void Enemy::Update() {

	//移動
	worldTransform_.translation_.x += velocity_.x;

	//タイマーを加算
	walkTimer_ += 1.0f / 60.0f;

	//回転アニメーション
	float param = std::sin(std::numbers::pi_v<float> * walkTimer_ / kWalkMotionTime);
	float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * ((param + 1.0f) / 2.0f);
	worldTransform_.rotation_.x = radian * std::numbers::pi_v<float> / 180;

	// 行列計算
	worldTransform_.UpdateMatrix();

}

void Enemy::Draw() {

	model_->Draw(worldTransform_, *viewProjection_);

}

void Enemy::OnCollision(const Player* player) {

	(void)player;

}

Vector3 Enemy::GetWorldPosition() { 

	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Enemy::GetAABB() { 
	AABB aabb;
	Vector3 worldPos = GetWorldPosition();
	aabb.min.x = worldPos.x - (kWidth / 2.0f);
	aabb.min.y = worldPos.y - (kWidth / 2.0f);
	aabb.min.z = worldPos.z - (kWidth / 2.0f);

	aabb.max.x = worldPos.x + (kWidth / 2.0f);
	aabb.max.y = worldPos.y + (kWidth / 2.0f);
	aabb.max.z = worldPos.z + (kWidth / 2.0f);

	return aabb;
}
