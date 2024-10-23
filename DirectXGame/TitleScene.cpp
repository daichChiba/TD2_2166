#include "TitleScene.h"
#include <numbers>

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {
	delete modelFont_;
	delete modelSpace_;
	finished_ = false;
}

void TitleScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();



// ビュープロジェクション
	camera_.Initialize();

	modelFont_ = Model::CreateFromOBJ("titleFont");
	modelSpace_ = Model::CreateFromOBJ("push_Space");

	worldTransformFont_.Initialize();
	worldTransformSpace_.Initialize();

	worldTransformFont_.translation_.y = 10;
	worldTransformSpace_.translation_.y = -8;
	worldTransformFont_.scale_ = {2, 2, 2};
	worldTransformSpace_.scale_ = {2, 2, 2};

}

void TitleScene::Update() {
	if (Input::GetInstance()->ReleseKey(DIK_SPACE)) {
		finished_ = true;
	}
	// タイマーを加算
	timer_ += 1.0f / 60.0f;

	//worldTransformPlayer_.rotation_.y =
	//	std::numbers::pi_v<float> + std::sin(std::numbers::pi_v<float> * 2.0f * timer_ / kMotionTime);

	// 行列を更新
	worldTransformFont_.UpdateMatrix();
	worldTransformSpace_.UpdateMatrix();
}

void TitleScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	// ここに3Dオブジェクトの描画処理を追加できる
	modelFont_->Draw(worldTransformFont_, camera_);
	modelSpace_->Draw(worldTransformSpace_, camera_);

	// 3Dオブジェクト描画処理後
	Model::PostDraw();
}
