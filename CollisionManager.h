#pragma once
#include <algorithm>
#include <list>
#include "Collider.h"

/// <summary>
/// 衝突マネージャ
/// </summary>
class CollisionManager {
public:

	//リセット
	void Reset();

	//すべての当たり判定をチェック
	void CheckAllCollisions();

	void AddCollider(Collider* collider);

private:

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckColliderPair(Collider* colliderA, Collider* colliderB);

	//コライダー
	std::list<Collider*> colliders_;

};
