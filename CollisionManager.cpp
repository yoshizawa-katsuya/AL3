#include "CollisionManager.h"
#include "Vector.h"
#include "Matrix.h"

void CollisionManager::Reset() {

	//リストを空っぽにする
	colliders_.clear();

}

void CollisionManager::CheckAllCollisions() {

	//リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;

		//イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;

			//ペアの当たり判定
			CheckColliderPair(colliderA, colliderB);
		}
	}

}

void CollisionManager::AddCollider(Collider* collider) {

	colliders_.push_back(collider);

}

void CollisionManager::CheckColliderPair(Collider* colliderA, Collider* colliderB) {

	//コライダーAの座標を取得
	Vector3 posA = colliderA->GetCenterPosition();
	//コライダーBの座標を取得
	Vector3 posB = colliderB->GetCenterPosition();
	//座標の差分ベクトル
	Vector3 subtract = posB - posA;
	//座標AとBの距離を求める
	float distance = Length(subtract);
	//球と球の交差判定
	if (distance < colliderA->GetRadius() + colliderB->GetRadius()) {
	
		//コライダーAの衝突時コールバックを呼び出す
		colliderA->OnCollision();
		// コライダーBの衝突時コールバックを呼び出す
		colliderB->OnCollision();

	}
}
