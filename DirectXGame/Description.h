#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;
#include "MapChipField.h"
#include "Player.h"
#include "Skydome.h"
#include <vector>
class Description {
public:
	Description();
	~Description();

	/// <summary>
	/// 初期化
	/// <summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// <summary>
	void Update();

	/// <summary>
	/// 描画
	/// <summary>
	void Draw();

	// デスフラグのgetter
	bool IsFinished() const { return finished_; }

	// 終了フラグ
	bool finished_ = false;

private:
	Model* modelDescription_ = nullptr;
	Model* modelSpace_ = nullptr;

	// ビュープロジェクション
	Camera camera_;

	WorldTransform worldTransformDescription_;
	WorldTransform worldTransformSpace_;

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	float timer_ = 0.0f;
	static inline const float kMotionTime = 1.0f;
};
