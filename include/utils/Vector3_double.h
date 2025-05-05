#ifndef Vector3_double_h
#define Vector3_double_h

#include <iostream>

//Make this a Vector template with class
class Vector3_double {
private:
	double x = 0.;
	double y = 0.;
	double z = 0.;

public:
	Vector3_double();
	Vector3_double(double ix, double iy, double iz);

	double getX() const { return x; }
	double getY() const { return y; }
	double getZ() const { return z; }

	void setX(double ix) { x = ix; }
	void setY(double iy) { y = iy; }
	void setz(double iz) { z = iz; }
	void addX(double ix) { x += ix; }
	void addY(double iy) { y += iy; }
	void addz(double iz) { z += iz; }

	Vector3_double& Add(const Vector3_double& v);
	Vector3_double& Subtract(const Vector3_double& v);
	Vector3_double& Multiply(const Vector3_double& v);
	Vector3_double& Divide(const Vector3_double& v);

	friend Vector3_double& operator+(Vector3_double& v1, const Vector3_double& v2);
	friend Vector3_double& operator-(Vector3_double& v1, const Vector3_double& v2);
	friend Vector3_double& operator*(Vector3_double& v1, const Vector3_double& v2);
	friend Vector3_double& operator/(Vector3_double& v1, const Vector3_double& v2);

	Vector3_double& operator+=(const Vector3_double& v);
	Vector3_double& operator-=(const Vector3_double& v);
	Vector3_double& operator*=(const Vector3_double& v);
	Vector3_double& operator/=(const Vector3_double& v);

	friend std::ostream& operator<<(std::ostream& stream, const Vector3_double& v);
};

#endif // Vector3_double_h
