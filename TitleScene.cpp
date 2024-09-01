#include "TitleScene.h"
#include "Input.h"
#include <numbers>

TitleScene::~TitleScene() {

	delete modelPlayer_;

	delete modelTitle_;

	delete fade_;
}

void TitleScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();

	modelPlayer_ = Model::CreateFromOBJ("Player03", true);
	worldTransform_.Initialize();
	worldTransform_.rotation_.y = std::numbers::pi_v<float>;

	modelTitle_ = Model::CreateFromOBJ("Title02", true);
	worldTransformTitle_.Initialize();
	worldTransformTitle_.translation_.y += 2.0f;
	worldTransformTitle_.rotation_.y = std::numbers::pi_v<float>;


	viewProjection_.Initialize();
	viewProjection_.translation_.z = -10.0f;

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void TitleScene::Update() {

	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_START){
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadoOut;
		}
	}

	
	switch (phase_) {
	case TitleScene::Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			fade_->Stop();
			phase_ = Phase::kMain;
		}
		break;
	case TitleScene::Phase::kFadoOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			// fade_->Stop();
			isFinished_ = true;
		}
		break;
	}

	worldTransform_.rotation_.y += std::numbers::pi_v<float> / 30.0f;
	worldTransform_.UpdateMatrix();

	worldTransformTitle_.UpdateMatrix();

	// ビュープロジェクションの更新と転送
	viewProjection_.UpdateMatrix();
}

void TitleScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	modelPlayer_->Draw(worldTransform_, viewProjection_);

	modelTitle_->Draw(worldTransformTitle_, viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

	fade_->Draw();
}