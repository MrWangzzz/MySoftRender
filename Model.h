#pragma once
#include<vector>
#include "geometry.h"
class Model
{



public:
	Model(const char* fileName);
	~Model();

	struct Face {
		std::vector<int> v_idx;
		std::vector<int> vt_idx;
		std::vector<int> vn_idx;

		// 构造函数
		Face() = default;

		// 获取顶点数量
		int nverts() const { return v_idx.size(); }

		// 获取第i个顶点的索引
		int vert(int i) const { return v_idx[i]; }

		// 获取第i个纹理坐标的索引
		int texcoord(int i) const { return vt_idx[i]; }

		// 获取第i个法线的索引
		int normal(int i) const { return vn_idx[i]; }
	};

	int nverts();
	int nfaces();

	Face face(int idx);
	Vec3f vert(int i);

	Vec2f uv(int i);
private:
	std::vector<Vec3f> verts_;

	std::vector<Vec2f> uv_;


	std::vector<Face> faces_;

};
