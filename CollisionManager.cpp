#include "CollisionManager.h"
#include "Vector.h"

void CollisionManager::SetCollider(Collider* collider) {

	colliders_.push_back(collider);

}

void CollisionManager::ClearColliders() {

	colliders_.clear();

}

void CollisionManager::CheckAllColision() {

	// リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;

		// イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}


}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {

	
	// 衝突フィルタリング
	if (((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0b0 || 
		((colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) == 0b0))) {
		return;
	}

	Vector3 worldPosA = colliderA->GetWorldPosition();
	Vector3 worldPosB = colliderB->GetWorldPosition();

	float distance = Length(Subtract(worldPosA, worldPosB));
	// 球と球の交差判定
	if (distance <= colliderA->GetRadius() + colliderB->GetRadius()) {
		// 自キャラ衝突時コールバックを呼び出す
		colliderA->OnCollision();
		// 敵弾の衝突時コールバックを呼び出す
		colliderB->OnCollision();
	}


}
