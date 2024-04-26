#include "Player.h"
#include "cassert"

void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {
	//NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_.y += 2;
	viewProjection_ = viewProjection;

}

void Player::Update() {

	worldTransform_.UpdateMatrix();

}

void Player::Draw() {

	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);

}