#include "DrawLine.h"


void DrawLine::line(HDC hdc, int ax, int ay, int bx, int by, COLORREF color)
{
	bool steep = std::abs(ax - bx) < std::abs(ay - by);

	if (steep)
	{
		std::swap(ax, ay);
		std::swap(bx, by);
	}
	if (ax > bx)
	{
		std::swap(ax, bx);
		std::swap(ay, by);
	}
	int y = ay;
	int ierror = 0;
	for (int x = ax; x <= bx; x++)
	{
		if (steep)
		{
			SetPixel(hdc, y, x, color);
		}
		else
		{
			SetPixel(hdc, x, y, color);
		}

		ierror += 2 * std::abs(by - ay);
		y += (by > ay ? 1 : -1) * (ierror > bx - ax);
		ierror -= 2 * (bx - ax) * (ierror > bx - ax);
	}
}

void DrawLine::line(HDC hdc, Vec2f a, Vec2f b, COLORREF color)
{
	DrawLine::line(hdc,a.x,a.y,b.x,b.y,color);

}
