#pragma once
#include "Model.h"
#include "WorldTransform.h"

//敵
class Enemy {
public:
	// 初期化
	void Initialize(Model* model, uint32_t textureHandle);

	// 更新
	void Update();

	void ApproachUpdate();
	void LeaveUpdate();

	// 描画
	void Draw(const ViewProjection& viewProjection);

private:

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};
	//フェーズ
	Phase phase_ = Phase::Approach;

	// キャラクターの移動速さ
	Vector3 ApproachVelocity_ = {0.0f, 0.0f, -0.2f};
	Vector3 LeaveVelocity_ = {0.2f, -0.2f, 0.0f};

};