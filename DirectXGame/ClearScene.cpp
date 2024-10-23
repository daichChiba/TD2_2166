#include "ClearScene.h"
#include <numbers>

ClearScene::ClearScene() {}

ClearScene::~ClearScene() {
	delete modelFont_;
}

void ClearScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ビュープロジェクション
	camera_.Initialize();

	modelFont_ = Model::CreateFromOBJ("clearFont");

	worldTransformFont_.Initialize();

	worldTransformFont_.translation_.y = 10;
	worldTransformFont_.scale_ = {2, 2, 2};
}

void ClearScene::Update() {
	if (Input::GetInstance()->ReleseKey(DIK_SPACE)) {
		finished_ = true;
	}
	// タイマーを加算
	timer_ += 1.0f / 60.0f;


	// 行列を更新
	worldTransformFont_.UpdateMatrix();
}

void ClearScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	// ここに3Dオブジェクトの描画処理を追加できる
	modelFont_->Draw(worldTransformFont_, camera_);

	// 3Dオブジェクト描画処理後
	Model::PostDraw();
}
