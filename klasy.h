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
	tu umieœciæ trzeba wszystkie zmienne aby gra mog³a dzia³aæ
	*/
	sf::RenderWindow* window;
	sf::VideoMode videomode;
	sf::Event event;
	sf::View view1;

	// tu takie zasady dzia³ania gry 
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
	
	//inicjalizacja obiektów

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

	// wyœwietlanie gry
	void wyswietlPlayer();
	void wyswietlEnemies();
	void wyswietlPlatform();
	// wyœwietlenie wszystkiego
	void wyswietlenie();



};

