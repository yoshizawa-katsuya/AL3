#pragma once
#include <list>
#include "Collider.h"

class CollisionManager {
public:

	void SetCollider(Collider* collider);

	void ClearColliders();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllColision();

private:

	/// コライダー2つの衝突判定と応答
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	//コライダーリスト
	std::list<Collider*> colliders_;

};
