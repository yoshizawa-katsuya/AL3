#pragma once
#define NOMINMAX
#include "WorldTransform.h"
#include "Model.h"
#include <numbers>
#include <Input.h>
#include <algorithm>
#include "Struct.h"
class Player;

///<summary>
///敵
///</summary>
class Enemy {

public:

	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

	//衝突応答
	void OnCollision(const Player* player);

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// AABBを取得
	AABB GetAABB();

private:
	
	//歩行の速さ
	static inline const float kWalkSpeed = 0.01f;

	const float kWidth = 0.8f;
	const float kHeight = 0.8f;

	//速度
	Vector3 velocity_ = {};

	//最初の角度[度]
	static inline const float kWalkMotionAngleStart = 0.0f;
	//最後の角度[度]
	static inline const float kWalkMotionAngleEnd = 30.0f;
	//アニメーションの周期となる時間[秒]
	static inline const float kWalkMotionTime = 0.5f;
	//経過時間
	float walkTimer_ = 0.0f;

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;

	ViewProjection* viewProjection_ = nullptr;

};