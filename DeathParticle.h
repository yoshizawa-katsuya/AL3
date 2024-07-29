#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <array>
#include <numbers>

/// <summary>
/// デス演出用パーティクル
/// </summary>
class DeathParticle {
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

	// デスフラグのgetter
	bool GetIsFinished() const { return isFinished_; }

private:

	//パーティクルの個数
	static inline const uint32_t kNumParticles_ = 8;

	std::array<WorldTransform, kNumParticles_> worldTransforms_;
	
	// モデル
	Model* model_ = nullptr;

	ViewProjection* viewProjection_ = nullptr;

	//存続時間(消滅までの時間)<秒>
	static inline const float kDuration_ = 1.0f;
	//移動の速さ
	static inline const float kSpeed_ = 0.1f;
	//分割した一個分の角度
	static inline const float kAngleUnit_ = std::numbers::pi_v<float> * 2 / kNumParticles_;

	//終了フラグ
	bool isFinished_ = false;
	//経過時間カウント
	float counter_ = 0.0f;

	//色変更オブジェクト
	ObjectColor objectColor_;
	//色の数値
	Vector4 color_;

};
