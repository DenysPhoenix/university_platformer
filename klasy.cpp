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
// wyœwietlanie


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







