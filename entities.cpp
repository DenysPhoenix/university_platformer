#include "entities.h"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>

Entity::Entity()
{
}

Entity::~Entity()
{
}

Entity::GetCollider()
{
    return getGlobalBounds();
}

Platform::SetAccelaration() {
    g = 981.f;
}


enum class PlatformType
{
    Static,
    Levitating,
    Moveable,
};



Platform::Platform(sf::Texture* texture, sf::Vector2f size, sf::Vector2f position, PlatformType type)
    :Entity(), type(type)
{
    setTexture(*texture);

    // Масштабирование текстуры по желаемому размеру
    const sf::Texture* currentTexture = getTexture();
    if (currentTexture && currentTexture->getSize().x > 0 && currentTexture->getSize().y > 0)
    {
        setScale(size.x / currentTexture->getSize().x, size.y / currentTexture->getSize().y);
    }
    else
    {
        std::cerr << "Warning: Texture for platform type " << static_cast<int>(type) << " is invalid or has zero size, cannot scale." << std::endl;
        setScale(1.f, 1.f);
    }
    velocity.x = 0.0f;
    setPosition(position);
    setOrigin(size / 2.f);

}

Platform::~Platform()
{
}

enum class PlayerState
{
    Idle,
    Running,
    Jumping,
    Falling,
    Attacking,
    Dying
};

enum class EnemyType
{
    Boar,
    Bee
};

enum class EnemyState
{
    Idle,
    Walking,
    Running,
    Hitted,

    Flying,
    Attacking
};

class Player;




Player::Player(sf::Texture& textureRun, sf::Texture& textureDead, sf::Texture& textureIdle, sf::Texture& textureJump, sf::Texture& textureAttack)
    : Entity(),
    animationRun(&textureRun, sf::Vector2u(8, 1), 0.1f),
    animationDead(&textureDead, sf::Vector2u(8, 1), 0.1f),
    animationIdle(&textureIdle, sf::Vector2u(4, 1), 0.2f),
    animationJump(&textureJump, sf::Vector2u(15, 1), 0.1f),
    animationAttack(&textureAttack, sf::Vector2u(8, 1), 0.1f),
    canJump(true),
    isOnGround(true),
    currentState(PlayerState::Idle),
    runTexture(&textureRun),
    deadTexture(&textureDead),
    idleTexture(&textureIdle),
    jumpTexture(&textureJump),
    attackTexture(&textureAttack),

    attackCooldown(0.0f),
    hasAttackedThisFrame(false),
    invulnerabilityTimer(0.0f)
{
    this->setTexture(textureIdle);
    speed = 300.f;
    setTextureRect(animationIdle.uvRect);
    sf::IntRect(0+, 9, 77, 83)
    setOrigin((animationIdle.uvRect.width) / 2.f, animationIdle.uvRect.height / 2.f);
    setScale(1.5f, 1.5f);
    setPosition(400.f, 325.f);
}

Player::~Player()
{
}

void Player::update(float dt)
{
    //isOnGround = false;
    bool moving = false;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))// && isOnGround)
    {
        currentState = PlayerState::Attacking;
    }

    if (hp <= 0.001)
    {
        currentState = PlayerState::Dying;
    }

    if (currentState != PlayerState::Dying && currentState != PlayerState::Attacking)
    {
        // poruszanie sie w lewo
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            velocity.x = -speed;
            moving = true;
            faceRight = false;
        }

        // poruszanie sie w prawo
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            velocity.x = speed;
            moving = true;
            faceRight = true;
        }

        // skok
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            && canJump && isOnGround)
        {
            velocity.y = -sqrtf(2.0f * g * h);
            canJump = false;
            isOnGround = false;
        }


        velocity.y += g * dt;
        move(velocity * dt);

        if (!isOnGround)
        {
            if (velocity.y < 800.0f) {
                currentState = PlayerState::Jumping;
            }
            else {
                currentState = PlayerState::Idle;
            }
        }
        else
        {

            if (std::abs(velocity.x) > 0.0f) {
                currentState = PlayerState::Running;
            }
            else {
                currentState = PlayerState::Idle;
            }
        }
    }

    switch (currentState)
    {
    case PlayerState::Running:
        setTexture(*runTexture);
        animationRun.update(0, dt, faceRight);
        setTextureRect(animationRun.uvRect);
        break;
    case PlayerState::Idle:
        setTexture(*idleTexture);
        animationIdle.update(0, dt, faceRight);
        setTextureRect(animationIdle.uvRect);
        break;
    case PlayerState::Jumping:
        setTexture(*jumpTexture);
        animationJump.update(0, dt, faceRight);
        setTextureRect(animationJump.uvRect);
        break;
    case PlayerState::Attacking:
        animationAttack.setFinished(false);
        setTexture(*attackTexture);
        animationAttack.update(0, dt, faceRight);
        setTextureRect(animationAttack.uvRect);
        velocity.x = 0.0f;
        velocity.y = 10000.0f * dt;
        move(velocity * dt);
        if (animationAttack.isFinished())
        {
            currentState = PlayerState::Idle;
        }

        break;
    case PlayerState::Dying:
        animationDead.setFinished(false);
        setTexture(*deadTexture);
        animationDead.update(0, dt, faceRight);
        setTextureRect(animationDead.uvRect);
        if (animationAttack.isFinished())
        {
            currentState = PlayerState::Idle;
        }
        break;
    }
}

sf::Vector2f Player::GetPosition() const { return this->getPosition(); }

void Player::PlayerCheckPlatformCollision(std::vector<Platform>& platforms, std::vector<Enemy>& enemies, float dt)
{
    sf::FloatRect heroBounds = GetCollider();

    for (Platform& platform : platforms)
    {
        sf::FloatRect platformBounds = platform.GetCollider();
        if (heroBounds.intersects(platformBounds))
        {
            float overlapX = std::min(heroBounds.left + heroBounds.width, platformBounds.left + platformBounds.width) - std::max(heroBounds.left, platformBounds.left);
            float overlapY = std::min(heroBounds.top + heroBounds.height, platformBounds.top + platformBounds.height) - std::max(heroBounds.top, platformBounds.top);


            if (overlapY < overlapX) // Столкновение по оси Y
            {
                if (heroBounds.top < platformBounds.top) // Игрок приземлился сверху
                {
                    // Корректируем позицию игрока, чтобы он был НА платформе
                    setPosition(getPosition().x, platformBounds.top - heroBounds.height / 2.0f);
                    isOnGround = true;
                    velocity.y = 0;
                    velocity.x = 0;
                    canJump = true;

                    if (platform.GetType() == PlatformType::Levitating)
                    {
                        platform.SetAccelaration();
                    }

                }
                else // Игрок столкнулся снизу (головой)
                {
                    setPosition(getPosition().x, platformBounds.top + platformBounds.height + heroBounds.height / 2.0f);
                    velocity.y = 0;
                }
            }
            else // Столкновение по оси X
            {

                if (heroBounds.left < platformBounds.left)
                {
                    setPosition(platformBounds.left - heroBounds.width / 2.0f, getPosition().y);
                }
                else
                {
                    setPosition(platformBounds.left + platformBounds.width + heroBounds.width / 2.0f, getPosition().y);
                }

                //if (platform.GetType() == PlatformType::Moveable)
                //{
                //    if (overlapX > 0 /*&& overlapY <= 0*/)
                //    {
                //        platform.SetVelocity(velocity.x);
                //    }
                //    else
                //    {
                //        platform.SetVelocity(0);
                //    }

                //}
            }

            heroBounds = GetCollider(); // Обновить границы после корректировки
        }
    }
    /* for (Enemy& enemy : enemies)
     {
         sf::FloatRect enemyBounds = enemy.GetCollider();
         if (enemyBounds.intersects(heroBounds) && currentState == PlayerState::Attacking)
         {
             enemy.SetHP(damage);

         }
     }*/
}


Enemy::Enemy(vector<sf::Texture*> textures, sf::Vector2f pos, EnemyType type) :
    Entity(),

    animationIdle(idleTexture, sf::Vector2u(4, 1), 0.1f),
    animationWalk(walkTexture, sf::Vector2u(6, 1), 0.1f),
    animationRun(runTexture, sf::Vector2u(6, 1), 0.1f),
    animationHit(hitTexture, sf::Vector2u(4, 1), 0.1f),

    idleTexture(textures[0]),
    walkTexture(textures[1]),
    runTexture(textures[2]),
    hitTexture(textures[3])

    /*  animationFly(flyTexture, sf::Vector2u(4, 1), 0.1f),
      animationAttack(attackTexture, sf::Vector2u(4, 1), 0.1f)*/
{

    /*if (type == EnemyType::Boar)
    {*/
    /*idleTexture = textures[0];
     walkTexture = textures[1];
     runTexture = textures[2];
     hitTexture = textures[3];*/

    currentState = EnemyState::Idle;
    this->setTexture(*idleTexture);
    setTextureRect(animationIdle.uvRect);
    setOrigin((animationIdle.uvRect.width) / 2.f, animationIdle.uvRect.height / 2.f);
    /* }
     else if (type == EnemyType::Bee)
     {
         flyTexture = textures[0];
         attackTexture = textures[1];
         hitTexture = textures[2];

         currentState = EnemyState::Flying;
         this->setTexture(*flyTexture);
         setTextureRect(animationFly.uvRect);
         setOrigin((animationFly.uvRect.width) / 2.f, animationFly.uvRect.height / 2.f);
     }*/
    setPosition(pos);
    setScale(2.5f, 2.5f);
}

Enemy::~Enemy()
{
}

void Enemy::Update(float dt, Player& player)
{
    if (hp = 0.0) //!!!
    {
        currentState = EnemyState::Hitted;

    }

    sf::Vector2f playerPos = player.GetPosition(); // Получаем позицию игрока
    sf::Vector2f enemyPos = GetPosition(); // Получаем позицию врага

    velocity.y += g * dt;
    move(velocity * dt);

    /* if (std::abs(velocity.x) > 0.0f) {
        currentState = EnemyState::Running;
    }
    else {
        currentState = EnemyState::Idle;
    }*/

    // === Логика поведения AI врага ===
    // Эту часть нужно выполнять ТОЛЬКО ОДИН РАЗ за кадр и
    // ТОЛЬКО если враг не находится в состоянии "Hitted" или "Dying".
    if (currentState != EnemyState::Hitted /* && currentState != EnemyState::Dying */)
    {
        // Вычисляем ДИСТАНЦИЮ по X, а не перекрытие
        float distanceX = std::abs(playerPos.x - enemyPos.x);

        // Условие для бега/преследования
        if (distanceX < abs(300.0f)) // Например, если игрок в пределах 300 пикселей по горизонтали
        {
            currentState = EnemyState::Running;
            if (playerPos.x < enemyPos.x) { // Если игрок слева
                velocity.x = -150.0f; // Бежим влево
                faceRight = true;
            }
            else
            {
                // Если игрок справа
                velocity.x = 150.0f; // Бежим вправо
                faceRight = false;
            }
        }
        else // Если игрок не в зоне обнаружения, враг бездействует или патрулирует
        {
            currentState = EnemyState::Idle; // Или EnemyState::Walking для патрулирования
            if (faceRight)
            {
                velocity.x = -100.0f;
            }
            else
            {
                velocity.x = 100.0f;
            }
        }
    }
    else // Если враг в состоянии Hitted, он не двигается
    {
        velocity.x = 0.0f;
    }

    switch (currentState)
    {
    case EnemyState::Running:
        setTexture(*runTexture);
        animationRun.update(0, dt, faceRight);
        setTextureRect(animationRun.uvRect);
        // velocity.x = -200.0f;
        break;
    case EnemyState::Idle:
        setTexture(*idleTexture);
        animationIdle.update(0, dt, faceRight);
        setTextureRect(animationIdle.uvRect);
        /* if (playerPos.x < enemyPos.x)
         {
             velocity.x = -100.f;
         }
         else
         {
             velocity.x = 100.f;
         }*/

         /*velocity.x = -100.f;*/
        break;
    case EnemyState::Walking:
        setTexture(*walkTexture);
        animationWalk.update(0, dt, faceRight);
        setTextureRect(animationWalk.uvRect);
        break;
    case EnemyState::Hitted:
        animationHit.setFinished(false);
        setTexture(*hitTexture);
        animationHit.update(0, dt, faceRight);
        setTextureRect(animationHit.uvRect);

        if (animationHit.isFinished())
        {
            currentState = EnemyState::Idle;
        }

        break;


        //case EnemyState::Flying:
        //    setTexture(*flyTexture);
        //    animationFly.update(0, dt, faceRight);
        //    setTextureRect(animationFly.uvRect);

        //    break;
        //case EnemyState::Attacking:
        //    setTexture(*attackTexture);
        //    animationAttack.update(0, dt, faceRight);
        //    setTextureRect(animationAttack.uvRect);

        //    if (animationAttack.isFinished())
        //    {
        //        currentState = EnemyState::Idle;
        //    }

        //    break;
    }
}


void Enemy::CheckPlatformCollision(std::vector<Platform>& platforms, Player& player, float dt)
{
    sf::FloatRect enemyBounds = GetCollider();
    sf::FloatRect heroBounds = player.GetCollider();

    for (Platform& platform : platforms)
    {
        sf::FloatRect platformBounds = platform.GetCollider();
        if (enemyBounds.intersects(platformBounds))
        {
            float overlapX = std::min(enemyBounds.left + enemyBounds.width, platformBounds.left + platformBounds.width) - std::max(enemyBounds.left, platformBounds.left);
            float overlapY = std::min(enemyBounds.top + enemyBounds.height, platformBounds.top + platformBounds.height) - std::max(enemyBounds.top, platformBounds.top);

            if (overlapY < overlapX) // Столкновение по оси Y
            {
                if (enemyBounds.top < platformBounds.top) // Враг приземлился сверху
                {
                    setPosition(getPosition().x, platformBounds.top - enemyBounds.height / 2.0f);
                    isOnGround = true;
                    velocity.y = 0;
                }
            }
            else // Столкновение по оси X
            {
                if (enemyBounds.left < platformBounds.left)
                {
                    setPosition(platformBounds.left - enemyBounds.width / 2.0f, getPosition().y);
                }
                else
                {
                    setPosition(platformBounds.left + platformBounds.width + enemyBounds.width / 2.0f, getPosition().y);
                }
                velocity.x = 0; // Останавливаем горизонтальное движение при столкновении со стеной
            }

            enemyBounds = GetCollider(); // Обновить границы после корректировки
        }
    }

    /* if (enemyBounds.intersects(heroBounds) && player.GetCurrentState() == PlayerState::Attacking)
     {
         player.SetHP(damage);
         currentState = EnemyState::Hitted;
     }*/
}

static const float VIEW_HEIGHT = 512.0f;

void ResizeView(const sf::RenderWindow& window, sf::View& view)
{
    float aspectRatio = static_cast<float>(window.getSize().x) / static_cast<float>(window.getSize().y);
    view.setSize(VIEW_HEIGHT * aspectRatio, VIEW_HEIGHT);
}

Animation::Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime) : imageCount(imageCount), switchTime(switchTime)
{
    totalTime = 0.0f;
    currentImage.x = 0;


    /* uvRect.width = texture->getSize().x / float(imageCount.x);
     uvRect.height = texture->getSize().y / float(imageCount.y);*/
     /*uvRect.width = static_cast<int>(texture->getSize().x / float(imageCount.x));
     uvRect.height = static_cast<int>(texture->getSize().y / float(imageCount.y));*/
    uvRect.width = (static_cast<int>((texture->getSize().x) / static_cast<float>(imageCount.x)));
    uvRect.height = static_cast<int>(texture->getSize().y / static_cast<float>(imageCount.y));
}

Animation::~Animation()
{
}

void Animation::update(int row, float deltaTime, bool faceRight)
{
    if (isPaused)
        return;

    //finished = false;

    //realizacja animacji
    currentImage.y = row;
    totalTime += deltaTime;

    if (totalTime >= switchTime)
    {
        totalTime -= switchTime;
        currentImage.x++;

        if (currentImage.x >= imageCount.x)
        {
            currentImage.x = 0;
            finished = true;
        }
    }

    uvRect.top = currentImage.y * uvRect.height;

    //kierunek animacji (lewo-prawo)
    if (faceRight)
    {
        uvRect.left = currentImage.x * uvRect.width;
        uvRect.width = abs(uvRect.width);
    }
    else
    {
        uvRect.left = (currentImage.x + 1) * abs(uvRect.width);
        uvRect.width = -abs(uvRect.width);
    }
}
//void Animation::update(int row, float deltaTime, bool faceRight)
//{
//    currentImage.y = row;
//    totalTime += deltaTime;
//
//    if (totalTime >= switchTime)
//    {
//        totalTime -= switchTime;
//        currentImage.x++;
//
//        if (currentImage.x >= imageCount.x)
//        {
//            currentImage.x = 0;
//        }
//    }
//
//    uvRect.top = currentImage.y * uvRect.height;
//
//    //kierunek animacji (lewo-prawo)
//    if (faceRight)
//    {
//        uvRect.left = currentImage.x * uvRect.width;
//        uvRect.width = abs(uvRect.width);
//    }
//    else
//    {
//        uvRect.left = (currentImage.x + 1) * abs(uvRect.width);
//        uvRect.width = -abs(uvRect.width);
//    }
//}
//
//void Animation::pause()
//{
//    //isPaused = true;
//    paused = true;
//}

void Animation::resume()
{
    isPaused = false;
}

void Animation::reset()
{
    currentImage.x = 1; //druga klatka
    totalTime = 0.f;
    // aktualizacja uvRect, aby wyswietlic druga klatke (niezbedne dla animacji wlewo)
    uvRect.left = currentImage.x * abs(uvRect.width);
}

/*
int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Game");
    window.setFramerateLimit(60);

    sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(VIEW_HEIGHT, VIEW_HEIGHT));
    ResizeView(window, view);

    sf::Texture heroRun, heroDead, heroIdle, heroJump, heroAttack;
    sf::Texture staticTex, levTex, moveTex;
    heroRun.loadFromFile("hero-Run.png");
    //heroIdle.loadFromFile("character.png");
    heroDead.loadFromFile("hero-Dead.png");
    heroIdle.loadFromFile("hero-Idle.png");
    heroJump.loadFromFile("hero-Jump.png");
    heroAttack.loadFromFile("hero-Attack.png");

    Player hero(heroRun, heroDead, heroIdle, heroJump, heroAttack);

    staticTex.loadFromFile("static.png");
    levTex.loadFromFile("levitating.png");
    moveTex.loadFromFile("moveable.png");

    vector<Platform> platforms;
     platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(100.f, 400.f), PlatformType::Static));
     platforms.emplace_back(Platform(&levTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(300.f, 400.f), PlatformType::Levitating));
     platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(500.f, 400.f), PlatformType::Static));
     platforms.emplace_back(Platform(&moveTex,sf::Vector2f(100.f, 75.f), sf::Vector2f(590.f, 325.f), PlatformType::Moveable));
     platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(670.f, 325.f), PlatformType::Static));

    platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(100.f, 400.f), PlatformType::Static));
    platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(200.f, 400.f), PlatformType::Static));
    platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(300.f, 400.f), PlatformType::Static));
    platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(400.f, 400.f), PlatformType::Static));
    platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(500.f, 400.f), PlatformType::Static));
    platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(600.f, 400.f), PlatformType::Static));
    platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(700.f, 400.f), PlatformType::Static));
    platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(800.f, 400.f), PlatformType::Static));
    platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(900.f, 400.f), PlatformType::Static));
    platforms.emplace_back(Platform(&staticTex, sf::Vector2f(100.f, 75.f), sf::Vector2f(1000.f, 400.f), PlatformType::Static));


    sf::Texture boarIdle, boarWalk, boarRun, boarHit;
    boarIdle.loadFromFile("boar-Idle.png");
    boarWalk.loadFromFile("boar-Walk.png");
    boarRun.loadFromFile("boar-Run.png");
    boarHit.loadFromFile("boar-Hit.png");

    vector<sf::Texture*> boar_textures;
    boar_textures.emplace_back(&boarIdle);
    boar_textures.emplace_back(&boarWalk);
    boar_textures.emplace_back(&boarRun);
    boar_textures.emplace_back(&boarHit);



    //sf::Texture beeFly, beeAttack, beeHit;
    //beeFly.loadFromFile("bee-Fly.png");
    //beeAttack.loadFromFile("bee-Attack.png");
    //beeHit.loadFromFile("bee-Hit.png");

    //vector<sf::Texture*> bee_textures;
    //bee_textures.emplace_back(&beeFly);
    //bee_textures.emplace_back(&beeAttack);
    //bee_textures.emplace_back(&beeHit);

    vector<Enemy> enemies;
    enemies.emplace_back(Enemy(boar_textures, sf::Vector2f(950.f, 325.f), EnemyType::Boar));
    enemies.emplace_back(Enemy(boar_textures, sf::Vector2f(160.f, 325.f), EnemyType::Boar));
    //  enemies.emplace_back(Enemy(bee_textures, sf::Vector2f(600.f, 150.f), EnemyType::Bee));
     // enemies.emplace_back(Enemy(bee_textures, sf::Vector2f(150.f, 150.f), EnemyType::Bee));


    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized)
                ResizeView(window, view);
        }

        float dt = clock.restart().asSeconds();

        if (dt > 1.0f / 20.0f) //by nie bylo baga przy naglej zmianie okna
            dt = 1.0f / 20.0f;

        window.clear(sf::Color::Black);
        view.setCenter(hero.GetPosition());

        window.setView(view);


        for (Platform& platform : platforms)
        {
            platform.update(dt);
            window.draw(platform);
        }

        for (Enemy& enemy : enemies)
        {
            enemy.Update(dt, hero);
            enemy.CheckPlatformCollision(platforms, hero, dt);
            window.draw(enemy);
        }
        window.draw(hero);
        hero.update(dt);
        hero.PlayerCheckPlatformCollision(platforms, enemies, dt);


        window.display();
    }
    return 0;
}
*/