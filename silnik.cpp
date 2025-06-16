#include "silnik.h"
#include "klasy.h"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <algorithm>
#include <cstdlib> 
#include <ctime>   


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
	std::srand(static_cast<unsigned>(std::time(nullptr)));

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
	hitboxPlayer.setSize(sf::Vector2f(64.0f, 48.0f));
	hitboxPlayer.setOrigin(32.0f, 10.0f);
	hitboxPlayer.setFillColor(sf::Color(0, 0, 0, 0));
	//hitboxPlayer.setFillColor(sf::Color::Red);
	player->setPosition(orient_x, orient_y);
}

void Silnik::spawnEnemy()
{
	enemies.clear();

	boarIdle.loadFromFile("boar-Idle.png");
	boarWalk.loadFromFile("boar-Walk.png");
	boarRun.loadFromFile("boar-Run.png");
	boarHit.loadFromFile("boar-Hit.png");

	bufferBoar.loadFromFile("soundAttackBoar.mp3");
	boar_sounds.emplace_back(&bufferBoar);

	boar_textures.emplace_back(&boarIdle);
	boar_textures.emplace_back(&boarWalk);
	boar_textures.emplace_back(&boarRun);
	boar_textures.emplace_back(&boarHit);

	beeBuzzing.loadFromFile("soundBuzzingBee.mp3");
	vector<sf::SoundBuffer*> bee_sounds;
	bee_sounds.emplace_back(&beeBuzzing);
	
	beeFly.loadFromFile("bee-Fly.png");
	beeAttack.loadFromFile("bee-Attack.png");
	beeHit.loadFromFile("bee-Hit.png");

	bee_textures.emplace_back(&beeFly);
	bee_textures.emplace_back(&beeAttack);
	bee_textures.emplace_back(&beeHit);
	for (size_t i = 0; i < 5; i++)
	{
		int los = std::rand() % 10;
		int los1 = std::rand() % 10;
		int los2 = std::rand() % 2;
		if (los2)
		{
			enemies.emplace_back(make_unique<Boar>(boar_textures, boar_sounds, sf::Vector2f(200+100 * los, 400 + (50 * los1))));
		}
		else
		{
			enemies.emplace_back(make_unique<Bee>(bee_textures, bee_sounds, sf::Vector2f(180 * los, 400 + (50 * los1))));
		}
		
	}
	

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

	if (poz_y == 0)
	{
		//background krzaki
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 13; j++)
			{
				pl.setScale(4, 4);
				pl.setPosition(i * 128, j * 52);
				pl.setTextureRect(sf::IntRect(22, 191, 41, 32));
				backgrounds.push_back(pl);
			}
		}
		//background drzewa
		for (int i = 0; i < 10; i++)
		{
			pl.setScale(8, 8);
			if (i % 2 == 0)
			{
				pl.setTextureRect(drzewo);
				pl.setPosition(i * 180, 500);

			}
			else
			{
				pl.setTextureRect(drzewo);
				pl.setPosition((i * 180) + 30, 450);
			}
			backgrounds.emplace_back(pl);

		}
		// liście drzew
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				int los = std::rand() % 4;
				pl.setScale(5, 5);
				pl.setPosition(-50 + i * 180 + ((los - 2) * 10), j * 180);
				pl.setTextureRect(sf::IntRect(274, 5 + (los * 47), 123, 42));
				backgrounds.emplace_back(pl);
				pl.setScale(5, -5);
				pl.setPosition(-50 + i * 180 + ((los - 2) * 10), 400 + j * 180);
				backgrounds.emplace_back(pl);
			}
		}
	}
	


	if (poz_y <= -1)
	{
		for (int i = 0; i < 15; i++)
		{
			pl.setTextureRect(sf::IntRect(0, 300, 47,35 ));
			pl.setScale(4, 4);
			pl.setPosition(-40 + i * 155, 0);
			backgrounds.push_back(pl);
			pl.setTextureRect(sf::IntRect(95, 300, 35, 20));
			pl.setPosition(-40 + i * 155, 180);
			pl.setScale(4, -4);
			backgrounds.push_back(pl);
			pl.setScale(3, 4);
			pl.setPosition(-50+i * 128, 180);
			pl.setTextureRect(sf::IntRect(0, 91, 77,36 ));
			backgrounds.push_back(pl);
			for (int j = 0; j < 13; j++)
			{
				pl.setScale(4, 4);
				pl.setPosition(i * 128,300+ j * 60);
				pl.setTextureRect(sf::IntRect(164, 192, 41, 31));
				backgrounds.push_back(pl);
			}
		}
		for (int i = 0; i < 10; i++)
		{
			int los = std::rand() % 10;
			int los1 = std::rand() % 10;
			pl.setPosition(180*los,400+(50*los1));
			pl.setTextureRect(sf::IntRect(0, 225, 48, 45));
			pl.setScale(4, 4);
			backgrounds.push_back(pl);
		}
	}
	if (poz_y > 0)
	{
		for (int i = 0; i < 10; i++)
		{
			int los = std::rand() % 4;
			pl.setScale(5, 5);
			pl.setPosition(-50 + i * 180 + ((los - 2) * 10),1000);
			pl.setTextureRect(sf::IntRect(274, 5 + (los * 47), 123, 42));
			backgrounds.emplace_back(pl);
		}
	}
}

void Silnik::spawnPlatforms()
{
	platforms.clear();
	sf::Texture staticTex, levTex;
	staticTex.loadFromFile("static.png");
	levTex.loadFromFile("levitating.png");
	float x1 = this->window->getSize().x;
	float y1 = this->window->getSize().y;
	Platform pl(&staticTex, sf::Vector2f(200.f, 175.f), sf::Vector2f(0, 0), PlatformType::Static);
	Platform lev(&levTex, sf::Vector2f(2000.f, 175.f), sf::Vector2f(1000, 800), PlatformType::Levitating);
	platforms.emplace_back(lev);

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
		pl.setPosition(300, 150);
		platforms.emplace_back(pl);
	}
	else if (poz_x == 1 && poz_y == 1)
	{
		for (int i = 0; i < 10; i++)
		{
			int los = std::rand() % 10;
			int los1 = std::rand() % 10;
			pl.setPosition(180 * los, 400 + (50 * los1));
			platforms.push_back(pl);
		}
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
	else if (poz_x == 3 && poz_y == -1)
	{
		pl.setScale(4, 2);
		pl.setPosition(350, 150);
		platforms.emplace_back(pl);

		// SKOK W BOK (w prawo, < 300px)
		pl.setScale(2, 2);
		pl.setPosition(600, 230);  // 250px w bok, 80px w górę
		platforms.emplace_back(pl);

		// ŚCIANA pionowa jako przeszkoda
		pl.setScale(1, 8);
		pl.setPosition(850, 150);
		platforms.emplace_back(pl);

		// SKOK PO DRUGIEJ STRONIE ŚCIANY
		pl.setScale(3, 2);
		pl.setPosition(950, 300); // za ścianą
		platforms.emplace_back(pl);

		// SKOK W DÓŁ — spadający segment
		pl.setScale(2, 2);
		pl.setPosition(1050, 500);
		platforms.emplace_back(pl);

		// ŚCIANA pionowa wąska po lewej
		pl.setScale(1, 6);
		pl.setPosition(800, 450);
		platforms.emplace_back(pl);

		// PÓŁKA po lewej — z powrotem
		pl.setScale(2, 2);
		pl.setPosition(650, 650);
		platforms.emplace_back(pl);

		// KOŃCOWA szeroka platforma
		pl.setScale(5, 2);
		pl.setPosition(350, 850);
		platforms.emplace_back(pl);
	}
	else if (poz_x == 3 && poz_y == -2)
	{
		pl.setScale(4, 2);
		pl.setPosition(450, 50);
		platforms.emplace_back(pl);

		// Skok w prawo — lekko w górę
		pl.setScale(2, 2);
		pl.setPosition(750, 150);
		platforms.emplace_back(pl);

		// Ściana — pionowa przeszkoda w środku
		pl.setScale(1, 8);
		pl.setPosition(950, 100);
		platforms.emplace_back(pl);

		// Skok na drugą stronę ściany
		pl.setScale(2, 2);
		pl.setPosition(1100, 250);
		platforms.emplace_back(pl);

		// Spadek niżej — szeroka półka
		pl.setScale(3, 2);
		pl.setPosition(950, 450);
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
	else
	{
		poz_x = 0;
		poz_y = 0;
		orient_x = 240;
		orient_y = 270;
		inicjalizacjaZmiennych();
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
	hitboxPlayer.setPosition(player->GetPosition());
	player->update(dt);
	/*player->ResetAttackFlag();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
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
	player->wasOnGround = false;
	for (Platform& platform : platforms)
	{
		if (hitboxPlayer.getGlobalBounds().intersects(platform.GetCollider()))
		{
			player->OnCollision(platform, dt);
			player->wasOnGround = true;
			
		}
	}
	//player->isOnGround = player->wasOnGround;

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
	if (player->getPosition().y < -150)
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
	for (std::unique_ptr<Enemy>& enemyPtr : enemies)
	{
	    Enemy& enemy = *enemyPtr;
	    for (Platform& platform : platforms)
	    {
	        if (enemy.GetCollider().intersects(platform.GetCollider()))
	        {
	            enemy.OnCollision(platform, dt);
	            //platform.OnCollision(enemy, dt);
	        }
	    }
	}

	// 3) hero and enemies
	for (std::unique_ptr<Enemy>& enemyPtr : enemies)
	{
	    Enemy& enemy = *enemyPtr;
	    if (player->GetCollider().intersects(enemy.GetCollider()))
	    {
	  
	        player->OnCollision(enemy, dt);
	        enemy.OnCollision(*player, dt);
	    }
	}

	//hero.ResetAttackFlag();

	enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
		[](const std::unique_ptr<Enemy>& enemy) {
			return enemy->GetHP() <= 0.0f;
		}),
		enemies.end());
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

	if (dt > 0.1f) dt = 0.1f;
	this->aktualizacjaEvents();
	this->aktualizacjaPlayer();
	this->aktualizacjaEnemies();
	this->aktualizacjaStatystyk();
	this->aktualizacjaPlatform();
}
// wyswietlanie


void Silnik::wyswietlPlayer()
{
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
	player->setTexture(hero);
	player->setTextures(dt);
	this->window->draw(*player);
	this->window->draw(hitboxPlayer);
}

void Silnik::wyswietlEnemies()
{
	for (unique_ptr<Enemy>& enemy : enemies)
	{
		enemy->Update(dt, *player);
		enemy->update(dt);
		this->window->draw(*enemy);
	}
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


