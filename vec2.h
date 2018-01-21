#pragma once
#include <iostream>

template <class T>
class vec2
{
private:
	T x;
	T y;

public:
	//Constructors
	vec2(): x(0), y(0) {}
	vec2(T x_pos, T y_pos) : x(x_pos), y(y_pos) {}
	vec2(const vec2<T>& rhs) : x(rhs.x), y(rhs.y) {}
	vec2(vec2<T>&& rhs) : x(0), y(0) {
		x = rhs.x;
		y = rhs.y;
		rhs.x = 0;
		rhs.y = 0;
	}
	~vec2() {}

	unsigned magnitude() {
		return static_cast<unsigned>(std::sqrt(x * x + y * y));
	}


	//Operators
	vec2<T>& operator=(const vec2<T>& rhs) {
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	vec2<T>& operator=(vec2<T>&& rhs) noexcept {
		if (this != &rhs)
		{
			x = rhs.x;
			y = rhs.y;
			rhs.x = 0;
			rhs.y = 0;
		}
		return *this;
	}

	vec2<T>& operator+=(const vec2<T>& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	friend vec2<T> operator+(vec2<T> lhs, const vec2<T>& rhs) {
		lhs += rhs;
		return lhs;
	}

	vec2<T>& operator-=(const vec2<T>& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	friend vec2<T> operator-(vec2<T> lhs, const vec2<T>& rhs) {
		lhs -= rhs;
		return lhs;
	}

	vec2<T>& operator*=(int scale) {
		x *= scale;
		y *= scale;
		return *this;
	}

	friend vec2<T> operator*(vec2<T> lhs, int scale) {
		lhs *= scale;
		return lhs;
	}

	friend std::wostream& operator<<(std::wostream& os, const vec2<T>& vec) {
		os << "(" << vec.x << ", " << vec.y << ")";
		return os;
	}

	bool operator==(const vec2<T>& rhs) {
		return x - rhs.x < 0.001 && y - rhs.y < 0.001;
	}

	//Getters
	T getX() const {
		return x;
	}

	T getY() const {
		return y;
	}
};

