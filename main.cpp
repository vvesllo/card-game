#include <SFML/Graphics.hpp>
#include <string>
#include <deque>
#include <fstream>

#include "ResourcesManager.h"
#include "Card.h"

#define STREAK_MAX_TIME 5.f

enum class WindowChallenge
{
    NONE,
    MOVE,
    SHAKE,
    SPEEDUP,
};

struct WINDOW_STRUCT
{
    const sf::Vector2u size = { 720, 480 };
    const sf::Vector2u center = { size / 2u };
    const std::string title = "CARD GAME";
    std::vector<WindowChallenge> challenges = {
        WindowChallenge::NONE,
        WindowChallenge::MOVE,
        WindowChallenge::SHAKE,
        WindowChallenge::SPEEDUP,
    };
    WindowChallenge challenge = WindowChallenge::NONE;
} window_struct;

struct KEY_STATE_STRUCT
{
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool space = false;
} key_state_struct;

struct PLAYER_STRUCT
{
    unsigned long long score;
    unsigned long long streak;
    unsigned long long max_streak;
} player_struct;

static float streak_time = 0;

void savePlayerData()
{
    std::fstream file("save.card", std::ios::out | std::ios::binary);

    file.write(reinterpret_cast<char*>(&player_struct.score), sizeof(player_struct.score));
    file.write(reinterpret_cast<char*>(&player_struct.max_streak), sizeof(player_struct.max_streak));

    file.close();
}

void loadPlayerData()
{
    std::fstream file("save.card", std::ios::in | std::ios::binary);

    file.read(reinterpret_cast<char*>(&player_struct.score), sizeof(player_struct.score));
    file.read(reinterpret_cast<char*>(&player_struct.max_streak), sizeof(player_struct.max_streak));

    file.close();
}

const sf::Vector2f getOrderedPosition(const unsigned short& order)
{
    return sf::Vector2f{
        //Card::size.x * std::max(0.f, -(float)std::pow(.5f, order) - 2), 0
        Card::size.x * -order / 10.f, 0
    };
}

void pushCard(std::deque<Card*>& cards_deque)
{
    short random = rand() % 100;
    if (random > 90)
    {
        cards_deque.push_back(new Card{ CardType::JOKER });
    }
    else
    {
        CardType types[] = {
            CardType::SPACE,
            CardType::UP,
            CardType::DOWN,
            CardType::LEFT,
            CardType::RIGHT,
            CardType::DONT_LEFT,
            CardType::DONT_RIGHT,
            CardType::DONT_UP,
            CardType::DONT_DOWN,
            CardType::DONT_SPACE
        };
        cards_deque.push_back(new Card{ types[random % 10] });
    }

    ++Card::ammount;
}

void popCard(std::deque<Card*>& cards_deque)
{
    delete cards_deque.front();
    cards_deque.pop_front();

    --Card::ammount;
    for (std::deque<Card*>::iterator it = cards_deque.begin(); it != cards_deque.end(); ++it)
    {
        --(*it)->getOrder();
    }
}

void drawCard(const std::deque<Card*>& cards_deque, sf::RenderWindow& window, const float& dt)
{
    sf::Sprite* sprite;
    Card* card;

    for (std::deque<Card*>::const_reverse_iterator it = cards_deque.crbegin(); it != cards_deque.crend(); ++it)
    {
        card = (*it);
        sprite = card->getSprite();

        window.draw(*sprite);
    }
}

void updateCard(std::deque<Card*>& cards_deque, const float& dt)
{
    Card* card;
    sf::Sprite* sprite;
    sf::Vector2f position;
    sf::Vector2f offset;

    float color_k;

    for (std::deque<Card*>::iterator it = cards_deque.begin(); it != cards_deque.end(); ++it)
    {
        card = (*it);
        sprite = card->getSprite();
        position = sprite->getPosition();
        offset = sf::Vector2f(window_struct.center) + card->getOffset();
        color_k = (float)card->getOrder() / card->ammount;


        sprite->setColor(
            sf::Color(
                255 - 192 * color_k,
                255 - 192 * color_k,
                255 - 192 * color_k
            )
        );

        sprite->setPosition(sf::Vector2f(
                std::lerp(
                    position.x,
                    offset.x + getOrderedPosition(card->getOrder()).x,
                    10.f * dt),
                offset.y
            )
        );
        card->update(dt);
    }
}

void restartStreak(sf::RenderWindow& window)
{
    window.setTitle(window_struct.title);
    window_struct.challenge = WindowChallenge::NONE;
    player_struct.streak = 0;
    streak_time = 0;
}

void checkCard(std::deque<Card*>& cards_deque, const CardType& type, sf::RenderWindow& window)
{
    if (
        (cards_deque.front()->getType() == type) ||
        (cards_deque.front()->getType() == CardType::JOKER) ||
        (cards_deque.front()->getType() == CardType::DONT_LEFT && type != CardType::LEFT) ||
        (cards_deque.front()->getType() == CardType::DONT_RIGHT && type != CardType::RIGHT) ||
        (cards_deque.front()->getType() == CardType::DONT_UP && type != CardType::UP) ||
        (cards_deque.front()->getType() == CardType::DONT_DOWN && type != CardType::DOWN) ||
        (cards_deque.front()->getType() == CardType::DONT_SPACE && type != CardType::SPACE)
        )
    {
        if (cards_deque.front()->getType() == CardType::JOKER)
        {
            window_struct.challenge = window_struct.challenges.at(rand() % window_struct.challenges.size());
            std::string _suffix = "";
            if (window_struct.challenge == WindowChallenge::NONE)
                window.setTitle(window_struct.title);
            else
            {
                if (window_struct.challenge == WindowChallenge::MOVE)
                    _suffix = "MOVE";
                else if (window_struct.challenge == WindowChallenge::SHAKE)
                    _suffix = "SHAKE";
                else if (window_struct.challenge == WindowChallenge::SPEEDUP)
                    _suffix = "SPEEDUP";

                window.setTitle(std::format("{} // JOKER: {}", window_struct.title, _suffix));
            }
            
        }


        popCard(cards_deque);
        pushCard(cards_deque);
        streak_time = STREAK_MAX_TIME;
        ++player_struct.streak;
        ++player_struct.score;
        player_struct.max_streak = std::max(player_struct.streak, player_struct.max_streak);
    }
    else 
    {
        restartStreak(window);
    }
}

int WinMain()
{
    srand(time(0));
    sf::RenderWindow window(
        sf::VideoMode(window_struct.size), 
        window_struct.title,
        sf::Style::Titlebar | sf::Style::Close
    );

    short screen_width = sf::VideoMode::getDesktopMode().size.x;
    short screen_height = sf::VideoMode::getDesktopMode().size.y;

    loadPlayerData();
    loadResources();


    sf::Text score_label(ResourceManager::font_main, std::to_string(player_struct.score));
    score_label.setOrigin(score_label.getGlobalBounds().size / 2.f);
    score_label.setPosition({ window_struct.size.x * 0.25f, window_struct.size.y * 0.1f });

    sf::Text streak_label(ResourceManager::font_main, std::to_string(player_struct.streak));
    streak_label.setOrigin(streak_label.getGlobalBounds().size / 2.f);
    streak_label.setPosition({ window_struct.size.x * 0.5f, window_struct.size.y * 0.1f });


    sf::Text max_streak_label(ResourceManager::font_main, std::to_string(player_struct.max_streak));
    max_streak_label.setOrigin(max_streak_label.getGlobalBounds().size / 2.f);
    max_streak_label.setPosition({ window_struct.size.x * 0.75f, window_struct.size.y * 0.1f });

    
    std::deque<Card*> cards_deque;
    for (unsigned short i = 0; i < 10; i++)
    {
        pushCard(cards_deque);
    }

    sf::Clock clock;
    float dt;

    sf::Vector2f window_velocity = {1, 1};

    sf::RectangleShape indicator({ (float)window_struct.size.x - 50, 20 });
    indicator.setScale({0, 1});
    indicator.setFillColor(sf::Color(255, 218, 138));
    indicator.setOrigin(indicator.getSize() / 2.f);
    indicator.setPosition({ (float)window_struct.center.x, window_struct.size.y * 0.9f });

    while (window.isOpen())
    {
        dt = clock.restart().asSeconds();
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const sf::Event::KeyReleased* key_pressed = event->getIf<sf::Event::KeyReleased>())
            {
                if (key_state_struct.up && (key_pressed->code == sf::Keyboard::Key::W || key_pressed->code == sf::Keyboard::Key::Up))
                    key_state_struct.up = false;
                else if (key_state_struct.down && (key_pressed->code == sf::Keyboard::Key::S || key_pressed->code == sf::Keyboard::Key::Down))
                    key_state_struct.down = false;
                else if (key_state_struct.left && (key_pressed->code == sf::Keyboard::Key::A || key_pressed->code == sf::Keyboard::Key::Left))
                    key_state_struct.left = false;
                else if (key_state_struct.right && (key_pressed->code == sf::Keyboard::Key::D || key_pressed->code == sf::Keyboard::Key::Right))
                    key_state_struct.right = false;
                else if (key_state_struct.space && key_pressed->code == sf::Keyboard::Key::Space)
                    key_state_struct.space = false;
            }
            else if (const sf::Event::KeyPressed* key_pressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (!key_state_struct.left && key_pressed->code == sf::Keyboard::Key::Left)
                {
                    checkCard(cards_deque, CardType::LEFT, window);
                    key_state_struct.left = true;
                }

                else if (!key_state_struct.right && key_pressed->code == sf::Keyboard::Key::Right)
                {
                    checkCard(cards_deque, CardType::RIGHT, window);
                    key_state_struct.right = true;
                }

                else if (!key_state_struct.up && key_pressed->code == sf::Keyboard::Key::Up)
                {
                    checkCard(cards_deque, CardType::UP, window);
                    key_state_struct.up = true;
                }

                else if (!key_state_struct.down && key_pressed->code == sf::Keyboard::Key::Down)
                {
                    checkCard(cards_deque, CardType::DOWN, window);
                    key_state_struct.down = true;
                }

                else if (!key_state_struct.space && key_pressed->code == sf::Keyboard::Key::Space)
                {
                    checkCard(cards_deque, CardType::SPACE, window);
                    key_state_struct.space = true;
                }
            }
        }

        switch (window_struct.challenge)
        {
        case WindowChallenge::MOVE:
            window.setPosition(window.getPosition() + sf::Vector2i(window_velocity));

            if (window.getPosition().x < 0 || window.getPosition().x + window_struct.size.x > screen_width)
                window_velocity.x = -window_velocity.x;
            else if (window.getPosition().y < 0 || window.getPosition().y + window_struct.size.y > screen_height)
                window_velocity.y = -window_velocity.y;

            break;

        case WindowChallenge::SHAKE:
            window.setPosition(window.getPosition() + sf::Vector2i(2 - rand() % 5, 2 - rand() % 5));
            break;

        case WindowChallenge::SPEEDUP:
            dt *= 2.f;
            break;
        }

        if (streak_time > 0.f)
        {
            streak_time = std::max(streak_time - dt, 0.f);
            if (streak_time == 0.f)
            {
                restartStreak(window);
            }
        }
        

        indicator.setScale({ streak_time / STREAK_MAX_TIME, 1 });

        score_label.setString(std::to_string(player_struct.score));
        score_label.setOrigin(score_label.getGlobalBounds().size / 2.f);
        streak_label.setString(std::to_string(player_struct.streak));
        streak_label.setOrigin(streak_label.getGlobalBounds().size / 2.f);
        max_streak_label.setString(std::to_string(player_struct.max_streak));
        max_streak_label.setOrigin(max_streak_label.getGlobalBounds().size / 2.f);

        updateCard(cards_deque, dt);

        window.clear(sf::Color(64, 10, 14));

        drawCard(cards_deque, window, dt);
        window.draw(indicator);
        window.draw(score_label);
        window.draw(streak_label);
        window.draw(max_streak_label);

        window.display();
    }


    for (std::deque<Card*>::const_iterator it = cards_deque.cbegin(); it != cards_deque.cend(); ++it)
        delete (*it);

    savePlayerData();
}