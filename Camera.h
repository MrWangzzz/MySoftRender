#pragma once
#include "geometry.h"
#include <iostream>

class Camera {
public:
    Vec3f Position;    // 位置
    Vec3f Target;      // 观察目标
    Vec3f Up;          // 上方向
    float Near;        // 近平面
    float Far;         // 远平面
    float Size;        // 视锥体截面大小

    Camera(Vec3f pos = Vec3f(0, 0, 5),
        Vec3f target = Vec3f(0, 0, 0),
        Vec3f up = Vec3f(0, 1, 0),
        float fnear = 0.1f,
        float ffar = 100.0f,
        float size = 5.0f)
        : Position(pos), Target(target), Up(up),
        Near(fnear), Far(ffar), Size(size) {}

    // 获取视图矩阵
    Matrix GetViewMatrix() {
        Vec3f z = (Position - Target).normalize();  // 前方向
        Vec3f x = cross(Up, z).normalize();         // 右方向
        Vec3f y = cross(z, x).normalize();          // 上方向

        Matrix view(4, 4);
        // 旋转部分
        view[0][0] = x.x; view[0][1] = x.y; view[0][2] = x.z;
        view[1][0] = y.x; view[1][1] = y.y; view[1][2] = y.z;
        view[2][0] = z.x; view[2][1] = z.y; view[2][2] = z.z;
        
        // 平移部分
        view[0][3] = -dot(x, Position);
        view[1][3] = -dot(y, Position);
        view[2][3] = -dot(z, Position);
        view[3][3] = 1.0f;

        // 打印视图矩阵用于调试
        std::cout << "\nView Matrix:" << std::endl;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                std::cout << view[i][j] << " ";
            }
            std::cout << std::endl;
        }

        return view;
    }

    // 获取正交投影矩阵
    Matrix GetOrthographicMatrix(float width, float height) {
        float aspect = width / height;
        Matrix proj = Matrix::orthographic(
            -Size * aspect, Size * aspect,  // 左右边界
            -Size, Size,                    // 上下边界
            Near, Far                       // 远近平面
        );

        // 打印投影矩阵用于调试
        std::cout << "\nProjection Matrix:" << std::endl;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                std::cout << proj[i][j] << " ";
            }
            std::cout << std::endl;
        }

        return proj;
    }
};