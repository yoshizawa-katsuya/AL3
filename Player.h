#pragma once
#define NOMINMAX
#include "WorldTransform.h"
#include "Model.h"
#include <numbers>
#include <Input.h>
#include <algorithm>

///<summary>
///自キャラ
/// </summary>
class Player {
public:
	/// <summary>
	/// 初期化
	///  </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	/// <summary>
	/// 更新
	///  </summary>
	void Update();

	/// <summary>
	/// 描画
	///  </summary>
	void Draw();

	WorldTransform& GetWorldTransform();

	const Vector3& GetVelocity() const { return velocity_; }

private:
	
	enum  class LRDirection {
		kRight,
		kLeft,
	};

	LRDirection lrDirection_ = LRDirection::kRight;

	//旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	//旋回タイマー
	float turnTimer_ = 0.0f;
	//旋回時間<秒>
	static inline const float kTimeTurn = 0.03f;

	Vector3 velocity_ = {};

	static inline const float kAcceleration = 0.01f;

	static inline const float kAttenuation = 0.1f;

	static inline const float kLimitRunSpeed = 10.0f;

	//設置状態フラグ
	bool onGround_ = true;

	//重力加速度
	static inline const float kGravityAcceleration = 0.1f;

	static inline const float kLimitFallSpeed = 1.0f;
	static inline const float kJumpAcceleration = 1.0f;

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	

	ViewProjection* viewProjection_ = nullptr;

};