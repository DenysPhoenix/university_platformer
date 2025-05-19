#pragma once

#include "SFML/Graphics.hpp"
#include<vector>
#include<iostream>
#include <string>
#include "SFML\Window.hpp"
#include "SFML\System.hpp"

class Engine
{
private:
	/*
	tu umie�ci� trzeba wszystkie zmienne aby gra mog�a dzia�a�
	*/
	sf::RenderWindow* window;
	sf::VideoMode videomode;
	sf::Event event;
	sf::View view1;

	// tu takie zasady dzia�ania gry 
	bool playerAlive;
	int maxEnemies;
	float enemySpawnTimer;
	float enemySpawnTimerMAX;

	// obiekty gry 
	sf::Sprite player;
	std::vector<sf::Sprite> enemies;
	sf::Text text;


	// prywatne funkcje 
	void inicjalizacjaZmiennych();
	void inicjalizacjaOkna();
public:
	Engine();
	~Engine();
	
	//inicjalizacja obiekt�w

	void spawnPlayer();
	void spawnEnemy();
	void spawnPlatforms();
	void statistics();


	// blok aktualizacji

	void aktualizacjaEvents();
	void aktualizacjaPlayer();
	void aktualizacjaEnemies();
	void aktualizacjaStatystyk();
	void aktualizacjaPlatform();
	//aktualizacja wszystkiego
	void aktualizacja();

	// wy�wietlanie gry
	void wyswietlPlayer();
	void wyswietlEnemies();
	void wyswietlPlatform();
	// wy�wietlenie wszystkiego
	void wyswietlenie();



};

