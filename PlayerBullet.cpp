#include "PlayerBullet.h"
#include "cassert"
#include "TextureManager.h"
#include "Vector.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

	//NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("./Resources/black.png");

	worldTransform_.Initialize();
	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

}

void PlayerBullet::Update() {

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	//座標を移動させる
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	worldTransform_.UpdateMatrix();

}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}