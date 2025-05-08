#pragma once
#define NOMINMAX     
#include "geometry.h"
#include "tgaimage.h"
#include <wtypes.h>
#include"Model.h"
#include "Camera.h"

class Render
{
public:

	Render() : width(0), height(0), camera(nullptr) {}  // 构造函数初始化成员变量

	 void SetSize(int w, int h);
	 void SetCamera(Camera* cam) { camera = cam; }

	 void RenderModel(HDC hdc, Model* model, TGAImage& texture, Vec3f light);


	 void Triangle(HDC hdc,Vec3f* points,Vec2f*uv, float* zbuff, TGAImage& texture, float intensity);
	
	 Vec3f World2Screen(Vec3f v);


private:
	 Vec3f barycentric(Vec3f a, Vec3f b, Vec3f c, Vec3f p);
	Matrix projectionMatrix;
	Matrix viewMatrix;
	Camera* camera;
	int width, height;
};

