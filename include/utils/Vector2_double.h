#ifndef Vector2_double_h
#define Vector2_double_h

#include <iostream>

//Make this a Vector template with class
class Vector2_double {
private:
	double x = 0.;
	double y = 0.;

public:
	Vector2_double();
	Vector2_double(double ix, double iy);

	double getX() const { return x; }
	double getY() const { return y; }

	void setX(double ix) { x = ix; }
	void setY(double iy) { y = iy; }

	Vector2_double& Add(const Vector2_double& v);
	Vector2_double& Subtract(const Vector2_double& v);
	Vector2_double& Multiply(const Vector2_double& v);
	Vector2_double& Divide(const Vector2_double& v);

	friend Vector2_double& operator+(Vector2_double& v1, const Vector2_double& v2);
	friend Vector2_double& operator-(Vector2_double& v1, const Vector2_double& v2);
	friend Vector2_double& operator*(Vector2_double& v1, const Vector2_double& v2);
	friend Vector2_double& operator/(Vector2_double& v1, const Vector2_double& v2);

	Vector2_double& operator+=(const Vector2_double& v);
	Vector2_double& operator-=(const Vector2_double& v);
	Vector2_double& operator*=(const Vector2_double& v);
	Vector2_double& operator/=(const Vector2_double& v);

	friend std::ostream& operator<<(std::ostream& stream, const Vector2_double& v);
};

#endif // Vector2_double_h
