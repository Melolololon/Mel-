#pragma once

//•âŠÔƒNƒ‰ƒX
class Interpolation
{


public:
	template<class VALUE>
	static VALUE Lerp(const VALUE start,const VALUE end , const float t)
	{
		return start * (1.0f - t) + end * t;
	}

};

