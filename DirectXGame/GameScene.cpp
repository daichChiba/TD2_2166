#include "GameScene.h"
#include <cassert>

using namespace KamataEngine;
// インストラクタ
GameScene::GameScene() {}
// デストラクタ
GameScene::~GameScene() {
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
	// 3Dモデルデータの解放
	delete model_;
}

void GameScene::Initialize() {

	//// ファイル名を指定してテクスチャを読み込む
	// textureHandle_ = TextureManager::Load("mario.jpg");

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// 3Dモデルデータの生成
	model_->Model::CreateFromOBJ("block", true);
	// ビュープロジェクションの初期化
	camera_.Initialize();

	worldTransform_.Initialize();

	//mapChipField_ = new MapChipField;
	//mapChipField_->LoadMapChipCsv("Resources/blocks_csv/blocks.csv");

	//GenerateBlocks();
}

void GameScene::GenerateBlocks() {
	// 要素数
	uint32_t kNumBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t kNumBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	//// ブロック１個分の横幅
	// const float kBlockWidth = 1.0f;
	// const float kBlockHeight = 1.0f;

	// 要素数を変更する
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}
	// キューブの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				worldTransformBlocks_[i][j] = new WorldTransform();
				worldTransformBlocks_[i][j]->Initialize();
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::Update() {
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif // DEBUG
       // カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();

		// デバックカメラのビュー行列
		camera_.matView = debugCamera_->GetCamera().matView;
		// デバックカメラのプロジェクション行列
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
	} else {
		//cameraController_->Update();

		//// ビュープロジェクション行列の更新と転送
		//camera_.matView = cameraController_->GetViewProjection().matView;
		//camera_.matProjection = cameraController_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		camera_.TransferMatrix();
	}
	worldTransform_.TransferMatrix();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	//for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
	//	for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
	//		if (!worldTransformBlock) {
	//			continue;
	//		}

	//		model_->Draw(*worldTransformBlock, camera_);
	//	}
	//}
	//model_->Draw(worldTransform_, camera_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
