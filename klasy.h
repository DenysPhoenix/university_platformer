#pragma once
#include "SFML/Graphics.hpp"
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

//class Animation
//{
//public:
//	Animation(sf::Texture* texture, int imageCount, float switchTime);
//	~Animation();
//
//    void update(float deltaTime, bool faceRight);
//    void reset();
//    //sf::IntRect uvRect;
//    
//
//private:
//    sf::Texture texture1;
//
//    float wys, szer;
//    int currentImage = 0;
//    int imageCount;
//    float totalTime = 0.0f;
//    float switchTime;
//
//    bool paused;
//    bool finished;
//    bool loop; 
//    bool isPaused = false;
//};

class Animation
{
public:
    //metody
    Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime);

    void update(int row, float deltaTime, bool faceRight); //aktualizacja animacji
    void reset(); // reset animacji do pierwszej klatki

    bool isFinished() const;
    void setFinished(bool value);

    ~Animation();

public:
    //zmienne
    sf::IntRect uvRect;

private:
    sf::Vector2u imageCount;
    sf::Vector2u currentImage;

    float totalTime;
    float switchTime;

    bool finished;
};


enum class EntityState
{
    //stany ogólne
    Idle,
    Running,
    Attacking,
    Hitted,

    //stany osobliwe dla Player
    Jumping,
    Dying,

    //stany osobliwe dla Enemy
    //dla Boar
    Walking,

    //dla Bee
    Flying
};

class Entity : public sf::Sprite
{
public:
    Entity(sf::Vector2f pos);

    virtual void update(float dt) = 0;

    sf::FloatRect GetCollider() const { return getGlobalBounds(); };

    sf::Vector2f GetPosition() const { return getPosition(); };

    virtual void OnCollision(Entity& other, float dt) = 0;

    EntityState GetState() { return currentState; };

    virtual ~Entity();

protected:
    float speed;
    bool faceRight;
    EntityState currentState;
};


enum class PlatformType
{
    Static,
    Levitating,
    Moveable,
};

class Platform :public Entity
{
public:
    Platform(sf::Texture* texture, sf::Vector2f size, sf::Vector2f position, PlatformType type);


    PlatformType GetType() const { return type; };

    sf::Vector2f GetVelocity() const { return velocity; };

    void OnCollision(Entity& other, float dt) {};

    void SetAccelaration() { g = 981.f; };

    void SetVelocity(float v) { velocity.x = v; };

    void update(float dt)
    {
        velocity.y += g * dt;
        move(velocity * dt);
    };

    ~Platform();

private:
    PlatformType type;
    bool isFalling;

    sf::Vector2f velocity;
    float g;
};

class Player : public Entity
{
public:
    //metody
    Player(vector<sf::Texture*> textures, sf::Vector2f position);

    EntityState GetCurrentState() const { return currentState; };

    float GetDamage() const { return damage; }

    float GetHP() const { return hp; };

    sf::Vector2f GetPosition() const { return this->getPosition(); };

    bool HasAttackedThisFrame() const { return hasAttackedThisFrame; }

    bool IsInvulnerable() const { return invulnerabilityTimer > 0; }

    //void PlayerCheckPlatformCollision(std::vector<Platform>& platforms/*, std::vector<Enemy>& enemies*/, float dt);

    void OnCollision(Entity& other, float dt);

    void ResetAttackFlag() {
        hasAttackedThisFrame = false;
    }

    void SetHP(float dHP)
    {
        cout << "Player Do: " << hp << endl;
        if (dHP < 0)
        {
            hp += dHP;
        }
        else
        {
            hp -= dHP;
        }
        cout << "Player Po: " << hp << endl;

        //if (hp <= 0) {
        //    hp = 0; // Убедимся, что HP не уходит в минус
        //    // Здесь можно переключить состояние на Dying, если еще не
        //    if (currentState != EntityState::Dying) {
        //        currentState = EntityState::Dying;
        //        animationDead.reset(); // Сброс анимации смерти
        //    }
        //}
    }

    void SetInvulnerable(float duration) { invulnerabilityTimer = duration; }

    void update(float dt);

    ~Player();
public:
    //zmienne
    const float INVULNERABILITY_DURATION = 1.0f;

private:
    //wlasnosci fizyczne
    sf::Vector2f velocity;
    float h, g, hp, damage;

    //wlasnosci oddzialywania na inne Entity
    bool canJump, isOnGround, hasAttackedThisFrame;
    float attackCooldown, invulnerabilityTimer;
    const float ATTACK_COOLDOWN_MAX = 0.5f;
    float attackDuration = 0.5f;
    float currentAttackTime = 0.0f;

    //ladowanie tekstur
    sf::Texture* idleTexture;
    sf::Texture* runTexture;
    sf::Texture* jumpTexture;
    sf::Texture* attackTexture;
    sf::Texture* deadTexture;

    //animacje dla roznych stanow
    Animation animationIdle;
    Animation animationRun;
    Animation animationJump;
    Animation animationAttack;
    Animation animationDead;
};

class Enemy :public Entity
{
public:
    Enemy(vector<sf::Texture*> textures, sf::Vector2f pos);

    float GetDamage() const { return damage; }

    float GetHP() const { return hp; };

    virtual bool IsFlying() const { return false; };

    //void CheckPlatformCollision(std::vector<Platform>& platforms, Player& player, float dt);
    void OnCollision(Entity& other, float dt);

    void SetHP(float dHP)
    {
        cout << "Enemy Do: " << hp << endl;
        if (dHP < 0)
        {
            hp += dHP;
        }
        else
        {
            hp -= dHP;
        }
        cout << "Enemy Po: " << hp << endl;
        //if (hp <= 0) {
        //    hp = 0;
        //    // if (currentState != EntityState::Dying) { // Изменение: EntityState
        //    //     currentState = EntityState::Dying; // Если есть анимация смерти для врагов
        //    // }
        //}
    }

    void update(float dt) = 0;

    virtual void Update(float dt, Player& player) = 0;

    virtual ~Enemy();

    bool IsInvulnerable() const { return invulnerabilityTimer > 0; };
    void SetInvulnerable(float duration) { invulnerabilityTimer = duration; };
    const float INVULNERABILITY_DURATION = 0.5f;

protected:
    //wlasnosci fizyczne
    sf::Vector2f velocity;
    float damage, hp, g;

    //wlasnosci oddzialywania na inne Entity
    bool isOnGround;

    float invulnerabilityTimer;
};

    class Boar :public Enemy
    {
    public:
        Boar(vector<sf::Texture*> textures, sf::Vector2f pos);

        void Update(float dt, Player& player);
        void update(float dt) {};

        ~Boar();

    private:
        //wlasnosci fizyczne
        const float CHASE_RANGE = 300.0f;
        const float ATTACK_RANGE = 50.0f;
        const float RUN_SPEED = 150.0f;
        const float WALK_SPEED = 100.0f;

        //ladowanie tekstur
        sf::Texture* idleTexture;
        sf::Texture* walkTexture;
        sf::Texture* runTexture;
        sf::Texture* hitTexture;

        //animacje
        Animation animationIdle;
        Animation animationWalk;
        Animation animationRun;
        Animation animationHit;

    };

class Bee :public Enemy
{
public:
    Bee(vector<sf::Texture*> textures, sf::Vector2f pos);

    bool isFlying() const { return true; }

    void Update(float dt, Player& player);
    void update(float dt) {};

    ~Bee();

private:
    //wlasnosci fizyczne
    const float ATTACK_RANGE_XY = 50.0f;
    const float FLY_SPEED = 150.0f;

    //ladowanie tekstur
    sf::Texture* flyTexture;
    sf::Texture* attackTexture;
    sf::Texture* hitTexture;

    //animacja
    Animation animationFly;
    Animation animationAttack;
    Animation animationHit;
};

