#include "BaseCharacter.h"
#include "cassert"

void BaseCharacter::Initialize(const std::vector<Model*>& models, ViewProjection* viewProjection) {

	// NULLポインタチェック
	for (Model* model : models) {
		assert(model);
	}

	models_ = models;
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;

}

void BaseCharacter::Update() {

	worldTransform_.UpdateMatrix();

}

void BaseCharacter::Draw() {

	//モデル描画
	for (Model* model : models_) {
	
		model->Draw(worldTransform_, *viewProjection_);
	
	}

}