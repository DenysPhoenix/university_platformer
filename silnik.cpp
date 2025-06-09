#include "silnik.h"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>


// konstruktor i destruktor

Silnik::Silnik() {
	this->inicjalizacjaOkna();
	this->inicjalizacjaZmiennych();
}

Silnik::~Silnik() {
	delete this->window;
}

// funkcje prywatne

void Silnik::inicjalizacjaZmiennych()
{
	spawnPlayer();
	spawnPlatforms();
	spawnEnemy();
	statistics();
}

void Silnik::inicjalizacjaOkna()
{
	this->videomode.height = 1000;
	this->videomode.width = 1800;
	this->window = new sf::RenderWindow(this->videomode, "Edge of fantasy", sf::Style::Default);

	this->window->setFramerateLimit(60);

}


// spawny


void Silnik::spawnPlayer()
{
	player.setPosition(this->window->getSize().x / 8, this->window->getSize().y / 2);
	player.setScale(3, 3);
}

void Silnik::spawnEnemy()
{
}

void Silnik::spawnPlatforms()
{
	if (platforms.size() > 0)
	{
		platforms.clear();
	}
	sf::Sprite pl;
	float x1 = this->window->getSize().x;
	float y1 = this->window->getSize().y;
	if (poz_x == 2 && poz_y == 1)
	{
		for (size_t i = 0; i < 3; i++)
		{
			pl.setPosition(x1 / 9 * (1 + (i * 3)), y1 / 4 * 3);
			pl.setScale(4, 4);
			pl.setTextureRect(sf::IntRect(0, 9, 77, 83));
			this->platforms.push_back(pl);
		}
		pl.setPosition(x1 / 9 * 8, 0);
		pl.setTextureRect(sf::IntRect(160, 0, 30, 120));
		pl.setScale(4, 4);
		this->platforms.push_back(pl);
	}
	else if (poz_x == 3 && poz_y == 1)
	{
		for (size_t i = 0; i < 6; i++)
		{
			pl.setPosition(x1 / 9 * (1 + (i)), y1 / 4 * 3);
			pl.setScale(2, 2);
			pl.setTextureRect(sf::IntRect(0, 9, 77, 83));
			this->platforms.push_back(pl);
		}
	}
	
}

void Silnik::statistics()
{
	
}

//aktualizacje


const bool Silnik::czyokno()
{
	{
		return this->window->isOpen();
	}
}

void Silnik::aktualizacjaEvents()
{
	while (this->window->pollEvent(this->event))
	{
		switch (this->event.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->event.key.code == sf::Keyboard::Escape)
			{
				this->window->close();
			}
		}
	}
}

void Silnik::aktualizacjaPlayer()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))// && player.getPosition().y >= 0
	{
		player.move(0.f, -9.5f);

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))// && player.getPosition().y <= 550
	{
		player.move(0.f, 9.5f);

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))// && player.getPosition().x >= 0
	{
		player.move(-9.5f, 0.f);

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) )//&& player.getPosition().x <= 800
	{
		player.move(9.5f, 0.f);

	}
	if (player.getPosition().x > this->window->getSize().x)
	{
		poz_x += 1;
		this->inicjalizacjaZmiennych();
	}
	if (player.getPosition().y > this->window->getSize().y)
	{
		poz_y -= 1;
		this->inicjalizacjaZmiennych();
	}
	if (player.getPosition().x < 0)
	{
		poz_x -= 1;
		this->inicjalizacjaZmiennych();
	}
	if (player.getPosition().y < 0)
	{
		this->inicjalizacjaZmiennych();
		poz_y += 1;
	}
}

void Silnik::aktualizacjaEnemies()
{
}

void Silnik::aktualizacjaStatystyk()
{
	std::ostringstream ss;
	ss << "(" << poz_x << ", " << poz_y << ")";
	text.setString(ss.str());
}

void Silnik::aktualizacjaPlatform()
{
	

}

void Silnik::aktualizacja()
{
	this->aktualizacjaEvents();
	this->aktualizacjaPlayer();
	this->aktualizacjaEnemies();
	this->aktualizacjaStatystyk();
	this->aktualizacjaPlatform();
	//std::cout << "mango" << std::endl;
}
// wyswietlanie


void Silnik::wyswietlPlayer()
{
	sf::Texture texture;
	texture.loadFromFile("idle_knight_1.png");
	player.setTexture(texture);
	this->window->draw(player);
}

void Silnik::wyswietlEnemies()
{
}

void Silnik::wyswietlPlatform()
{
	sf::Font font;
	font.loadFromFile("arial.ttf");
	text.setFont(font);
	text.setPosition(0, 0);
	this->window->draw(text);
	sf::Texture texture;
	texture.loadFromFile("Tiles.png");
	for (auto pl : this->platforms)
	{
		pl.setTexture(texture);
		this->window->draw(pl);

	}
}

void Silnik::wyswietlenie()
{
	this->window->clear();
	this->wyswietlEnemies();
	this->wyswietlPlatform();
	this->wyswietlPlayer();
	this->window->display();
	//std::cout << "polo" << std::endl;

}



// ***************************************************************************************************************************





MaszynaStanow::MaszynaStanow()
{
	 
}

MaszynaStanow::~MaszynaStanow()
{
}

bool MaszynaStanow::czy(int a,int b)
{
	if (a == 2 && b == 1)
	{

	}
	return false;
}




Poziom::Poziom(sf::Sprite p1, std::vector<sf::Sprite> v2, std::vector<sf::Sprite> v3)
{
	player = p1;
	enemy = v2;
	platform = v3;
}

Poziom::~Poziom()
{
}