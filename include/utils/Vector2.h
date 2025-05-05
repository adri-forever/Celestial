#ifndef Vector2_h
#define Vector2_h

#include <iostream>

//Make this a Vector template with class
template <typename T>
class Vector2 {
private:
	T x = T();
	T y = T();

public:
	Vector2() {}
	Vector2(T ix, T iy) : x(ix), y(iy) {
		x = ix;
		y = iy;
	}
	~Vector2() {};

	T getX() const { return x; }
	T getY() const { return y; }

	void setX(T ix) { x = ix; }
	void setY(T iy) { y = iy; }
	void set(T ix, T iy) {
		x = ix;
		y = iy;
	}

	Vector2& Add(const Vector2& v) {
		x += v.getX();
		y += v.getY();
		return *this;
	}
	Vector2& Subtract(const Vector2& v) {
		x -= v.getX();
		y -= v.getY();
		return *this;
	}
	Vector2& Multiply(const Vector2& v) {
		x *= v.getX();
		y *= v.getY();
		return *this;
	}
	Vector2& Divide(const Vector2& v) {
		x /= v.getX();
		y /= v.getY();
		return *this;
	}

	friend Vector2& operator+(Vector2& v1, const Vector2& v2) { return v1.Add(v2); }
	friend Vector2& operator-(Vector2& v1, const Vector2& v2) { return v1.Subtract(v2); }
	friend Vector2& operator*(Vector2& v1, const Vector2& v2) { return v1.Multiply(v2); }
	friend Vector2& operator/(Vector2& v1, const Vector2& v2) { return v1.Divide(v2); }

	Vector2& operator+=(const Vector2& v) { return this->Add(v); }
	Vector2& operator-=(const Vector2& v) { return this->Subtract(v); }
	Vector2& operator*=(const Vector2& v) { return this->Multiply(v); }
	Vector2& operator/=(const Vector2& v) { return this->Divide(v); }

	friend std::ostream& operator<<(std::ostream& stream, const Vector2& v) {
		stream << "(" << v.getX() << ", " << v.getY() << ")";
		return stream;
	}
};

#endif // Vector2_h
