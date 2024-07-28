#include "DeathParticle.h"
#include <cassert>
#include "Matrix.h"
#include "algorithm"

void DeathParticle::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {

	// NULLポインタチェック
	assert(model);

	model_ = model;

	viewProjection_ = viewProjection;

	//ワールド変数の初期化
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}

	objectColor_.Initialize();
	color_ = {1.0f, 1.0f, 1.0f, 1.0f};

}

void DeathParticle::Update() {

	//終了なら何もしない
	if (isFinished_) {
		return;
	}

	for (uint32_t i = 0; i < kNumParticles_; ++i) {
		//基本となる速度ベクトル
		Vector3 velocity = {kSpeed_, 0.0f, 0.0f};
		//回転角を計算する
		float angle = kAngleUnit_ * i;
		//Z軸回り回転行列
		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);
		//基本ベクトルを回転させて速度ベクトルを得る
		velocity = Transform(velocity, matrixRotation);
		//移動処理
		worldTransforms_[i].translation_ += velocity;
	}

	
	// ワールド変数の更新
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.UpdateMatrix();
	}

	color_.w = std::clamp(1.0f - (counter_ / kDuration_), 0.0f, 1.0f);
	//色変更オブジェクトに色の数値を設定する
	objectColor_.SetColor(color_);
	//色変更オブジェクトをVRAMに転送
	objectColor_.TransferMatrix();

	// カウンターを１フレーム分の秒数進める
	counter_ += 1.0f / 60.0f;

	// 存続時間の上限に達したら
	if (counter_ >= kDuration_) {
		counter_ = kDuration_;
		// 終了扱いにする
		isFinished_ = true;
	}


}


void DeathParticle::Draw() {

	// 終了なら何もしない
	if (isFinished_) {
		return;
	}


	for (auto& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, *viewProjection_, &objectColor_);
	}
}
