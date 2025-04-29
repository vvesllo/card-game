#pragma once

#include <SFML/Graphics.hpp>



struct ResourceManager
{
	static sf::Texture texture_card_back;
	static sf::Texture texture_card_front;
	
	static sf::Texture texture_space;
	static sf::Texture texture_up;
	static sf::Texture texture_down;
	static sf::Texture texture_left;
	static sf::Texture texture_right;

	static sf::Texture texture_dont_space;
	static sf::Texture texture_dont_up;
	static sf::Texture texture_dont_down;
	static sf::Texture texture_dont_left;
	static sf::Texture texture_dont_right;
	
	static sf::Texture texture_joker;

	static sf::Font font_main;
};

void loadResources();