#include "EnemyBullet.h"
#include "cassert"
#include "TextureManager.h"
#include "Vector.h"
#include "Matrix.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3 velocity) {

	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("./Resources/red.png");

	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	//Z方向に伸びた形状
	worldTransform_.scale_ = {0.5f, 0.5f, 3.0f};

	velocity_ = velocity;

	// Y軸回り角度(θy)
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	Matrix4x4 minusThetaY = MakeRotateYMatrix(-worldTransform_.rotation_.y);
	Vector3 velocityZ = Transform(velocity_, minusThetaY);
	//X軸回り角度(θx)
	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);

}

void EnemyBullet::Update() {

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// 座標を移動させる
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	worldTransform_.UpdateMatrix();

}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);


}