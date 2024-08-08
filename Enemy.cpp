#include "Enemy.h"
#include "cassert"
#include "Vector.h"

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}

	//timedCalls_の開放
	for (TimedCall* timedCall : timedCalls_) {
		delete timedCall;
	}

}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {

	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = {10.0f, 0.0f, 30.0f};

	ApproachInitialize();
}

void Enemy::ApproachInitialize() {

	//発射タイマーをセットする
	timedCalls_.push_back(new TimedCall(std::bind(&Enemy::FireCallBack, this), kFireInterval));

	//発射タイマーを初期化
	//fireTimer = kFireInterval;
}

void Enemy::Update() {

	//終了したタイマーを削除
	timedCalls_.remove_if([](TimedCall* timedCall) {
		if (timedCall->IsFinished()) {
			delete timedCall;
			return true;
		}
		return false;
	});

	// デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

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
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	worldTransform_.UpdateMatrix();

}

void Enemy::ApproachUpdate() {

	//範囲for文でリストの全要素について回す
	for (TimedCall* timedCall : timedCalls_) {
		timedCall->Update();
	}

	/*
	//発射タイマーカウントダウン
	fireTimer--;
	//指定時間に達した
	if (fireTimer == 0) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		fireTimer = kFireInterval;
	}
	*/

	// 移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, ApproachVelocity_);
	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;

		// timedCalls_の開放
		for (TimedCall* timedCall : timedCalls_) {
			delete timedCall;
		}
		timedCalls_.clear();
	}
}

void Enemy::LeaveUpdate() {
	// 移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, LeaveVelocity_);
	
}

void Enemy::Fire() {

	
	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_);

	bullets_.push_back(newBullet);

}

void Enemy::FireCallBack() {

	//弾を発射する
	Fire();

	//発射タイマーをセットする
	timedCalls_.push_back(new TimedCall(std::bind(&Enemy::FireCallBack, this), kFireInterval));

}

void Enemy::Draw(const ViewProjection& viewProjection) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

}