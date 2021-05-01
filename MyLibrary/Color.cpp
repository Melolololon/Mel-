#include"Color.h"

Color::Color()
{
}

Color::Color(const unsigned char r, const unsigned char g, const unsigned char b, const float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;

	AlphaClamp();
}

Color::Color(const unsigned char rgb, const float a)
{
	this->r = rgb;
	this->g = rgb;
	this->b = rgb;
	this->a = a;

	AlphaClamp();
}

Color Color::operator+(const Color& color)
{
	return { (unsigned char)(r + color.r),(unsigned char)(g + color.g ),(unsigned char)(b + color.b) ,a + color.a };
}
Color Color::operator-(const Color& color)
{
	return { (unsigned char)(r - color.r),(unsigned char)(g - color.g),(unsigned char)(b -color.b) ,a - color.a };
}
Color Color::operator*(const Color& color)
{
	return { (unsigned char)(r * color.r),(unsigned char)(g * color.g),(unsigned char)(b * color.b) ,a * color.a };
}
Color Color::operator/(const Color& color)
{
	return { (unsigned char)(r / color.r),(unsigned char)(g / color.g),(unsigned char)(b / color.b) ,a / color.a };
}



	/*
Color Color::operator+=(Color color) {}
Color Color::operator-=(Color color) {}
Color Color::operator*=(Color color) {}
Color Color::operator/=(Color color) {}*/


void Color::AlphaClamp()
{
	if (this->a > 100.0f)this->a = 100.0f;
	if (this->a < 0.0f)this->a = 0.0f;
}
