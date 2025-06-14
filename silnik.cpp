
#include "silnik.h"
#include "klasy.h"
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
	spawnBackground();
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
	sf::Texture heroIdle, heroRun, heroJump, heroAttack, heroDead;
    heroIdle.loadFromFile("grafiki/hero-Idle.png");
    heroRun.loadFromFile("grafiki/hero-Run.png");
    heroJump.loadFromFile("grafiki/hero-Jump.png");
    heroAttack.loadFromFile("grafiki/hero-Attack.png");
    heroDead.loadFromFile("grafiki/hero-Dead.png");

    vector<sf::Texture*> player_textures;
    player_textures.emplace_back(&heroIdle);
    player_textures.emplace_back(&heroRun);
    player_textures.emplace_back(&heroJump);
    player_textures.emplace_back(&heroAttack);
    player_textures.emplace_back(&heroDead);
	player = new Player(player_textures, sf::Vector2f(400.f, 325.f));
	player->setPosition(this->window->getSize().x / 8, this->window->getSize().y / 2);
}

void Silnik::spawnEnemy()
{
	//Bee przeciwnik(sf::Vector2f(window->getSize().x * 0.9 , 0));
	//enemies.push_back(przeciwnik);
}

void Silnik::spawnBackground()
{
	backgrounds.clear();
	sf::Sprite pl;
	float x1 = this->window->getSize().x;
	float y1 = this->window->getSize().y;
	if (poz_y == 1)
	{
		pl.setTextureRect(sf::IntRect(100, 300, 40, 34));
		pl.setPosition(0, y1 * 0.9);
		pl.setScale(100, 3);
		this->backgrounds.push_back(pl);
	}
	if (poz_x == 2 && poz_y == 1)
	{
		for (size_t i = 0; i < 3; i++)
		{
			pl.setPosition(x1 / 9 * (1 + (i * 3)), y1 / 4 * 3);
			pl.setScale(4, 4);
			pl.setTextureRect(sf::IntRect(0, 9, 77, 83));
			this->backgrounds.push_back(pl);
		}
		pl.setPosition(x1 / 9 * 8, 0);
		pl.setTextureRect(sf::IntRect(160, 0, 30, 120));
		pl.setScale(4, 4);
		this->backgrounds.push_back(pl);
		
	}
	else if (poz_x == 3 && poz_y == 1)
	{
		for (size_t i = 0; i < 6; i++)
		{
			pl.setPosition(x1 / 9 * (1 + (i)), y1 / 4 * 3);
			pl.setScale(2, 2);
			pl.setTextureRect(sf::IntRect(0, 9, 77, 83));
			this->backgrounds.push_back(pl);
		}
	}
	
}

void Silnik::spawnPlatforms()
{
	sf::Texture staticTex;
	staticTex.loadFromFile("grafiki/static.png");
	for (int i = 0; i < 10; i++)
	{
		platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(i * 100.f,window->getSize().y * 0.9), PlatformType::Static));
	}
	
    //platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(200.f, 400.f), PlatformType::Static));
   
}

void Silnik::statistics()
{
	std::ostringstream ss;
	ss << "(" << poz_x << ", " << poz_y << ")";
	text.setString(ss.str());
	if (poz_y == 1)
	{
		this->tlo = new sf::Color(255, 186, 0, 0);
	}
	else if (poz_y == 2)
	{
		this->tlo = new sf::Color(255, 217, 0, 0);
	}
	else if (poz_y == 0)
	{
		this->tlo = new sf::Color(39, 127, 163, 0);
	}
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
	player->update(dt);
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))// && player.getPosition().y >= 0
	//{
	//	player->move(0.f, -9.5f);

	//}
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))// && player.getPosition().y <= 550
	//{
	//	player->move(0.f, 9.5f);

	//}
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))// && player.getPosition().x >= 0
	//{
	//	player->move(-9.5f, 0.f);

	//}
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) )//&& player.getPosition().x <= 800
	//{
	//	player->move(9.5f, 0.f);

	//}
	for (Platform& platform : platforms)
	{
		if (player->GetCollider().intersects(platform.GetCollider()))
		{
			player->OnCollision(platform, dt);
			// Если платформа должна реагировать на игрока, то раскомментируйте:
			// platform.OnCollision(hero, dt);
		}
	}
	if (player->getPosition().x > this->window->getSize().x)
	{
		poz_x += 1;
		this->inicjalizacjaZmiennych();
	}
	if (player->getPosition().y > this->window->getSize().y)
	{
		poz_y -= 1;
		this->inicjalizacjaZmiennych();
	}
	if (player->getPosition().x < 0)
	{
		poz_x -= 1;
		this->inicjalizacjaZmiennych();
	}
	if (player->getPosition().y < 0)
	{
		poz_y += 1;
		this->inicjalizacjaZmiennych();
	}
}

void Silnik::aktualizacjaEnemies()
{
	

        // 2) enemies and platforms
        //for (std::unique_ptr<Enemy>& enemyPtr : enemies)
        //{
        //    Enemy& enemy = *enemyPtr;
        //    for (Platform& platform : platforms)
        //    {
        //        if (enemy.GetCollider().intersects(platform.GetCollider()))
        //        {
        //            enemy.OnCollision(platform, dt);
        //            // Если платформа должна реагировать на врага, то раскомментируйте:
        //            // platform.OnCollision(enemy, dt);
        //        }
        //    }
        //}

        // 3) hero and enemies
        //for (std::unique_ptr<Enemy>& enemyPtr : enemies)
        //{
        //    Enemy& enemy = *enemyPtr;
        //    if (hero.GetCollider().intersects(enemy.GetCollider()))
        //    {
        //        // Игрок атакует врага, или враг атакует игрока
        //        hero.OnCollision(enemy, dt);
        //        enemy.OnCollision(hero, dt);
        //    }
        //}

        //hero.ResetAttackFlag();



       /* enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
            [](const std::unique_ptr<Enemy>& enemy) {
                return enemy->GetHP() <= 0.0f;
            }),
            enemies.end());*/
}

void Silnik::aktualizacjaStatystyk()
{
	
}

void Silnik::aktualizacjaPlatform()
{
	

}

void Silnik::aktualizacja()
{
	dt = clock.restart().asSeconds();
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
	//sf::Texture texture;
	//texture.loadFromFile("idle_knight_1.png");
	sf::Texture hero;
	switch (player->GetCurrentState())
	{
	case EntityState::Idle:
		hero.loadFromFile("grafiki/hero-Idle.png");
		break;
	case EntityState::Running:
		hero.loadFromFile("grafiki/hero-Run.png");
		break;
	case EntityState::Jumping:
		hero.loadFromFile("grafiki/hero-Jump.png");
		break;
	case EntityState::Attacking:
		hero.loadFromFile("grafiki/hero-Attack.png");
		break;
	case EntityState::Dying:
		hero.loadFromFile("grafiki/hero-Dead.png");
		break;
	}
	//hero.loadFromFile("grafiki/hero-Idle.png");
	player->setTexture(hero);
	player->setTextures(dt);
	this->window->draw(*player);
}

void Silnik::wyswietlEnemies()
{
}

void Silnik::wyswietlPlatform()
{
	sf::Texture texture;
	texture.loadFromFile("grafiki/static.png");
	for (Platform& platform : platforms)
	{
		platform.setTexture(texture);
		window->draw(platform);
	}
}

void Silnik::wyswietlBackground()
{
	
	sf::Font font;
	font.loadFromFile("arial.ttf");
	text.setFont(font);
	text.setPosition(0, 0);
	this->window->draw(text);

	sf::Texture texture;
	texture.loadFromFile("Tiles.png");
	for (auto pl : backgrounds)
	{
		pl.setTexture(texture);
		this->window->draw(pl);

	}
}

void Silnik::wyswietlenie()
{
	sf::Color kolor1 = *tlo;
	this->window->clear(kolor1);
	wyswietlBackground();
	this->wyswietlEnemies();
	this->wyswietlPlatform();
	this->wyswietlPlayer();
	this->window->display();

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
