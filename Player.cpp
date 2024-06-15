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
}

void Player::Update() {

	//ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		//速さ
		const float speed = 0.3f;

		//移動量
		Vector3 move = {static_cast<float>(joyState.Gamepad.sThumbLX), 0.0f, static_cast<float>(joyState.Gamepad.sThumbLY)};
		//移動量に速さを反映
		move = Multiply(speed, Normalize(move));

		Matrix4x4 rotationMatrix = MakeRotateYMatrix(cameraViewProjection_->rotation_.y);

		move = TransformNormal(move, rotationMatrix);

		//回転
		if (!(move.x == 0.0f && move.y == 0.0f)) {
			worldTransform_.rotation_.y = std::atan2(move.x, move.z);
		}
		

		//移動
		worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	}
	//行列を更新
	worldTransform_.UpdateMatrix();
}

void Player::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_);
}