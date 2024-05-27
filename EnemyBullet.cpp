#include "EnemyBullet.h"
#include "cassert"
#include "TextureManager.h"
#include "Vector.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position) {

	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("./Resources/red.png");

	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	velocity_ = {0.0f, 0.0f, -1.0f};

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