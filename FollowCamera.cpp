#include "FollowCamera.h"
#include "Vector.h"
#include "Matrix.h"
#include "Input.h"

void FollowCamera::Initialize() {
	viewProjection_.Initialize();
}

void FollowCamera::Update() {

	

	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
	
		float rotationSpeed = 0.1f;

		viewProjection_.rotation_.y += static_cast<float>(joyState.Gamepad.sThumbRX) / SHRT_MAX * rotationSpeed;
	
	}

	if (target_) {
		// 追従対象からカメラまでのオフセット
		Vector3 offset = {0.0f, 2.0f, -20.0f};

		Matrix4x4 rotationMatrix = MakeRotateYMatrix(viewProjection_.rotation_.y);

		// オフセットをカメラの回転に合わせて回転させる
		offset = TransformNormal(offset, rotationMatrix);

		// 座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = Add(target_->translation_, offset);
	}

	//ビュー行列の更新
	viewProjection_.UpdateViewMatrix();

}