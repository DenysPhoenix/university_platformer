#pragma once
#include "SFML/Graphics.hpp"
#include "klasy.h"

class Button
{
private:

	sf::RectangleShape shape;
	sf::Text text;
	sf::Font font;

public:

	Button(const sf::Vector2f& position, const sf::Vector2f& size, const string& label);
	void draw(sf::RenderWindow& window);
	bool isClicked(const sf::Vector2i& mousePosition, sf::RenderWindow& window);
};

enum GameState {
	MENU,
	PLAYING
};

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

	//obsluga gracza

	sf::Texture heroIdle, heroRun, heroJump, heroAttack, heroDead, heroPushDown;
	vector<sf::Texture*> player_textures;
	sf::SoundBuffer PlayerRun, PlayerJump, PlayerAttack, PlayerDie;
	vector<sf::SoundBuffer*> player_sounds;
	sf::RectangleShape hitboxPlayer;

	// obsługa mapy
	sf::Sprite mapa;
	sf::RectangleShape shape1;

	//obsluga Enemy

	//obsluga dzika
	sf::SoundBuffer bufferBoar;
	vector<sf::SoundBuffer*> boar_sounds;
	sf::Texture boarIdle, boarWalk, boarRun, boarHit;
	vector<sf::Texture*> boar_textures;
	vector<unique_ptr<Enemy>> enemies;

	//obsluga osy
	sf::SoundBuffer beeBuzzing;
	sf::Texture beeFly, beeAttack, beeHit;
	vector<sf::Texture*> bee_textures;

	// tu takie zasady dzialania gry 
	bool playerAlive;
	int maxEnemies;
	float enemySpawnTimer;
	float enemySpawnTimerMAX;
	float dt;
	int poz_x = 0;
	int poz_y = 0;
	int orient_x = 240;
	int orient_y = 270;

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

	//inicjalizacja obiektow
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
	void aktualizacja();

	// wyswietlanie gry
	void wyswietlPlayer();
	void wyswietlEnemies();
	void wyswietlPlatform();
	void wyswietlBackground();
	void wyswietlenie();
};

class MaszynaStanow
{
public:
	MaszynaStanow();
	~MaszynaStanow();
	bool czy(int a, int b);

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



