#include "Player.h"
#include "cassert"
#include "TextureManager.h"
#include "WinApp.h"
#include "ViewProjection.h"
#include "Vector.h"
#include "Matrix.h"
#include "ImGuiManager.h"

Player::~Player() {

	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}

	delete sprite2DReticle_;
}

void Player::Initialize(Model* model, uint32_t textureHandle, const Vector3& position) {

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	//NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	//3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	//レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("./Resources/reticle.png");

	//スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle, {static_cast<float>(WinApp::kWindowWidth) / 2.0f, static_cast<float>(WinApp::kWindowHeight) / 2.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});

}

void Player::Update(const ViewProjection& viewProjection) {

	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	Rotate();

	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	
	//キャラクターの移動速さ
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

	//ゲームパッドの状態を得る関数(XINPUT)
	XINPUT_STATE joyState;

	//ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += static_cast<float>(joyState.Gamepad.sThumbLX) / SHRT_MAX * kCharacterSpeed;
		move.y += static_cast<float>(joyState.Gamepad.sThumbLY) / SHRT_MAX * kCharacterSpeed;

	}

	//座標移動(ベクトルの加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	//移動限界座標
	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);

	worldTransform_.UpdateMatrix();

	/*
	//自機のワールド座標から3Dレティクルのワールド座標を計算
	{
		//自機から3Dレティクルへの距離
		const float kDistancePlayerTo3DReticle = 50.0f;
		//自機から3Dレティクルへのオフセット(Z+向き)
		Vector3 offset = {0, 0, 1.0f};
		//自機のワールド行列の回転を反映
		offset = TransformNormal(offset, worldTransform_.matWorld_);
		//ベクトルの長さを整える
		offset = Multiply(kDistancePlayerTo3DReticle, Normalize(offset));
		//3Dレティクルの座標を設定
		worldTransform3DReticle_.translation_ = Add(GetWorldPosition(), offset);
		worldTransform3DReticle_.UpdateMatrix();

	}

	//3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	{
		Vector3 positionReticle = worldTransform3DReticle_.GetWorldPosition();

		//ビューポート行列
		Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

		//ビュー行列とプロジェクション行列、ビューポート行列を合成する
		Matrix4x4 matViewProjectionViewport = Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);

		//ワールド→スクリーン座標変換(ここで3Dから2Dになる)
		positionReticle = Transform(positionReticle, matViewProjectionViewport);

		//スプライトのレティクルに座標設定
		sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	}
	*/
	//マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置
	Reticle(viewProjection);

	//キャラクター攻撃処理
	Attack();

	//弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// キャラクターの座標を画面表示する処理
	ImGui::Begin("player");
	ImGui::DragFloat3("player.translate", &worldTransform_.translation_.x, 0.01f);
	ImGui::End();

}

void Player::Rotate() {

	

	//回転速さ
	const float kRotSpeed = 0.02f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
	
}

void Player::Attack() {

	XINPUT_STATE joyState;

	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 自機から照準オブジェクトへのベクトル
		velocity = Subtract(worldTransform3DReticle_.translation_, GetWorldPosition());
		velocity = Multiply(kBulletSpeed, Normalize(velocity));

		// 速度ベクトルを自機の向きに合わせて回転させる
		// velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		bullets_.push_back(newBullet);

	}

	/*
	if (input_->TriggerKey(DIK_SPACE)) {
		
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//自機から照準オブジェクトへのベクトル
		velocity = Subtract(worldTransform3DReticle_.translation_, GetWorldPosition());
		velocity = Multiply(kBulletSpeed, Normalize(velocity));

		//速度ベクトルを自機の向きに合わせて回転させる
		//velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		//弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		bullets_.push_back(newBullet);


	}
	*/


}

void Player::Reticle(const ViewProjection& viewProjection) {
	/*
	POINT mousePosition;
	//マウス座標を取得する
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	sprite2DReticle_->SetPosition({static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)});
	*/

	//スプライトの現在座標を取得
	Vector2 spritePosition = sprite2DReticle_->GetPosition();

	XINPUT_STATE joyState;

	//ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		spritePosition.x += static_cast<float>(joyState.Gamepad.sThumbRX) / SHRT_MAX * 5.0f;
		spritePosition.y -= static_cast<float>(joyState.Gamepad.sThumbRY) / SHRT_MAX * 5.0f;
		//スプライトの座標変更を反映
		sprite2DReticle_->SetPosition(spritePosition);
	}

	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	//ビュープロジェクションビューポート合成行列
	Matrix4x4 matVPV = Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);

	//合成行列の逆行列の計算をする
	Matrix4x4 matInveraseVPV = Inverse(matVPV);

	//スクリーン座標
	Vector3 posNear = Vector3(spritePosition.x, spritePosition.y, 0);
	Vector3 posFar = Vector3(spritePosition.x, spritePosition.y, 1);

	//スクリーン座標系からワールド座標系へ
	posNear = Transform(posNear, matInveraseVPV);
	posFar = Transform(posFar, matInveraseVPV);

	//マウスレイの方向
	Vector3 mouseDirection = Subtract(posFar, posNear);
	mouseDirection = Normalize(mouseDirection);
	//カメラから照準オブジェクトへの距離
	const float kDistanceTestObject = 50.0f;
	worldTransform3DReticle_.translation_ = Add(posNear, Multiply(kDistanceTestObject, mouseDirection));
	worldTransform3DReticle_.UpdateMatrix();

	ImGui::Begin("player");
	ImGui::Text("2DReticle:(%f,%f)", spritePosition.x, spritePosition.y);
	ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text("3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x,
		worldTransform3DReticle_.translation_.y,worldTransform3DReticle_.translation_.z);
	ImGui::End();

}

void Player::OnCollision() {

}

void Player::Draw(const ViewProjection& viewProjection) {

	//3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

	//3Dレティクルを描画
	model_->Draw(worldTransform3DReticle_, viewProjection);

}

void Player::DrawUI() {

	sprite2DReticle_->Draw();

}

void Player::SetParent(const WorldTransform* parent) {

	//親子関係を結ぶ
	worldTransform_.parent_ = parent;

}

Vector3 Player::GetWorldPosition() {

	//ワールド座標を入れる変数
	Vector3 worlsPos;
	//ワールド行列の平行移動成分を取得
	worlsPos.x = worldTransform_.matWorld_.m[3][0];
	worlsPos.y = worldTransform_.matWorld_.m[3][1];
	worlsPos.z = worldTransform_.matWorld_.m[3][2];

	return worlsPos;

}