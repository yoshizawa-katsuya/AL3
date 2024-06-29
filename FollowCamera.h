#pragma once
#include <ViewProjection.h>
#include <WorldTransform.h>

///<summary>
///追従カメラ
///</summary>
class FollowCamera {
public:

	//初期化
	void Initialize();

	void Reset();

	//更新
	void Update();

	//追従対象の設定
	void SetTarget(const WorldTransform* target);

	//オフセット計算
	Vector3 OffsetCalculation();

	const ViewProjection& GetViewProjection() { return viewProjection_; }

private:

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//追従対象
	const WorldTransform* target_ = nullptr;

	//追従対象の残像座標
	Vector3 interTarget_ = {};

	//目標角度
	float destinationAngleY_;

};