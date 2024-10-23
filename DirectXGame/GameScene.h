#pragma once

#include "Botton.h"
#include "GoalParticles.h"
#include "MapChipField.h"
#include "Mirror.h"
#include "Player.h"
#include "Skydome.h"
#include <vector>

using namespace KamataEngine;
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	void GenerateBlocks();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// デスフラグのgetter
	bool IsFinished() const { return finished_; }

private: // メンバ変数
	// 終了フラグ
	bool finished_ = false;

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	//// テクスチャハンドル
	// uint32_t textureHandle_ = 0;

	// 3Dモデルデータ
	Model* model_ = nullptr;

	// カメラ
	Camera camera_;

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	// マップチップフィールド
	MapChipField* mapChipField_;

	// デバックカメラ有効
	bool isDebugCameraActive_ = false;

	// デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	// トランスフォーム
	WorldTransform worldTransform_;

	// player
	Player* player_ = nullptr;

	// playerModel
	Model* ModelPlayer_ = nullptr;

	// botton
	Botton* botton_ = nullptr;

	// botton
	Botton* botton2_ = nullptr;

	// bottonModel
	Model* ModelBotton_ = nullptr;

	// mirror
	Mirror* mirror_ = nullptr;

	// すべての当たり判定を行う
	void CheckAllCollisions();

	// スカイドーム
	Skydome* skydome_ = nullptr;

	// 天球モデル
	Model* modelSkydome_ = nullptr;

	////　パーティクル
	//GoalParticles* goalParticles_ = nullptr;
	////　モデルパーティクル
	//Model* modelGoalParticle_ = nullptr;

	// ゲームのフェーズ(型)
	enum class Phase {
		kPlay,  // ゲームプレイ
		kDeath, // デス演出
	};

	// ゲームの現在フェーズ(変数)
	Phase phase_;

	//std::list<GoalParticles*> goalPartiy_;

	// playerとmirrorの状態を切り替える
	void ChangePhase();
};