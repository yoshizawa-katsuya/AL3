#include "Player.h"
#include "cassert"
#include "Input.h"
#include "Vector.h"
#include "Matrix.h"
#include "ViewProjection.h"
#include <cmath>

void Player::Initialize(Model* model, ViewProjection* viewProjection) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
	targetAngle_ = worldTransform_.rotation_.y;
}

void Player::Update() {

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
			worldTransform_.translation_ = Add(worldTransform_.translation_, move);

			// 目標角度の算出
			targetAngle_ = std::atan2(move.x, move.z);
		}
	}

	//最短角度補完
	worldTransform_.rotation_.y = LeapShortAngle(worldTransform_.rotation_.y, targetAngle_, angleCompletionRate_);

	//行列を更新
	worldTransform_.UpdateMatrix();
}

void Player::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_);
}