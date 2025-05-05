#ifndef TransformComponent_h
#define TransformComponent_h

#include <glm/glm.hpp>

#include "ECS.h"
#include "utils/Vector2_double.h"

class TransformComponent : public Component {
protected:
	/*Vector2_double position;
	Vector2_double velocity;

	int speed = 3;*/

public:
	/*TransformComponent() {
		position.setX(0);
		position.setY(0);
	}

	TransformComponent(double x, double y) {
		position.setX(x);
		position.setY(y);
	}
	~TransformComponent() = default;

	void init() override {
		velocity.setX(0);
		velocity.setY(0);
	}

	void update() override {
		position.setX(position.getX() + velocity.getX() * speed);
		position.setY(position.getY() + velocity.getY() * speed);
	}

	double getX() { return position.getX(); }
	double getY() { return position.getY(); }
	void setX(double x) { position.setX(x); }
	void setY(double y) { position.setY(y); }
	void setPos(double x, double y) {
		setX(x);
		setY(y);
	}
	void printPos() { std::cout << position << std::endl; }
	Vector2_double getPos() { return position; }

	double getVx() { return velocity.getX(); }
	double getVy() { return velocity.getY(); }
	void setVx(double vx) { velocity.setX(vx); }
	void setVy(double vy) { velocity.setY(vy); }
	void setVel(double vx, double vy) {
		setVx(vx);
		setVy(vy);
	}
	void printVel() { std::cout << velocity << std::endl; }
	Vector2_double getVel() { return velocity; }*/

	TransformComponent() = default;
	TransformComponent(double x, double y, double z) : position{ x,y,z } {}

	glm::vec3 position{ 0., 0., 0. };

};

#endif // !TransformComponent_h
