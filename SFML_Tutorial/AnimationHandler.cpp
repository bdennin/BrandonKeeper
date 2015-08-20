#include <SFML/Graphics.hpp>
#include <vector>
#include "AnimationHandler.hpp"

void AnimationHandler::update(const float dt) {

	if (index >= this->animations.size() || index < 0)
		return;

	float duration = this->animations[index].duration;

	if (int((current + dt) / duration) > int(current / duration)) {

		int frame = int((current + dt) / duration);
		frame %= this->animations[index].getLength();

		sf::IntRect rectangle = this->frameSize;
		rectangle.left = rectangle.width * frame;
		rectangle.top = rectangle.height * frame;
		this->bounds = rectangle;
	}

	this->current += dt;

	if (this->current > duration * this->animations[index].getLength())
		this->current = 0.0f;
}

void AnimationHandler::addAnimation(Animation& animation) {
	this->animations.push_back(animation);
}

void AnimationHandler::setAnimation(unsigned int index) {

	if (this->index == index || index >= this->animations.size() || index < 0)
		return;

	this->index = index;
	
	sf::IntRect rectangle = this->frameSize;
	rectangle.top = rectangle.height * index;
	this->bounds = rectangle;
	this->current = 0.0f;
}