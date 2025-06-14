#pragma once
#include "SFML/Graphics.hpp"
#include "klasy.h"

class Silnik
{
private:
	
	//tu umiescic trzeba wszystkie zmienne aby gra mogla dzialac
	
	sf::RenderWindow* window;
	sf::VideoMode videomode;
	sf::Event event;
	sf::View view1;
	sf::Color* tlo;
	sf::Clock clock;

	// tu takie zasady dzialania gry 
	bool playerAlive;
	int maxEnemies;
	float enemySpawnTimer;
	float enemySpawnTimerMAX;
	float dt;
	int poz_x = 2;
	int poz_y = 1;

	// obiekty gry 
	Player* player;
	//std::vector<Bee> enemies;
	std::vector<sf::Sprite> backgrounds;
	std::vector<Platform> platforms;
	sf::Text text;


	// prywatne funkcje 
	void inicjalizacjaZmiennych();
	void inicjalizacjaOkna();
public:
	Silnik();
	~Silnik();

	//inicjalizacja obiektуw

	void spawnPlayer();
	void spawnEnemy();
	void spawnBackground();
	void spawnPlatforms();
	void statistics();


	// blok aktualizacji
	const bool czyokno();

	void aktualizacjaEvents();
	void aktualizacjaPlayer();
	void aktualizacjaEnemies();
	void aktualizacjaStatystyk();
	void aktualizacjaPlatform();
	//aktualizacja wszystkiego
	void aktualizacja();

	// wyswietlanie gry
	void wyswietlPlayer();
	void wyswietlEnemies();
	void wyswietlPlatform();
	void wyswietlBackground();
	// wyswietlenie wszystkiego
	void wyswietlenie();



};



class MaszynaStanow
{
public:
	MaszynaStanow();
	~MaszynaStanow();
	bool czy(int a ,int b);

private:

};

class Poziom
{
public:
	Poziom(sf::Sprite p1, std::vector<sf::Sprite> v2, std::vector<sf::Sprite> v3);
	~Poziom();

	

private:
	sf::Sprite player;
	std::vector<sf::Sprite> enemy;
	std::vector<sf::Sprite> platform;
};





