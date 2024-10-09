#pragma once
#include "WorldTransform.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

/// <summary>
/// マップチップフィールド
/// </summary>

enum class MapChipType {
	kBlank,//　空白
	kBlock,//　ブロック
};

struct MapChipDate {
	std::vector<std::vector<MapChipType>> date;
};





class MapChipField {
public:

	struct IndexSet {
		uint32_t xIndex;
		uint32_t yIndex;
	};

	// 矩形
	struct Rect {
		float left ;	// 左端
		float right ;	// 右端
		float bottom;	// 下端
		float top ;		// 上端
	};

	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	static inline const uint32_t GetNumBlockVirtical() { return kNumBlockVirtical; }
	static inline const uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal; }

	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);

	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

private:
	// 1ブロックのサイズ
	static inline const float kBlockWidth_ = 1.0f;
	static inline const float kBlockHeight_ = 1.0f;
	// ブロックの個数
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipDate mapChipDate_;

};
