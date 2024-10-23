#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;
#include "MapChipField.h"
#include "Player.h"
#include "Skydome.h"
#include <vector>

class ClearScene {
public:
	ClearScene();
	~ClearScene();

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
	Model* modelFont_ = nullptr;
	Model* modelSpace_ = nullptr;

	// ビュープロジェクション
	Camera camera_;

	WorldTransform worldTransformFont_;
	WorldTransform worldTransformSpace_;


	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	float timer_ = 0.0f;
	static inline const float kMotionTime = 1.0f;

	// サウドデータハンドル
	uint32_t soundDataHandle_ = 0;
	// 音声再生ハンドル
	uint32_t voiceHandle_ = 0;
};
