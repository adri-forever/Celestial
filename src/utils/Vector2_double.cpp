#include "utils/Vector2_double.h"

Vector2_double::Vector2_double() {}

Vector2_double::Vector2_double(double ix, double iy) {
	x = ix;
	y = iy;
}

Vector2_double& Vector2_double::Add(const Vector2_double& v) {
	x += v.getX();
	y += v.getY();
	return *this;
}

Vector2_double& Vector2_double::Subtract(const Vector2_double& v) {
	x -= v.getX();
	y -= v.getY();
	return *this;
}

Vector2_double& Vector2_double::Multiply(const Vector2_double& v) {
	x *= v.getX();
	y *= v.getY();
	return *this;
}

Vector2_double& Vector2_double::Divide(const Vector2_double& v) {
	x /= v.getX();
	y /= v.getY();
	return *this;
}


Vector2_double& operator+(Vector2_double& v1, const Vector2_double& v2) { return v1.Add(v2); }
Vector2_double& operator-(Vector2_double& v1, const Vector2_double& v2) { return v1.Subtract(v2); }
Vector2_double& operator*(Vector2_double& v1, const Vector2_double& v2) { return v1.Multiply(v2); }
Vector2_double& operator/(Vector2_double& v1, const Vector2_double& v2) { return v1.Divide(v2); }

Vector2_double& Vector2_double::operator+=(const Vector2_double& v) { return this->Add(v); }
Vector2_double& Vector2_double::operator-=(const Vector2_double& v) { return this->Subtract(v); }
Vector2_double& Vector2_double::operator*=(const Vector2_double& v) { return this->Multiply(v); }
Vector2_double& Vector2_double::operator/=(const Vector2_double& v) { return this->Divide(v); }

std::ostream& operator<<(std::ostream& stream, const Vector2_double& v) {
	stream << "(" << v.getX() << ", " << v.getY() << ")";
	return stream;
}