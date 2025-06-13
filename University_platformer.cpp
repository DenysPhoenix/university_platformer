#include <iostream>
#include "silnik.h"
#include "klasy.h"


//static const float VIEW_HEIGHT = 512.0f;
//
//void ResizeView(const sf::RenderWindow& window, sf::View& view)
//{
//    float aspectRatio = static_cast<float>(window.getSize().x) / static_cast<float>(window.getSize().y);
//    view.setSize(VIEW_HEIGHT * aspectRatio, VIEW_HEIGHT);
//}
//
//
//int main()
//{
//    sf::RenderWindow window(sf::VideoMode(800, 600), "Game");
//    window.setFramerateLimit(60);
//
//    sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(VIEW_HEIGHT, VIEW_HEIGHT));
//    ResizeView(window, view);
//
//    sf::Texture heroIdle, heroRun, heroJump, heroAttack, heroDead;
//    heroIdle.loadFromFile("hero-Idle.png");
//    heroRun.loadFromFile("hero-Run.png");
//    heroJump.loadFromFile("hero-Jump.png");
//    heroAttack.loadFromFile("hero-Attack.png");
//    heroDead.loadFromFile("hero-Dead.png");
//
//    vector<sf::Texture*> player_textures;
//    player_textures.emplace_back(&heroIdle);
//    player_textures.emplace_back(&heroRun);
//    player_textures.emplace_back(&heroJump);
//    player_textures.emplace_back(&heroAttack);
//    player_textures.emplace_back(&heroDead);
//
//    Player hero(player_textures, sf::Vector2f(400.f, 325.f));
//
//    sf::Texture staticTex, levTex, moveTex;
//    staticTex.loadFromFile("static.png");
//    levTex.loadFromFile("levitating.png");
//    moveTex.loadFromFile("moveable.png");
//
//    vector<Platform> platforms;
//    /*platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(100.f, 400.f), PlatformType::Static));
//    platforms.emplace_back(Platform(&levTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(300.f, 400.f), PlatformType::Levitating));
//    platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(500.f, 400.f), PlatformType::Static));
//    platforms.emplace_back(Platform(&moveTex,sf::Vector2f(100.f, 75.f), sf::Vector2f(590.f, 325.f), PlatformType::Moveable));
//    platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(670.f, 325.f), PlatformType::Static));*/
//
//    platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(100.f, 400.f), PlatformType::Static));
//    platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(200.f, 400.f), PlatformType::Static));
//    platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(300.f, 400.f), PlatformType::Static));
//    platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(400.f, 400.f), PlatformType::Static));
//    platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(500.f, 400.f), PlatformType::Static));
//    platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(600.f, 400.f), PlatformType::Static));
//    platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(700.f, 400.f), PlatformType::Static));
//    platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(800.f, 400.f), PlatformType::Static));
//    platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(900.f, 400.f), PlatformType::Static));
//    platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(1000.f, 400.f), PlatformType::Static));
//
//    sf::Texture boarIdle, boarWalk, boarRun, boarHit;
//    boarIdle.loadFromFile("boar-Idle.png");
//    boarWalk.loadFromFile("boar-Walk.png");
//    boarRun.loadFromFile("boar-Run.png");
//    boarHit.loadFromFile("boar-Hit.png");
//
//    vector<sf::Texture*> boar_textures;
//    boar_textures.emplace_back(&boarIdle);
//    boar_textures.emplace_back(&boarWalk);
//    boar_textures.emplace_back(&boarRun);
//    boar_textures.emplace_back(&boarHit);
//
//    sf::Texture beeFly, beeAttack, beeHit;
//    beeFly.loadFromFile("bee-Fly.png");
//    beeAttack.loadFromFile("bee-Attack.png");
//    beeHit.loadFromFile("bee-Hit.png");
//
//    vector<sf::Texture*> bee_textures;
//    bee_textures.emplace_back(&beeFly);
//    bee_textures.emplace_back(&beeAttack);
//    bee_textures.emplace_back(&beeHit);
//
//    vector<unique_ptr<Enemy>> enemies;
//    enemies.emplace_back(make_unique<Boar>(boar_textures, sf::Vector2f(950.f, 325.f)));
//    enemies.emplace_back(make_unique<Bee>(bee_textures, sf::Vector2f(600.f, 150.f)));
//
//    sf::Clock clock;
//
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//            if (event.type == sf::Event::Resized)
//                ResizeView(window, view);
//        }
//
//        float dt = clock.restart().asSeconds();
//
//        if (dt > 1.0f / 20.0f) //by nie bylo baga przy naglej zmianie okna
//            dt = 1.0f / 20.0f;
//
//        window.clear(sf::Color::Black);
//        view.setCenter(hero.GetPosition());
//
//        window.setView(view);
//
//
//        for (Platform& platform : platforms)
//        {
//            platform.update(dt);
//            window.draw(platform);
//        }
//
//        //for (Enemy& enemy : enemies)
//        //{
//        //    enemy.Update(dt, hero);
//        //    //enemy.CheckPlatformCollision(platforms, hero, dt);
//        //    //window.draw(enemy);
//        //}
//
//        for (unique_ptr<Enemy>& enemy : enemies)
//        {
//            enemy->Update(dt, hero);
//            enemy->update(dt);
//            window.draw(*enemy);
//        }
//
//        window.draw(hero);
//        hero.update(dt);
//        // hero.PlayerCheckPlatformCollision(platforms/*, enemies*/, dt);
//
//         //================collisions====================
//          // 1) hero and platforms
//        for (Platform& platform : platforms)
//        {
//            if (hero.GetCollider().intersects(platform.GetCollider()))
//            {
//                hero.OnCollision(platform, dt);
//                // Если платформа должна реагировать на игрока, то раскомментируйте:
//                // platform.OnCollision(hero, dt);
//            }
//        }
//
//        // 2) enemies and platforms
//        for (std::unique_ptr<Enemy>& enemyPtr : enemies)
//        {
//            Enemy& enemy = *enemyPtr;
//            for (Platform& platform : platforms)
//            {
//                if (enemy.GetCollider().intersects(platform.GetCollider()))
//                {
//                    enemy.OnCollision(platform, dt);
//                    // Если платформа должна реагировать на врага, то раскомментируйте:
//                    // platform.OnCollision(enemy, dt);
//                }
//            }
//        }
//
//        // 3) hero and enemies
//        // Важно: в этом цикле может происходить изменение состояния игрока и врагов
//        // Поэтому нужно быть осторожным с удалением врагов, если они умирают.
//        // Здесь мы просто обрабатываем столкновения. Удаление мертвых врагов можно сделать после этого цикла.
//        for (std::unique_ptr<Enemy>& enemyPtr : enemies)
//        {
//            Enemy& enemy = *enemyPtr;
//            if (hero.GetCollider().intersects(enemy.GetCollider()))
//            {
//                // Игрок атакует врага, или враг атакует игрока
//                hero.OnCollision(enemy, dt);
//                enemy.OnCollision(hero, dt);
//            }
//        }
//
//        // Сброс флага атаки игрока для следующего кадра, если он был установлен
//        hero.ResetAttackFlag();
//
//
//
//        enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
//            [](const std::unique_ptr<Enemy>& enemy) {
//                return enemy->GetHP() <= 0.0f;
//            }),
//            enemies.end());
//
//        window.display();
//    }
//    return 0;
//}

using namespace std;

int main()
{
	// powołujemy silnik który całą grę nam uruchamia i tylko aktualizuje i wyświetla i do tamtych funkcji wkładamy funkcje które mają działać


    Silnik gra;
	while (gra.czyokno())
	{
		gra.aktualizacja();
		gra.wyswietlenie();
	}
	

    return 0;
}


