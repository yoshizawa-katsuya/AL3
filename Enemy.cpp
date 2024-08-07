#include "Enemy.h"
#include "cassert"
#include "Vector.h"

//フェーズの関数テーブル
void (Enemy::*Enemy::spFuncTable[])() = {
    &Enemy::ApproachUpdate, // 接近
    &Enemy::LeaveUpdate     // 離脱
};

void Enemy::Initialize(Model* model, uint32_t textureHandle) {

	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 5.0f, 10.0f};

	//接近フェーズの更新関数を代入
	//pFunk_ = &Enemy::ApproachUpdate;

}

void Enemy::Update() {


	//メンバ関数ポインタに入っている関数を呼び出す
	(this->*spFuncTable[static_cast<size_t>(phase_)])();

	//(this->*pFunk_)();

	/*
	switch (phase_) {
	case Phase::Approach:
	default:
		ApproachUpdate();
		break;
	case Phase::Leave:
		LeaveUpdate();
		break;
	}
	*/

	worldTransform_.UpdateMatrix();

}

void Enemy::ApproachUpdate() {
	// 移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, ApproachVelocity_);
	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveUpdate() {
	// 移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, LeaveVelocity_);
	
}

void Enemy::Draw(const ViewProjection& viewProjection) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);


}