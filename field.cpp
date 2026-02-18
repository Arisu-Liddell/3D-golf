#include "directx.h"
#include "shader.h"
#include "keyboard.h"
#include "model.h"
#include "camera.h"
#include "field.h"
#include <fstream>
#include <sstream>
#include <string>


static MODEL* g_Model[10] = {};
static XMFLOAT3 g_Rotation;


BLOCK* GetFieldBlock(void)
{
	return g_Block1;
}
BLOCK* GetFieldItem(void)
{
	return g_Block2;
}

bool Field_LoadBlocksFromCSV(const char* path, BLOCK* outBlocks, int maxCount, int* outCount)
{
	if (!outBlocks || maxCount <= 0) return false;

	std::ifstream file(path);
	if (!file.is_open()) return false;

	std::string line;
	int count = 0;

	auto isHeaderLine = [](const std::string& s) -> bool
		{
			// "id,x,y,z,type" みたいなヘッダをざっくり検出
			return (s.find("id") != std::string::npos &&
				s.find("x") != std::string::npos &&
				s.find("y") != std::string::npos &&
				s.find("z") != std::string::npos);
		};

	while (std::getline(file, line))
	{
		// 空行 / コメント行スキップ
		if (line.empty()) continue;
		if (line[0] == '#') continue;
		if (line.size() >= 2 && line[0] == '/' && line[1] == '/') continue;

		// ヘッダ行スキップ
		if (count == 0 && isHeaderLine(line)) continue;

		std::stringstream ss(line);

		// CSVは最低でも id,x,y,z,type の5列を想定（後ろにcomment列があっても無視）
		std::string tmp;
		int id = 0;
		float x = 0, y = 0, z = 0;
		int type = 0;

		// id
		if (!std::getline(ss, tmp, ',')) continue;
		try { id = std::stoi(tmp); }
		catch (...) { continue; }

		// x
		if (!std::getline(ss, tmp, ',')) continue;
		try { x = std::stof(tmp); }
		catch (...) { continue; }

		// y
		if (!std::getline(ss, tmp, ',')) continue;
		try { y = std::stof(tmp); }
		catch (...) { continue; }

		// z
		if (!std::getline(ss, tmp, ',')) continue;
		try { z = std::stof(tmp); }
		catch (...) { continue; }

		// type
		if (!std::getline(ss, tmp, ',')) continue;
		try { type = std::stoi(tmp); }
		catch (...) { continue; }

		if (count >= maxCount) break;

		outBlocks[count].Position = XMFLOAT3(x, y, z);
		outBlocks[count].Type = type;
		++count;
	}
	if (outCount) *outCount = count;
	return (count > 0);
}

void InitPositions()
{
	int i = 0;
	for (int z = GridMinZ; z <= GridMaxZ; ++z)
	{
		for (int x = GridMinX; x <= GridMaxX; ++x)
		{
			g_Block1[i].Position = DirectX::XMFLOAT3((float)x, 0.0f, (float)z);
			g_Block1[i].Type = 0;
			++i;
		}
	}
}

void FieldInitialize(void)
{
	int loaded = 0;
	if (Field_LoadBlocksFromCSV("asset\\data\\field_objects.csv", g_Block2, Grid2Count, &loaded))
	{
	} 
	g_Model[0] = ModelLoad("asset\\model\\cube.fbx");
	g_Model[1] = ModelLoad("asset\\model\\tree.fbx");
	g_Model[2] = ModelLoad("asset\\model\\Goal.fbx");
	g_Model[3] = ModelLoad("asset\\model\\ball.fbx");
	g_Model[4] = ModelLoad("asset\\model\\Bear_Normal.fbx");
	g_Model[5] = ModelLoad("asset\\model\\Slope0.fbx");
	g_Model[6] = ModelLoad("asset\\model\\Slope1.fbx");
	g_Model[7] = ModelLoad("asset\\model\\Slope2.fbx");
	g_Model[8] = ModelLoad("asset\\model\\Slope3.fbx");
	g_Model[9] = ModelLoad("asset\\model\\invisibleCube.fbx");

	g_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);//回転初期化
}

void FieldFinalize(void)
{
	ModelRelease(g_Model[0]);
	ModelRelease(g_Model[1]);
	ModelRelease(g_Model[2]);
	ModelRelease(g_Model[3]);
	ModelRelease(g_Model[4]);
	ModelRelease(g_Model[5]);
	ModelRelease(g_Model[6]);
	ModelRelease(g_Model[7]);
	ModelRelease(g_Model[8]);
	ModelRelease(g_Model[9]);
}

void FieldUpdate(void)
{
}

void FieldDraw(void)
{
	for (int i = 0; i < GridCount; i++)
	{
		//頂点シェーダーに変換行列を設定
		MATRIX matrix;

		matrix.World = XMMatrixIdentity();
		matrix.Mtx = XMMatrixIdentity();


		//拡大縮小マトリクス
		matrix.World *= XMMatrixScaling(1.0f, 1.0f, 1.0f);

		//回転マトリクス
		matrix.World *= XMMatrixRotationRollPitchYaw(g_Rotation.x, g_Rotation.y, g_Rotation.z);

		//移動マトリクス
		matrix.World *= XMMatrixTranslation(g_Block1[i].Position.x, g_Block1[i].Position.y, g_Block1[i].Position.z);
		matrix.Mtx = matrix.World;

		//	ビューマトリクス
		matrix.Mtx *= GetCameraViewMatrix();

		//プロジェクションマトリクス
		matrix.Mtx *= GetCameraProjectionMatrix();

		Shader_SetMatrix(matrix);//シェーダーに行列を設定

		//足場モデル描画
		ModelDraw(g_Model[g_Block1[i].Type]);
	}

	for (int i = 0; i < Grid2Count; i++)
	{
		//頂点シェーダーに変換行列を設定
		MATRIX matrix;

		matrix.World = XMMatrixIdentity();
		matrix.Mtx = XMMatrixIdentity();


		//拡大縮小マトリクス
		matrix.World *= XMMatrixScaling(1.0f, 1.0f, 1.0f);

		//回転マトリクス
		matrix.World *= XMMatrixRotationRollPitchYaw(g_Rotation.x, g_Rotation.y, g_Rotation.z);

		//移動マトリクス
		matrix.World *= XMMatrixTranslation(g_Block2[i].Position.x, g_Block2[i].Position.y, g_Block2[i].Position.z);

		matrix.Mtx = matrix.World;

		//ビューマトリクス
		matrix.Mtx *= GetCameraViewMatrix();

		//プロジェクションマトリクス
		matrix.Mtx *= GetCameraProjectionMatrix();

		Shader_SetMatrix(matrix);//シェーダーに行列を設定

		//足場モデル描画
		ModelDraw(g_Model[g_Block2[i].Type]);
	}
}
