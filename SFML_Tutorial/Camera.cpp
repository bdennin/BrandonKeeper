#include <SFML\Graphics.hpp>
#include <math.h>
#include "Camera.h"

Camera::Camera(int w, int h, float speed) {
	
	size.x = w;
	size.y = h;

	if (speed < 0.0)
		speed = 0.0;
	if (speed > 1.0)
		speed = 1.0;

	this->speed = speed;
}

Camera::~Camera() {

}

void Camera::move(int x, int y) {
	
	current.x = (float)x;
	current.y = (float)y;
	target.x = (float)x;
	target.y = (float)y;
}

void Camera::moveCenter(int x, int y) {

	x = x - (size.x / 2);
	y = y - (size.y / 2);

	move(x, y);
}

void Camera::goToPosition(int x, int y) {
	
	target.x = (float)x;
	target.y = (float)y;
}

void Camera::goToCenter(int x, int y) {
	
	x = x - (size.x / 2);
	y = y - (size.y / 2);

	goToPosition(x, y);
}

void Camera::update() {

	float x, y, d, vx, vy, v;

	x = (float)(target.x - current.x);
	y = (float)(target.y - current.y);

	if ((x * x + y * y) <= 1) {
		current.x = target.x;
		current.y = current.y;
	}
	else {
		d = sqrt((x * x + y * y));
		v = (d * speed) / 60;

		if (v < 1.0f)
			v = 1.0f;

		vx = x * (v / d);
		vy = y * (v / d);

		current.x += vx;
		current.y += vy;
	}
}

sf::IntRect Camera::getTileBounds(int tileSize) {
	
	int x = (int)(current.x / tileSize);
	int y = (int)(current.y / tileSize);

	int w = (int)(size.x / tileSize + 2);
	int h = (int)(size.y / tileSize + 2);

	if (x % tileSize != 0)
		w++;
	if (y % tileSize != 0)
		h++;

	return sf::IntRect(x, y, w, h);
}