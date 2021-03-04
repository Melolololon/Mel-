#include"Color.h"

Color Color::operator+(Color color) 
{
	return { (unsigned char)(r + color.r),(unsigned char)(g + color.g ),(unsigned char)(b + color.b) ,(unsigned char)(a + color.a) };
}
Color Color::operator-(Color color) 
{
	return { (unsigned char)(r - color.r),(unsigned char)(g - color.g),(unsigned char)(b -color.b) ,(unsigned char)(a -color.a) };
}
Color Color::operator*(Color color) 
{
	return { (unsigned char)(r * color.r),(unsigned char)(g * color.g),(unsigned char)(b * color.b) ,(unsigned char)(a * color.a) };
}
Color Color::operator/(Color color) 
{
	return { (unsigned char)(r / color.r),(unsigned char)(g / color.g),(unsigned char)(b / color.b) ,(unsigned char)(a / color.a) };
}

unsigned char Color::alphaChangePar(float par)
{
	return (unsigned char)(par / 100 * 255);
}

	/*
Color Color::operator+=(Color color) {}
Color Color::operator-=(Color color) {}
Color Color::operator*=(Color color) {}
Color Color::operator/=(Color color) {}*/