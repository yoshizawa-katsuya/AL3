#include "Player.h"
#include "cassert"

void Player::Initialize(Model* model, ViewProjection* viewProjection) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
}

void Player::Update() {
	worldTransform_.TransferMatrix(); 
}

void Player::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_);
}