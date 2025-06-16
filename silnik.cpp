
#include "silnik.h"
#include "klasy.h"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <algorithm>


Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, const string& label)
{
	shape.setPosition(position);
	shape.setSize(size);
	shape.setFillColor(sf::Color(255, 186, 0, 0));//(sf::Color(255, 255, 255));
	//shape.setOutlineColor(sf::Color::Blue);
	//shape.setOutlineThickness(2.f);
	font.loadFromFile("Super-Mario-Font.ttf");


	text.setFont(font);
	text.setString(label);
	text.setCharacterSize(36);
	text.setFillColor(sf::Color::White);
	sf::FloatRect shapeBounds = shape.getGlobalBounds();
	sf::FloatRect textBounds = text.getLocalBounds();

	float x = shapeBounds.left + (shapeBounds.width - textBounds.width) / 2.f;
	float y = shapeBounds.top + (shapeBounds.height - textBounds.height) / 2.f;

	// textBounds.left/top > 0
	x -= textBounds.left;
	y -= textBounds.top;

	text.setPosition(x, y);
}

void Button::draw(sf::RenderWindow& window)
{
	window.draw(shape);

	window.draw(text);
}

bool Button::isClicked(const sf::Vector2i& mousePosition, sf::RenderWindow& window)
{
	sf::Vector2f worldPos = window.mapPixelToCoords(mousePosition);
	return shape.getGlobalBounds().contains(worldPos.x, worldPos.y);
}


// konstruktor i destruktor

Silnik::Silnik() {
	this->inicjalizacjaOkna();
	this->inicjalizacjaZmiennych();
	if (!PlayerRun.loadFromFile("soundRunHero.mp3")) {
		std::cerr << "ERROR: Failed to load soundRunHero.mp3" << std::endl;
	}
	if (!PlayerJump.loadFromFile("soundJump.mp3")) {
		std::cerr << "ERROR: Failed to load soundJump.mp3" << std::endl;
	}
	if (!PlayerAttack.loadFromFile("soundAttackHero.mp3")) {
		std::cerr << "ERROR: Failed to load soundAttackHero.mp3" << std::endl;
	}
	if (!PlayerDie.loadFromFile("soundDieHero.mp3")) {
		std::cerr << "ERROR: Failed to load soundDieHero.mp3" << std::endl;
	}
}

Silnik::~Silnik() {
	delete this->window;
	delete this->player;

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
	this->videomode.height = 1080;
	this->videomode.width = 1920;
	this->window = new sf::RenderWindow(this->videomode, "Moon walker", sf::Style::Default);

	this->window->setFramerateLimit(60);

}


// spawny


void Silnik::spawnPlayer()
{
	//tektury
	//sf::Texture heroIdle, heroRun, heroJump, heroAttack, heroDead, heroPushDown;
	heroIdle.loadFromFile("hero-Idle.png");
	heroRun.loadFromFile("hero-Run.png");
	heroJump.loadFromFile("hero-Jump.png");
	heroAttack.loadFromFile("hero-Attack.png");
	heroDead.loadFromFile("hero-Dead.png");
	heroPushDown.loadFromFile("hero-PushDown.png");

	//vector<sf::Texture*> player_textures;
	player_textures.emplace_back(&heroIdle);
	player_textures.emplace_back(&heroRun);
	player_textures.emplace_back(&heroJump);
	player_textures.emplace_back(&heroAttack);
	player_textures.emplace_back(&heroDead);
	player_textures.emplace_back(&heroPushDown);

	//dzwieki
	//sf::SoundBuffer PlayerRun, PlayerJump, PlayerAttack, PlayerDie;
	PlayerRun.loadFromFile("soundRunHero.mp3");
	PlayerJump.loadFromFile("soundJump.mp3");
	PlayerAttack.loadFromFile("soundAttackHero.mp3");
	PlayerDie.loadFromFile("soundDieHero.mp3");

	////vector<sf::SoundBuffer*> player_sounds;
	player_sounds.emplace_back(&PlayerRun);
	player_sounds.emplace_back(&PlayerJump);
	player_sounds.emplace_back(&PlayerAttack);
	player_sounds.emplace_back(&PlayerDie);

	//std::vector<sf::SoundBuffer> permanent_player_sounds; // Используем const sf::SoundBuffer*
	//permanent_player_sounds.emplace_back(PlayerRun);
	//permanent_player_sounds.emplace_back(PlayerJump);
	//permanent_player_sounds.emplace_back(PlayerAttack);
	//permanent_player_sounds.emplace_back(PlayerDie);


	player = new Player(player_textures, player_sounds, sf::Vector2f(400.f, 325.f));
	player->setPosition(orient_x, orient_y);
	if (poz_y == -7 && poz_x == 6) {
		player->setPosition(640, 200);
	}
}

void Silnik::spawnEnemy()
{
	/*vector<unique_ptr<Enemy>> enemies;
	enemies.emplace_back(make_unique<Boar>(boar_textures, boar_sounds, sf::Vector2f(950.f, 325.f)));*/
	// enemies.emplace_back(make_unique<Bee>(bee_textures, bee_sounds, sf::Vector2f(600.f, 150.f)));


	/*Bee przeciwnik(sf::Vector2f(window->getSize().x * 0.9 , 0));
	enemies.push_back(przeciwnik);*/
}

void Silnik::spawnBackground()
{
	backgrounds.clear();
	sf::Sprite pl;
	float x1 = this->window->getSize().x;
	float y1 = this->window->getSize().y;

	sf::IntRect drzewo = sf::IntRect(160, 0, 30, 120);
	sf::IntRect most = sf::IntRect(80, 114, 82, 45);
	sf::IntRect woda = sf::IntRect(95, 300, 40, 34);
	sf::IntRect platforma = sf::IntRect(0, 9, 77, 83);


	if (poz_y == 1)
		if (poz_y == -7 && poz_x == 6)
		{
			pl.setTextureRect(sf::IntRect(64, 160, 30, 30));
			pl.setScale(4, 4.5);
			for (int i = 0; i < 14; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					pl.setPosition(120 + i * 120, 100 + j * 120);
					backgrounds.push_back(pl);
				}

			}

		}
	//if (poz_y == 1)
	//{
	//	pl.setTextureRect(sf::IntRect(100, 300, 40, 34));
	//	pl.setPosition(0, y1 * 0.9);
	//	pl.setScale(100, 3);
	//	this->backgrounds.push_back(pl);
	//}
	//if (poz_x == 2 && poz_y == 1)
	//{
	//	for (size_t i = 0; i < 3; i++)
	//	{
	//		pl.setPosition(x1 / 9 * (1 + (i * 3)), y1 / 4 * 3);
	//		pl.setScale(4, 4);
	//		pl.setTextureRect(sf::IntRect(0, 9, 77, 83));
	//		this->backgrounds.push_back(pl);
	//	}
	//	pl.setPosition(x1 / 9 * 8, 0);
	//	pl.setTextureRect(sf::IntRect(160, 0, 30, 120));
	//	pl.setScale(4, 4);
	//	this->backgrounds.push_back(pl);
	//	
	//}
	//else if (poz_x == 3 && poz_y == 1)
	//{
	//	for (size_t i = 0; i < 6; i++)
	//	{
	//		pl.setPosition(x1 / 9 * (1 + (i)), y1 / 4 * 3);
	//		pl.setScale(2, 2);
	//		pl.setTextureRect(sf::IntRect(0, 9, 77, 83));
	//		this->backgrounds.push_back(pl);
	//	}
	//}

}

void Silnik::spawnPlatforms()
{
	platforms.clear();
	sf::Texture staticTex;
	staticTex.loadFromFile("static.png");
	float x1 = this->window->getSize().x;
	float y1 = this->window->getSize().y;
	Platform pl(&staticTex, sf::Vector2f(200.f, 175.f), sf::Vector2f(0, 0), PlatformType::Static);


	if (poz_x == 0 && poz_y == 0)
	{
		for (int i = 0; i < 4; i++)
		{
			pl.setPosition(i * 350 + 150, y1 - 200);
			pl.setScale(2.5, 2);
			platforms.emplace_back(pl);
		}
		pl.setPosition(1680, y1 - 300);
		platforms.emplace_back(pl);
	}


	else if (poz_x == 1 && poz_y == 0)
	{
		// platforma dolna , platformy blokujące i platformy do poziomu wyżej
		pl.setPosition(0, y1 - 100);
		pl.setScale(24, 2);
		platforms.emplace_back(pl);

		pl.setPosition(x1 / 4, 800);
		pl.setScale(3, 4);
		platforms.emplace_back(pl);

		pl.setPosition(x1 / 4 * 3, 800);
		pl.setScale(3, 4);
		platforms.emplace_back(pl);

		pl.setScale(2, 5);
		pl.setPosition(0, 500);
		platforms.emplace_back(pl);

		pl.setScale(2, 5);
		pl.setPosition(300, 100);
		platforms.emplace_back(pl);
	}


	else if (poz_x == 2 && poz_y == 0)
	{
		//górna ,dolna, ściany boczne
		pl.setPosition(0, y1 - 100);
		pl.setScale(24, 2);
		platforms.emplace_back(pl);

		pl.setPosition(0, 100);
		pl.setScale(24, 2);
		platforms.emplace_back(pl);

		pl.setPosition(x1 - 250, 200);
		pl.setScale(2, 8);
		platforms.emplace_back(pl);

		pl.setPosition(50, 200);
		pl.setScale(2, 8);
		platforms.emplace_back(pl);


	}

	else if (poz_x == 3 && poz_y == 0)
	{
		// platformy w fałdy + platformy od wejść

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				pl.setPosition(640 * i + (j * 50), j * 200 + 200);
				platforms.emplace_back(pl);
			}
		}
		pl.setPosition(500, y1 - 200);
		pl.setScale(2.5, 2);
		platforms.emplace_back(pl);

		pl.setPosition(x1 - 200, 200);
		pl.setScale(2.5, 2);
		platforms.emplace_back(pl);
	}
	else if (poz_x == 4 && poz_y == 0)
	{
		pl.setScale(4, 2);
		pl.setPosition(0, 980);
		platforms.emplace_back(pl);

		pl.setScale(4, 2);
		pl.setPosition(400, 880);
		platforms.emplace_back(pl);

		pl.setScale(4, 2);
		pl.setPosition(800, 780);
		platforms.emplace_back(pl);

		pl.setScale(3, 2);
		pl.setPosition(1200, 680);
		platforms.emplace_back(pl);

		pl.setScale(2, 2);
		pl.setPosition(1500, 580);
		platforms.emplace_back(pl);

		pl.setScale(1.5, 2);
		pl.setPosition(100, 450);
		platforms.emplace_back(pl);

		pl.setScale(1.5, 2);
		pl.setPosition(350, 350);
		platforms.emplace_back(pl);

		pl.setScale(1.5, 2);
		pl.setPosition(600, 250);
		platforms.emplace_back(pl);

		pl.setScale(2, 2);
		pl.setPosition(1000, 400);
		platforms.emplace_back(pl);

		pl.setScale(2, 2);
		pl.setPosition(1300, 400);
		platforms.emplace_back(pl);

		pl.setScale(2, 2);
		pl.setPosition(1600, 400);
		platforms.emplace_back(pl);

		// === Dwa słupy pionowe ===
		pl.setScale(1.5, 8);
		pl.setPosition(300, 200);
		platforms.emplace_back(pl);

		pl.setScale(1.5, 8);
		pl.setPosition(1600, 100);
		platforms.emplace_back(pl);
	}
	else if (poz_x == 5 && poz_y == 0)
	{
		// 1. Startowa platforma po lewej
		pl.setScale(3, 2);
		pl.setPosition(100, 950);
		platforms.emplace_back(pl);

		// 2. Wysoka platforma do skoku w górę
		pl.setScale(2, 5);
		pl.setPosition(400, 800);
		platforms.emplace_back(pl);

		// 3. Lewy fragment platformy z dziurą (PB)
		pl.setScale(1.5, 1.5);
		pl.setPosition(700, 850);
		platforms.emplace_back(pl);

		// 4. Prawy fragment platformy z dziurą (PB)
		pl.setScale(1.5, 1.5);
		pl.setPosition(900, 850);
		platforms.emplace_back(pl);

		// 5. Platforma pozioma po skoku
		pl.setScale(2, 2);
		pl.setPosition(1100, 870);
		platforms.emplace_back(pl);

		// 6. Wysoka ściana z małym stopniem obok
		pl.setScale(0.8, 3);
		pl.setPosition(1320, 880);
		platforms.emplace_back(pl);

		// 7. Duża pionowa ściana (przeszkoda)
		pl.setScale(1.5, 6);
		pl.setPosition(1450, 780);
		platforms.emplace_back(pl);

		// 8. Mała platforma u góry za ścianą
		pl.setScale(1.5, 1.5);
		pl.setPosition(1600, 660);
		platforms.emplace_back(pl);
	}
	else if (poz_x == 6 && poz_y == 0)
	{

	}

	else if (poz_x == 3 && poz_y == -1)
	{
		pl.setScale(4, 2);
		pl.setPosition(300, 100);
		platforms.emplace_back(pl);

		// SKOK W BOK (w prawo, < 300px)
		pl.setScale(2, 2);
		pl.setPosition(550, 180);  // 250px w bok, 80px w górę
		platforms.emplace_back(pl);

		// ŚCIANA pionowa jako przeszkoda
		pl.setScale(1, 8);
		pl.setPosition(800, 100);
		platforms.emplace_back(pl);

		// SKOK PO DRUGIEJ STRONIE ŚCIANY
		pl.setScale(3, 2);
		pl.setPosition(900, 250); // za ścianą
		platforms.emplace_back(pl);

		// SKOK W DÓŁ — spadający segment
		pl.setScale(2, 2);
		pl.setPosition(1000, 450);
		platforms.emplace_back(pl);

		// ŚCIANA pionowa wąska po lewej
		pl.setScale(1, 6);
		pl.setPosition(750, 400);
		platforms.emplace_back(pl);

		// PÓŁKA po lewej — z powrotem
		pl.setScale(2, 2);
		pl.setPosition(600, 600);
		platforms.emplace_back(pl);

		// KOŃCOWA szeroka platforma
		pl.setScale(5, 2);
		pl.setPosition(300, 800);
		platforms.emplace_back(pl);
	}
	else if (poz_x == 3 && poz_y == -2)
	{
		pl.setScale(4, 2);
		pl.setPosition(400, 0);
		platforms.emplace_back(pl);

		// Skok w prawo — lekko w górę
		pl.setScale(2, 2);
		pl.setPosition(700, 100);
		platforms.emplace_back(pl);

		// Ściana — pionowa przeszkoda w środku
		pl.setScale(1, 8);
		pl.setPosition(900, 50);
		platforms.emplace_back(pl);

		// Skok na drugą stronę ściany
		pl.setScale(2, 2);
		pl.setPosition(1050, 200);
		platforms.emplace_back(pl);

		// Spadek niżej — szeroka półka
		pl.setScale(3, 2);
		pl.setPosition(900, 400);
		platforms.emplace_back(pl);

		// Ściana po lewej
		pl.setScale(1, 10);
		pl.setPosition(600, 340);
		platforms.emplace_back(pl);

		// Skok w lewo za ścianę
		pl.setScale(2, 2);
		pl.setPosition(450, 520);
		platforms.emplace_back(pl);

		// Skok w prawo, lekki spadek
		pl.setScale(2, 2);
		pl.setPosition(700, 640);
		platforms.emplace_back(pl);

		// Finalna platforma (np. checkpoint)
		pl.setScale(5, 2);
		pl.setPosition(600, 880);
		platforms.emplace_back(pl);
	}


	if (poz_y == -7 && poz_x == 6)
	{
		for (int i = 1; i < 3; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				Platform pl(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(640 * i, 300 + (600 * j)), PlatformType::Static);
				pl.setScale(2.5, 2);
				platforms.emplace_back(pl);
			}

		}
		for (int i = 0; i < 4; i++)
		{
			Platform pl(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(480 * i, 600), PlatformType::Static);
			pl.setScale(2.5, 2);
			platforms.emplace_back(pl);
		}
	}


	//platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(200.f, 400.f), PlatformType::Static));

}

void Silnik::statistics()
{
	std::ostringstream ss;
	ss << "(" << poz_x << ", " << poz_y << ")";
	text.setString(ss.str());
	if (poz_y >= 1)
	{
		this->tlo = new sf::Color(255, 186, 0, 0);
	}
	else if (poz_y == 2)
	{
		this->tlo = new sf::Color(255, 217, 0, 0);
	}
	else if (poz_y <= 0)
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
	player->ResetAttackFlag();
	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		player->move(0.f, -9.5f);

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		player->move(0.f, 9.5f);

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		player->move(-9.5f, 0.f);

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) )
	{
		player->move(9.5f, 0.f);

	}*/
	for (Platform& platform : platforms)
	{
		if (player->GetCollider().intersects(platform.GetCollider()))
		{
			player->OnCollision(platform, dt);
		}
	}
	if (player->getPosition().x > this->window->getSize().x)
	{
		poz_x += 1;
		orient_y = player->GetPosition().y;
		orient_x = 150;
		this->inicjalizacjaZmiennych();
	}
	if (player->getPosition().y > this->window->getSize().y)
	{
		poz_y -= 1;
		orient_x = player->GetPosition().x;
		orient_y = 150;
		this->inicjalizacjaZmiennych();
	}
	if (player->getPosition().x < 0)
	{
		poz_x -= 1;
		orient_y = player->GetPosition().y;
		orient_x = window->getSize().x - 150;
		this->inicjalizacjaZmiennych();
	}
	if (player->getPosition().y < -100)
	{
		poz_y += 1;
		orient_x = player->GetPosition().x;
		orient_y = window->getSize().y - 150;
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
	sf::SoundBuffer Play;

	switch (player->GetCurrentState())
	{
	case EntityState::Idle:
		hero.loadFromFile("hero-Idle.png");
		break;
	case EntityState::Running:
		hero.loadFromFile("hero-Run.png");
		
		break;
	case EntityState::Jumping:
		hero.loadFromFile("hero-Jump.png");
		
		break;
	case EntityState::Attacking:
		hero.loadFromFile("hero-Attack.png");
		
		break;
	case EntityState::Dying:
		hero.loadFromFile("hero-Dead.png");
		break;
	case EntityState::PushingDown:
		hero.loadFromFile("hero-PushDown.png");
		break;
	}
	//hero.loadFromFile("hero-Idle.png");
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
	texture.loadFromFile("Tiles.png");
	for (Platform& platform : platforms)
	{
		platform.setTexture(texture);
		platform.setTextureRect(sf::IntRect(0, 9, 77, 60));
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

bool MaszynaStanow::czy(int a, int b)
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