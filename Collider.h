#pragma once
#include <Vector3.h>

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider {
public:

	virtual ~Collider() = default;

	//衝突時に呼ばれる関数
	virtual void OnCollision() {}

	//中心座標を取得
	virtual Vector3 GetCenterPosition() const = 0;

	//半径を取得
	float GetRadius() { return radius_; }

	void SetRadius(float radius) { radius_ = radius; }

private:

	//衝突半径
	float radius_ = 1.5f;

};
