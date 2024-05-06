#include "Player.h"
#include "cassert"
#include "Easing.h"

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	//NULLポインタチェック
	assert(model);

	model_ = model;
	
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	viewProjection_ = viewProjection;

}

void Player::Update() {

	
	//旋回制御
	if (turnTimer_ > 0.0f)
	{
		turnTimer_ -= 1.0f / 60.0f; 

		//左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {
			std::numbers::pi_v<float> / 2.0f,
			std::numbers::pi_v<float> * 3.0f / 2.0f
		};

		//状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		//自キャラの角度を設定する
		worldTransform_.rotation_.y = turnFirstRotationY_ * EaseOut(turnTimer_) + destinationRotationY * (1 - EaseOut(turnTimer_));

	}

	//移動入力
	//接地状態
	if (onGround_) {

		// 左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				// 左移動中の右入力
				if (velocity_.x < 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = 1.0f;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				// 右入力中の左入力
				if (velocity_.x > 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = 1.0f;
				}
			}
			// 加速/減速
			velocity_.x += acceleration.x;

			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

		} else {

			// 非入力時には移動減衰をかける
			velocity_.x *= (1.0f - kAttenuation);
		}

		if (Input::GetInstance()->PushKey(DIK_UP)) {
			//ジャンプ加速
			velocity_.y += kJumpAcceleration;
		}

	}
	//空中
	else {
		
		//落下速度
		velocity_.y += -kGravityAcceleration;
		//落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);

	}

	//移動
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	bool landing = false;

	//地面との当たり判定
	//下降中?
	if (velocity_.y < 0) {
		//Y座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= 1.0f) {
			landing = true;
		}
	}

	//接地判定
	if (onGround_) {
		//ジャンプ開始
		if (velocity_.y > 0.0f) {
			//空中状態に移行
			onGround_ = false;
		}
	} 
	else
	{
		//着地
		if (landing) {
			//めり込み排斥
			worldTransform_.translation_.y = 2.0f;
			//摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuation);
			//下方向速度をリセット
			velocity_.y = 0.0f;
			//設置状態に移行
			onGround_ = true;
		}
	}

	//行列計算
	worldTransform_.UpdateMatrix();

}

void Player::Draw() {

	model_->Draw(worldTransform_, *viewProjection_);

}