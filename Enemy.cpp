#include "Enemy.h"
#include "cassert"
#include "Vector.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle) {

	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 5.0f, 10.0f};
}

void Enemy::Update() {

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, {0.0f, 0.0f, -kCharacterSpeed});

	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);


}