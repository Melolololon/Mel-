#pragma once

struct Color
{
	//rgbaは型を統一しないといけないっぽい
	//charだとaがわかりにくいし、
	//floatだとrgbがわかりにくい。内部で255で割ればいいが、メモリの使用量が多い
	//Mapするとき、ColorじゃなくてXMFLOAT4にすれば、floatとUCHARを混合できる
	unsigned char r, g, b, a;
	
	Color operator+(Color color);
	Color operator-(Color color);
	Color operator*(Color color);
	Color operator/(Color color);

	/// <summary>
	/// パーセントを0～255の数値に変換します
	/// </summary>
	/// <param name="par">パーセント</param>
	/// <returns></returns>
	static unsigned char alphaChangePar(float par);

	//Color operator+=(Color color);
	//Color operator-=(Color color);
	//Color operator*=(Color color);
	//Color operator/=(Color color);
};