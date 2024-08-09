#include "RailCamera.h"
#include "Matrix.h"
#include "Vector.h"
#include "ImGuiManager.h"
#include "Curve.h"
#include "PrimitiveDrawer.h"

void RailCamera::Initialize(const Vector3 worldPos, const Vector3 rotate) {

	//ワールドトランスフォームの初期設定
	worldTransform_.Initialize();
	worldTransform_.translation_ = worldPos;
	worldTransform_.rotation_ = rotate;

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	controlPoints_ = {
	    {0,  0,  0},
        {10, 10, 0},
        {10, 15, 0},
        {20, 15, 0},
        {20, 0,  0},
        {30, 0,  0},
	};

	pointsDrawing_ = GenerateCatmullRomSplinePoints(controlPoints_, segmentCount_);

	
	
}

void RailCamera::Update() {

	if (pointsDrawing_.size() > moveCount_) {

		eye_ = pointsDrawing_[moveCount_];
		worldTransform_.translation_ = eye_;

		if (pointsDrawing_.size() > moveCount_ + difference_) {
			target_ = pointsDrawing_[moveCount_ + difference_];
			forward_ = Subtract(target_, eye_);

			worldTransform_.rotation_.y = std::atan2(forward_.x, forward_.z);
			if (worldTransform_.rotation_.y < 0.0f) {
				worldTransform_.rotation_.y = -worldTransform_.rotation_.y;
			}
			Matrix4x4 minusThetaYMatrix = MakeRotateYMatrix(-worldTransform_.rotation_.y);
			Vector3 targetZ = Transform(forward_, minusThetaYMatrix);
			worldTransform_.rotation_.x = std::atan2(-targetZ.y, targetZ.z);
		}

		// worldTransform_.translation_.z -= 0.1f;
		// worldTransform_.rotation_.y += 0.1f;
		worldTransform_.UpdateMatrix();

		offset_ = {0.0f, 1.0f, 0.0f};
		offset_ = TransformNormal(offset_, worldTransform_.matWorld_);
		worldTransform_.translation_ += offset_;

		moveCount_++;

	}

	worldTransform_.UpdateMatrix();

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	viewProjection_.TransferMatrix();

	//カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("translate", &worldTransform_.translation_.x, -20.0f, 20.0f);
	ImGui::SliderFloat3("rotation", &worldTransform_.rotation_.x, -4.0f, 4.0f);
	if (ImGui::Button("Reset")) {
		moveCount_ = 0;
	}
	ImGui::End();


}

void RailCamera::Draw(const ViewProjection& viewProjection) {

	Vector4 red = {1.0f, 0.0f, 0.0f, 1.0f};

	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection);

	for (size_t i = 0; i < pointsDrawing_.size() - 1; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(pointsDrawing_[i], pointsDrawing_[i + 1], red);
	}

}
