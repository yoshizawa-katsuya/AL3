#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

class Player;

/// <summary>
/// カメラコントローラ
/// </summary>
class CameraController {

public:

	// 矩形
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

	/// <summary>
	/// 初期化
	///  </summary>
	void Initialize();

	/// <summary>
	/// 更新
	///  </summary>
	void Update();

	void Reset();

	void SetTarget(Player* target);

	void SetMovableArea(Rect area);

	ViewProjection& GetViewProjection();

private:
	
	

	//カメラ移動範囲
	Rect movableArea_ = {0, 100, 0, 100};

	//追従対象の各方向へのカメラ移動範囲
	static inline const Rect margin_ = {-8.0f, 8.0f, -5.0f, 5.0f};

	//ビュープロジェクション
	ViewProjection viewProjection_;

	Player* target_ = nullptr;

	//追従対象とカメラの座標の差(オフセット)
	Vector3 targetOffset_ = {0, 0, -15.0f};

	//カメラの目標座標
	Vector3 targetVector_;

	//座標補完割合
	static inline const float kInterpolationRate = 0.3f;

	//速度掛け率
	static inline const float kVelocityBies = 15.0f;

};