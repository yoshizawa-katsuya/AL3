#include "Player.h"
#include "cassert"
#include "Input.h"
#include "Vector.h"
#include "Matrix.h"
#include "ViewProjection.h"
#include "ImGuiManager.h"
#include <cmath>
#include "GlobalVariables.h"

//コンボ定数表
const std::array<Player::ConstAttack, Player::ComboNum> Player::kConstAttacks_ = {
    {
     //振りかぶり、攻撃前硬直、攻撃振り時間、硬直<frame> 各フェーズの移動速さ<m/sec>
        {0, 0, 20, 20, 0.0f, 0.0f, 0.15f},
     {15, 25, 40, 30, 0.2f, 0.0f, 0.0f},
     {15, 25, 40, 70, 0.2f, 0.0f, 0.0f},
     }
};

void Player::Initialize(const std::vector<Model*>& models, ViewProjection* viewProjection) {
	
	
	BaseCharacter::Initialize(models, viewProjection);

	velocity_ = {0.0f, 0.0f, 0.0f};

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

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	//グループを追加
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Head Translation", worldTransformHead_.translation_);
	globalVariables->AddItem(groupName, "ArmL Translation", worldTransformL_arm_.translation_);
	globalVariables->AddItem(groupName, "ArmR Translation", worldTransformR_arm_.translation_);
	globalVariables->AddItem(groupName, "floatingCycle", floatingCycle_);
	globalVariables->AddItem(groupName, "floatingAmplitude", floatingAmplitude_);
	globalVariables->AddItem(groupName, "idleArmAngleMax", idleArmAngleMax_);

	ApplyGlobalVariables();

}

void Player::ApplyGlobalVariables() {

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	worldTransformHead_.translation_ = globalVariables->GetVector3Value(groupName, "Head Translation");
	worldTransformL_arm_.translation_ = globalVariables->GetVector3Value(groupName, "ArmL Translation");
	worldTransformR_arm_.translation_ = globalVariables->GetVector3Value(groupName, "ArmR Translation");
	floatingCycle_ = globalVariables->GetIntValue(groupName, "floatingCycle");
	floatingAmplitude_ = globalVariables->GetFloatValue(groupName, "floatingAmplitude");
	idleArmAngleMax_ = globalVariables->GetFloatValue(groupName, "idleArmAngleMax");

}

void Player::BehaviorRootInitialize() {

	worldTransformBody_.rotation_.y = 0.0f;

	InitializeFloatingGimmick();

	InitializeRollArmGimmick();

}

void Player::BehaviorAttackInitialize() {

	workAttack_.comboIndex = 0;
	workAttack_.attackParameter_ = 0;
	
	worldTransformBody_.rotation_.y = 0.8f;

	worldTransformL_arm_.rotation_.x = -1.6f;
	worldTransformR_arm_.rotation_.x = -1.6f;
	worldTransformHammer_.rotation_.x = 1.6f;	

}

void Player::BehaviorDashInitialize() {

	workDash_.dashParameter_ = 0;
	worldTransform_.rotation_.y = targetAngle_;

}

void Player::BehaviorJumpInitialize() {

	worldTransformBody_.translation_.y = 0;
	worldTransformL_arm_.rotation_.x = 0;
	worldTransformR_arm_.rotation_.x = 0;

	//ジャンプ初速
	const float kJumpFirstSpeed = 1.0f;
	//ジャンプ初速を与える
	velocity_.y = kJumpFirstSpeed;

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
		case Behavior::kJump:
			BehaviorJumpInitialize();
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
	case Behavior::kJump:
		BehaviorJumpUpdate();
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
		velocity_ = {static_cast<float>(joyState.Gamepad.sThumbLX) / SHRT_MAX, 0.0f, static_cast<float>(joyState.Gamepad.sThumbLY) / SHRT_MAX};

		if (Length(velocity_) > thresholdValue) {
			isMoving = true;
		}

		if (isMoving) {
			// 速さ
			const float speed = 0.3f;

			// 移動量に速さを反映
			velocity_ = Multiply(speed, Normalize(velocity_));

			Matrix4x4 rotationMatrix = MakeRotateYMatrix(cameraViewProjection_->rotation_.y);

			velocity_ = TransformNormal(velocity_, rotationMatrix);

			// 移動
			worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

			// 目標角度の算出
			targetAngle_ = std::atan2(velocity_.x, velocity_.z);
		}

		//攻撃入力
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			behaviorRequest_.emplace(Behavior::kAttack);
		}

		//ダッシュボタンを押したら
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
			behaviorRequest_.emplace(Behavior::kDash);
		}

		//ジャンプボタンを押したら
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			behaviorRequest_.emplace(Behavior::kJump);
		}
	}

	// 最短角度補完
	worldTransform_.rotation_.y = LeapShortAngle(worldTransform_.rotation_.y, targetAngle_, angleCompletionRate_);


}

void Player::BehaviorAttackUpdate() {

	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;
	XINPUT_STATE joyStatePre;

	//コンボ上限に達していない
	if (workAttack_.comboIndex < ComboNum - 1) {
		if (Input::GetInstance()->GetJoystickState(0, joyState) && Input::GetInstance()->GetJoystickStatePrevious(0, joyStatePre)) {
			//攻撃ボタンをトリガーしたら
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B && !(joyStatePre.Gamepad.wButtons & XINPUT_GAMEPAD_B)) {
				//コンボ有効
				workAttack_.comboNext = true;
			}
		}
	}

	// 攻撃の全体フレーム
	const uint32_t kAttackTime = kConstAttacks_[workAttack_.comboIndex].recoveryTime;
	// 予備動作
	const uint32_t kExtraOperationEndTime = kConstAttacks_[workAttack_.comboIndex].anticipationTime;
	// 振り下ろし
	const uint32_t kSwingStartTime = kConstAttacks_[workAttack_.comboIndex].chargeTime;
	const uint32_t kSwingEndTime = kConstAttacks_[workAttack_.comboIndex].swingTime;

	//コンボ段階によってモーションを分岐
	switch (workAttack_.comboIndex) {
	case 0:

		if (workAttack_.attackParameter_ < kExtraOperationEndTime) {

			const float extoraOperationSpeed = 0.12f;

			worldTransformL_arm_.rotation_.y -= extoraOperationSpeed;
			worldTransformR_arm_.rotation_.y -= extoraOperationSpeed;
			worldTransformHammer_.rotation_.y -= extoraOperationSpeed;

		} else if (workAttack_.attackParameter_ >= kSwingStartTime && workAttack_.attackParameter_ < kSwingEndTime) {

			const float swingSpeed = 0.1f;

			worldTransformBody_.rotation_.y -= swingSpeed;
		}

		break;

	case 1:
		
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

		break;

	case 2:
	default:

		if (workAttack_.attackParameter_ < kExtraOperationEndTime) {

			const float extoraOperationSpeed = 0.1f;

			worldTransformBody_.rotation_.y += extoraOperationSpeed;

		} else if (workAttack_.attackParameter_ >= kSwingStartTime && workAttack_.attackParameter_ < kSwingEndTime) {

			const float swingSpeed = 0.6f;

			worldTransformBody_.rotation_.y -= swingSpeed;
		}

		break;
	}

	

	workAttack_.attackParameter_++;

	if (workAttack_.attackParameter_ >= kAttackTime) {
		//コンボ継続なら次のコンボに進む
		if (workAttack_.comboNext) {
			//コンボ継続フラグをリセット
			workAttack_.comboNext = false;

			workAttack_.attackParameter_ = 0;
			workAttack_.inComboPhase = 0;
			workAttack_.comboIndex++;

			worldTransformBody_.rotation_.y = 0.0f;

			worldTransformL_arm_.rotation_.x = -1.6f;
			worldTransformR_arm_.rotation_.x = -1.6f;
			worldTransformHammer_.rotation_.x = 1.6f;

		} else {

			behaviorRequest_.emplace(Behavior::kRoot);

		}
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

void Player::BehaviorJumpUpdate() {

	//移動
	worldTransform_.translation_ += velocity_;
	//重力加速度
	const float kGravityAcceleration = 0.05f;
	//加速度ベクトル
	Vector3 accelerationVector = {0, -kGravityAcceleration, 0};
	//加速する
	velocity_ += accelerationVector;

	//着地
	if (worldTransform_.translation_.y <= 0.0f) {
	
		worldTransform_.translation_.y = 0;
		//ジャンプ終了
		behaviorRequest_ = Behavior::kRoot;

	}

}

void Player::UpdateFloatingGimmick() {

	

	
	//1フレームでのパラメーター加算値
	const float step = 2.0f * static_cast<float>(M_PI) / floatingCycle_;

	//パラメーターを1ステップ分加算
	floatingParameter_ += step;
	//2πを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * static_cast<float>(M_PI));

	

	

	//浮遊を座標に反映
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * floatingAmplitude_;

	

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

	

	// 腕振りを座標に反映
	worldTransformL_arm_.rotation_.x = std::sin(rollArmParameter_) * idleArmAngleMax_;
	worldTransformR_arm_.rotation_.x = std::sin(rollArmParameter_) * idleArmAngleMax_;

	

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