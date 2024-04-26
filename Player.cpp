#include "Player.h"
#include "cassert"

void Player::Initialize(Model* model, ViewProjection* viewProjection) {
	//NULLポインタチェック
	assert(model);

	model_ = model;
	
	worldTransform_.Initialize();
	worldTransform_.translation_.y += 2;
	worldTransform_.translation_.x -= 2;
	viewProjection_ = viewProjection;

}

void Player::Update() {

	worldTransform_.UpdateMatrix();

}

void Player::Draw() {

	model_->Draw(worldTransform_, *viewProjection_);

}