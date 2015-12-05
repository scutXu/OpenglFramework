#pragma once
#include<QVector4D>
#include<memory>
using std::shared_ptr;
struct Viewport
{
	Viewport(int pX = 0, int pY = 0, int pW = 800, int pH = 800) : x(pX), y(pY), width(pW), height(pH) {}
	int x;
	int y;
	int width;
	int height;
	QVector4D backgroundColor;
};

typedef shared_ptr<Viewport> ViewportPtr;