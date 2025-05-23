#pragma once

#include "SFML/Graphics.hpp"
#include<vector>
#include<iostream>
#include <string>
#include "klasy.cpp"
#include "SFML\Window.hpp"
#include "SFML\System.hpp"

class Engine
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
	
	//inicjalizacja obiektуw

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

	// wyswietlanie gry
	void wyswietlPlayer();
	void wyswietlEnemies();
	void wyswietlPlatform();
	// wyswietlenie wszystkiego
	void wyswietlenie();



};

bool checkCollision(const sf::FloatRect& a, const sf::FloatRect& b)
{
	return a.intersects(b);
}

class Entity :public sf::Sprite
{
protected:
	//podstawowe wlasnosci obiektow
	sf::Texture texture;
	vector<sf::IntRect> frames;
	int fps;

	//wlasnosci do korzystania
	sf::Vector2f pos;

public:
	Entity(int animation_fps = 0, sf::Texture tex): fps(animation_fps), texture(tex) {};

	//funckja animacji entity
	virtual void animation();
	virtual ~Entity() {};
};

class Player :public Entity
{
private:
	float hp;
public:
	Player() {};
	~Player() {};
};

class Enemy :public Entity
{
private:
	float hp;
public:
	Enemy(float hp) {};
	~Enemy() {};
};

class Platform :public Entity
{
private:

public:
	Platform(const sf::Vector2f size, const sf::Vector2f position) {};
	~Platform() {};
};