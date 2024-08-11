#include "Enemy.h"
#include "cassert"
#include "Vector.h"
#include "Matrix.h"
#include "Player.h"
#include "GameScene.h"

Enemy::~Enemy() {
	/*
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
	*/
}

void Enemy::Initialize(Model* model, uint32_t textureHandle, const Vector3& position) {

	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	ApproachInitialize();
}

void Enemy::ApproachInitialize() {
	//発射タイマーを初期化
	fireTimer = kFireInterval;
}

void Enemy::Update() {

	// デスフラグの立った弾を削除
	/*
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	*/
	switch (phase_) {
	case Phase::Approach:
	default:
		ApproachUpdate();
		break;
	case Phase::Leave:
		LeaveUpdate();
		break;
	}

	// 弾更新
	/*
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}
	*/
	worldTransform_.UpdateMatrix();

}

void Enemy::ApproachUpdate() {

	//発射タイマーカウントダウン
	fireTimer--;
	//指定時間に達した
	if (fireTimer == 0) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		fireTimer = kFireInterval;
	}

	// 移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, ApproachVelocity_);
	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveUpdate() {
	// 移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, LeaveVelocity_);
	
}

void Enemy::Fire() {

	//弾の速さ
	const float kBulletSpeed = 0.5f;

	Vector3 toPosition = player_->GetWorldPosition();
	Vector3 fromPosition = GetWorldPosition();
	Vector3 velocity = Subtract(toPosition, fromPosition);
	velocity = Normalize(velocity);
	velocity = Multiply(kBulletSpeed, velocity);

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	gameScene_->AddEnemybullet(newBullet);
	//enemyBullets_.push_back(newBullet);

}

void Enemy::OnCollision() {

	isDead_ = true;

}

void Enemy::Draw(const ViewProjection& viewProjection) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
	/*
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection);
	}
	*/
}

Vector3 Enemy::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worlsPos;
	// ワールド行列の平行移動成分を取得
	worlsPos.x = worldTransform_.matWorld_.m[3][0];
	worlsPos.y = worldTransform_.matWorld_.m[3][1];
	worlsPos.z = worldTransform_.matWorld_.m[3][2];

	return worlsPos;

}

Vector3 Enemy::GetScreenPosition(const ViewProjection& viewProjection) {
	
	
	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);

	// ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	Vector3 screenPosition = Transform(GetWorldPosition(), matViewProjectionViewport);

	return screenPosition;
}
