#pragma once
#include "SFML/Graphics.hpp"


class Silnik
{
private:
	/*
	tu umiescic trzeba wszystkie zmienne aby gra mogla dzialac
	*/
	sf::RenderWindow* window;
	sf::VideoMode videomode;
	sf::Event event;
	sf::View view1;

	// tu takie zasady dzialania gry 
	bool playerAlive;
	int maxEnemies;
	float enemySpawnTimer;
	float enemySpawnTimerMAX;
	int poz_x = 2;
	int poz_y = 1;

	// obiekty gry 
	sf::Sprite player;
	std::vector<sf::Sprite> enemies;
	std::vector<sf::Sprite> platforms;
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





