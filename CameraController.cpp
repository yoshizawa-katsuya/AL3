#define NOMINMAX
#include "CameraController.h"
#include "Player.h"
#include "Lerp.h"
#include <algorithm>

void CameraController::Initialize() {

	viewProjection_.Initialize();
	movableArea_ = {11, 88, 6, 100};

}

void CameraController::Update() {

	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	Vector3 targetVelocity = target_->GetVelocity();
	// 追従対象とオフセットからカメラの目標座標を計算
	targetVector_ = targetWorldTransform.translation_;
	targetVector_.x += targetVelocity.x * kVelocityBies + targetOffset_.x;
	targetVector_.y += targetVelocity.y * kVelocityBies + targetOffset_.y;
	targetVector_.z += targetVelocity.z * kVelocityBies + targetOffset_.z;

	
	//座標補完によりゆったり追従
	viewProjection_.translation_ = Leap(viewProjection_.translation_, targetVector_, kInterpolationRate);

	//追従対象が画面外に出ないように補正
	viewProjection_.translation_.x = std::max(viewProjection_.translation_.x, targetWorldTransform.translation_.x + margin_.left);
	viewProjection_.translation_.x = std::min(viewProjection_.translation_.x, targetWorldTransform.translation_.x + margin_.right);
	viewProjection_.translation_.y = std::max(viewProjection_.translation_.y, targetWorldTransform.translation_.y + margin_.bottom);
	viewProjection_.translation_.y = std::min(viewProjection_.translation_.y, targetWorldTransform.translation_.y + margin_.top);

	//移動範囲制限
	viewProjection_.translation_.x = std::max(viewProjection_.translation_.x, movableArea_.left);
	viewProjection_.translation_.x = std::min(viewProjection_.translation_.x, movableArea_.right);
	viewProjection_.translation_.y = std::max(viewProjection_.translation_.y, movableArea_.bottom);
	viewProjection_.translation_.y = std::min(viewProjection_.translation_.y, movableArea_.top);

	//行列を更新する
	viewProjection_.UpdateMatrix();


}

void CameraController::Reset() {

	//追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	//追従対象とオフセットからカメラの座標を計算
	viewProjection_.translation_ = targetWorldTransform.translation_;
	viewProjection_.translation_.z += targetOffset_.z;

}

void CameraController::SetTarget(Player* target) { target_ = target; }

void CameraController::SetMovableArea(Rect area) { movableArea_ = area; }

ViewProjection& CameraController::GetViewProjection() { return viewProjection_; }