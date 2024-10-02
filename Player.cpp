#include "Player.h"
#include "cassert"
#include "TextureManager.h"
#include "WinApp.h"
#include "ViewProjection.h"
#include "Vector.h"
#include "Matrix.h"
#include "ImGuiManager.h"
#include "Lerp.h"
#include "Enemy.h"

Player::~Player() {

	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}

	delete sprite2DReticle_;
	for (Sprite* lockOnReticle : lockOnReticles_) {
		delete lockOnReticle;
	}
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
	sprite2DReticle_ = Sprite::Create(textureReticle, {640.0f, 360.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});

}

void Player::Update(const ViewProjection& viewProjection) {

	// デスフラグの立った敵を削除
	targets_.remove_if([](Enemy* target) {
		if (target->IsDead()) {
			return true;
		}
		return false;
	});
	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	if (targets_.empty()) {
		isLockOn_ = false;
		for (Sprite* lockOnReticle : lockOnReticles_) {
			delete lockOnReticle;
		}
		lockOnReticles_.clear();
	}

	Rotate();

	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

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
		//positionReticle = Lerp({sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y, 0.0f}, positionReticle, 0.2f);
		sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
		sprite2DReticle_->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
	}

	//キャラクター攻撃処理
	Attack();

	//弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// キャラクターの座標を画面表示する処理
#ifdef _DEBUG

	ImGui::Begin("player");
	ImGui::DragFloat3("player.translate", &worldTransform_.translation_.x, 0.01f);
	ImGui::End();

#endif // _DEBUG

	

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

	if (input_->TriggerKey(DIK_SPACE)) {
		
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//自機から照準オブジェクトへのベクトル
		if (isLockOn_) {

			for (Enemy* target : targets_) {

				velocity = Subtract(target->GetWorldPosition(), GetWorldPosition());
				velocity = Multiply(kBulletSpeed, Normalize(velocity));
				// 速度ベクトルを自機の向きに合わせて回転させる
				// velocity = TransformNormal(velocity, worldTransform_.matWorld_);

				// 弾を生成し、初期化
				PlayerBullet* newBullet = new PlayerBullet();
				newBullet->Initialize(model_, GetWorldPosition(), velocity);

				bullets_.push_back(newBullet);
			}

		} else {

			velocity = Subtract(worldTransform3DReticle_.translation_, GetWorldPosition());
			velocity = Multiply(kBulletSpeed, Normalize(velocity));
			// 速度ベクトルを自機の向きに合わせて回転させる
			// velocity = TransformNormal(velocity, worldTransform_.matWorld_);

			// 弾を生成し、初期化
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(model_, GetWorldPosition(), velocity);

			bullets_.push_back(newBullet);
		}

	}



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
	//model_->Draw(worldTransform3DReticle_, viewProjection);

}

void Player::DrawUI(const ViewProjection& viewProjection) {

	sprite2DReticle_->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
	sprite2DReticle_->Draw();


	std::list<Sprite*>::iterator lockOnReticlesIterator = lockOnReticles_.begin();
	for (Enemy* target : targets_) {
		
		Sprite* lockOnReticle = *lockOnReticlesIterator;
		lockOnReticle->SetPosition({target->GetScreenPosition(viewProjection).x, target->GetScreenPosition(viewProjection).y});
		lockOnReticle->Draw();

		lockOnReticlesIterator++;
		if (lockOnReticlesIterator == lockOnReticles_.end()) {
			break;
		}
	}

}

void Player::LockOn(Enemy* target, const ViewProjection& viewProjection) {

	isLockOn_ = true;
	for (Enemy* target_ : targets_) {
		if (target_ == target) {
			return;
		}
	}
	targets_.push_back(target);
	Sprite* lockOnReticle = Sprite::Create(sprite2DReticle_->GetTextureHandle(), {target->GetScreenPosition(viewProjection).x, target->GetScreenPosition(viewProjection).y}
											, {1.0f, 0.0f, 0.0f, 1.0f}, {0.5f, 0.5f});
	
	lockOnReticles_.push_back(lockOnReticle);

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