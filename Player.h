#pragma once
#define NOMINMAX
#include "WorldTransform.h"
#include "Model.h"
#include <numbers>
#include <Input.h>
#include <algorithm>

class MapChipField;

///<summary>
///自キャラ
/// </summary>
class Player {
private:

	// マップとの当たり判定情報
	struct CollisionMapInfo {
		bool isCeilingCollision = false;
		bool landing = false;
		bool isWallCollision = false;
		Vector3 move;
	};

	enum Corner {
		kRightBottom,	//右下
		kLeftBottom,	//左下
		kRightTop,		//右上
		kLeftTop,		//左上

		kNumCorner		//要素数

	};

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

	//移動
	void Move();

	//マップ衝突判定
	void MapCollision(CollisionMapInfo& info);
	void MapCollisionUp(CollisionMapInfo& info);
	void MapCollisionBottom(CollisionMapInfo& info);
	void MapCollisionRight(CollisionMapInfo& info);
	void MapCollisionLeft(CollisionMapInfo& info);

	//判定結果を反映して移動させる
	void MoveAppli(const CollisionMapInfo& info);

	//天井に接触している場合の処理
	void CeilingCollision(const CollisionMapInfo& info);

	//地面に接触している場合の処理
	void GroundCollision(const CollisionMapInfo& info);

	//壁に接触している場合の処理
	void WallCollision(const CollisionMapInfo& info);

	Vector3 CornerPosition(const Vector3& center, Corner corner);

	/// <summary>
	/// 描画
	///  </summary>
	void Draw();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

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
	static inline const float kGravityAcceleration = 0.03f;

	static inline const float kLimitFallSpeed = 1.0f;
	static inline const float kJumpAcceleration = 0.5f;

	//着地時の速度減衰率
	static inline const float kAttenuationLanding = 0.1f;

	//壁に接触時の速度減衰比率
	static inline const float kAttenuationWall = 1.0f;


	//キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	//余白
	static inline const float kBlank = 0.01f;

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	

	ViewProjection* viewProjection_ = nullptr;

	//マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

};