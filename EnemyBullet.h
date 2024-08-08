#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Player.h"

class EnemyBullet {
public:
	//初期化
	void Initialize(Model* model, const Vector3& position, const Vector3 velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	void SetPlayer(Player* player) { player_ = player; }

private:

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Player* player_ = nullptr;

	// 速度
	Vector3 velocity_;

	// 弾の速さ
	const float kBulletSpeed_ = 0.5f;

	// 寿命
	static const int32_t kLifeTime = 60 * 5;

	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;

};