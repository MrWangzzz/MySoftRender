#pragma once
#include "tgaimage.h"
#include <wtypes.h> 
#include"geometry.h"
class DrawLine
{
public:
	static void line(HDC hdc, int ax, int ay, int bx, int by, COLORREF color);

	static void line(HDC hdc, Vec2f a, Vec2f b, COLORREF color);
	
private:

};

