#ifndef ANIMATION_HANDLER_HPP
#define ANIMATION_HANDLER_HPP

#include <SFML/Graphics.hpp>
#include <vector>

class Animation {
	
public:
	
	unsigned int startFrame;
	unsigned int endFrame;
	float duration;

	Animation(unsigned int startFrame, unsigned int endFrame, float duration) {
		
		this->startFrame = startFrame;
		this->endFrame = endFrame;
		this->duration = duration;
	}

	unsigned int getLength() {
		return endFrame - startFrame + 1;
	}
};

class AnimationHandler {
	
private:

	std::vector<Animation> animations;

	float current;
	int index;

public:

	void addAnimation(Animation& animation);
	void setAnimation(unsigned int index);
	void update(const float dt);

	sf::IntRect bounds;
	sf::IntRect frameSize;

	AnimationHandler() {
		
		this->current = 0.0f;
		this->index = -1;
	}

	AnimationHandler(const sf::IntRect& frameSize) {

		this->frameSize = frameSize;
		this->current = 0.0f;
		this->index = -1;
	}
};

#endif