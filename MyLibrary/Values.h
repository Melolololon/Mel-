#pragma once
//ílÇï°êîÇ‹Ç∆ÇﬂÇΩÇ‡ÇÃ

template<class V>
struct Value2
{
	V v1, v2;

	Value2(const V v1, const V v2) :v1(v1), v2(v2)
	{}

	Value2<V> operator+(const Value2<V> value) { return Value2(v1 + value.v1, v2 + value.v2); }
	Value2<V> operator-(const Value2<V> value) { return Value2(v1 - value.v1, v2 - value.v2); }
	Value2<V> operator*(const Value2<V> value) { return Value2(v1 * value.v1, v2 * value.v2); }
	Value2<V> operator/(const Value2<V> value) { return Value2(v1 / value.v1, v2 / value.v2); }
	
	void operator=(const Value2<V> value)
	{
		v1 = value.v1;
		v2 = value.v2;
	}


};


template<class V>
struct Value3
{
	V v1, v2, v3;
	Value3(V v1, V v2) :v1(v1), v2(v2), v3(v3) {}

	Value3<V> operator+(const Value3<V> value) { return Value2(v1 + value.v1, v2 + value.v2,v3 + value.v3); }
	Value3<V> operator-(const Value3<V> value) { return Value2(v1 - value.v1, v2 - value.v2,v3 - value.v3); }
	Value3<V> operator*(const Value3<V> value) { return Value2(v1 * value.v1, v2 * value.v2,v3 * value.v3); }
	Value3<V> operator/(const Value3<V> value) { return Value2(v1 / value.v1, v2 / value.v2,v3 / value.v3); }

	void operator=(const Value3<V> value)
	{
		v1 = value.v1;
		v2 = value.v2;
		v3 = value.v3;
	}
};


template<class V>
struct Value4
{
	V v1, v2, v3, v4;
	Value4(V v1, V v2) :v1(v1), v2(v2), v3(v3), v4(v4) {}

	Value4<V> operator+(const Value4<V> value) { return Value2(v1 + value.v1, v2 + value.v2, v3 + value.v3,v4 + value.v4); }
	Value4<V> operator-(const Value4<V> value) { return Value2(v1 - value.v1, v2 - value.v2, v3 - value.v3,v4 - value.v4); }
	Value4<V> operator*(const Value4<V> value) { return Value2(v1 * value.v1, v2 * value.v2, v3 * value.v3,v4 * value.v4); }
	Value4<V> operator/(const Value4<V> value) { return Value2(v1 / value.v1, v2 / value.v2, v3 / value.v3,v4 / value.v4); }

	void operator=(const Value4<V> value)
	{
		v1 = value.v1;
		v2 = value.v2;
		v3 = value.v3;
		v4 = value.v4;
	}
};

