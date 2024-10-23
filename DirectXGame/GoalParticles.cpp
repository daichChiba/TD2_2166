#include "GoalParticles.h"
#include <cassert>
#include <algorithm>
using namespace KamataEngine;
void GoalParticles::Initialize(Model* model, Camera* camera, const Vector3& position) {
	//NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	//ローカル変数は関数が終わると消える
	model_ = model;
	camera_ = camera;

	//ワールド変換の初期化
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
}

void GoalParticles::Update() {
	//終了なら何もしない
	if (isFinished_) {
		return;
	}
	//カウンターを１フレーム分の秒数進める
	counter_ += 1.0f / 60.0f;

	//存続時間の上限に達したら
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		//終了扱いにする
		isFinished_ = true;
	}
	for (uint32_t i = 0; i < kNumParticles; ++i) {
	    //基本となる速度ベクトル
		Vector3 velocity = {kSpeed, 0, 0};
		//回転角を計算する
		float angle = kAngleUnit * i;
		//Z軸まわり回転行列
		Matrix4x4 matixRotation = MakeRotateZMatrix(angle);

		//基本ベクトルを回転させて速度ベクトルを得る
		velocity = Transform(velocity, matixRotation);
		//移動処理
		worldTransforms_[i].translation_ += velocity;
	}
	// 行列を更新
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.UpdateMatrix();
	}

	color_.w = std::clamp(1.0f - counter_ / kDuration, 0.0f, 1.0f);
	// 色変更オブジェクトに色の数値を設定する
	objectColor_.SetColor(color_);
	// 色変更オブジェクト
	//objectColor_.TransferMatrix();


}

void GoalParticles::Draw() {
	//終了なら何もしない
	if (isFinished_) {
		return;
	}

	//3Dモデルを描画
	for (auto& worldTransform : worldTransforms_) {
		//model_->Draw(worldTransform, *camera_);
		model_->Draw(worldTransform, *camera_, &objectColor_);
	}
}
