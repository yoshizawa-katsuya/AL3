#include "RailCamera.h"
#include "Matrix.h"
#include "Vector.h"
#include "ImGuiManager.h"
#include "Input.h"

void RailCamera::Initialize(const Vector3 worldPos, const Vector3 rotate) {

	//ワールドトランスフォームの初期設定
	worldTransform_.Initialize();
	worldTransform_.translation_ = worldPos;
	worldTransform_.rotation_ = rotate;
	//ビュープロジェクションの初期化
	viewProjecttion_.Initialize();

}

void RailCamera::Update() {

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;
	/*
	//押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
	    move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
	    move.x += kCharacterSpeed;
	}

	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_DOWN)) {
	    move.y -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_UP)) {
	    move.y += kCharacterSpeed;
	}
	*/

	// ゲームパッドの状態を得る関数(XINPUT)
	XINPUT_STATE joyState;

	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += static_cast<float>(joyState.Gamepad.sThumbLX) / SHRT_MAX * kCharacterSpeed;
		move.y += static_cast<float>(joyState.Gamepad.sThumbLY) / SHRT_MAX * kCharacterSpeed;
	}

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	// 移動限界座標
	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);

	worldTransform_.translation_.z += 0.1f;
	//worldTransform_.rotation_.y += 0.1f;
	worldTransform_.UpdateMatrix();

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjecttion_.matView = Inverse(worldTransform_.matWorld_);

	//カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("translate", &worldTransform_.translation_.x, -20.0f, 20.0f);
	ImGui::SliderFloat3("rotation", &worldTransform_.rotation_.x, -20.0f, 20.0f);
	ImGui::End();


}