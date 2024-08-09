#pragma once
#include "Vector3.h"

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider {
public:

	//衝突時に呼ばれる関数
	virtual void OnCollision() = 0;

	// ワールド座標を取得
	virtual Vector3 GetWorldPosition() = 0;

	//半径を取得
	const float GetRadius() const { return radius_; }

	//半径を設定
	void SetRadius(float radius) { radius_ = radius; }

protected:
	//衝突半径
	float radius_ = 1.0f;

};
