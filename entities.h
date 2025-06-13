#pragma once
#include "SFML/Graphics.hpp"

class Entity : public sf::Sprite
{
public:
    Entity();
    virtual ~Entity();

    virtual void update(float dt) = 0;

    sf::FloatRect GetCollider();
protected:
    float speed;
    bool faceRight;
};

class Platform :public Entity
{
public:
    Platform(sf::Texture* texture, sf::Vector2f size, sf::Vector2f position, PlatformType type);
    ~Platform();

    void SetAccelaration();

    PlatformType GetType() const { return type; };

    void SetVelocity(float v) { velocity.x = v; };
    sf::Vector2f GetVelocity() const { return velocity; };

    void update(float dt)
    {
        velocity.y += g * dt;
        move(velocity * dt);
    };

private:
    PlatformType type;
    bool isFalling = true;

    sf::Vector2f velocity = sf::Vector2f(0.0f, 0.0f);
    float g = 0.0f; //przyspieszenie ziemskie
};

class Enemy :public Entity
{
public:
    Enemy(vector<sf::Texture*> textures, sf::Vector2f pos, EnemyType type);
    ~Enemy();

    void update(float dt) {};
    void Update(float dt, Player& player);
    void CheckPlatformCollision(std::vector<Platform>& platforms, Player& player, float dt);
    sf::Vector2f GetPosition() const { return this->getPosition(); };

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
    }

private:
    float damage = -40.0f;
    float hp = 60.0f;
    float attackCooldown; // Время до следующей атаки врага
    const float ENEMY_ATTACK_COOLDOWN_MAX = 1.5f; // Например, враг атакует каждые 1.5 секунды

    float g = 981.0f;

    sf::Vector2f velocity = sf::Vector2f(0.0f, 0.0f);

    // Ссылки на текстуры, чтобы можно было переключать их
    sf::Texture* idleTexture;
    sf::Texture* walkTexture;
    sf::Texture* runTexture;
    sf::Texture* hitTexture;

    /*  sf::Texture* flyTexture;
      sf::Texture* attackTexture;*/

    Animation animationIdle;
    Animation animationWalk;
    Animation animationRun;
    Animation animationHit;
    bool isOnGround; // Новый флаг

    /*  Animation animationFly;
      Animation animationAttack;*/

    EnemyState currentState; // Текущее состояние enemy
    EnemyType type;
};

class Player : public Entity
{
public:
    Player(sf::Texture& textureRunRef, sf::Texture& textureDeadRef, sf::Texture& textureIdleRef, sf::Texture& textureJumpRef, sf::Texture& textureAttackRef);
    ~Player();

    void update(float dt);
    sf::Vector2f GetPosition() const;

    void PlayerCheckPlatformCollision(std::vector<Platform>& platforms, std::vector<Enemy>& enemies, float dt);

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
    }

    PlayerState GetCurrentState() { return currentState; }

private:
    float h = 200.0f;
    float attackCooldown; // Время до следующей атаки игрока
    const float ATTACK_COOLDOWN_MAX = 0.5f; // Например, атака каждые 0.5 секунды
    bool hasAttackedThisFrame; // Флаг, чтобы урон наносился только один раз за атаку
    float invulnerabilityTimer; // Таймер неуязвимости после получения урона
    const float INVULNERABILITY_DURATION = 1.0f; // Например, 1 секунда неуязвимости


    bool canJump;

    Animation animationRun;
    Animation animationDead;
    Animation animationIdle;
    Animation animationJump;
    Animation animationAttack;
    bool isOnGround; // Новый флаг

    PlayerState currentState; // Текущее состояние игрока
    // Ссылки на текстуры, чтобы можно было переключать их
    sf::Texture* runTexture;
    sf::Texture* deadTexture;
    sf::Texture* idleTexture;
    sf::Texture* jumpTexture;
    sf::Texture* attackTexture;

    sf::Vector2f velocity = sf::Vector2f(0.0f, 0.0f);
    float g = 981.0f; //przyspieszenie ziemskie

    float hp = 100.0f;
    float damage = -20.0f;
};

class Animation
{
public:
    Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime);//, bool loop);
    ~Animation();

    void update(int row, float deltaTime, bool faceRight); //aktualizacja animacji
    void reset(); // reset animacji do pierwszej klatki
    void pause(); // zatrzymanie aktualizacji animacji
    void resume();  // wznowienie aktualizacji animacji
    sf::IntRect uvRect;

    bool isFinished() const { return finished; }
    void setFinished(bool state) { finished = state; }

private:
    sf::Vector2u imageCount;
    sf::Vector2u currentImage;

    float totalTime;
    float switchTime;

    bool paused;
    bool finished;
    bool loop; // Новый член: зацикливать ли анимацию
    bool isPaused = false;
};
