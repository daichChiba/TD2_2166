#pragma once
#include "ViewProjection.h"

/// <summary>
/// カメラコントローラー
/// </summary>

// 前方宣言
class Player;

// 矩形
struct Rect {
	float left = 0.0f;   // 左端
	float right = 1.0f;  // 右端
	float bottom = 0.0f; // 下端
	float top = 1.0f;    // 上端
};

class CameraController {
public:
	/// <summary>
	///	初期化
	/// </summary>
	void Initialize();

	/// <summary>
	///	更新
	/// </summary>
	void Update();

	void SetTarget(Player* target) { target_ = target; }

	const ViewProjection& GetViewProjection() { return viewProjection_; }

	void Reset();

	void SetMovableArea(Rect area) { movaArea_ = area; }

	// 座標補間割合
	static inline const float kInterpolationRate = 1.5f;

private:
	// ビュープロジェクション
	ViewProjection viewProjection_;
	Player* target_ = nullptr;

	// 追従対象とカメラの座標の差(オフセット)
	Vector3 targetOffset_ = {0, 0, -15.0f};

	Rect movaArea_ = {0, 100, 0, 100};

	// カメラの目標座標
	Vector3 targetCoordinates;

	// 速度掛け率
	static inline const float kVelocityBias = 10.0f;

	//追従対象の各方向へのカメラ移動範囲
	static inline const Rect margin_ = {-640.0f, 640.0f, -320.0, 320.0f};
};
