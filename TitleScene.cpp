#include "TitleScene.h"
#include <numbers>
#include "Input.h"

TitleScene::~TitleScene() {

	delete modelPlayer_;

	delete modelTitle_;

}

void TitleScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();

	modelPlayer_ = Model::CreateFromOBJ("player", true);
	worldTransform_.Initialize();
	worldTransform_.rotation_.y = std::numbers::pi_v<float>;

	modelTitle_ = Model::CreateFromOBJ("Title01", true);
	worldTransformTitle_.Initialize();
	worldTransformTitle_.translation_.y += 2.0f;

	viewProjection_.Initialize();
	viewProjection_.translation_.z = -10.0f;

}

void TitleScene::Update() {

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		isFinished_ = true;
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

}
