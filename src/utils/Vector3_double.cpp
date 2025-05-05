#include "utils/Vector3_double.h"


Vector3_double::Vector3_double() {}

Vector3_double::Vector3_double(double ix, double iy, double iz) {
	x = ix;
	y = iy;
	z = iz;
}

Vector3_double& Vector3_double::Add(const Vector3_double& v) {
	x += v.getX();
	y += v.getY();
	z += v.getZ();
	return *this;
}

Vector3_double& Vector3_double::Subtract(const Vector3_double& v) {
	x -= v.getX();
	y -= v.getY();
	z -= v.getZ();
	return *this;
}

Vector3_double& Vector3_double::Multiply(const Vector3_double& v) {
	x *= v.getX();
	y *= v.getY();
	z *= v.getZ();
	return *this;
}

Vector3_double& Vector3_double::Divide(const Vector3_double& v) {
	x /= v.getX();
	y /= v.getY();
	z /= v.getZ();
	return *this;
}

Vector3_double& operator+(Vector3_double& v1, const Vector3_double& v2) { return v1.Add(v2); }
Vector3_double& operator-(Vector3_double& v1, const Vector3_double& v2) { return v1.Subtract(v2); }
Vector3_double& operator*(Vector3_double& v1, const Vector3_double& v2) { return v1.Multiply(v2); }
Vector3_double& operator/(Vector3_double& v1, const Vector3_double& v2) { return v1.Divide(v2); }

Vector3_double& Vector3_double::operator+=(const Vector3_double& v) { return this->Add(v); }
Vector3_double& Vector3_double::operator-=(const Vector3_double& v) { return this->Subtract(v); }
Vector3_double& Vector3_double::operator*=(const Vector3_double& v) { return this->Multiply(v); }
Vector3_double& Vector3_double::operator/=(const Vector3_double& v) { return this->Divide(v); }

std::ostream& operator<<(std::ostream& stream, const Vector3_double& v) {
	stream << "(" << v.getX() << ", " << v.getY() << ", " << v.getZ() << ")";
	return stream;
}