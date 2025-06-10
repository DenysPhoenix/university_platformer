#pragma once
#include "SFML/Graphics.hpp"

class Animation
{
public:
	Animation(sf::Texture* texture, int imageCount, float switchTime);
	~Animation();

    void update(float deltaTime, bool faceRight);
    void reset();
    //sf::IntRect uvRect;
    

private:
    sf::Texture texture1;

    float wys, szer;
    int currentImage = 0;
    int imageCount;
    float totalTime = 0.0f;
    float switchTime;

    bool paused;
    bool finished;
    bool loop; 
    bool isPaused = false;
};




