#include "klasy.h"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>

//Animation::Animation(sf::Texture* texture, int imageCount, float switchTime)
//{
//    texture1;
//    szer = static_cast<int>((texture->getSize().x) / static_cast<float>(imageCount));
//    wys = static_cast<int>(texture->getSize().y);
//}
//
//Animation::~Animation()
//{
//}
//
//void Animation::update(float deltaTime, bool faceRight)
//{
//    if (!isPaused) {
//        //realizacja animacji
//        totalTime += deltaTime;
//
//        if (totalTime >= switchTime)
//        {
//            totalTime -= switchTime;
//            currentImage++;
//
//            if (currentImage >= imageCount)
//            {
//                currentImage = 0;
//                finished = true;
//            }
//        }
//    }
//}


Animation::Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime) :
    totalTime(0.0f),
    imageCount(imageCount),
    switchTime(switchTime),
    currentImage(0, 0),
    finished(false)
{
    uvRect.width = static_cast<int>(texture->getSize().x / static_cast<float>(imageCount.x));
    uvRect.height = static_cast<int>(texture->getSize().y / static_cast<float>(imageCount.y));

    //kierunek animacji (lewo-prawo)
    //if (currentImage.x == 0)
    //{
    //    uvRect.left = 0.3125 * uvRect.width;
    //    uvRect.width *= 0.4375; /*uvRect.left + abs(uvRect.width)*0.5625;*/
    //}
    /* else
     {
         uvRect.left = currentImage.x * uvRect.width + 0.25 * uvRect.width;
         uvRect.width = uvRect.left + abs(uvRect.width) * 0.5;
     }*/
}

Animation::~Animation()
{
}

void Animation::update(int row, float deltaTime, bool faceRight)
{
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
    //if (faceRight)
    //{
    //    if (currentImage.x == 0)
    //    {
    //        std::cout << uvRect.width;
    //        uvRect.left = /*currentImage.x * uvRect.width/0.5625 +*/ 0.3125 * uvRect.width / 0.4375;
    //        uvRect.width = abs(uvRect.width);
    //    }
    //    else
    //    {
    //        uvRect.left = currentImage.x * uvRect.width / 0.4375 + 0.5625 * uvRect.width / 0.4375;
    //        uvRect.width = abs(uvRect.width);
    //    }
    //}
    //else
    //{
    //    uvRect.left = (currentImage.x + 1) * abs(uvRect.width);
    //    uvRect.width = -abs(uvRect.width);
    //}
    if (faceRight)
    {
        uvRect.left = currentImage.x * uvRect.width;
        uvRect.width = std::abs(uvRect.width);
    }
    else
    {
        uvRect.left = (currentImage.x + 1) * std::abs(uvRect.width);
        uvRect.width = -std::abs(uvRect.width);
    }
}

bool Animation::isFinished() const
{
    return finished;
}

void Animation::setFinished(bool value)
{
    finished = value;
}

void Animation::reset()
{
    currentImage.x = 1; //druga klatka
    totalTime = 0.f;
    // aktualizacja uvRect, aby wyswietlic druga klatke (niezbedne dla animacji wlewo)
    uvRect.left = currentImage.x * abs(uvRect.width);

    finished = false;
}


Entity::Entity(sf::Vector2f pos) :
    speed(0.0f),
    faceRight(true)
{
    setPosition(pos);
}

Entity::~Entity()
{
}

Platform::Platform(sf::Texture* texture, sf::Vector2f size, sf::Vector2f position, PlatformType type) :
    Entity(position),
    type(type),
    isFalling(false),
    velocity(sf::Vector2f(0.0f, 0.0f)),
    g(0.0f)
{
    setTexture(*texture);

    //skalowanie tekstury do zadanego rozmiaru
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
    setOrigin(size / 2.f);
}

Platform::~Platform()
{
}


Player::Player(vector<sf::Texture*> textures, sf::Vector2f position) :
    Entity(position),

    velocity(sf::Vector2f(0.0f, 0.0f)),
    h(200.0f),
    g(981.0f),
    hp(10000.0f),
    damage(-20.0f),

    canJump(true),
    isOnGround(false),
    attackCooldown(0.0f),
    hasAttackedThisFrame(false),
    invulnerabilityTimer(0.0f),

    idleTexture(textures[0]),
    runTexture(textures[1]),
    jumpTexture(textures[2]),
    attackTexture(textures[3]),
    deadTexture(textures[4]),

    animationIdle(idleTexture, sf::Vector2u(4, 1), 0.2f),
    animationRun(runTexture, sf::Vector2u(8, 1), 0.1f),
    animationJump(jumpTexture, sf::Vector2u(15, 1), 0.1f),
    animationAttack(attackTexture, sf::Vector2u(8, 1), 0.1f),
    animationDead(deadTexture, sf::Vector2u(8, 1), 0.1f)
{
    currentState = EntityState::Idle;
    this->setTexture(*idleTexture);
    setTextureRect(animationIdle.uvRect);
    setOrigin((animationIdle.uvRect.width) / 2.f, animationIdle.uvRect.height / 2.f);
    setScale(1.5f, 1.5f);

    speed = 300.f;
}

Player::~Player()
{
}

void Player::OnCollision(Entity& other, float dt)
{
    bool isPushingMoveablePlatform = false;

    Platform* platform = dynamic_cast<Platform*>(&other);
    if (platform)
    {
        sf::FloatRect playerBounds = GetCollider();
        sf::FloatRect platformBounds = platform->GetCollider();

        if (playerBounds.intersects(platformBounds))
        {
            sf::Vector2f overlap = sf::Vector2f
            (
                std::min(playerBounds.left + playerBounds.width, platformBounds.left + platformBounds.width) - std::max(playerBounds.left, platformBounds.left),
                std::min(playerBounds.top + playerBounds.height, platformBounds.top + platformBounds.height) - std::max(playerBounds.top, platformBounds.top)
            );

            if (overlap.y < overlap.x)
            {
                if (playerBounds.top < platformBounds.top)
                {
                    setPosition(getPosition().x, platformBounds.top - playerBounds.height / 2.0f);
                    isOnGround = true;
                    velocity.y = 0;
                    canJump = true;
                    if (platform->GetType() == PlatformType::Levitating)
                    {
                        platform->SetAccelaration();
                    }
                }

                else
                {
                    setPosition(getPosition().x, platformBounds.top + platformBounds.height + playerBounds.height / 2.0f);
                    velocity.y = 0;
                }
            }
            else
            {
                if (playerBounds.left < platformBounds.left) setPosition(platformBounds.left - playerBounds.width / 2.0f, getPosition().y);
                else setPosition(platformBounds.left + platformBounds.width + playerBounds.width / 2.0f, getPosition().y);


                if (platform->GetType() == PlatformType::Moveable)
                {
                    float platformPushVelocity = 0.0f;

                    if (playerBounds.left < platformBounds.left && velocity.x > 0.0f)
                    {
                        platformPushVelocity = velocity.x;
                    }
                    else if (playerBounds.left > platformBounds.left && velocity.x < 0.0f)
                    {
                        platformPushVelocity = velocity.x;
                    }

                    platform->SetVelocity(platformPushVelocity);
                    // ”станавливаем флаг, что мы толкаем moveable платформу
                    isPushingMoveablePlatform = true;

                }
                velocity.x = 0;
            }
        }
        return;
    }

    Enemy* enemy = dynamic_cast<Enemy*>(&other);
    if (enemy)
    {
        if (currentState == EntityState::Attacking && hasAttackedThisFrame && !enemy->IsInvulnerable())
        {
            enemy->SetHP(damage);
            enemy->SetInvulnerable(enemy->INVULNERABILITY_DURATION);
            hasAttackedThisFrame = true;
        }

        if (enemy->GetState() == EntityState::Attacking && !IsInvulnerable() && currentState != EntityState::Dying)
        {
            SetHP(enemy->GetDamage());
            SetInvulnerable(INVULNERABILITY_DURATION);
        }
    }
}

void Player::update(float dt)
{
    if (invulnerabilityTimer > 0)
    {
        invulnerabilityTimer -= dt;
    }

    if (attackCooldown > 0)
    {
        attackCooldown -= dt;
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && attackCooldown <= 0)
    {
        currentState = EntityState::Attacking;
        hasAttackedThisFrame = true;
        attackCooldown = ATTACK_COOLDOWN_MAX;
        animationAttack.reset();

    }

    if (hp <= 0.001)
    {
        currentState = EntityState::Dying;
        //animationDead.reset();
    }

    if (currentState != EntityState::Dying && currentState != EntityState::Attacking)
    {
        velocity.x = 0.0f;

        // poruszanie sie w lewo
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            velocity.x = -speed;
            faceRight = false;
        }

        // poruszanie sie w prawo
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            velocity.x = speed;
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
            if (velocity.y < 0.0f) currentState = EntityState::Jumping; //830.0f
            else currentState = EntityState::Idle;
        }
        else
        {
            if (std::abs(velocity.x) > 0.0f) currentState = EntityState::Running;
            else currentState = EntityState::Idle;
            velocity.y = 0.0f;
        }
    }
    else if (currentState == EntityState::Attacking)
    {
        velocity.x = 0.0f;
    }
    else if (currentState == EntityState::Dying)
    {
        velocity.x = 0.0f;
        velocity.y = 0.0f;
    }

    switch (currentState)
    {
    case EntityState::Idle:
        setTexture(*idleTexture);
        animationIdle.update(0, dt, faceRight);
        setTextureRect(animationIdle.uvRect);
        break;
    case EntityState::Running:
        setTexture(*runTexture);
        animationRun.update(0, dt, faceRight);
        setTextureRect(animationRun.uvRect);
        break;
    case EntityState::Jumping:
        setTexture(*jumpTexture);
        animationJump.update(0, dt, faceRight);
        setTextureRect(animationJump.uvRect);
        break;
    case EntityState::Attacking:
        animationAttack.setFinished(false);
        setTexture(*attackTexture);
        animationAttack.update(0, dt, faceRight);
        setTextureRect(animationAttack.uvRect);
        /*velocity.x = 0.0f;*/
        velocity.y += 981.0f * dt;
        move(velocity * dt);
        if (animationAttack.isFinished())
        {
            currentState = EntityState::Idle;
        }
        break;
    case EntityState::Dying:
        animationDead.setFinished(false);
        setTexture(*deadTexture);
        animationDead.update(0, dt, faceRight);
        setTextureRect(animationDead.uvRect);
        if (animationAttack.isFinished())
        {
            currentState = EntityState::Idle;
        }
        break;
    }
}

Enemy::Enemy(vector<sf::Texture*> textures, sf::Vector2f position) :
    Entity(position),

    damage(-40.0f),
    //hp(160.0f),
    g(981.0f),
    velocity(sf::Vector2f(0.0f, 0.0f)),
    isOnGround(false),

    invulnerabilityTimer(0.0f)
{
    setScale(2.5f, 2.5f);
}

Enemy::~Enemy()
{
}

void Enemy::OnCollision(Entity& other, float dt)
{
    Platform* platform = dynamic_cast<Platform*>(&other);
    if (platform)
    {
        if (!IsFlying())
        {
            sf::FloatRect enemyBounds = GetCollider();
            sf::FloatRect platformBounds = platform->GetCollider();

            if (enemyBounds.intersects(platformBounds))
            {
                sf::Vector2f overlap = sf::Vector2f
                (
                    std::min(enemyBounds.left + enemyBounds.width, platformBounds.left + platformBounds.width) - std::max(enemyBounds.left, platformBounds.left),
                    std::min(enemyBounds.top + enemyBounds.height, platformBounds.top + platformBounds.height) - std::max(enemyBounds.top, platformBounds.top)
                );

                if (overlap.y < overlap.x)
                {
                    if (enemyBounds.top < platformBounds.top)
                    {
                        setPosition(getPosition().x, platformBounds.top - enemyBounds.height / 2.0f);
                        isOnGround = true;
                    }
                    else setPosition(getPosition().x, platformBounds.top + platformBounds.height + enemyBounds.height / 2.0f);

                    velocity.y = 0;
                }
                else
                {
                    if (enemyBounds.left < platformBounds.left)
                        setPosition(platformBounds.left - enemyBounds.width / 2.0f, getPosition().y);
                    else setPosition(platformBounds.left + platformBounds.width + enemyBounds.width / 2.0f, getPosition().y);

                    velocity.x = 0;
                }
            }
        }
    }

    /* Player* player = dynamic_cast<Player*>(&other);
     if (player)
     {
         if (player->GetCurrentState() == EntityState::Attacking && player->HasAttackedThisFrame())
         {
             SetHP(player->GetDamage());
             if (hp > 0 && currentState != EntityState::Hitted)
                 currentState = EntityState::Hitted;
         }

         if (currentState == EntityState::Attacking && !player->IsInvulnerable() && player->GetCurrentState() != EntityState::Dying)
         {
             player->SetHP(GetDamage());
             player->SetInvulnerable(player->INVULNERABILITY_DURATION);
         }
     }*/
    Player* player = dynamic_cast<Player*>(&other);
    if (player)
    {
        if (player->GetState() == EntityState::Attacking && player->HasAttackedThisFrame() && !IsInvulnerable())
        {
            SetHP(player->GetDamage());
            SetInvulnerable(INVULNERABILITY_DURATION);
            if (hp > 0)
                currentState = EntityState::Hitted;
        }


        // ”рон от врага к игроку
        // »грок получает урон, только если враг атакует и игрок не неу€звим
        if (GetState() == EntityState::Attacking && !player->IsInvulnerable() && player->GetState() != EntityState::Dying)
        {
            player->SetHP(GetDamage());
            player->SetInvulnerable(player->INVULNERABILITY_DURATION);
        }
    }
}



Boar::Boar(vector<sf::Texture*> textures, sf::Vector2f pos) :
    Enemy(textures, pos),

    idleTexture(textures[0]),
    walkTexture(textures[1]),
    runTexture(textures[2]),
    hitTexture(textures[3]),

    animationIdle(idleTexture, sf::Vector2u(4, 1), 0.1f),
    animationWalk(walkTexture, sf::Vector2u(6, 1), 0.1f),
    animationRun(runTexture, sf::Vector2u(6, 1), 0.1f),
    animationHit(hitTexture, sf::Vector2u(4, 1), 0.1f)
{
    hp = 160;

    currentState = EntityState::Idle;
    this->setTexture(*idleTexture);
    setTextureRect(animationIdle.uvRect);
    setOrigin((animationIdle.uvRect.width) / 2.f, animationIdle.uvRect.height / 2.f);
}

Boar::~Boar()
{
}

void Boar::Update(float dt, Player& player)
{
    if (invulnerabilityTimer > 0)
    {
        invulnerabilityTimer -= dt;
    }

    if (hp <= 0.0f)
    {
        if (currentState != EntityState::Hitted)
        {
            currentState = EntityState::Hitted;
            animationHit.reset();
        }
    }

    sf::Vector2f playerPos = player.GetPosition();
    sf::Vector2f enemyPos = GetPosition();

    velocity.y += g * dt;
    move(velocity * dt);

    /* if (std::abs(velocity.x) > 0.0f) {
        currentState = EnemyState::Running;
    }
    else {
        currentState = EnemyState::Idle;
    }*/

    //zachowanie sie
    if (currentState != EntityState::Hitted)
    {
        float distanceX = std::abs(playerPos.x - enemyPos.x);

        /*if (distanceX < ATTACK_RANGE && std::abs(playerPos.y - enemyPos.y) < GetCollider().height / 2.0f + player.GetCollider().height / 2.0f)*/
        if (distanceX < CHASE_RANGE)
        {
            currentState = EntityState::Running;
            if (playerPos.x < enemyPos.x)
            {
                velocity.x = -RUN_SPEED;
                faceRight = true;
            }
            else
            {
                velocity.x = RUN_SPEED;
                faceRight = false;
            }
        }
        else if (distanceX == 0.0)
        {
            velocity.x = 0;
            faceRight = true;
        }
        else
        {
            currentState = EntityState::Walking;
            if (faceRight)
            {
                velocity.x = -WALK_SPEED;
            }
            else
            {
                velocity.x = WALK_SPEED;
            }
        }
    }
    else
    {
        velocity.x = 0.0f;
        velocity.y = 0.0f;
    }

    //animacja na podstawie stanu
    switch (currentState)
    {
    case EntityState::Running:
        setTexture(*runTexture);
        animationRun.update(0, dt, faceRight);
        setTextureRect(animationRun.uvRect);
        // velocity.x = -200.0f;
        break;
    case EntityState::Idle:
        setTexture(*idleTexture);
        animationIdle.update(0, dt, faceRight);
        setTextureRect(animationIdle.uvRect);
        break;
    case EntityState::Walking:
        setTexture(*walkTexture);
        animationWalk.update(0, dt, faceRight);
        setTextureRect(animationWalk.uvRect);
        break;
    case EntityState::Hitted:
        animationHit.setFinished(false);
        setTexture(*hitTexture);
        animationHit.update(0, dt, faceRight);
        setTextureRect(animationHit.uvRect);
        if (animationHit.isFinished())
        {
            currentState = EntityState::Idle;
        }
        break;
    }
}

Bee::Bee(vector<sf::Texture*> textures, sf::Vector2f pos) :
    Enemy(textures, pos),

    flyTexture(textures[0]),
    attackTexture(textures[1]),
    hitTexture(textures[2]),

    animationFly(flyTexture, sf::Vector2u(4, 1), 0.1f),
    animationAttack(attackTexture, sf::Vector2u(4, 1), 0.1f),
    animationHit(hitTexture, sf::Vector2u(4, 1), 0.1f)
{

    hp = 100;
    currentState = EntityState::Flying;
    this->setTexture(*flyTexture);
    setTextureRect(animationFly.uvRect);
    setOrigin((animationFly.uvRect.width) / 2.f, animationFly.uvRect.height / 2.f);
}

Bee::~Bee()
{
}

void Bee::Update(float dt, Player& player)
{
    if (invulnerabilityTimer > 0)
    {
        invulnerabilityTimer -= dt;
    }

    if (hp <= 0.0f)
    {
        if (currentState != EntityState::Hitted)
        {
            currentState = EntityState::Hitted;
            //animationHit.reset();
        }
    }

    sf::Vector2f playerPos = player.GetPosition();
    sf::Vector2f enemyPos = GetPosition();

    if (currentState != EntityState::Hitted)
    {
        sf::Vector2f distance
        (
            std::abs(playerPos.x - enemyPos.x),
            std::abs(playerPos.y - enemyPos.y)
        );

        velocity.x = 0.0f;
        velocity.y = 0.0f;

        if (playerPos.x < enemyPos.x)
        {
            velocity.x = -FLY_SPEED;
            faceRight = true;
        }
        else if (playerPos.x > enemyPos.x)
        {
            velocity.x = FLY_SPEED;
            faceRight = false;
        }

        if (playerPos.y < enemyPos.y)
        {
            velocity.y = -FLY_SPEED;
        }
        else if (playerPos.y > enemyPos.y)
        {
            velocity.y = FLY_SPEED;
        }

        if (distance.x < ATTACK_RANGE_XY && distance.y < ATTACK_RANGE_XY)
        {
            currentState = EntityState::Attacking;
            velocity.x = 0.0f;
            velocity.y = 0.0f;
            if (player.GetHP() <= 0)
            {
                currentState = EntityState::Flying;
            }
        }
        else
        {
            currentState = EntityState::Flying;
        }
    }
    else
    {
        velocity.x = 0.0f;
        velocity.y = 0.0f;
    }

    move(velocity * dt);

    //animacja na podstawie stanu
    switch (currentState)
    {
    case EntityState::Flying:
        setTexture(*flyTexture);
        animationFly.update(0, dt, faceRight);
        setTextureRect(animationFly.uvRect);

        break;
    case EntityState::Attacking:
        animationAttack.setFinished(false);
        setTexture(*attackTexture);
        animationAttack.update(0, dt, faceRight);
        setTextureRect(animationAttack.uvRect);

        if (animationAttack.isFinished())
        {
            currentState = EntityState::Flying;
        }
        break;
    case EntityState::Hitted:
        animationHit.setFinished(false);
        setTexture(*hitTexture);
        animationHit.update(0, dt, faceRight);
        setTextureRect(animationHit.uvRect);
        if (animationHit.isFinished())
        {
            if (hp <= 0.0f)
            {
                //enemy is dead
            }
            else currentState = EntityState::Flying;
        }
        break;
    }
}





