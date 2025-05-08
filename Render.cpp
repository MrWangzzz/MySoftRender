#include "Render.h"
#include <iostream>

void Render::SetSize(int w, int h)
{
	this->width = w;    // ʹ this ָʳԱ
	this->height = h;   // ʹ this ָʳԱ
}

void Render::RenderModel(HDC hdc,Model* model, TGAImage& texture,Vec3f light)
{
	// 添加调试输出
	std::cout << "=== Model Information ===" << std::endl;
	std::cout << "Number of vertices: " << model->nverts() << std::endl;
	std::cout << "Number of faces: " << model->nfaces() << std::endl;
	
	// 检查第一个面的顶点信息
	if (model->nfaces() > 0) {
		Model::Face f = model->face(0);
		std::cout << "\nFirst face vertices:" << std::endl;
		for (int j = 0; j < 3; j++) {
			Vec3f v = model->vert(f.v_idx[j]);
			std::cout << "Vertex " << j << ": (" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
		}
	}

	float* zbuffer = new float[width * height];
	for (int i = width * height; i--; zbuffer[i] = -std::numeric_limits<float>::max());

	for (int i = 0; i < model->nfaces(); i++)
	{
		Model::Face f = model->face(i);

		Vec3f world_coords[3];
		Vec3f pts[3];
		Vec2f uv[3];

		for (int j = 0; j < 3; j++) {
			world_coords[j] = model->vert(f.v_idx[j]);
			pts[j] = World2Screen(model->vert(f.v_idx[j]));
			uv[j] = model->uv(f.vt_idx[j]);
		}

		// 计算法线
		Vec3f n = cross((world_coords[2] - world_coords[0]), (world_coords[1] - world_coords[0]));
		n.normalize();

		// 计算光照强度
		float intensity = dot(n, light);
		
		// 将光照强度限制在0-1范围内
		intensity = std::max(0.0f, std::min(1.0f, intensity));
		
		// 添加环境光，防止完全黑暗
		intensity = 0.2f + 0.8f * intensity;

		Triangle(hdc, pts, uv, zbuffer, texture, intensity);
	}
	

}

void Render::Triangle(HDC hdc,Vec3f* points, Vec2f* uv, float* zbuff, TGAImage& texture, float intensity)
{
	Vec2f boxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

	Vec2f boxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{

			boxmin[i] = std::min(boxmin[i], points[j][i]);

			boxmax[i] = std::max(boxmax[i], points[j][i]);
		}
	}


	Vec3f p;
	for (p.x = boxmin.x; p.x <= boxmax.x; p.x++)
	{
		for (p.y = boxmin.y; p.y < boxmax.y; p.y++)
		{
			Vec3f bc_screen = barycentric(points[0], points[1], points[2], p);
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
			{
				continue;
			}
			p.z = 0;
			for (int i = 0; i < 3; i++)
			{
				p.z += points[i][2] * bc_screen[i];
			}
			if (zbuff[int(p.x + p.y * width)] < p.z)
			{
				zbuff[int(p.x + p.y * width)] = p.z;

				float u = bc_screen.x * uv[0].x + bc_screen.y * uv[1].x + bc_screen.z * uv[2].x;
				float v = bc_screen.x * uv[0].y + bc_screen.y * uv[1].y + bc_screen.z * uv[2].y;
				TGAColor textco = texture.get(u * texture.width(), v * texture.height());

				// 修改颜色计算，确保正确的通道顺序
				int r = static_cast<int>(textco.bgra[2] * intensity);  // R
				int g = static_cast<int>(textco.bgra[1] * intensity);  // G
				int b = static_cast<int>(textco.bgra[0] * intensity);  // B

				// 确保颜色值在有效范围内
				r = std::min(255, std::max(0, r));
				g = std::min(255, std::max(0, g));
				b = std::min(255, std::max(0, b));

				COLORREF destColor = RGB(r, g, b);  // Windows RGB宏使用BGR顺序
				SetPixel(hdc, p.x, p.y, destColor);

				
			}

		}
	}
}

Vec3f Render::World2Screen(Vec3f v)
{
	static bool firstVertex = true;  // 用于标记是否是第一个顶点
	if (!camera) {
		return Vec3f(v.x, v.y, v.z);
	}
	// 获取视图矩阵和投影矩阵
	Matrix view = camera->GetViewMatrix();
	Matrix proj = camera->GetOrthographicMatrix(width, height);

	// 只打印第一个顶点的信息
	if (firstVertex) {
		std::cout << "\n=== First Vertex Transform Process ===" << std::endl;
		std::cout << "World coordinates: (" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
		firstVertex = false;
	}

	// 应用视图变换
	Vec4f v_camera;
	v_camera[0] = v.x * view[0][0] + v.y * view[0][1] + v.z * view[0][2] + view[0][3];
	v_camera[1] = v.x * view[1][0] + v.y * view[1][1] + v.z * view[1][2] + view[1][3];
	v_camera[2] = v.x * view[2][0] + v.y * view[2][1] + v.z * view[2][2] + view[2][3];
	v_camera[3] = 1.0f;

	// 应用投影变换
	Vec4f v_proj;
	v_proj[0] = v_camera[0] * proj[0][0] + v_camera[1] * proj[0][1] + v_camera[2] * proj[0][2] + v_camera[3] * proj[0][3];
	v_proj[1] = v_camera[0] * proj[1][0] + v_camera[1] * proj[1][1] + v_camera[2] * proj[1][2] + v_camera[3] * proj[1][3];
	v_proj[2] = v_camera[0] * proj[2][0] + v_camera[1] * proj[2][1] + v_camera[2] * proj[2][2] + v_camera[3] * proj[2][3];
	v_proj[3] = v_camera[0] * proj[3][0] + v_camera[1] * proj[3][1] + v_camera[2] * proj[3][2] + v_camera[3] * proj[3][3];

	// 透视除法
	if (v_proj[3] != 0.0f) {
		v_proj[0] /= v_proj[3];
		v_proj[1] /= v_proj[3];
		v_proj[2] /= v_proj[3];
	}

	// 转换到屏幕坐标
	float x = (v_proj[0] + 1.0f) * width / 2.0f;
	float y = height - ((v_proj[1] + 1.0f) * height / 2.0f);

	// 确保坐标在屏幕范围内
	x = std::max(0.0f, std::min(static_cast<float>(width - 1), x));
	y = std::max(0.0f, std::min(static_cast<float>(height - 1), y));

	// 只打印第一个顶点的最终屏幕坐标
	if (!firstVertex) {
		std::cout << "Screen coordinates: (" << x << ", " << y << ", " << v_proj[2] << ")" << std::endl;
	}

	return Vec3f(x, y, v_proj[2]);
}

Vec3f Render::barycentric(Vec3f a, Vec3f b, Vec3f c, Vec3f p)
{
	Vec3f s[2];

	for (int i = 2; i--;)
	{
		s[i][0] = c[i] - a[i];
		s[i][1] = b[i] - a[i];
		s[i][2] = a[i] - p[i];
	}


	Vec3f u = cross(s[0], s[1]);

	if (std::abs(u[2]) > 1e-2)
	{
		return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
	}

	return Vec3f(-1, 1, 1);
}
