#include "WorldTransform.h"
#include "Matrix.h"

void WorldTransform::UpdateMatrix() {

	//スケール、回転、平行移動を合成して行列を計算する
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);

	//親があれば親のワールド行列を掛ける
	if (parent_) {
		matWorld_ = Multiply(matWorld_, parent_->matWorld_);
	}

	//定数バッファに転送する
	TransferMatrix();

}

Vector3 WorldTransform::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worlsPos;
	// ワールド行列の平行移動成分を取得
	worlsPos.x = matWorld_.m[3][0];
	worlsPos.y = matWorld_.m[3][1];
	worlsPos.z = matWorld_.m[3][2];

	return worlsPos;
}