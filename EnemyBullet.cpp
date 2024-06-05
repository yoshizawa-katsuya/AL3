#include "EnemyBullet.h"
#include "cassert"
#include "TextureManager.h"
#include "Vector.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("./Resources/red.png");

	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	velocity_ = velocity;

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

void EnemyBullet::OnCollision() { isDead_ = true; }

void EnemyBullet::Draw(const ViewProjection& viewProjection) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);


}

Vector3 EnemyBullet::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worlsPos;
	// ワールド行列の平行移動成分を取得
	worlsPos.x = worldTransform_.matWorld_.m[3][0];
	worlsPos.y = worldTransform_.matWorld_.m[3][1];
	worlsPos.z = worldTransform_.matWorld_.m[3][2];

	return worlsPos;
}