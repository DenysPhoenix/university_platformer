#include "SFML/Graphics.hpp"
#include <iostream>
#include <cmath>
#include <string>
#include "klasy.h"


// konstruktor i destruktor

Engine::Engine() {
	this->inicjalizacjaOkna();
	this->inicjalizacjaZmiennych();
}

Engine::~Engine() {
	delete this->window;
}

// funkcje prywatne

void Engine::inicjalizacjaZmiennych()
{
}

void Engine::inicjalizacjaOkna()
{
}


// spawny


void Engine::spawnPlayer()
{
}

void Engine::spawnEnemy()
{
}

void Engine::spawnPlatforms()
{
}

void Engine::statistics()
{
}

//aktualizacje


void Engine::aktualizacjaEvents()
{
}

void Engine::aktualizacjaPlayer()
{
}

void Engine::aktualizacjaEnemies()
{
}

void Engine::aktualizacjaStatystyk()
{
}

void Engine::aktualizacjaPlatform()
{
}

void Engine::aktualizacja()
{
}
// wyswietlanie


void Engine::wyswietlPlayer()
{
}

void Engine::wyswietlEnemies()
{
}

void Engine::wyswietlPlatform()
{
}

void Engine::wyswietlenie()
{
}

/////////////////////////////////////////////////////////////////
///////////////	INICJALIZACJA ELEMENTOW KLAS ////////////////////
/////////////////////////////////////////////////////////////////

Entity::Entity(int animation_fps = 0, sf::Texture tex)
{

}

Entity::~Entity()
{

}

void Entity::animation()
{
     for (int i = 0; i <= 150; i+50)
     {
         //obcinanie tekstury z jednego obrazka
         frames.push_back(sf::IntRect(200 + i, 0, 37, 37)); //można zmienic teksturę
         if (frames.size() == 1) 
         {
             setTextureRect(frames[0]);
         }
     }
}


//
//Player::Player()
//{
//
//}
//
//Player::~Player()
//{
//
//}
//
//Enemy::Enemy()
//{
//
//}
//
//Enemy::~Enemy()
//{
//
//}
//
//Platform::Platform()
//{
//	setPosition(position);
//}
//
//Platform::~Platform()
//{
//
//}
//
//


