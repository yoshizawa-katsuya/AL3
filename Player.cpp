#include "Player.h"
#include "cassert"
#include "Input.h"
#include "Vector.h"
#include "Matrix.h"
#include "ViewProjection.h"
#include "ImGuiManager.h"
#include <cmath>

void Player::Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm, ViewProjection* viewProjection) {
	// NULLポインタチェック
	assert(modelBody);
	assert(modelHead);
	assert(modelL_arm);
	assert(modelR_arm);

	modelBody_ = modelBody;
	modelHead_ = modelHead;
	modelL_arm_ = modelL_arm;
	modelR_arm_ = modelR_arm;

	worldTransformBase_.Initialize();

	worldTransformBody_.Initialize();
	worldTransformBody_.parent_ = &worldTransformBase_;

	worldTransformHead_.Initialize();
	worldTransformHead_.translation_ = Vector3(0.0f, 3.5f, 0.0f);
	worldTransformHead_.parent_ = &worldTransformBody_;

	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.translation_ = Vector3(-1.3f, 2.8f, 0.0f);
	worldTransformL_arm_.parent_ = &worldTransformBody_;

	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.translation_ = Vector3(1.3f, 2.8f, 0.0f);
	worldTransformR_arm_.parent_ = &worldTransformBody_;

	viewProjection_ = viewProjection;
	targetAngle_ = worldTransformBase_.rotation_.y;

	InitializeFloatingGimmick();

	InitializeRollArmGimmick();

}

void Player::InitializeFloatingGimmick() {

	floatingParameter_ = 0.0f;

}

void Player::InitializeRollArmGimmick() {

	rollArmParameter_ = 0.0f;

}

void Player::Update() {

	UpdateFloatingGimmick();

	UpdateRollArmGimmick();

	//ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		const float thresholdValue = 0.7f;
		bool isMoving = false;

		// 移動量
		Vector3 move = {static_cast<float>(joyState.Gamepad.sThumbLX) / SHRT_MAX, 0.0f, static_cast<float>(joyState.Gamepad.sThumbLY) / SHRT_MAX};

		if (Length(move) > thresholdValue) {
			isMoving = true;
		}

		if (isMoving) {
			// 速さ
			const float speed = 0.3f;

			// 移動量に速さを反映
			move = Multiply(speed, Normalize(move));

			Matrix4x4 rotationMatrix = MakeRotateYMatrix(cameraViewProjection_->rotation_.y);

			move = TransformNormal(move, rotationMatrix);

			// 移動
			worldTransformBase_.translation_ = Add(worldTransformBase_.translation_, move);

			// 目標角度の算出
			targetAngle_ = std::atan2(move.x, move.z);
		}
	}

	//最短角度補完
	worldTransformBase_.rotation_.y = LeapShortAngle(worldTransformBase_.rotation_.y, targetAngle_, angleCompletionRate_);

	//行列を更新
	worldTransformBase_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
}

void Player::UpdateFloatingGimmick() {

	//浮遊移動のサイクル<frame>
	const uint16_t cycle = 120;

	
	//1フレームでのパラメーター加算値
	const float step = 2.0f * static_cast<float>(M_PI) / cycle;

	//パラメーターを1ステップ分加算
	floatingParameter_ += step;
	//2πを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * static_cast<float>(M_PI));

	//浮遊の振幅<m>
	const float amplitude = 0.3f;

	

	//浮遊を座標に反映
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * amplitude;

	

}

void Player::UpdateRollArmGimmick() {

	// 腕振りのサイクル<frame>
	const uint16_t cycle = 120;

	// 1フレームでのパラメーター加算値
	const float step = 2.0f * static_cast<float>(M_PI) / cycle;

	// パラメーターを1ステップ分加算
	floatingParameter_ += step;
	// 2πを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * static_cast<float>(M_PI));

	// 腕振りの振幅<m>
	const float amplitude = 0.5f;

	// 腕振りを座標に反映
	worldTransformL_arm_.rotation_.x = std::sin(floatingParameter_) * amplitude;
	worldTransformR_arm_.rotation_.x = std::sin(floatingParameter_) * amplitude;

	

}

void Player::Draw() { 
	modelBody_->Draw(worldTransformBody_, *viewProjection_);
	modelHead_->Draw(worldTransformHead_, *viewProjection_);
	modelL_arm_->Draw(worldTransformL_arm_, *viewProjection_);
	modelR_arm_->Draw(worldTransformR_arm_, *viewProjection_);
}