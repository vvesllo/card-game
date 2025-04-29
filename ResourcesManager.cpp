#include "ResourcesManager.h"




sf::Texture ResourceManager::texture_card_back;
sf::Texture ResourceManager::texture_card_front;

sf::Texture ResourceManager::texture_space;
sf::Texture ResourceManager::texture_up;
sf::Texture ResourceManager::texture_down;
sf::Texture ResourceManager::texture_left;
sf::Texture ResourceManager::texture_right;

sf::Texture ResourceManager::texture_dont_space;
sf::Texture ResourceManager::texture_dont_up;
sf::Texture ResourceManager::texture_dont_down;
sf::Texture ResourceManager::texture_dont_left;
sf::Texture ResourceManager::texture_dont_right;

sf::Texture ResourceManager::texture_joker;


sf::Font ResourceManager::font_main;

void loadResources()
{
	ResourceManager::texture_card_front.loadFromFile("Assets/Images/card_front.png");
	ResourceManager::texture_card_back.loadFromFile("Assets/Images/card_back.png");

	ResourceManager::texture_space.loadFromFile("Assets/Images/Bottom/space.png");
	ResourceManager::texture_dont_space.loadFromFile("Assets/Images/Bottom/nospace.png");

	ResourceManager::texture_up.loadFromFile("Assets/Images/Bottom/up.png");
	ResourceManager::texture_down.loadFromFile("Assets/Images/Bottom/down.png");
	ResourceManager::texture_left.loadFromFile("Assets/Images/Bottom/left.png");
	ResourceManager::texture_right.loadFromFile("Assets/Images/Bottom/right.png");

	ResourceManager::texture_dont_up.loadFromFile("Assets/Images/Bottom/noup.png");
	ResourceManager::texture_dont_down.loadFromFile("Assets/Images/Bottom/nodown.png");
	ResourceManager::texture_dont_left.loadFromFile("Assets/Images/Bottom/noleft.png");
	ResourceManager::texture_dont_right.loadFromFile("Assets/Images/Bottom/noright.png");

	ResourceManager::texture_joker.loadFromFile("Assets/Images/Bottom/joker.png");

	ResourceManager::font_main.openFromFile("Assets/Fonts/editundo.ttf");
	ResourceManager::font_main.setSmooth(0);
}
