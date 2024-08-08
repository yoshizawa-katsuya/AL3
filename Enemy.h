#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
#include "TimedCall.h"

//敵
class Enemy {
public:

	//デストラクタ
	~Enemy();

	// 初期化
	void Initialize(Model* model, uint32_t textureHandle);

	void ApproachInitialize();

	// 更新
	void Update();

	void ApproachUpdate();
	void LeaveUpdate();

	//弾発射
	void Fire();

	//弾を発射し、タイマーをリセットするコールバック関数
	void FireCallBack();

	// 描画
	void Draw(const ViewProjection& viewProjection);

private:

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 弾
	std::list<EnemyBullet*> bullets_;
	//発射間隔
	static const int kFireInterval = 60;
	//発射タイマー
	int32_t fireTimer = 0;

	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};
	//フェーズ
	Phase phase_ = Phase::Approach;

	// キャラクターの移動速さ
	Vector3 ApproachVelocity_ = {0.0f, 0.0f, -0.1f};
	Vector3 LeaveVelocity_ = {0.1f, -0.1f, 0.0f};

	//時限発動のリスト
	std::list<TimedCall*> timedCalls_;

};