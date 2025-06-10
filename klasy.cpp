#include "klasy.h"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>

Animation::Animation(sf::Texture* texture, int imageCount, float switchTime)
{
    texture1;
    szer = static_cast<int>((texture->getSize().x) / static_cast<float>(imageCount));
    wys = static_cast<int>(texture->getSize().y);
}

Animation::~Animation()
{
}

void Animation::update(float deltaTime, bool faceRight)
{
    if (!isPaused) {
        //realizacja animacji
        totalTime += deltaTime;

        if (totalTime >= switchTime)
        {
            totalTime -= switchTime;
            currentImage++;

            if (currentImage >= imageCount)
            {
                currentImage = 0;
                finished = true;
            }
        }
    }
}









