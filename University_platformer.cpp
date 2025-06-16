#include <iostream>
#include "silnik.h"
#include "SFML/Graphics.hpp"
#include <vector>
#include "klasy.h"


int main()
{
	//sf::SoundBuffer buffer;
	//if (!buffer.loadFromFile("dzwieki/soundJump.mp3"))
	//	return -1;

	//sf::Sound sound;
	//sound.setBuffer(buffer);
	//sound.play();
	
    
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Menu");
	window.setFramerateLimit(60);

	Button PlayButton(sf::Vector2f(660, 300), sf::Vector2f(600, 200), "PLAY");
	Button ExitButton(sf::Vector2f(660, 500), sf::Vector2f(600, 200), "EXIT");
	GameState currentGameState = MENU;

	sf::Texture heroIdle, staticTex, boarIdle, beeIdle;
	heroIdle.loadFromFile("hero-Idle.png");
	staticTex.loadFromFile("Tiles.png");
	boarIdle.loadFromFile("boar-Idle.png");
	beeIdle.loadFromFile("bee-Fly.png");

	sf::Sprite bohater, boar, bee, krzak;
	bohater.setTexture(heroIdle);
	bohater.setScale(3.0, 3.0f);
	bohater.setTextureRect(sf::IntRect(0, 0, heroIdle.getSize().x / 4, heroIdle.getSize().y));
	bohater.setPosition(400.f, 815.f);

	boar.setTexture(boarIdle);
	boar.setScale(4.0, 4.0f);
	boar.setTextureRect(sf::IntRect(0, 0, boarIdle.getSize().x / 4, boarIdle.getSize().y));
	boar.setPosition(1000.f, 835.f);

	bee.setTexture(beeIdle);
	bee.setScale(4.0, 4.0f);
	bee.setTextureRect(sf::IntRect(0, 0, beeIdle.getSize().x / 4, beeIdle.getSize().y));
	bee.setPosition(1450.f, 650.f);

	vector<sf::Sprite> ziemia;
	for (int i = 0; i < 14; i++)
	{
		sf::Sprite el;
		el.setTexture(staticTex);
		el.setScale(2.0, 2.0f);
		el.setTextureRect(sf::IntRect(0, 9, 77, 83));
		el.setPosition(i*140.f-20, 950.f);
		ziemia.emplace_back(el);
	}

	sf::RectangleShape menu(sf::Vector2f(120.f, 50.f));
	menu.setSize(sf::Vector2f(1920, 250));
	menu.setFillColor(sf::Color(255, 150, 0));

	sf::Font font;
	sf::Text title;
	title.setString("MOON WALKER");
	font.loadFromFile("Super-Mario-Font.ttf");
	title.setFont(font);
	title.setFillColor(sf::Color(39, 127, 163));
	title.setCharacterSize(70);

	sf::FloatRect shapeBounds = menu.getGlobalBounds();
	sf::FloatRect textBounds = title.getLocalBounds();
	float x = shapeBounds.left + (shapeBounds.width - textBounds.width) / 2.f;
	float y = shapeBounds.top + (shapeBounds.height - textBounds.height) / 2.f;
	// textBounds.left/top > 0
	x -= textBounds.left;
	y -= textBounds.top;
	title.setPosition(x, y);


	Silnik* gra = nullptr;
	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			

			if (currentGameState == MENU)
			{
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				{
					sf::Vector2i mousePosition(event.mouseButton.x, event.mouseButton.y);
					if (PlayButton.isClicked(mousePosition, window))
					{
						window.close();
						if (gra == nullptr) { 
							gra = new Silnik();
							while (gra->czyokno())
							{
								gra->aktualizacja();
								gra->wyswietlenie();
							}
						}
					}
					if (ExitButton.isClicked(mousePosition, window))
					{
						window.close();
					}
				}
			}
		}

		window.clear(sf::Color(255, 186, 0, 0));
		window.setView(window.getDefaultView()); 
		PlayButton.draw(window);
		ExitButton.draw(window);
		window.draw(menu);
		window.draw(title);
		window.draw(bohater);
		window.draw(boar);
		window.draw(bee);

		for (sf::Sprite& el : ziemia)
		{
			window.draw(el);
		}
		window.display();

	}

	delete gra;
    return 0;
}

