#pragma once

constexpr int GridMinX = -10;
constexpr int GridMaxX = 10;
constexpr int GridMinZ = 0;
constexpr int GridMaxZ = 20;

constexpr int GridWidth = GridMaxX - GridMinX + 1;     // 21
constexpr int GridHeight = GridMaxZ - GridMinZ + 1;     // 21
constexpr int GridCount = GridWidth * GridHeight;      // 441
constexpr int Grid2Count = 634;

struct BLOCK
{
	XMFLOAT3 Position;
	int Type;
};

static BLOCK g_Block1[GridCount];
static BLOCK g_Block2[Grid2Count] = 
{
	{{7.0f,		1.0f,	10.0f},	1},
	{{-7.0f,	1.0f,	10.0f},	1},
	{{7.0f,		1.0f,	14.0f},	1},
	{{-7.0f,	1.0f,	14.0f},	1},
	{{1.0f,		1.0f,	20.0f},	1},
	{{-1.0f,	1.0f,	20.0f},	1},
	{{0.0f,		2.0f,	10.0f},	1},
	{{10.0f,	1.0f,	1.0f},	0},
	{{10.0f,	1.0f,	2.0f},	0},
	{{10.0f,	1.0f,	3.0f},	0},
	{{10.0f,	1.0f,	4.0f},	0},
	{{10.0f,	1.0f,	5.0f},	0},
	{{10.0f,	1.0f,	6.0f},	0},
	{{10.0f,	1.0f,	7.0f},	0},
	{{10.0f,	1.0f,	8.0f},	0},
	{{10.0f,	1.0f,	9.0f},	0},
	{{10.0f,	1.0f,	10.0f},	0},
	{{10.0f,	1.0f,	11.0f},	0},
	{{10.0f,	1.0f,	12.0f},	0},
	{{10.0f,	1.0f,	13.0f},	0},
	{{10.0f,	1.0f,	14.0f},	0},
	{{10.0f,	1.0f,	15.0f},	0},
	{{10.0f,	1.0f,	16.0f},	0},
	{{10.0f,	1.0f,	17.0f},	0},
	{{10.0f,	1.0f,	18.0f},	0},
	{{10.0f,	1.0f,	19.0f},	0},//30
	{{10.0f,	1.0f,	20.0f},	0},
	{{-10.0f,	1.0f,	0.0f},	0},
	{{-10.0f,	1.0f,	1.0f},	0},
	{{-10.0f,	1.0f,	2.0f},	0},
	{{-10.0f,	1.0f,	3.0f},	0},
	{{-10.0f,	1.0f,	4.0f},	0},
	{{-10.0f,	1.0f,	5.0f},	0},
	{{-10.0f,	1.0f,	6.0f},	0},
	{{-10.0f,	1.0f,	7.0f},	0},
	{{-10.0f,	1.0f,	8.0f},	0},
	{{-10.0f,	1.0f,	9.0f},	0},
	{{-10.0f,	1.0f,	10.0f},	0},
	{{-10.0f,	1.0f,	11.0f},	0},
	{{-10.0f,	1.0f,	12.0f},	0},
	{{-10.0f,	1.0f,	13.0f},	0},
	{{-10.0f,	1.0f,	14.0f},	0},
	{{-10.0f,	1.0f,	15.0f},	0},
	{{-10.0f,	1.0f,	16.0f},	0},
	{{-10.0f,	1.0f,	17.0f},	0},
	{{-10.0f,	1.0f,	18.0f},	0},
	{{-10.0f,	1.0f,	19.0f},	0},
	{{-10.0f,	1.0f,	20.0f},	0},
	{{00.0f,	1.0f,	7.0f},	0},
	{{00.0f,	1.0f,	8.0f},	0},
	{{00.0f,	1.0f,	9.0f},	0},
	{{00.0f,	1.0f,	10.0f},	0},
	{{00.0f,	1.0f,	11.0f},	0},
	{{00.0f,	1.0f,	12.0f},	0},//58
	{{00.0f,	2.0f,	5.0f},	0},
	{{00.0f,	2.0f,	15.0f},	0},
	{{6.0f,		1.0f,	3.0f},	8},
	{{7.0f,		1.0f,	2.0f},	5},
	{{8.0f,		1.0f,	3.0f},	6},
	{{7.0f,		1.0f,	5.0f},	7},
	{{7.0f,		1.0f,	4.0f},	0},
	{{7.0f,		1.0f,	3.0f},	0},
	{{7.0f,		1.0f,	0.0f},	0},
	{{7.0f,		1.0f,	7.0f},	0},

};

BLOCK* GetFieldBlock(void);
BLOCK* GetFieldItem(void);

// CSVからBLOCK配列を読み込む（成功: true）
bool Field_LoadBlocksFromCSV(const char* path, BLOCK* outBlocks, int maxCount, int* outCount);


void InitPositions(void);
void FieldInitialize(void);//ポリゴン初期化
void FieldFinalize(void);//ポリゴン終了
void FieldUpdate(void);//ポリゴン更新
void FieldDraw(void);//ポリゴン描画