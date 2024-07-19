#pragma once
#include "Collider.h"

/// <summary>
/// ハンマー
/// </summary>
class Hammer : public Collider {
public:

	//初期化
	void Initialize(Model* model);

	// ワールドトランスフォームの更新
	void UpdateWorldTransform();

	//描画
	void Draw(const ViewProjection& viewProjection);

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision([[maybe_unused]] Collider* other) override;

	Vector3 GetCenterPosition() const override;

	const Vector3& GetRotation() const { return worldTransform_.rotation_; }
	void SetRotation(const Vector3& rotation) { worldTransform_.rotation_ = rotation; }

	const Vector3& GetTranslation() const { return worldTransform_.translation_; }
	void SetTranslation(const Vector3& translation) { worldTransform_.translation_ = translation; }

	void SetParent(WorldTransform* parent) { worldTransform_.parent_ = parent; }

private:

	Model* model_ = nullptr;
	WorldTransform worldTransform_;

};
