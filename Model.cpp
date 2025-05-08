#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"
#include "geometry.h"
Model::Model(const char* fileName)
{
	std::ifstream in;
	in.open(fileName, std::ifstream::in);

	if (in.fail())
	{
		return;
	}

	std::string line;


	while (!in.eof())
	{
		std::getline(in, line);

		std::istringstream iss(line.c_str());
		char trash;

		if (!line.compare(0, 2, "v "))
		{
			iss >> trash;
			Vec3f v;

			for (int i = 0; i < 3; i++)
			{
				iss >> v[i];
			}
			verts_.push_back(v);
		}
		else if (!line.compare(0, 4, "vt  "))
		{
			iss >> trash >> trash;
			Vec2f v;

			for (int i = 0; i < 2; i++)
			{
				iss >> v[i];
			}
			v.y = 1 - v.y;
			uv_.push_back(v);
		}



		else if (!line.compare(0, 2, "f "))
		{


			int v_idx, vt_idx, vn_idx;
			char trash;  // ÓÃÓÚ¶ÁÈ¡'/'×Ö·û

			Face f;
			iss >> trash;
			while (iss >> v_idx >> trash >> vt_idx >> trash >> vn_idx) {
				// in wavefront obj all indices start at 1, not zero
				v_idx--;
				vt_idx--;
				vn_idx--;
				f.v_idx.push_back(v_idx);
				f.vt_idx.push_back(vt_idx);
				f.vn_idx.push_back(vn_idx);
			}
			faces_.push_back(f);


		}

	}
	std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << std::endl;



}
Model::~Model() {
}
int Model::nverts() {
	return (int)verts_.size();
}

int Model::nfaces() {
	return (int)faces_.size();
}

Model::Face Model::face(int idx) {
	return faces_[idx];
}




Vec3f Model::vert(int i) {
	return verts_[i];
}
Vec2f Model::uv(int i)
{
	return uv_[i];
}