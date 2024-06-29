#include "Player.h"
#include "cassert"
#include "Input.h"
#include "Vector.h"
#include "Matrix.h"
#include "ViewProjection.h"
#include "ImGuiManager.h"
#include <cmath>

void Player::Initialize(const std::vector<Model*>& models, ViewProjection* viewProjection) {
	
	
	BaseCharacter::Initialize(models, viewProjection);

	worldTransformBody_.Initialize();
	worldTransformBody_.parent_ = &worldTransform_;

	worldTransformHead_.Initialize();
	worldTransformHead_.translation_ = Vector3(0.0f, 3.5f, 0.0f);
	worldTransformHead_.parent_ = &worldTransformBody_;

	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.translation_ = Vector3(-1.3f, 2.8f, 0.0f);
	worldTransformL_arm_.parent_ = &worldTransformBody_;

	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.translation_ = Vector3(1.3f, 2.8f, 0.0f);
	worldTransformR_arm_.parent_ = &worldTransformBody_;

	worldTransformHammer_.Initialize();
	worldTransformHammer_.translation_ = Vector3(0.0, 3.0f, 0.0f);
	worldTransformHammer_.parent_ = &worldTransformBody_;

	viewProjection_ = viewProjection;
	targetAngle_ = worldTransform_.rotation_.y;

	InitializeFloatingGimmick();

	InitializeRollArmGimmick();

}

void Player::BehaviorRootInitialize() {

	InitializeFloatingGimmick();

	InitializeRollArmGimmick();

}

void Player::BehaviorAttackInitialize() {

	workAttack_.attackParameter_ = 0;
	worldTransformL_arm_.rotation_.x = -1.6f;
	worldTransformR_arm_.rotation_.x = -1.6f;
	worldTransformHammer_.rotation_.x = 1.6f;	

}

void Player::BehaviorDashInitialize() {

	workDash_.dashParameter_ = 0;
	worldTransform_.rotation_.y = targetAngle_;

}


void Player::InitializeFloatingGimmick() {

	floatingParameter_ = 0.0f;

}

void Player::InitializeRollArmGimmick() {

	rollArmParameter_ = 0.0f;

}

void Player::Update() {

	if (behaviorRequest_) {
		//振るまいを変更する
		behavior_ = behaviorRequest_.value();
		//各振るまいごとの初期化を実行
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		case Behavior::kDash:
			BehaviorDashInitialize();
			break;
		}
		//振るまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_) {
		//通常行動
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	case Behavior::kDash:
		BehaviorDashUpdate();
		break;
	}

	//行列を更新
	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	worldTransformHammer_.UpdateMatrix();
}

void Player::BehaviorRootUpdate() {

	UpdateFloatingGimmick();

	UpdateRollArmGimmick();

	// ゲームパッドの状態を得る変数
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

		//攻撃入力
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			behaviorRequest_.emplace(Behavior::kAttack);
		}

		//ダッシュボタンを押したら
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			behaviorRequest_.emplace(Behavior::kDash);
		}
	}

	// 最短角度補完
	worldTransform_.rotation_.y = LeapShortAngle(worldTransform_.rotation_.y, targetAngle_, angleCompletionRate_);


}

void Player::BehaviorAttackUpdate() {

	// 攻撃の全体フレーム
	const uint16_t kAttackTime = 60;
	// 予備動作
	const uint16_t kExtraOperationEndTime = 25;
	// 振り下ろし
	const uint16_t kSwingStartTime = 35;
	const uint16_t kSwingEndTime = 45;

	if (workAttack_.attackParameter_ < kExtraOperationEndTime) {
		
		const float extoraOperationSpeed = 0.12f;

		worldTransformL_arm_.rotation_.x -= extoraOperationSpeed;
		worldTransformR_arm_.rotation_.x -= extoraOperationSpeed;
		worldTransformHammer_.rotation_.x -= extoraOperationSpeed;
	
	} else if (workAttack_.attackParameter_ >= kSwingStartTime && workAttack_.attackParameter_ < kSwingEndTime) {

		const float swingSpeed = 0.3f;

		worldTransformL_arm_.rotation_.x += swingSpeed;
		worldTransformR_arm_.rotation_.x += swingSpeed;
		worldTransformHammer_.rotation_.x += swingSpeed;
	}

	workAttack_.attackParameter_++;

	if (workAttack_.attackParameter_ >= kAttackTime) {
	
		
		behaviorRequest_.emplace(Behavior::kRoot);
	}
}

void Player::BehaviorDashUpdate() {

	// 速さ
	const float speed = 5.0f;

	Vector3 move = TransformNormal({0.0f, 0.0f, 1.0f}, MakeRotateYMatrix(worldTransform_.rotation_.y));

	// 移動量に速さを反映
	move = Multiply(speed, Normalize(move));

	

	// 移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);


	// ダッシュの時間<frame>
	const uint32_t behaviorDashTime = 18;

	//規定の時間経過で通常行動に戻る
	if (++workDash_.dashParameter_ >= behaviorDashTime) {
		behaviorRequest_ = Behavior::kRoot;
	}

}

void Player::UpdateFloatingGimmick() {

	//浮遊移動のサイクル<frame>
	const uint16_t cycle = 90;

	
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
	const uint16_t cycle = 90;

	// 1フレームでのパラメーター加算値
	const float step = 2.0f * static_cast<float>(M_PI) / cycle;

	// パラメーターを1ステップ分加算
	rollArmParameter_ += step;
	// 2πを超えたら0に戻す
	rollArmParameter_ = std::fmod(rollArmParameter_, 2.0f * static_cast<float>(M_PI));

	// 腕振りの振幅<m>
	const float amplitude = 0.5f;

	// 腕振りを座標に反映
	worldTransformL_arm_.rotation_.x = std::sin(rollArmParameter_) * amplitude;
	worldTransformR_arm_.rotation_.x = std::sin(rollArmParameter_) * amplitude;

	

}

void Player::Draw() {
	models_[kModelIndexBody]->Draw(worldTransformBody_, *viewProjection_);
	models_[kModelIndexHead]->Draw(worldTransformHead_, *viewProjection_);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, *viewProjection_);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, *viewProjection_);

	if (behavior_ == Behavior::kAttack) {
	
	models_[kModelIndexHammer]->Draw(worldTransformHammer_, *viewProjection_);
	}
}